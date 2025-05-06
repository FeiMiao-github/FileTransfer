#include "http_session.h"
#include "http_exception.h"

using namespace std::chrono_literals;
using namespace ft::server;

void http_session::run() {
    pre_config();

    boost::asio::dispatch(m_stream.get_executor(),
                          boost::beast::bind_front_handler(
                              static_cast<void (http_session::*)()>(&http_session::do_read_header), shared_from_this()));
}

void http_session::pre_config() {
    get_parser<boost::beast::http::empty_body>()->body_limit(8 * 1024 * 1024);
    get_parser<boost::beast::http::string_body>()->body_limit(8 * 1024 * 1024);
    get_parser<boost::beast::http::file_body>()->body_limit(8 * 1024 * 1024);
    get_parser<boost::beast::http::dynamic_body>()->body_limit(8 * 1024 * 1024);
}

void http_session::do_read_header() {
    LOG_INFO() << "do_read_header";
    get_parser<boost::beast::http::empty_body>().emplace();
    get_parser<boost::beast::http::empty_body>()->body_limit(8 * 1024 * 1024);
    m_stream.expires_after(std::chrono::seconds(30));

    boost::beast::http::async_read_header(
        m_stream, m_buffer, *get_parser<boost::beast::http::empty_body>(), 
        boost::beast::bind_front_handler(&http_session::on_read_header, shared_from_this()));
}

void http_session::on_read_header(error_code_t ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    LOG_INFO() << get_parser<boost::beast::http::empty_body>()->get().method_string() << " "
        << get_parser<boost::beast::http::empty_body>()->get().target() << " " << ec.message();
    if (ec)
    {
        return do_close();
    }

    switch (m_parser_header->get().method()) {
        case boost::beast::http::verb::post:
            {
                LOG_INFO() << "content-type: " << m_parser_header->get()[boost::beast::http::field::content_type];
                if (m_parser_header->get()[boost::beast::http::field::content_type].find("multipart/form-data") != std::string::npos) {
                    get_parser<boost::beast::http::string_body>().emplace(std::move(*m_parser_header));
                    do_read_body(get_parser<boost::beast::http::string_body>());
                }
                else
                {
                    get_parser<boost::beast::http::dynamic_body>().emplace(std::move(*m_parser_header));
                    do_read_body(get_parser<boost::beast::http::dynamic_body>());
                }
            }
            break;
        case boost::beast::http::verb::options:
            {
                auto sp = m_mgr.lock();
                auto req = get_parser<boost::beast::http::empty_body>()->release();
                send_response(sp->handle_options(std::move(req)));
            }
            break;
        default:
            get_parser<boost::beast::http::string_body>().emplace(std::move(*m_parser_header));
            do_read_body(get_parser<boost::beast::http::string_body>());
            break;
    }
}

void http_session::send_response(http_response_t&& response) {
    bool keep_alive = response.keep_alive();

    boost::beast::async_write(
        m_stream, std::forward<http_response_t>(response),
        boost::beast::bind_front_handler(&http_session::on_write,
                                         shared_from_this(), keep_alive));
}

void http_session::on_write(bool keep_alive, error_code_t ec,
                            std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        throw http_base_exception(ec.message());
    }

    if (!keep_alive) {
        do_close();
    } else {
        do_read_header();
    }
}

void http_session::do_close() {
    error_code_t ec;
    if (ec)
        LOG_WARNING() << "do_close: " <<m_stream.socket().shutdown(tcp_socket_t::shutdown_send, ec);
}