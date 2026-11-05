# CLI26 - Easy command line parsing based on CLI11

CLI26 is a wrapper around [CLI11](https://github.com/CLIUtils/CLI11). It yields the full functionality of CLI11 while providing a structured way for the most common tasks.
CLI26 makes intensive use of C++-26 reflection.

## Installation
CLI26 consistis of a single header `cli26.hpp`. Copy that header into your project and make sure [CLI11.hpp](https://github.com/CLIUtils/CLI11/releases) is in your include path.

For compiling the tests run
```cmake
cmake -S . -B build
cmake --build build -j
```

## Hello World
You want a simple command line interface (CLI) with just a boolean flag and an integer that has a help message like
```
My first App


myapp [OPTIONS]


OPTIONS:
  -h,     --help              Print this help message and exit
  -f,     --flag              Example of a bool flag
          --count INT [-1]    Example of an optional int option with default value
```
Just define a `struct` like
```c++
#include <cli26.hpp>

using namespace CLI26;

struct
[[=app_name("myapp")]]
[[=app_description("My first App")]]
Options
{
    [[=option_name("-f,--flag")]]
    [[=option_description("Example of a bool flag")]]
    bool f{false};

    [[=option_description("Example of an optional int option with default value")]]
    [[=params(capture_default_str)]]
    int count = -1;
};

int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    add_content(app, options);
    CLI11_PARSE(app, argc, argv);
}
```
It all happens in `add_content`. Therein various options and parameters of `app` are set based on the variables
and attributes in `struct Options`. If no `option_name` is specified `add_content` determines one based on the
variable's name, like for `count`.
