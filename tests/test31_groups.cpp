#include <print>
#include <cli26.hpp>

#include "formatter.hpp"

using namespace CLI26;

struct
[[=app_description("Separating options into groups")]]
Options
{
    [[=option_name("-n,--name")]]
    [[=option_description("Person name")]]
    [[=group("Personal information")]]
    std::string name;

    [[=option_name("-t,--htown")]]
    [[=option_description("Person hometown")]]
    [[=group("Personal information")]]
    std::string hometown;

    [[=option_name("-g,--grownup")]]
    [[=option_description("Is the person grown up?")]]
    [[=group("Voluntary information")]]
    std::optional<bool> grownup;

    [[=option_name("-a,--age")]]
    [[=option_description("Person age")]]
    [[=group("Voluntary information")]]
    std::optional<int> age;

    [[=option_description("Hidden flag")]]
    [[=group("")]]
    bool debug;
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
