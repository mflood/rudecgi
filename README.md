# rudecgi

A small C++ library for CGI programs: it parses form data (GET query
strings and POST bodies), cookies, `PATH_INFO`, and multipart file
uploads into one collection you query by field name.

```cpp
rude::CGI cgi;                 // parses the whole request, right here
const char *who = cgi.value("username");
```

First released in 2000 as part of the [RudeServer](https://github.com/mflood)
C++ CGI library family; modernized in 2026 (CMake, C++17, CI, tests).

> **Correct usage note:** old copies of the documentation (including
> comments still visible in the header) describe a `CGI::instance()`
> singleton accessor. That method does not exist. The real API is simply
> to construct a `rude::CGI` object — the first construction triggers
> parsing of the request, and every later `rude::CGI` object shares the
> same parsed data.

## Quick start

```cpp
#include <rude/cgi.h>
#include <iostream>

int main()
{
    rude::CGI cgi; // reads REQUEST_METHOD, QUERY_STRING, stdin, cookies...

    std::cout << "Content-type: text/plain\r\n\r\n";

    std::cout << "name:  " << cgi.value("name")  << "\n"; // "" if missing
    std::cout << "email: " << cgi["email"]       << "\n"; // operator[] shorthand

    // repeated fields (checkbox groups) keep every value
    for (int i = 0; i < cgi.numValues("color"); i++)
        std::cout << "color: " << cgi.value("color", i) << "\n";
}
```

Compile with:

```sh
c++ -std=c++17 app.cpp $(pkg-config --cflags --libs rudecgi)
```

Test it from a shell without a web server by faking the CGI environment:

```sh
REQUEST_METHOD=GET QUERY_STRING='name=John%20Doe&color=blue&color=red' \
HTTP_COOKIE='sessionid=429842' ./app
```

## Building

Requires CMake ≥ 3.16 and any C++17 compiler. No other dependencies.

```sh
git clone https://github.com/mflood/rudecgi
cd rudecgi
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build            # run the tests
cmake --install build             # add --prefix ~/some/dir for a local install
```

This builds a static library by default; add `-DBUILD_SHARED_LIBS=ON` for a
shared library. An example CGI program lives in
[`examples/demo.cpp`](examples/demo.cpp) (built as `build/examples/demo`).

### Using from CMake

```cmake
find_package(rudecgi REQUIRED)
target_link_libraries(myapp PRIVATE rudecgi::rudecgi)
```

Or vendor it with `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(rudecgi
    GIT_REPOSITORY https://github.com/mflood/rudecgi
    GIT_TAG v5.2.0)
FetchContent_MakeAvailable(rudecgi)
target_link_libraries(myapp PRIVATE rudecgi::rudecgi)
```

## API notes

- The full API is documented in [`src/cgi.h`](src/cgi.h) and the
  `rudecgi(3)` man page (ignore the `CGI::instance()` references — see the
  note above).
- **Parsing is lazy and happens once per process**: the first `rude::CGI`
  constructed reads the environment and stdin; configuration calls such as
  `CGI::parseCookies(false)` or `CGI::maxPostLength(bytes)` must happen
  *before* that first construction.
- **One namespace for everything**: query-string fields, POST fields,
  cookies, and `PATH_INFO` data all land in the same collection —
  `value("sessionid")` finds a cookie just as it finds a form field.
  Use `datasource(...)` to learn where a field came from: it returns
  `"form"`, `"cookie"`, or `"path"`.
- **Missing fields are safe**: `value(fieldname)` returns `""` (never
  `NULL`); use `exists(fieldname)` to distinguish missing from empty.
- **Repeated fields**: `numValues("f")` and `value("f", n)` give access to
  every value submitted under the same name, in order.
- **File uploads** (multipart/form-data): `isFile()`, `value()` (the raw
  bytes), `length()` (uploads may contain NUL bytes), `contenttype()`,
  `filename()` (a sanitized name), and `filepath()` (the client-supplied
  path).

## Security notes

- The POST reader trusts the `CONTENT_LENGTH` environment variable: it
  allocates a buffer of that size and by default accepts any length
  (`maxPostLength` defaults to unlimited, and a POST with a missing
  `CONTENT_LENGTH` is not handled defensively). Real web servers always
  set it truthfully, but if your program can be invoked with a hostile
  environment, call `CGI::maxPostLength(bytes)` before the first
  `rude::CGI` construction to cap it.
- This is a 2005-era codebase modernized to build cleanly; it has not had
  a security audit. Treat all returned values as untrusted user input.

## History

- **5.1.0** (2026) — CMake build; C++17; compiles warning-free with
  `-Wall -Wextra`; ctype calls made safe for high-bit (UTF-8) input on
  signed-char platforms; fixed a mismatched `delete` on upload filenames;
  test suite and CI on Linux (x86_64 + ARM), macOS, and Windows, plus
  ASan/UBSan. The legacy autotools files are still present but no longer
  maintained.
- **5.0.0** (2007) — last release of the original autotools era
  (`CGIParser` renamed to `CGI`).

## License

GPL-2.0-or-later — see [COPYING](COPYING).
