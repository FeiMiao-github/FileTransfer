#pragma once

#include <boost/json.hpp>
#include "../../comm/def.h"
#include "http_response_base_factory.h"

FT_SERVER_NSP_START

class http_json_response_factory
    : public http_response_base_factory_imp<http_response_base_factory::string_body, http_response_base_factory::string_body> {
public:
    using json_value_t = boost::json::value;

    response_t operator()(request_t& req, const json_value_t& json_value);
};

FT_SERVER_NSP_END
