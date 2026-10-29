#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct File
{
    [[=option_name("-n,--filename")]]
    [[=option_description("File name")]]
    std::string filename;

    [[=option_name("-r,--readonly")]]
    [[=option_description("Read-only flag")]]
    bool readonly{false};
};


struct
[[=app_description("Definition of options groups")]]
Options
{
    [[=option_name("-f,--flag")]]
    [[=option_description("One flag")]]
    bool f{false};

    [[=option_description("One int")]]
    [[=params(capture_default_str)]]
    int count = 0;

    [[=add_option_group("file_options", "File options")]]
    File file;
};

template <> struct std::formatter<Options> : UniversalFormatter { };
template <> struct std::formatter<File> : UniversalFormatter { };


int main(int argc, char **argv)
{
    Options options;
    CLI::App app;
    CLI26::get_app(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("Options: {}", options);

    return 0;
} // main
