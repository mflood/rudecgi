// Cookie parsing test (HTTP_COOKIE).
//
// Cookies land in the same field namespace as form data — value("name")
// finds them — and datasource() reports where each field came from.
#include <rude/cgi.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>

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

int main()
{
    set_env("PATH_INFO", "");

    set_env("REQUEST_METHOD", "GET");
    set_env("QUERY_STRING", "field=formval");
    set_env("HTTP_COOKIE", "sessionid=429842; theme=dark%20mode; user=mark");

    rude::CGI cgi; // parsing happens here

    // Cookies are looked up exactly like form fields.
    CHECK(std::strcmp(cgi.value("sessionid"), "429842") == 0);
    CHECK(std::strcmp(cgi.value("theme"), "dark mode") == 0);
    CHECK(std::strcmp(cgi.value("user"), "mark") == 0);

    // datasource() distinguishes cookies from form data.
    CHECK(std::strcmp(cgi.datasource("sessionid"), "cookie") == 0);
    CHECK(std::strcmp(cgi.datasource("field"), "form") == 0);

    CHECK(std::strcmp(cgi.value("field"), "formval") == 0);
    CHECK(cgi.numValues() == 4);

    if (failures) {
        std::fprintf(stderr, "%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("cookie OK\n");
    return 0;
}
