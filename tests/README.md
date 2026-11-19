# CLI26 test code

The example programs in this folder print the results of command-line parsing. Each program demonstrates the following features.

* [test01](test01_hello_world.cpp): The very basics with `option_name`, `option_description`, `capture_default_str`, `app_name`, and `app_description`.
* [test11](test11_datatypes.cpp): Various data types that can be used as options and flags. `ignore_case`, `ignore_underscore`, `expected`, `delimiter`, `allow_extra_args`, `ignore`, and positional options.
* [test13](test13_tuples.cpp): `std::tuple` can be used for command-line options. Default values and `std::optional` make parameters optional.
* [test15](test15_environment.cpp): Set an option from an environment variable with `envname`.
* [test17](test17_flags.cpp): Use booleans as flags or options, and integers as flags. Also
  demonstrates `multi_option_policy` and `option_name("-1{1},-2{2},-3{3}")`.
* [test19](test19_inheritance.cpp): Compose options using inheritance.
* [test21](test21_needs_excludes.cpp): Set option dependencies with `needs` and `excludes`.
* [test31](test31_groups.cpp): Use `group` to organize options in the help output and `group("")` to hide an option.
* [test41](test41_options_groups.cpp): The basics of option groups via `add_option_group`.
* [test44](test44_options_groups_relationships.cpp): Use option groups in combination with `require_option`
  to ensure the right number of options is provided.
* [test46](test46_options_groups_ranges.cpp): Shows how `expected`, `require_option`, and `params(required)`
  can be used to allow choosing between two different ways for specifying a range.
* [test51](test51_subcommands.cpp): The basics of subcommands via `add_subcommand`; also demonstrates `set_help_all_flag`.
* [test52](test52_subcommands_option_groups.cpp): Combination of subcommands and option groups.
* [test54](test54_hidden_subcommand.cpp): Two subcommands via `add_subcommand` plus a hidden subcommand via `group("")`.
* [test56](test56_multi_commands.cpp): Specify multiple subcommands in sequence with `require_subcommand(1, 0)`, `fallthrough(true)`. The subcommands have positional options.
