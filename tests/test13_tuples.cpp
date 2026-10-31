#include <print>
#include <tuple>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=app_description("Test tuples")]]
Options
{
    [[=option_description("Index of number")]]
    [[=option_name("-n,--number")]]
    [[=params(capture_default_str)]]
    std::tuple<int, double> int_double={0, 0.0};

};

template <> struct std::formatter<Options> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::add_content(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
