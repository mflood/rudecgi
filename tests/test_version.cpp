// Asserts CGI::version() reports the project version CMake was configured
// with. It previously returned a hardcoded "RudeServer CGI Version 3.0" that
// had not tracked a release since before 4.0, so callers could not tell what
// they had linked.
#include <rude/cgi.h>

#include <cstdio>
#include <cstring>

int main()
{
    const char *reported = rude::CGI::version();
    if (!reported) {
        std::fprintf(stderr, "FAIL: version() returned NULL\n");
        return 1;
    }
    if (!std::strstr(reported, EXPECTED_VERSION)) {
        std::fprintf(stderr, "FAIL: version() reported \"%s\", expected it to "
                             "contain \"%s\"\n", reported, EXPECTED_VERSION);
        return 1;
    }
    std::printf("version OK (%s)\n", reported);
    return 0;
}
