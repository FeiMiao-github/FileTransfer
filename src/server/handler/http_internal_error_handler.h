#pragma once

#include "../../comm/def.h"
#include "../http_exception.h"
#include "http_custom_handler.h"
#include "../http_response_factory.h"

FT_SERVER_NSP_START

template <typename T>
class http_internal_error_handler : public http_base_handler {
public:
    http_msg_genearator handle_request(request_t<T>& req) const override {
        return http_server_error_response_factory<T>()(req);
    }
};

FT_SERVER_NSP_END
