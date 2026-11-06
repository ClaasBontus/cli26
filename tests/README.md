# CLI26 test code

The example programs in this folder do nothing but print the results of the command line parsing. Methods used in
the programs are summarized in the following list.

* [test01](test01_hello_world.cpp): The very basics with `option_name`, `option_description`, `capture_default_str`, `app_name`, and `app_description`.
* [test11](test11_datatypes.cpp): Various data types that can be uses as options and flags. `ignore_case`, `ignore_underscore`, `expected`, `delimiter`, `allow_extra_args`, `ignore`, and positional options.
* [test13](test13_tuples.cpp): `std::tuple`'s can be used for command line options. Assignment of a default
  and `std::optional` are used for making parameters optional.
* [test15](test15_environment.cpp): Uses `envname` for setting an option via environment variable.
* [test17](test17_flags.cpp): Use booleans as flag or option, use integers as flag. `multi_option_policy`, and
  `option_name("-1{1},-2{2},-3{3}")`.
* [test21](test21_needs_excludes.cpp): Set dependencies between options via `needs` and `excludes`.
* [test31](test31_groups.cpp): Use `group` to sort options in the help output, use `group("")` for hiding an option.
* [test41](test41_options_groups.cpp): The basics of option groups via `add_option_group`.
* [test44](test44_options_groups_relationships.cpp): Use option groups in combination with `require_option`
  for making sure the right number of options is submitted.
* [test46](test46_options_groups_ranges.cpp): Provides two different ways for specifying a range. `expected`,
  `require_option`, and `params(required)`.
* [test51](test51_subcommands.cpp): The basics of subcommands via `add_subcommand`. Uses `set_help_all_flag`.
* [test52](test52_subcommands_option_groups.cpp): Combination of subcommands and option groups.
* [test54](test54_hidden_subcommand.cpp): Two subcommands via `add_subcommand` plus a hidden subcommand via `group("")`.
* [test56](test56_multi_commands.cpp): Shows how various subcommands can be specified in sequence. `require_subcommand(1, 0)`, `fallthrough(true)`, positional options.
