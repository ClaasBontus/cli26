#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

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

template <> struct std::formatter<Range> : UniversalFormatter { };
template <> struct std::formatter<MinMaxStep> : UniversalFormatter { };
template <> struct std::formatter<File> : UniversalFormatter { };
template <> struct std::formatter<MyCLI> : UniversalFormatter { };


int main(int argc, char **argv)
{
    MyCLI options;
    CLI::App app;
    CLI26::add_content(app, options);
    app.set_help_all_flag("--help-all", "Expand all help");
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
