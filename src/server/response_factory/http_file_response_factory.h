#pragma once

#include "../../comm/def.h"
#include "http_response_base_factory.h"
#include <boost/filesystem.hpp>

FT_SERVER_NSP_START

// todo：1. 添加不同的挂载点
class http_file_response_factory
    : public http_response_base_factory_imp<http_response_base_factory::string_body, http_response_base_factory::file_body> {
public:
    response_t operator()(request_t& req);
};

FT_SERVER_NSP_END
