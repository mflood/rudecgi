// Regression test for cookie-vs-form precedence.
//
// Cookies and form fields share one lookup namespace, disambiguated only via
// datasource(). Which one an unindexed value() returned used to depend on the
// request method: on GET the query string was parsed before cookies so form
// won, but on POST cookies were parsed before the body so the cookie won.
//
// A client controls its own cookies, so that let a cookie shadow any POST
// field at every unindexed value() call. Cookies are now parsed last on both
// methods, so form data consistently wins.
//
// Run as: test_precedence <get|post>
#include <rude/cgi.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
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

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		std::fprintf(stderr, "usage: test_precedence <get|post>\n");
		return 2;
	}
	const std::string method = argv[1];

	// Same field name from two sources, with different values.
	set_env("HTTP_COOKIE", "amount=9999; session=abc");
	set_env("PATH_INFO", "");

	if(method == "get")
	{
		set_env("REQUEST_METHOD", "GET");
		set_env("QUERY_STRING", "amount=10");
		set_env("CONTENT_TYPE", "");
		set_env("CONTENT_LENGTH", "0");
	}
	else if(method == "post")
	{
		const std::string body = "amount=10";
		std::FILE *out = std::fopen("prec_body.txt", "wb");
		if(!out)
		{
			std::fprintf(stderr, "cannot create prec_body.txt\n");
			return 1;
		}
		std::fwrite(body.data(), 1, body.size(), out);
		std::fclose(out);
		if(!std::freopen("prec_body.txt", "rb", stdin))
		{
			std::fprintf(stderr, "cannot reopen stdin\n");
			return 1;
		}
		char lenbuf[32];
		std::snprintf(lenbuf, sizeof lenbuf, "%zu", body.size());
		set_env("REQUEST_METHOD", "POST");
		set_env("CONTENT_TYPE", "application/x-www-form-urlencoded");
		set_env("CONTENT_LENGTH", lenbuf);
		set_env("QUERY_STRING", "");
	}
	else
	{
		std::fprintf(stderr, "unknown method: %s\n", method.c_str());
		return 2;
	}

	rude::CGI cgi;

	// Both sources are present...
	CHECK(cgi.numValues("amount") == 2);

	// ...and the form value must win, on either method. A client-supplied
	// cookie must not be able to shadow a form field.
	CHECK(cgi.value("amount") != 0);
	CHECK(std::strcmp(cgi.value("amount"), "10") == 0);
	CHECK(std::strcmp(cgi.datasource("amount"), "form") == 0);

	// Cookies with no form counterpart still resolve normally.
	CHECK(cgi.value("session") != 0);
	CHECK(std::strcmp(cgi.value("session"), "abc") == 0);
	CHECK(std::strcmp(cgi.datasource("session"), "cookie") == 0);

	std::remove("prec_body.txt");

	if(failures)
	{
		std::fprintf(stderr, "%d check(s) failed for method %s\n",
					 failures, method.c_str());
		return 1;
	}
	std::printf("precedence[%s] OK\n", method.c_str());
	return 0;
}
