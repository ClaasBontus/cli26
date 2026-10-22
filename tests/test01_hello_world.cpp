#include <cli26.hpp>
#include <print>

struct
[[=CLI26::app_name("Hello World")]]
[[=CLI26::app_description("Hello World App")]]
Options
{
    int count;
};

int main(int argc, char **argv)
{
    Options options;
    CLI26::App app;
    CLI26::get_app(app, options);
    CLI11_PARSE(app, argc, argv);

    std::println("App name: {}", app.get_name());
    std::println("App description: {}", app.get_description());

    return 0;
} // main
