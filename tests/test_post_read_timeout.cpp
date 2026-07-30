// Regression test for a stalled POST body.
//
// fread() blocks until it has the full CONTENT_LENGTH or hits EOF. A client
// that over-declares CONTENT_LENGTH and then holds the connection open
// therefore pinned the CGI process indefinitely: the round-2 playtest saw this
// as `exit 124` under `timeout`. Pipes already at EOF returned cleanly, which
// is why earlier testing concluded there was no hang - the real web-server
// case is a pipe that stays open.
//
// CGI::maxPostReadSeconds() bounds that wait. This test feeds stdin from a
// pipe whose write end is deliberately left open, declares far more than it
// writes, and requires the constructor to return on schedule with the partial
// body parsed.
//
// POSIX only: the fix relies on select() over a pipe.
#ifdef _WIN32
#include <cstdio>
int main()
{
    std::printf("SKIP: POST read timeout is POSIX-only\n");
    return 0;
}
#else

#include <rude/cgi.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

static int failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures; \
        } \
    } while (0)

int main()
{
    const char *body = "color=green";
    const size_t bodylen = std::strlen(body);

    int fds[2];
    if (::pipe(fds) != 0) {
        std::fprintf(stderr, "pipe() failed\n");
        return 1;
    }

    // Write a partial body and leave the write end OPEN, so the reader never
    // sees EOF -- exactly the stalled-client case.
    if (::write(fds[1], body, bodylen) != (ssize_t)bodylen) {
        std::fprintf(stderr, "write() failed\n");
        return 1;
    }

    if (::dup2(fds[0], STDIN_FILENO) < 0) {
        std::fprintf(stderr, "dup2() failed\n");
        return 1;
    }

    setenv("HTTP_COOKIE", "", 1);
    setenv("PATH_INFO", "", 1);
    setenv("QUERY_STRING", "", 1);
    setenv("REQUEST_METHOD", "POST", 1);
    setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
    setenv("CONTENT_LENGTH", "100000", 1); // far more than we wrote

    // Control path: with RUDECGI_TEST_NO_TIMEOUT set, the limit is left at its
    // default of 0 and the read must block indefinitely, demonstrating both
    // the original bug and that the default behaviour is unchanged. Run
    // manually under an external timeout; not registered as a ctest, because
    // the whole point is that it never returns.
    const bool useTimeout = (std::getenv("RUDECGI_TEST_NO_TIMEOUT") == 0);
    if (useTimeout) {
        rude::CGI::maxPostReadSeconds(2);
    } else {
        std::printf("control: no read limit set, expecting to block\n");
        std::fflush(stdout);
    }

    const std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();

    rude::CGI cgi; // reads the body here

    const long ms = (long)std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start).count();

    // Only reachable when a limit was set; the control path never gets here.
    CHECK(useTimeout);

    // It must have waited...
    CHECK(ms >= 1500);
    // ...but given up rather than blocking until the ctest timeout.
    CHECK(ms < 15000);

    // Whatever arrived in time is parsed, and nothing beyond it.
    CHECK(cgi.value("color") != 0);
    CHECK(std::strcmp(cgi.value("color"), "green") == 0);
    CHECK(cgi.numValues() == 1);

    std::printf("stalled POST body abandoned after %ld ms, parsed \"%s\"\n",
                ms, cgi.value("color"));

    ::close(fds[1]);

    if (failures) {
        std::fprintf(stderr, "%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("post read timeout OK\n");
    return 0;
}
#endif
