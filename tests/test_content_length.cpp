// Regression tests for CONTENT_LENGTH handling, fixed in 5.1.1.
//
// Parsing happens in the rude::CGI constructor, once per process, so each
// scenario runs as its own ctest case selected by argv[1].
//
// Before 5.1.1:
//   missing   -> atoi(getenv("CONTENT_LENGTH")) dereferenced NULL and crashed,
//                and CGI::maxPostLength() could not prevent it because the cap
//                was checked after the atoi.
//   over      -> the buffer was allocated but never zeroed and fread()'s return
//                value was ignored, so the unread tail was uninitialised heap
//                that got parsed and handed back as field values.
//   negative,
//   garbage,
//   overflow  -> silently reinterpreted as some other number.
#include <rude/cgi.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

static int failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
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

static void unset_env(const char *name)
{
#ifdef _WIN32
    _putenv_s(name, "");
#else
    unsetenv(name);
#endif
}

// Writes body to a file and makes it this process's stdin, so the CGI
// constructor reads it the way a web server would supply it.
static bool feed_stdin(const std::string &body)
{
    std::FILE *out = std::fopen("cl_body.txt", "wb");
    if (!out) {
        std::fprintf(stderr, "cannot create cl_body.txt\n");
        return false;
    }
    std::fwrite(body.data(), 1, body.size(), out);
    std::fclose(out);
    return std::freopen("cl_body.txt", "rb", stdin) != 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::fprintf(stderr, "usage: test_content_length <scenario>\n");
        return 2;
    }
    const std::string scenario = argv[1];
    const std::string body = "color=green&size=large";

    set_env("HTTP_COOKIE", "");
    set_env("PATH_INFO", "");
    set_env("QUERY_STRING", "");
    set_env("REQUEST_METHOD", "POST");
    set_env("CONTENT_TYPE", "application/x-www-form-urlencoded");

    if (!feed_stdin(body)) {
        std::fprintf(stderr, "cannot reopen stdin\n");
        return 1;
    }

    if (scenario == "missing") {
        // The crash case: no CONTENT_LENGTH at all. Must be treated as an
        // empty body, not a segfault.
        unset_env("CONTENT_LENGTH");
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
        CHECK(cgi.numValues("color") == 0);
    }
    else if (scenario == "missing_with_cap") {
        // Same, but with the documented mitigation in place. The cap used to
        // be applied after the NULL dereference, so it made no difference.
        unset_env("CONTENT_LENGTH");
        rude::CGI::maxPostLength(1048576);
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
    }
    else if (scenario == "empty") {
        set_env("CONTENT_LENGTH", "");
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
    }
    else if (scenario == "over") {
        // Declares far more than the body actually contains. Only the bytes
        // really read may be parsed; nothing from beyond the body may appear.
        char lenbuf[32];
        std::snprintf(lenbuf, sizeof lenbuf, "%zu", body.size() + 4096);
        set_env("CONTENT_LENGTH", lenbuf);

        rude::CGI cgi;
        CHECK(cgi.numValues() == 2);
        CHECK(cgi.value("color") != 0);
        CHECK(std::strcmp(cgi.value("color"), "green") == 0);
        CHECK(cgi.value("size") != 0);
        CHECK(std::strcmp(cgi.value("size"), "large") == 0);
        // The tail must not have leaked in as extra fields or extra values.
        CHECK(cgi.numValues("color") == 1);
        CHECK(cgi.numValues("size") == 1);
    }
    else if (scenario == "under") {
        // Declares less than the body contains. Truncation is the documented
        // consequence, but it must be clean: a prefix, never garbage.
        set_env("CONTENT_LENGTH", "11"); // exactly "color=green"
        rude::CGI cgi;
        CHECK(cgi.value("color") != 0);
        CHECK(std::strcmp(cgi.value("color"), "green") == 0);
        CHECK(cgi.numValues("size") == 0);
    }
    else if (scenario == "negative") {
        set_env("CONTENT_LENGTH", "-5");
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
    }
    else if (scenario == "garbage") {
        set_env("CONTENT_LENGTH", "not-a-number");
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
    }
    else if (scenario == "trailing_garbage") {
        set_env("CONTENT_LENGTH", "22bogus");
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
    }
    else if (scenario == "overflow") {
        // Larger than any int; must be rejected outright rather than wrapped
        // or turned into a multi-gigabyte allocation.
        set_env("CONTENT_LENGTH", "99999999999999999999");
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
    }
    else if (scenario == "capped") {
        // Body is within int range but over the caller's cap: no body, and in
        // particular no allocation of the declared size.
        char lenbuf[32];
        std::snprintf(lenbuf, sizeof lenbuf, "%zu", body.size());
        set_env("CONTENT_LENGTH", lenbuf);
        rude::CGI::maxPostLength(4);
        rude::CGI cgi;
        CHECK(cgi.numValues() == 0);
    }
    else {
        std::fprintf(stderr, "unknown scenario: %s\n", scenario.c_str());
        return 2;
    }

    std::remove("cl_body.txt");

    if (failures) {
        std::fprintf(stderr, "%d check(s) failed in scenario %s\n",
                     failures, scenario.c_str());
        return 1;
    }
    std::printf("content_length[%s] OK\n", scenario.c_str());
    return 0;
}
