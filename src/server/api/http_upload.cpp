#include "http_upload.h"
#include "../http_response_factory.h"
#include "../log.h"
#include <fstream>
#include <map>
#include "formdata_parser.h"

static constexpr std::string SECTION_FILE = "file";
static constexpr std::string SECTION_FILEID = "fileId";
static constexpr std::string SECTION_CHUNKINDEX = "chunkIndex";
static constexpr std::string SECTION_TOTALCHUNKS = "totalChunks";
static constexpr std::string SECTION_FILENAME = "filename";

static constexpr std::string REQUIRED_SECTIONS[] = {SECTION_FILE, SECTION_FILEID, SECTION_CHUNKINDEX, SECTION_TOTALCHUNKS, SECTION_FILENAME};

ft::server::http_base_handler::http_msg_genearator ft::server::http_upload_handler::handle_request(
    request_t<string_body>& req) const {
    auto url = boost::urls::url_view(req.target());
    auto params = url.params();
    auto dir = params.find("path");

    LOG_INFO() << "content-type: " << req["Content-Type"];
    // LOG_INFO() << "upload\n" << req.body();
    // 处理 multipart 数据

    // 需要去掉前后空白和引号
    // boundary.erase(std::remove(boundary.begin(), boundary.end(), '\"'), boundary.end());

    // std::ofstream file("hello_world.txt", std::ios::out | std::ios::trunc);
    // file << req.body();
    // file.close();

    process_multipart(req);

    boost::json::object data = {{"code", 0}, {"msg", "ok"}};
    http_json_response_factory json_response_creator;
    return json_response_creator(req, data);
}

void ft::server::http_upload_handler::process_multipart(const request_t<string_body>& req) const {
    std::string boundary = req["Content-Type"].substr(req["Content-Type"].find("boundary=") + 9);
    boundary = "--" + boundary;

    formdata_parser parser;
    auto formdata_secs = parser.parse(req.body(), boundary);

    std::string_view body = req.body();

    for (auto & req_sec: REQUIRED_SECTIONS) {
        if (!formdata_secs.contains(std::string(req_sec))) {
            throw http_internal_error(std::string("required section ") + std::string(req_sec) + " not found.");
        }
    }

    auto & filename_sec = formdata_secs[SECTION_FILENAME];
    std::string_view filename = body.substr(filename_sec.begin, filename_sec.end - filename_sec.begin);
    LOG_INFO() << "filename: " << filename;

    auto& file_sec = formdata_secs[SECTION_FILE];
    std::ofstream ofs(std::string(filename), std::ios::binary);
    ofs << body.substr(file_sec.begin, file_sec.end - file_sec.begin);
    ofs.close();
}
