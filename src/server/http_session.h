#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "../comm/def.h"
#include "http_handler_mgr.h"
#include "log.h"

FT_SERVER_NSP_START

class http_session final : public std::enable_shared_from_this<http_session> {
public:
    using context_t = boost::asio::io_context;
    using error_code_t = boost::beast::error_code;
    using tcp_socket_t = boost::asio::ip::tcp::socket;
    using tcp_stream_t = boost::beast::tcp_stream;
    using buffer_t = boost::beast::flat_buffer;
    using http_response_t = boost::beast::http::message_generator;
    using http_request_t = boost::beast::http::request<boost::beast::http::string_body>;
    
    template <typename T>
    using request_parser_t = boost::beast::http::request_parser<T>;

    template <typename T>
    using parser_t = std::optional<request_parser_t<T>>;

    using parser_header_t = parser_t<boost::beast::http::empty_body>;
    using parser_string_request_t = parser_t<boost::beast::http::string_body>;
    using parser_file_body_t = parser_t<boost::beast::http::file_body>;
    using parser_dynamic_body_t = parser_t<boost::beast::http::dynamic_body>;

    http_session(tcp_socket_t&& sock, const std::weak_ptr<http_handler_mgr>& mgr)
        : m_stream(std::move(sock)), m_mgr(mgr) {}
    virtual ~http_session() = default;

    void run();

private:
    void pre_config();

    void do_read_header();
    void on_read_header(error_code_t ec, std::size_t bytes_transferred);

    template <typename T>
    void do_read_body(parser_t<T>& parser)
    {
        boost::beast::http::async_read(
            m_stream, m_buffer, *(get_parser<T>()),
            boost::beast::bind_front_handler(&http_session::on_read_body<T>, shared_from_this())
        );
    }

    template <typename T>
    void on_read_body(error_code_t ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec) {
            LOG_TRACE() << ec.message();
            return do_close();
        }
        
        auto sp = m_mgr.lock();
        try {
            auto req = get_parser<T>()->release();
            send_response(sp->handle(std::move(req)));
        }
        catch (boost::system::system_error& e) {
            LOG_ERROR() << e.what();
        }
    }

    template <typename T>
    parser_t<T>& get_parser() {
        if constexpr (std::is_same_v<T, boost::beast::http::empty_body>) {
            return m_parser_header;
        }
        else if constexpr (std::is_same_v<T, boost::beast::http::string_body>) {
            return m_parser_string_request;
        }
        else if constexpr (std::is_same_v<T, boost::beast::http::file_body>) {
            return m_parser_file_body;
        }
        else if constexpr (std::is_same_v<T, boost::beast::http::dynamic_body>) {
            return m_parser_dynamic_body;
        }
        else {
            // static_assert(false);
            return std::nullopt;
        }
    }

    void send_response(http_response_t&& response);
    void on_write(bool keep_alive, error_code_t ec, std::size_t bytes_transferred);
    void do_close();

private:
    tcp_stream_t m_stream;
    buffer_t m_buffer;
    std::weak_ptr<http_handler_mgr> m_mgr;
    parser_header_t m_parser_header;
    parser_string_request_t m_parser_string_request;
    parser_file_body_t m_parser_file_body;
    parser_dynamic_body_t m_parser_dynamic_body;
};

FT_SERVER_NSP_END
