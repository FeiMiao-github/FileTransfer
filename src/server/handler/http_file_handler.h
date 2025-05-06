#pragma once

#include "../../comm/def.h"
#include "../http_exception.h"
#include "http_custom_handler.h"
#include "../http_response_factory.h"

FT_SERVER_NSP_START

class http_file_handler : public http_base_handler {
public:
    http_msg_genearator handle_request(request_t<string_body>& req) const override {
        return http_file_response_factory()(req);
    }
};

FT_SERVER_NSP_END
