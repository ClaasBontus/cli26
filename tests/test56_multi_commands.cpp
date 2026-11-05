#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
Add
{
    [[=option_name("s1")]]
    double summand1;

    [[=option_name("s2")]]
    double summand2;
};

struct
Multiply
{
    [[=option_name("f1")]]
    double factor1;

    [[=option_name("f2")]]
    double factor2;
};

struct
Negate
{
    [[=option_name("value")]]
    double value;
};

struct
[[=app_description("Example for multiple subcommands that can be used together")]]
[[=require_subcommand(1, 0)]]
[[=fallthrough(true)]]  // Specify -v anywhere in between
Options
{
    [[=option_description("Enable verbose output")]]
    [[=option_name("-v")]]
    bool verbose=false;

    [[=add_subcommand("add", "Add numbers")]]
    Add add{};

    [[=add_subcommand("multiply", "Multiply numbers")]]
    Multiply multiply{};

    [[=add_subcommand("negate", "Negate values")]]
    Negate negate{};
};

template <> struct std::formatter<Options> : UniversalFormatter { };
template <> struct std::formatter<Add> : UniversalFormatter { };
template <> struct std::formatter<Multiply> : UniversalFormatter { };
template <> struct std::formatter<Negate> : UniversalFormatter { };


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
