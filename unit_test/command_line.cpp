#include "command_line.h"

#include <boost/test/unit_test.hpp>

#include <array>

BOOST_AUTO_TEST_SUITE(server_command_line)

BOOST_AUTO_TEST_CASE(it_throws_when_no_args) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver"};
    BOOST_CHECK_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                        const_cast<char**>(argv)),
                      std::exception);
}

BOOST_AUTO_TEST_CASE(it_throws_when_no_port) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0"};
    BOOST_CHECK_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                        const_cast<char**>(argv)),
                      std::exception);
}

BOOST_AUTO_TEST_CASE(it_throws_when_no_port_with_flag) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "--force-start", "0.0.0.0"};
    BOOST_CHECK_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                        const_cast<char**>(argv)),
                      std::exception);
}

BOOST_AUTO_TEST_CASE(it_throws_unknown_arg) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80", "--covfefe"};
    BOOST_CHECK_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                        const_cast<char**>(argv)),
                      std::exception);
}

BOOST_AUTO_TEST_CASE(it_parses_help) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "--help"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.print_help, true);
}

BOOST_AUTO_TEST_CASE(it_parses_version) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "--version"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.print_version, true);
}

BOOST_AUTO_TEST_CASE(it_parses_force_start) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80", "--force-start"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.force_start, true);
}

BOOST_AUTO_TEST_CASE(it_parses_ip_and_port) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.ip, "0.0.0.0");
    BOOST_CHECK_EQUAL(options.port, 80);
}

BOOST_AUTO_TEST_CASE(it_throw_with_invalid_port) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0",
                          "8O"}; // notice the O instead of 0
    BOOST_CHECK_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                        const_cast<char**>(argv)),
                      std::exception);
}

BOOST_AUTO_TEST_CASE(it_parses_arqmad_rpc_port) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80", "--arqmad-rpc-port",
                          "12345"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.arqmad_rpc_port, 12345);
}

BOOST_AUTO_TEST_CASE(it_parses_arqmad_key) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80", "--arqmad-key",
                          "foobar"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.arqmad_key_path, "foobar");
}

BOOST_AUTO_TEST_CASE(it_parses_data_dir) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80", "--data-dir",
                          "foobar"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.data_dir, "foobar");
}

BOOST_AUTO_TEST_CASE(it_returns_default_data_dir) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.data_dir, "");
}

BOOST_AUTO_TEST_CASE(it_parses_log_levels) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80", "--log-level",
                          "foobar"};
    BOOST_CHECK_NO_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                           const_cast<char**>(argv)));
    const auto options = parser.get_options();
    BOOST_CHECK_EQUAL(options.log_level, "foobar");
}

BOOST_AUTO_TEST_CASE(it_throws_with_config_file_not_found) {
    arqma::command_line_parser parser;
    const char* argv[] = {"httpserver", "0.0.0.0", "80", "--config-file",
                          "foobar"};
    BOOST_CHECK_THROW(parser.parse_args(sizeof(argv) / sizeof(char*),
                                        const_cast<char**>(argv)),
                      std::exception);
}

BOOST_AUTO_TEST_SUITE_END()
