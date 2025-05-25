#include "../http_utils.h"
#include "../http_exception.h"
#include "http_file_response_factory.h"
#include "../log.h"

using namespace ft::server;

 http_file_response_factory::response_t http_file_response_factory::operator()(request_t& req, const boost::filesystem::path& path, type tp) {
    response_t res;
    boost::beast::error_code ec;
    file_body::value_type body;
    // auto cwd = boost::filesystem::current_path();
    LOG_INFO() << "path: " << path.string() << std::endl;
    // auto res_base_path = cwd / "res";
    boost::filesystem::path res_base_path;    
    boost::filesystem::path res_path;

    switch (tp)
    {
        case type::FILE:
        {
            res_path = path;
        } break;
        case type::DIR:
        default:
        {
            auto cwd = path;
            res_base_path = cwd;
            res_path = boost::filesystem::weakly_canonical(res_base_path / std::string(req.target()));
        } break;
    }

    // 判断文件是否存在
    if (!res_path.string().starts_with(res_base_path.c_str())) {
        LOG_WARNING() << res_path << " does not exist";
        throw http_invalid_url(std::string(req.target()).append(" not found."));
    }

    if (req.target().back() == '/') {
        res_path.append("index.html");
    }

    body.open(res_path.c_str(), boost::beast::file_mode::scan, ec);
    auto const size = body.size();

    if (ec == boost::system::errc::no_such_file_or_directory) {
        LOG_ERROR() << res_path << " does not exist";
        std::string msg = std::string(res_path.string())
                              .append(" ")
                              .append(std::to_string(static_cast<int>(req.method())))
                              .append(" not found.");
        throw http_invalid_url(msg);
    }

    res.result(status::ok);
    res.version(req.version());
    res.body() = std::move(body);
    res.set(field::server, ("FT_Server"));
    res.set(field::content_type, mime_type(res_path.c_str()));
    res.content_length(size);
    res.keep_alive(req.keep_alive());

#if defined(FT_SERVER_ENABLE_CROS)
    res.set(field::access_control_allow_origin, "*");
#endif

    return res;
}
