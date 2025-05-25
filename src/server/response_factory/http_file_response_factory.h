#pragma once

#include "../../comm/def.h"
#include "http_response_base_factory.h"
#include <boost/filesystem.hpp>

FT_SERVER_NSP_START

class http_file_response_factory
    : public http_response_base_factory_imp<http_response_base_factory::string_body, http_response_base_factory::file_body> {
public:
    enum class type {
        FILE,
        DIR
    };

    response_t operator()(request_t& req, const boost::filesystem::path& base_dir, type tp = type::DIR);
};

FT_SERVER_NSP_END
