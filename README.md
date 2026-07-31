# RudeCGI

RudeCGI gives C++ CGI applications a compact interface to query parameters,
form submissions, cookies, path data, and uploaded files.

## Why RudeCGI?

- Parses GET query strings and POST request bodies.
- Supports URL-encoded and multipart form data, including file uploads.
- Preserves repeated fields such as checkbox groups.
- Provides access to cookies and `PATH_INFO` through the same query API.
- Records whether each value came from a form, cookie, or path component.
- Has no external dependencies beyond a C++17 standard library.

## Quick start

```cpp
#include <rude/cgi.h>

#include <iostream>

int main()
{
    rude::CGI::maxPostLength(10 * 1024 * 1024); // 10 MiB request limit
    rude::CGI::maxPostReadSeconds(30);           // 30-second read limit
    rude::CGI request;

    std::cout << "Content-type: text/plain\r\n\r\n";
    std::cout << "name: " << request.value("name") << "\n";
    std::cout << "email: " << request["email"] << "\n";

    for (int i = 0; i < request.numValues("color"); ++i) {
        std::cout << "color: " << request.value("color", i) << "\n";
    }
}
```

Compile an installed copy with pkg-config:

```sh
c++ -std=c++17 app.cpp $(pkg-config --cflags --libs rudecgi)
```

You can exercise a CGI program from the shell without a web server:

```sh
REQUEST_METHOD=GET \
QUERY_STRING='name=John%20Doe&color=blue&color=red' \
HTTP_COOKIE='sessionid=429842' \
./app
```

## Build and install

RudeCGI requires CMake 3.16 or newer and a C++17 compiler.

```sh
git clone https://github.com/mflood/rudecgi.git
cd rudecgi
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
cmake --install build --prefix ./install
```

The default build is static. Pass `-DBUILD_SHARED_LIBS=ON` to build a shared
library.

For an install outside the system prefix, point CMake consumers at it with
`-DCMAKE_PREFIX_PATH=/path/to/install`. For pkg-config, add
`/path/to/install/lib/pkgconfig` to `PKG_CONFIG_PATH`.

The complete runnable example is in
[`examples/demo.cpp`](examples/demo.cpp).

## Use from CMake

With an installed copy:

```cmake
find_package(rudecgi REQUIRED)
target_link_libraries(myapp PRIVATE rudecgi::rudecgi)
```

Or include it directly with `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(rudecgi
    GIT_REPOSITORY https://github.com/mflood/rudecgi.git
    GIT_TAG v5.4.0)
FetchContent_MakeAvailable(rudecgi)
target_link_libraries(myapp PRIVATE rudecgi::rudecgi)
```

## Request lifecycle

The first `rude::CGI` object constructed in a process reads the CGI
environment and request body. Later instances share that parsed request.
Configure parser limits before constructing the first instance:

```cpp
rude::CGI::maxPostLength(10 * 1024 * 1024); // 10 MiB
rude::CGI::maxPostReadSeconds(30);
rude::CGI request;
```

## Working with request data

### Sources and repeated fields

Query-string fields, POST fields, cookies, and path data share one collection.
Use `datasource()` to distinguish `"form"`, `"cookie"`, and `"path"` values.
Use `numValues(name)` and `value(name, index)` to read repeated values in
submission order.

### Missing fields

`value(name)` returns `""` for a missing field. Use `exists(name)` when an
empty submitted value must be distinguished from a missing one.

### File uploads

For multipart uploads, use `isFile()`, `value()` for the raw bytes,
`length()` for their size, `contenttype()`, `filename()` for the sanitized
name, and `filepath()` for the client-supplied path. Uploaded data may contain
NUL bytes, so use the reported length rather than treating it as a C string.

## Secure deployment

- Set `maxPostLength()` before constructing the first request object to bound
  memory use for POST bodies.
- Set `maxPostReadSeconds()` to suit the web server's request timeout policy.
- Treat every returned value as untrusted input and validate it for its
  destination context.
- Generate server-side storage names for uploads; do not use a client-supplied
  path as a filesystem destination.
- Apply output escaping appropriate to HTML, HTTP headers, SQL, shell commands,
  and other downstream consumers.

## Documentation and support

- Public API: [`src/cgi.h`](src/cgi.h)
- Runnable example: [`examples/demo.cpp`](examples/demo.cpp)
- Manual page: `rudecgi(3)`
- Release notes: [`NEWS`](NEWS)
- Bug reports: [GitHub Issues](https://github.com/mflood/rudecgi/issues)

## License

GPL-2.0-or-later. See [`COPYING`](COPYING).
