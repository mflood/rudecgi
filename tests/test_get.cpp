// GET query-string parsing test.
//
// RudeCGI reads the CGI environment variables lazily, the first time a
// rude::CGI object is constructed in the process (the parsed data is then
// cached in a process-wide singleton). So each test sets its environment
// with setenv() *before* constructing the first rude::CGI object.
#include <rude/cgi.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>

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
	// Start from a known-clean CGI environment (empty string means
	// "no data" to the parser; on Windows it removes the variable).
	set_env("HTTP_COOKIE", "");
	set_env("PATH_INFO", "");

	set_env("REQUEST_METHOD", "GET");
	set_env("QUERY_STRING",
			"name=John%20Doe&city=New+York&motto=fish%26chips"
			"&empty=&color=blue&color=red&color=yellow");

	rude::CGI cgi; // parsing happens here

	// URL-decoding: %20 and '+' both mean space, %26 is a literal '&'.
	CHECK(std::strcmp(cgi.value("name"), "John Doe") == 0);
	CHECK(std::strcmp(cgi.value("city"), "New York") == 0);
	CHECK(std::strcmp(cgi.value("motto"), "fish&chips") == 0);

	// A field submitted with an empty value exists but is "".
	CHECK(cgi.exists("empty"));
	CHECK(std::strcmp(cgi.value("empty"), "") == 0);

	// Repeated fields keep every value, in submission order.
	CHECK(cgi.numValues("color") == 3);
	CHECK(std::strcmp(cgi.value("color", 0), "blue") == 0);
	CHECK(std::strcmp(cgi.value("color", 1), "red") == 0);
	CHECK(std::strcmp(cgi.value("color", 2), "yellow") == 0);

	// Missing fields are safe: value() returns "" (never NULL).
	CHECK(!cgi.exists("missing"));
	CHECK(std::strcmp(cgi.value("missing"), "") == 0);

	// Total count and provenance.
	CHECK(cgi.numValues() == 7);
	CHECK(std::strcmp(cgi.datasource("name"), "form") == 0);

	// operator[] is shorthand for value(fieldname).
	CHECK(std::strcmp(cgi["city"], "New York") == 0);

	if(failures)
	{
		std::fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	std::printf("get OK\n");
	return 0;
}
