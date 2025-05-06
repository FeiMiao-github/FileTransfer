#pragma once

#include "http_base_handler.h"
#include "../../comm/def.h"
#include "../http_exception.h"

FT_SERVER_NSP_START

class http_custom_handler : public http_base_handler {
public:
    using base_type = http_base_handler;
    template <typename T>
    using request_t = base_type::request_t<T>;
    template <typename T>
    using response_t = base_type::response_t<T>;

    using http_msg_generator = base_type::http_msg_genearator;

    template <typename T>
    using func_t = std::function<http_msg_generator(request_t<T>&&)>;

    http_custom_handler() = default;
    template <typename T>
    explicit http_custom_handler(func_t<T> func) { get_func<T>() = func; }
    virtual ~http_custom_handler() = default;

    template <typename T>
    void set_func(func_t<T>&& func) { get_func<T>() = std::forward<func_t>(func); }

    template <typename T>
    std::optional<func_t<T>>& get_func() { return get_func<T>(); }

    // template <typename T>
    // http_msg_genearator handle_request(request_t<T>& req) const override {
    //     if (!get_func<T>()) {
    //         throw http_internal_error("m_func is nullopt.");
    //     }
    //     return get_func<T>()(std::move(req));
    // }
    // http_msg_generator handle_request(request_t<string_body>& req) const override {
    //     if (!get) {
    //         throw http_internal_error("m_str_func is nullopt.");
    //     }
    //     return m_str_func(req);
    // }

#if defined (DEF_FUNC_HANDLE_REQUEST)
#error "DEF_FUNC_HANDLE_REQUEST is defined."
#else
    #define DEF_FUNC_HANDLE_REQUEST(T) \
    http_msg_generator handle_request(request_t<T>& req) const override { \
        if (!get_func<T>()) { \
            throw http_internal_error(#T " func is nullopt."); \
        } \
        return (*get_func<T>())(std::move(req)); \
    }

    DEF_FUNC_HANDLE_REQUEST(string_body);
    // DEF_FUNC_HANDLE_REQUEST(dynamic_body);
    // DEF_FUNC_HANDLE_REQUEST(file_body);
    // DEF_FUNC_HANDLE_REQUEST(empty_body);
    #undef DEF_FUNC_HANDLE_REQUEST
#endif // DEF_FUNC_HANDLE_REQUEST
    template <typename T>
    std::optional<func_t<T>> get_func() const { 
        if constexpr (std::is_same_v<T, boost::beast::http::empty_body>) {
            return m_empty_func;
        }
        else if constexpr (std::is_same_v<T, boost::beast::http::string_body>) {
            return m_str_func;
        }
        else if constexpr (std::is_same_v<T, boost::beast::http::file_body>) {
            return m_file_func;
        }
        else if constexpr (std::is_same_v<T, boost::beast::http::dynamic_body>) {
            return m_dyn_func;
        }
        else
        {
            throw http_internal_error("unknown type.");
        }
    }

private:
    std::optional<func_t<string_body>> m_str_func;
    std::optional<func_t<dynamic_body>> m_dyn_func;
    std::optional<func_t<file_body>> m_file_func;
    std::optional<func_t<empty_body>> m_empty_func;
};

FT_SERVER_NSP_END
