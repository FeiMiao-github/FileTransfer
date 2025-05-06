#pragma once

#include "../../comm/def.h"
#include "../http_exception.h"
#include "http_custom_handler.h"
#include "../http_response_factory.h"

FT_SERVER_NSP_START

class http_options_handler : public http_base_handler {
public:
    http_msg_genearator handle_request(request_t<empty_body>& req) const override {
        return http_options_response_factory()(req);
    }
};

FT_SERVER_NSP_END