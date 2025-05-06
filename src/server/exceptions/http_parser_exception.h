#pragma once

#include "http_base_exception.h"
#include "../../comm/def.h"

FT_SERVER_NSP_START

class http_formdata_parse_error: public http_internal_error
{
public:
    explicit http_formdata_parse_error(const std::string& msg) noexcept;
};

FT_SERVER_NSP_END