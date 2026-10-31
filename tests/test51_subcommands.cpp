#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
Add
{
    [[=option_name("-f,--file")]]
    [[=option_description("File to be added")]]
    std::string file_name;

    [[=option_name("-r,--recursive")]]
    [[=option_description("Add files recursively")]]
    [[=params(capture_default_str)]]
    bool recursive{false};
};

struct Show
{
    [[=option_name("-q,--quiet")]]
    [[=option_description("Suppress differences")]]
    [[=params(capture_default_str)]]
    bool quiet{false};

    [[=option_description("Show source information")]]
    [[=params(capture_default_str)]]
    bool source{false};
};

struct
[[=app_description("Subcommands example")]]
[[=require_subcommand(1)]]
Options
{
    [[=params(as_flag)]]
    [[=option_name("-v,--verbose")]]
    [[=option_description("Int used as flag (counts verbosity level)")]]
    int verbose;

    [[=add_subcommand("add", "Add files")]]
    Add add{};

    [[=add_subcommand("show", "Show changes")]]
    Show show{};
};

template <> struct std::formatter<Options> : UniversalFormatter { };
template <> struct std::formatter<Add> : UniversalFormatter { };
template <> struct std::formatter<Show> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::add_content(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
