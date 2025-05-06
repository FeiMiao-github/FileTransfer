#pragma once

#include <stdexcept>
#include <string>
#include "../../comm/def.h"

FT_SERVER_NSP_START

class http_base_exception : public std::runtime_error {
public:
    explicit http_base_exception(const std::string& msg) noexcept;
    virtual ~http_base_exception() noexcept = default;
};

class http_not_implemented: public http_base_exception
{
public:
    explicit http_not_implemented(const std::string& msg) noexcept;
};

class http_invalid_url: public http_base_exception
{
public:
    explicit http_invalid_url(const std::string& msg) noexcept;
};

class http_not_found: public http_base_exception
{
public:
    explicit http_not_found(const std::string& msg) noexcept;
};

class http_internal_error: public http_base_exception
{
public:
    explicit http_internal_error(const std::string& msg) noexcept;
};

FT_SERVER_NSP_END
