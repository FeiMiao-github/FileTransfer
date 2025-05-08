#include "http_upload.h"
#include "../http_response_factory.h"
#include "../log.h"
#include <fstream>
#include <map>
#include "formdata_parser.h"
#include "../global_config.h"

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
    auto filename = body.substr(filename_sec.begin, filename_sec.end - filename_sec.begin);
    // std::string_view filename = body.substr(filename_sec.begin, filename_sec.end - filename_sec.begin);
    // LOG_INFO() << "filename: " << filename;

    auto& chunk_index_sec = formdata_secs[SECTION_CHUNKINDEX];
    auto& total_chunks_sec = formdata_secs[SECTION_TOTALCHUNKS];
    auto chunk_index = std::stoul(std::string(body.substr(chunk_index_sec.begin, chunk_index_sec.end - chunk_index_sec.begin)));
    auto total_chunks = std::stoul(std::string(body.substr(total_chunks_sec.begin, total_chunks_sec.end - total_chunks_sec.begin)));
    LOG_INFO() << "chunk index: " << chunk_index << ", total chunks: " << total_chunks;

    auto fileid_sec = formdata_secs[SECTION_FILEID];
    auto fileid = std::string(body.substr(fileid_sec.begin, fileid_sec.end - fileid_sec.begin));
    LOG_INFO() << "fileid: " << fileid;

    auto uploaddir = boost::filesystem::path(global_config::instance().get_upload_dir());
    if (!boost::filesystem::exists(uploaddir))
    {
        throw http_internal_error(std::string("upload dir ") + uploaddir.string() + " not found.");
    }

    // 检查fileid目录是否存在
    auto file_dir = uploaddir / fileid;
    if (!boost::filesystem::exists(file_dir))
    {
        boost::filesystem::create_directory(file_dir);
    }

    auto file_path = file_dir / (std::to_string(chunk_index) + "_" + std::to_string(total_chunks) + ".part");

    auto& file_sec = formdata_secs[SECTION_FILE];
    std::ofstream ofs(file_path.c_str(), std::ios::binary);
    ofs << body.substr(file_sec.begin, file_sec.end - file_sec.begin);
    ofs.close();

    // 计算file_path下的.part文件个数
    int count = 0;
    for (auto & entry : boost::filesystem::directory_iterator(file_dir)) {
        if (entry.path().extension() == ".part") {
            count++;
        }
    }

    if (count == total_chunks) {
        // 合并文件
        std::ofstream ofs(uploaddir / filename, std::ios::binary);
        for (int i = 0; i < total_chunks; i++) {
            auto part_file = file_dir / (std::to_string(i) + "_" + std::to_string(total_chunks) + ".part");
            std::ifstream ifs(part_file.c_str(), std::ios::binary);
            ofs << ifs.rdbuf();
            ifs.close();
            boost::filesystem::remove(part_file);
        }
        boost::filesystem::remove(file_dir);
        ofs.close();
    }
}
