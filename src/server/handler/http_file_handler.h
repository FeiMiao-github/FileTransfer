#pragma once

#include "../../comm/def.h"
#include "../http_exception.h"
#include "http_custom_handler.h"
#include "../http_response_factory.h"
#include <boost/filesystem.hpp>

FT_SERVER_NSP_START

class http_file_handler : public http_base_handler {
public:
    http_file_handler()
        : m_file_path(boost::filesystem::current_path()) {}

    http_msg_genearator handle_request(request_t<string_body>& req) const override;

    void set_file_path(const boost::filesystem::path& file_path);

private:
    boost::filesystem::path m_file_path;
};

FT_SERVER_NSP_END
