#include <print>
#include <cstdint>
#include <string>
#include <vector>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=app_description("Flag types")]]
Options
{
    [[=option_description("Classical flag")]]
    [[=option_name("-g")]]
    bool green;

    [[=params(as_option, ignore_case, ignore_underscore)]]
    [[=option_description("Required boolean")]]
    bool colored;

    [[=option_description("Boolean flag with not option")]]
    [[=option_name("--trigger,!--no-trigger")]]
    bool trigger;

    [[=params(as_flag)]]
    [[=option_description("Integer flag with not option")]]
    [[=option_name("--on,!--off")]]
    int yes_or_no;

    [[=params(as_flag)]]
    [[=option_name("-v,--verbose")]]
    [[=option_description("Int used as flag (counts verbosity level)")]]
    int verbose;

    [[=option_description("Compression level")]]
    [[=params(as_flag)]]
    [[=option_name("-1{1},-2{2},-3{3},-4{4},-5{5},-6{6}, -7{7}, -8{8}, -9{9}")]]
    int compression_level=0;
};

template <> struct std::formatter<Options> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::get_app(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
