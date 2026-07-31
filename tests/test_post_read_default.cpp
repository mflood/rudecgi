// Regression test for the *default* POST body read.
//
// 5.2.0 gave callers CGI::maxPostReadSeconds() to bound a stalled body read,
// but left the default at "wait forever". That put the burden on every
// application author to know about a hazard they had no reason to look for: a
// client that over-declares CONTENT_LENGTH and then simply holds the
// connection open pinned the CGI process indefinitely, in a default build,
// with no code change required on the client's part.
//
// From 5.4.0 the default is bounded. It is an idle limit, not a cap on the
// whole read -- a large upload over a slow link legitimately takes minutes
// while making steady progress, and only the absence of progress means a
// stall -- so this test sends a partial body, sends nothing further, and holds
// the pipe open. It requires the constructor to return on its own and parse
// what did arrive.
//
// This test spends the whole idle period waiting, by construction: there is no
// way to observe a 60-second limit in less than 60 seconds. It is the only
// slow test in the suite, and it is guarding an otherwise unbounded hang.
//
// POSIX only: the bound relies on select() over a pipe.
#ifdef _WIN32
#include <cstdio>
int main()
{
	std::printf("SKIP: bounded POST body read is POSIX-only\n");
	return 0;
}
#else

#include <rude/cgi.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

// Must match DEFAULT_IDLE_READ_SECONDS in src/POSTExtractor.cpp. Chosen to
// match nginx's client_body_timeout, which is the same idea applied to the
// same bytes one layer up.
static const long EXPECTED_IDLE_SECONDS = 60;

static int failures = 0;

#define CHECK(cond)                                                              \
	do                                                                           \
	{                                                                            \
		if(!(cond))                                                              \
		{                                                                        \
			std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
			++failures;                                                          \
		}                                                                        \
	} while(0)

int main()
{
	const char *body = "color=green";
	const size_t bodylen = std::strlen(body);

	int fds[2];
	if(::pipe(fds) != 0)
	{
		std::fprintf(stderr, "pipe() failed\n");
		return 1;
	}

	// Partial body, write end left OPEN so the reader never sees EOF.
	if(::write(fds[1], body, bodylen) != (ssize_t) bodylen)
	{
		std::fprintf(stderr, "write() failed\n");
		return 1;
	}

	if(::dup2(fds[0], STDIN_FILENO) < 0)
	{
		std::fprintf(stderr, "dup2() failed\n");
		return 1;
	}

	setenv("HTTP_COOKIE", "", 1);
	setenv("PATH_INFO", "", 1);
	setenv("QUERY_STRING", "", 1);
	setenv("REQUEST_METHOD", "POST", 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	setenv("CONTENT_LENGTH", "100000", 1); // far more than we wrote

	// Deliberately no maxPostReadSeconds() call. The whole point is that an
	// application that never heard of it is still protected.

	std::printf("no limit configured; expecting the default to give up after "
				"about %lds\n",
				EXPECTED_IDLE_SECONDS);
	std::fflush(stdout);

	const std::chrono::steady_clock::time_point start =
		std::chrono::steady_clock::now();

	rude::CGI cgi; // reads the body here

	const long ms = (long) std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::steady_clock::now() - start)
						.count();

	// It waited out the idle period rather than giving up early, which would
	// truncate a merely slow client...
	CHECK(ms >= (EXPECTED_IDLE_SECONDS - 5) * 1000);

	// ...and then stopped, instead of blocking until something kills it.
	CHECK(ms < (EXPECTED_IDLE_SECONDS + 60) * 1000);

	// Whatever arrived is parsed, and nothing beyond it: the unread tail of
	// the buffer must not surface as data.
	CHECK(cgi.value("color") != 0);
	CHECK(std::strcmp(cgi.value("color"), "green") == 0);
	CHECK(cgi.numValues() == 1);

	std::printf("stalled POST body abandoned after %ld ms with no limit set, "
				"parsed \"%s\"\n",
				ms, cgi.value("color"));

	::close(fds[1]);

	if(failures)
	{
		std::fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	std::printf("default POST read bound OK\n");
	return 0;
}
#endif
