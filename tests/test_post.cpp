// POST body parsing test (application/x-www-form-urlencoded).
//
// The POST body arrives on stdin, sized by CONTENT_LENGTH. The test writes
// the body to a file and re-opens it as stdin with freopen() before the
// first rude::CGI object is constructed (which is when parsing happens).
#include <rude/cgi.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

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
	const std::string body =
		"title=Hello%20World&tags=c%2B%2B&tags=web+dev&note=a%26b";

	// Write the body to a file and make it the process's stdin.
	{
		std::FILE *out = std::fopen("post_body.txt", "wb");
		if(!out)
		{
			std::fprintf(stderr, "cannot create post_body.txt\n");
			return 1;
		}
		std::fwrite(body.data(), 1, body.size(), out);
		std::fclose(out);
	}
	if(!std::freopen("post_body.txt", "rb", stdin))
	{
		std::fprintf(stderr, "cannot reopen stdin\n");
		return 1;
	}

	char lenbuf[32];
	std::snprintf(lenbuf, sizeof lenbuf, "%zu", body.size());

	set_env("HTTP_COOKIE", "");
	set_env("PATH_INFO", "");

	set_env("REQUEST_METHOD", "POST");
	set_env("CONTENT_TYPE", "application/x-www-form-urlencoded");
	set_env("CONTENT_LENGTH", lenbuf);
	// A POST request can carry a query string too; both are parsed.
	set_env("QUERY_STRING", "page=2");

	rude::CGI cgi; // parsing happens here

	// Fields from the POST body.
	CHECK(std::strcmp(cgi.value("title"), "Hello World") == 0);
	CHECK(cgi.numValues("tags") == 2);
	CHECK(std::strcmp(cgi.value("tags", 0), "c++") == 0);
	CHECK(std::strcmp(cgi.value("tags", 1), "web dev") == 0);
	CHECK(std::strcmp(cgi.value("note"), "a&b") == 0);

	// Field from the query string, merged into the same namespace.
	CHECK(std::strcmp(cgi.value("page"), "2") == 0);

	CHECK(cgi.numValues() == 5);
	CHECK(std::strcmp(cgi.datasource("title"), "form") == 0);
	CHECK(!cgi.isFile("title"));

	if(failures)
	{
		std::fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	std::printf("post OK\n");
	return 0;
}
