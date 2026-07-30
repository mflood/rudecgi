// Contract tests: every assertion here comes from a documented promise in
// cgi.h, not from observed behaviour.
//
// The header is unusually precise about NULL-versus-empty-string, and about
// which lookups terminate a sentinel loop. Those are exactly the paths the
// existing suite never touched.
#include <rude/cgi.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static int failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures; \
        } \
    } while (0)

#define CHECK_STR(actual, expected) \
    do { \
        const char *a_ = (actual); \
        if (!a_ || std::strcmp(a_, (expected)) != 0) { \
            std::fprintf(stderr, "FAIL %s:%d: expected \"%s\", got \"%s\"\n", \
                         __FILE__, __LINE__, (expected), a_ ? a_ : "(null)"); \
            ++failures; \
        } \
    } while (0)

static void set_env(const char *name, const char *value)
{
#ifdef _WIN32
    _putenv_s(name, value);
#else
    setenv(name, value, 1);
#endif
}

int main()
{
    // One request exercising several sources at once: repeated fields, an
    // empty value, a cookie, and PATH_INFO.
    set_env("REQUEST_METHOD", "GET");
    set_env("CONTENT_TYPE", "");
    set_env("CONTENT_LENGTH", "0");
    set_env("QUERY_STRING", "cat=books&cat=music&cat=film&title=Hello%20World&blank=");
    set_env("HTTP_COOKIE", "session=abc123");
    set_env("PATH_INFO", "/alpha/beta");

    rude::CGI cgi;

    // ---- numValues() / fieldnameAt() ------------------------------------
    const int total = cgi.numValues();
    CHECK(total > 0);
    CHECK(cgi.numValues("cat") == 3);
    CHECK(cgi.numValues("title") == 1);
    CHECK(cgi.numValues("does-not-exist") == 0);

    // ---- exists() -------------------------------------------------------
    CHECK(cgi.exists("cat"));
    CHECK(cgi.exists("session"));
    CHECK(!cgi.exists("does-not-exist"));

    // ---- value(int): "" when no value, NULL when out of bounds ----------
    // The header's worked example walks 0..numValues()-1, so every index in
    // that range must be non-NULL.
    for (int i = 0; i < total; ++i) {
        if (cgi.value(i) == 0) {
            std::fprintf(stderr, "FAIL %s:%d: value(%d) was NULL inside "
                                 "0..numValues()-1\n", __FILE__, __LINE__, i);
            ++failures;
        }
        if (cgi.fieldnameAt(i) == 0) {
            std::fprintf(stderr, "FAIL %s:%d: fieldnameAt(%d) was NULL inside "
                                 "0..numValues()-1\n", __FILE__, __LINE__, i);
            ++failures;
        }
    }
    CHECK(cgi.value(total) == 0);
    CHECK(cgi.value(total + 100) == 0);
    CHECK(cgi.value(-1) == 0);

    // ---- value(fieldname): first value, "" when absent, never NULL ------
    CHECK_STR(cgi.value("cat"), "books");            // first of three
    CHECK_STR(cgi.value("title"), "Hello World");
    CHECK_STR(cgi.value("blank"), "");               // present but empty
    // "If no value exists for the fieldname, or the fieldname is not a valid
    // identifier, the method returns "". This method will never return NULL."
    CHECK(cgi.value("does-not-exist") != 0);
    CHECK_STR(cgi.value("does-not-exist"), "");
    CHECK(cgi.value((const char *)0) != 0);

    // ---- value(fieldname, position): NULL past the end ------------------
    // The header's worked example is a sentinel loop that relies on this:
    //   while(category = parser->value("cat", x)) ...
    CHECK_STR(cgi.value("cat", 0), "books");
    CHECK_STR(cgi.value("cat", 1), "music");
    CHECK_STR(cgi.value("cat", 2), "film");
    CHECK(cgi.value("cat", 3) == 0);
    CHECK(cgi.value("cat", -1) == 0);
    CHECK(cgi.value("does-not-exist", 0) == 0);

    {
        int x = 0;
        int seen = 0;
        const char *category = 0;
        while ((category = cgi.value("cat", x)) != 0) {
            ++seen;
            ++x;
            if (seen > 100) {
                std::fprintf(stderr, "FAIL %s:%d: documented value(name, n) "
                                     "sentinel loop did not terminate\n",
                             __FILE__, __LINE__);
                ++failures;
                break;
            }
        }
        CHECK(seen == 3);
    }

    // ---- operator[] -----------------------------------------------------
    // Documented asymmetry: the string overload is value(fieldname), the int
    // overload is fieldnameAt(x). Odd, but it is what the header promises.
    CHECK_STR(cgi["title"], cgi.value("title"));
    CHECK_STR(cgi[0], cgi.fieldnameAt(0));

    // ---- datasource(): form / path / cookie -----------------------------
    CHECK_STR(cgi.datasource("title"), "form");
    CHECK_STR(cgi.datasource("session"), "cookie");
    for (int i = 0; i < total; ++i) {
        const char *src = cgi.datasource(i);
        if (!src) {
            std::fprintf(stderr, "FAIL %s:%d: datasource(%d) was NULL\n",
                         __FILE__, __LINE__, i);
            ++failures;
            continue;
        }
        if (std::strcmp(src, "form") != 0 && std::strcmp(src, "path") != 0 &&
            std::strcmp(src, "cookie") != 0) {
            std::fprintf(stderr, "FAIL %s:%d: datasource(%d) was \"%s\", "
                                 "documented values are form/path/cookie\n",
                         __FILE__, __LINE__, i, src);
            ++failures;
        }
    }

    // ---- isFile(): nothing here is an upload ----------------------------
    CHECK(!cgi.isFile("title"));
    CHECK(!cgi.isFile("cat", 1));
    for (int i = 0; i < total; ++i) {
        if (cgi.isFile(i)) {
            std::fprintf(stderr, "FAIL %s:%d: isFile(%d) true for non-upload\n",
                         __FILE__, __LINE__, i);
            ++failures;
        }
    }

    // ---- length(): matches the value for ordinary form data -------------
    CHECK(cgi.length("title") == (int)std::strlen("Hello World"));
    CHECK(cgi.length("blank") == 0);
    CHECK(cgi.length("cat", 1) == (int)std::strlen("music"));

    // ---- contenttype(): documented default for non-uploads --------------
    // "If the data is not an uploaded file, then the content-type defaults to
    // application/x-formdata"
    CHECK_STR(cgi.contenttype("title"), "application/x-formdata");

    // ---- case sensitivity ------------------------------------------------
    // Default is case-sensitive; turning it off makes lookups insensitive.
    CHECK(!cgi.exists("TITLE"));
    cgi.setCaseSensitive(false);
    CHECK_STR(cgi.value("TiTlE"), "Hello World");
    cgi.setCaseSensitive(true);
    CHECK_STR(cgi.value("TiTlE"), "");

    // ---- version() -------------------------------------------------------
    CHECK(rude::CGI::version() != 0);
    CHECK(rude::CGI::version()[0] != '\0');

    if (failures) {
        std::fprintf(stderr, "%d contract check(s) failed\n", failures);
        return 1;
    }
    std::printf("contract OK\n");
    return 0;
}
