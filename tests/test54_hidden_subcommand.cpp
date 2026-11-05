#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
List
{
    [[=option_name("-f,--file")]]
    [[=option_description("File to show")]]
    std::string file_name;

    [[=option_name("-l,--line-numbers")]]
    [[=option_description("Show line numbers")]]
    [[=params(capture_default_str)]]
    bool line_numbers{false};
};

struct Search
{
    [[=option_name("-f,--file")]]
    [[=option_description("File to show")]]
    std::string file_name;

    [[=option_name("-p,--pattern")]]
    [[=option_description("Pattern to search for")]]
    std::string pattern;
};

struct Hidden
{
    [[=option_name("-s,--status")]]
    [[=option_description("Status to show")]]
    std::string status;
};

struct
[[=app_description("Subcommands with hidden command")]]
[[=require_subcommand(1)]]
Options
{
    [[=add_subcommand("list", "List contents")]]
    List add{};

    [[=add_subcommand("search", "Search contents")]]
    Search search{};

    [[=add_subcommand("hidden", "Hidden subcommand")]]
    [[=group("")]]
    Hidden hidden{};
};

template <> struct std::formatter<Options> : UniversalFormatter { };
template <> struct std::formatter<List> : UniversalFormatter { };
template <> struct std::formatter<Search> : UniversalFormatter { };
template <> struct std::formatter<Hidden> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::add_content(app, options);
    app.set_help_all_flag("--help-all", "Expand all help");
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);
    std::println("Parsed subcommands:");
    for (const auto &subcommand : app.get_subcommands())
        std::println("  {}", subcommand->get_name());

    return 0;
} // main
