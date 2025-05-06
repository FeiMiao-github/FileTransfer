#include "http_parser_exception.h"

using namespace ft::server;

http_formdata_parse_error::http_formdata_parse_error(const std::string& msg) noexcept
    : http_internal_error(msg) {
}