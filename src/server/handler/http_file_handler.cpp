#include "http_file_handler.h"
#include "../log.h"

using namespace ft::server;

http_file_handler::http_msg_genearator http_file_handler::handle_request(request_t<string_body>& req) const {
    auto ret = http_file_response_factory();
    LOG_INFO() << "filepath: " << m_file_path;
    return ret(req, m_file_path, http_file_response_factory::type::FILE);
}
void http_file_handler::set_file_path(const boost::filesystem::path& file_path) {
    m_file_path = file_path;
}
