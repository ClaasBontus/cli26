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
[[=app_description("Definition of options groups")]]
[[=require_option(1)]]
Options
{
    [[=option_description("A range")]]
    [[=option_name("-R,--range")]]
    [[=expected(-2)]]
    std::vector<int> range{};

    [[=add_option_group("min_max_step", "Minimum, maximum, and step options")]]
    MinMaxStep min_max_step;
};

template <> struct std::formatter<Options> : UniversalFormatter { };
template <> struct std::formatter<MinMaxStep> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::get_app(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
