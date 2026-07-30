// demo.cpp - a minimal CGI program using RudeCGI.
//
// Construct a rude::CGI object and ask it for field values by name.
// (Parsing of the CGI environment happens when the first rude::CGI
// object is constructed — there is no CGI::instance() method.)
//
// Deploy it as a CGI program behind any web server, or try it straight
// from a shell by faking the CGI environment:
//
//   # GET request with a query string and a cookie
//   REQUEST_METHOD=GET \
//   QUERY_STRING='name=John%20Doe&color=blue&color=red' \
//   HTTP_COOKIE='sessionid=429842' \
//   ./demo
//
//   # POST request: body comes in on stdin, sized by CONTENT_LENGTH
//   BODY='title=Hello%20World&tag=web+dev'
//   printf '%s' "$BODY" |
//   REQUEST_METHOD=POST \
//   CONTENT_TYPE=application/x-www-form-urlencoded \
//   CONTENT_LENGTH=${#BODY} \
//   ./demo
#include <rude/cgi.h>
#include <iostream>

int main()
{
	rude::CGI cgi; // reads REQUEST_METHOD, QUERY_STRING, stdin, cookies...

	// A real CGI response starts with a header block.
	std::cout << "Content-type: text/plain\r\n\r\n";

	// Look up a single field (returns "" if it wasn't sent).
	std::cout << "Hello, "
			  << (cgi.exists("name") ? cgi.value("name") : "stranger")
			  << "!\n\n";

	// Enumerate every field that was sent, with its origin.
	int total = cgi.numValues();
	std::cout << total << " field(s) received:\n";
	for(int i = 0; i < total; i++)
	{
		std::cout << "  " << cgi.fieldnameAt(i)
				  << " = " << cgi.value(i)
				  << "   (from " << cgi.datasource(i) << ")\n";
	}

	// Repeated fields (e.g. checkbox groups) keep every value.
	int colors = cgi.numValues("color");
	for(int i = 0; i < colors; i++)
	{
		std::cout << "color #" << i << ": " << cgi.value("color", i) << "\n";
	}

	return 0;
}
