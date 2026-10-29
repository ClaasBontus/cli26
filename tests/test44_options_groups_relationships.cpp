#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=require_option(1)]]
Format
{
    [[=option_name("-c,--csv")]]
    [[=option_description("CSV format")]]
    bool csv{false};

    [[=option_name("-u,--human")]]
    [[=option_description("Human-readable format")]]
    bool human{false};

    [[=option_name("-b,--binary")]]
    [[=option_description("Binary format")]]
    bool binary{false};
};

struct
[[=require_option(0, 1)]]
Target
{
    [[=option_name("-o,--file")]]
    [[=option_description("specify the file location of the output")]]
    std::string file_loc="";

    [[=option_name("-a,--address")]]
    [[=option_description("specify a network address to send the file")]]
    std::string network_address="";
};


struct
[[=app_description("Definition of options groups")]]
Options
{
    [[=add_option_group("output_format", "formatting type for output")]]
    Format format;

    [[=add_option_group("output target", "target location for the output")]]
    Target target;
};

template <> struct std::formatter<Options> : UniversalFormatter { };
template <> struct std::formatter<Format> : UniversalFormatter { };
template <> struct std::formatter<Target> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::get_app(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
