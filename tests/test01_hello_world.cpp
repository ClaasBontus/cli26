#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

struct
[[=CLI26::app_name("Hello World")]]
[[=CLI26::app_description("Hello World App")]]
Options
{
    [[=CLI26::option_description("Example for a bool flag")]]
    bool f;

    [[=CLI26::option_description("Example for an optional int option with default value")]]
    [[=CLI26::capture_default_str()]]
    int count = -1;
};

template <> struct std::formatter<Options> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::get_app(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("App name: {}", app.get_name());
    std::println("App description: {}", app.get_description());
    std::println("Options: {}", options);

    return 0;
} // main
