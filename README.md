# CLI26: Modern Command-Line Parsing, Powered by CLI11

CLI26 is a wrapper around [CLI11](https://github.com/CLIUtils/CLI11), inspired by [clap](https://docs.rs/clap/latest/clap/) and [SimpleParsing](https://github.com/lebrice/SimpleParsing/). It provides CLI11's full functionality while offering a structured interface for common tasks.
CLI26 makes extensive use of C++26 reflection. It has been tested with gcc-16.

## Installation
CLI26 consists of a single header `cli26.hpp`. Copy that header into your project and make sure [CLI11.hpp](https://github.com/CLIUtils/CLI11/releases) is in your include path.

To compile the tests, run:
```cmake
cmake -S . -B build
cmake --build build -j
```

## Hello World
Suppose you want a simple command-line interface (CLI) with a boolean flag and an integer, plus a help message like this:
```
My first App


myapp [OPTIONS]


OPTIONS:
  -h,     --help              Print this help message and exit
  -f,     --flag              Example of a bool flag
          --count INT [-1]    Example of an optional int with default value
```
Define a `struct` like this:
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

    [[=option_description("Example of an optional int with default value")]]
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
The magic happens in function `add_content`, which sets various options and parameters of `app` based on the variables
and attributes in `struct Options`. If no `option_name` is specified for a member variable, the library derives one
from the variable's name. The `count` variable in the `struct` is an example. After command-line parsing, the variables
in `struct Options` contain the results.

The [tests](tests) folder contains examples of how to use CLI26.

## Details
### Naming and params
If the `CLI26::option_name` attribute is not specified for a variable, CLI26 derives it from the variable's name.
If the name consists of a single character, the option name takes the form `-x`, with a single dash. Otherwise,
it takes the form `--name`. For positional options, use `option_name("name")` without a dash.

By default, CLI26 treats a parameter as optional if it has a default value or is of type `std::optional<T>`. The `required` tag overrides this default:
```c++
    [[=params(required)]]
    std::string name="";
```
The `CLI26::params` function accepts any number of arguments. It accepts the tags `ignore_case`,
`ignore_underscore`, and `capture_default_str`, which correspond to CLI11 functions with the same names.

By default, booleans are treated as flags and all other variable types as options. This behavior can be
changed using the `as_option` and `as_flag` parameters:
```c++
    [[=params(as_option, ignore_case)]]
    bool colored;

    [[=params(as_flag)]]
    [[=option_name("-v,--verbose")]]
    int verbose;
```

To keep a member of your options struct out of the command-line interface, tag it with `ignore`:
```c++
    [[=CLI26::ignore]]
    double ignored_value;
```

### Further attributes
To expose CLI11's many configuration options, CLI26 provides the following attributes:
`needs(std::string_view)`, `excludes(std::string_view)`, `group(std::string_view)`,
`delimiter(char)`, `allow_extra_args(bool)`, `expected(int,int)`, `multi_option_policy(CLI::MultiOptionPolicy)`,
`envname(std::string_view)`.
Each corresponds to a CLI11 setting with the same name.
```c++
    [[=expected(2)]]
    [[=delimiter(',')]]
    [[=option_description("Vector of doubles with 2 expected elements separated by comma")]]
    std::vector<double> numbers;

    [[=needs("--age")]]
    std::optional<std::string> name;

    [[=option_description("An int option that requires name")]]
    [[=needs("--name")]]
    std::optional<int> age;
```

## Option groups and subcommands
The `group` attribute controls the order in which options appear in the help output.
Option groups go a step further: they let you compose and manage sets of options collectively. Subcommands,
on the other hand, are keywords that invoke a new set of options and features. The `git` command is a well-known example,
with subcommands such as `add`, `commit`, and `log`.

In CLI26, you add option groups and subcommands by defining additional structs.
The following example has two subcommands, `range` and `file`. The `range` subcommand uses an option group
to ensure that the range is specified in exactly one of two ways.
```c++
struct MinMaxStep
{
    [[=option_name("-m,--min")]]
    [[=option_description("The minimum")]]
    [[=params(required)]]
    int min=0;

    [[=option_name("-M,--max")]]
    [[=option_description("The maximum")]]
    [[=params(required)]]
    int max=0;

    [[=option_name("-s,--step")]]
    [[=option_description("The step")]]
    [[=params(capture_default_str)]]
    int step=1;
};

struct
[[=require_option(1)]]
Range
{
    [[=option_description("At least two values")]]
    [[=option_name("-t,--triple")]]
    [[=expected(-2)]]
    std::vector<int> triple{};

    [[=add_option_group("min_max_step", "Minimum, maximum, and step options")]]
    MinMaxStep min_max_step;
};

struct File
{
    [[=option_name("-f,--file")]]
    [[=option_description("The input file")]]
    std::string filename;

    [[=option_name("-c,--convolution")]]
    [[=option_description("Optional convolution file")]]
    std::optional<std::string> convolution;
};

struct
[[=app_description("Choose a subcommand which might have option groups")]]
MyCLI
{
    [[=add_subcommand("range", "A range")]]
    Range range;

    [[=add_subcommand("file", "File input")]]
    File file;
};

int main(int argc, char **argv)
{
    MyCLI options;
    CLI::App app;
    CLI26::add_content(app, options);
    app.set_help_all_flag("--help-all", "Expand all help");
    CLI11_PARSE(app, argc, argv);
} // main
```
Note the `require_option(1)` attribute on `struct Range`. It ensures that users specify either
`myapp range -t 0 12 2` or `myapp range -m 0 -M 12 -s 2`.

Other attributes available on structs include `require_subcommand(int,int)`, `fallthrough(bool)`, and `subcommand_fallthrough(bool)`.
These correspond to identically named CLI11 settings and are defined for objects of type `App`.

