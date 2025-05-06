#include "http_base_exception.h"

using namespace ft::server;

http_base_exception::http_base_exception(const std::string& msg) noexcept
    : std::runtime_error(msg) {
}

http_not_implemented::http_not_implemented(const std::string& msg) noexcept
    : http_base_exception(msg) {
}

http_invalid_url::http_invalid_url(const std::string& msg) noexcept
    : http_base_exception(msg)
{
}

http_internal_error::http_internal_error(const std::string& msg) noexcept
    : http_base_exception(msg)
{
}

http_not_found::http_not_found(const std::string& msg) noexcept
    : http_base_exception(msg)
{
}
