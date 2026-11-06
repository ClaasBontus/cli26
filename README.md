# CLI26 - Easy command line parsing based on CLI11

CLI26 is a wrapper around [CLI11](https://github.com/CLIUtils/CLI11) inspired by [clap](https://docs.rs/clap/latest/clap/) and [SimpleParsing](https://github.com/lebrice/SimpleParsing/). It yields the full functionality of CLI11 while providing a structured way for the most common tasks.
CLI26 makes intensive use of C++-26 reflection. It was tested with gcc-16.

## Installation
CLI26 consists of a single header `cli26.hpp`. Copy that header into your project and make sure [CLI11.hpp](https://github.com/CLIUtils/CLI11/releases) is in your include path.

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
          --count INT [-1]    Example of an optional int with default value
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
It all happens in `add_content`. Therein various options and parameters of `app` are set based on the variables
and attributes in `struct Options`. If no `option_name` is specified `add_content` determines one based on the
variable's name. With variable `count` in the example we illustrate this. After command line parsing variables in `struct Options` will contain the parsing results.

Folder [tests](tests) contains various examples for how to utilize CLI26.

## Details
### Naming and params
If attribute `CLI26::option_name` is not specified for a variable CLI26 constructs it from the variable's name.
In case the latter consists of a single character `option_name` will be of type `-x` with a single dash. Otherwise
it will be of type `--name`. For positional options use `option_name("name")` without any dash.

Per default CLI26 assumes a parameter is optional if it has a default value or is of type `std::optional<T>`. It
can be enforced to be required via tag `required`:
```c++
    [[=params(required)]]
    std::string name="";
```
Function `CLI26::params` accepts an arbitrary number of arguments. Here it accepts tags `ignore_case`,
`ignore_underscore`, `capture_default_str`, which correspond to CLI11 functions with identical names.

Per default booleans are treated as flags all other types of variables as options. This behavior can be
changed via params `as_option` and `as_flag`:
```c++
    [[=params(as_option, ignore_case)]]
    bool colored;

    [[=params(as_flag)]]
    [[=option_name("-v,--verbose")]]
    int verbose;
```

If you need an element in your options struct that is not part of the CLI tag it with `ignore`:
```c++
    [[=CLI26::ignore]]
    double ignored_value;
```

### Further attributes
Appreciating CLI11's large number of configuration possibilities CLI26 provides attributes
`needs(std::string_view)`, `excludes(std::string_view)`, `group(std::string_view)`,
`delimiter(char)`, `allow_extra_args(bool)`, `expected(int,int)`, `multi_option_policy(CLI::MultiOptionPolicy)`,
`envname(std::string_view)`.
They all have their named equivalent in CLI11 with identical names.
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
With attribute `group` we can manage the order in which options are listed in the help output.
Option groups go a step further. They allow specific sets of options to be composed
and controlled as a collective. Subcommands on the other hand are keywords that
invoke a new set of options and features. Here `git` serves as a well known example with its
subcommands `add`, `commit`, `log`, etc.

With CLI26 you add option groups and subcommands by definition of additional structs.
Here is an example with two subcommands `range` and `file`, where `range` uses an option group
for making sure the range is specified in exactly one of two possible ways.
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
Notice the `require_option(1)` attribute specified for `struct Range`. It makes sure we either say
`myapp range -t 0 12 2` or `myapp range -m 0 -M 12 -s 2`.

