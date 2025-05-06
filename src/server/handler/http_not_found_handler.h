#pragma once

#include "../../comm/def.h"
#include "http_custom_handler.h"
#include "../http_response_factory.h"

FT_SERVER_NSP_START

template <typename T>
class http_not_found_handler : public http_base_handler {
public:
    virtual http_msg_genearator handle_request(request_t<T>& req) const override {
        return http_not_found_response_factory<T>()(req);
    }
};

FT_SERVER_NSP_END