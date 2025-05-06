#include "formdata_parser.h"
#include "../log.h"
#include "../http_exception.h"

using namespace ft::server;

std::map<std::string, formdata_parser::section_t>  formdata_parser::parse(const std::string_view body, const std::string_view boundary) const
{
    std::map<std::string, section_t> sections;

    std::string_view::size_type sec_end = 0;
    std::string_view::size_type sec_begin;

    do {
        section_t sec;
        std::string name;
        std::string_view data;
        std::string_view::size_type pos;

        sec_begin = sec_end;
        sec_end = body.find(boundary, sec_begin + boundary.size());
        if (sec_end == std::string_view::npos) {
            break;
        }

        sec_begin += boundary.size();
        data = body.substr(sec_begin, sec_end - sec_begin);
        pos = data.find(CONTENT_DISPOSITION_TEXT.data());
        if (pos == std::string_view::npos) {
            throw http_formdata_parse_error("formdata format is invalid: cannot find 'Content-Disposition'.");
        }

        sec_begin += CONTENT_DISPOSITION_TEXT.size() + pos;
        data = body.substr(sec_begin, sec_end - sec_begin);
        pos = data.find(NAME_TEXT_START.data());
        // LOG_INFO() << body.substr(sec_begin, 100);

        if (pos == std::string_view::npos) {
            throw http_formdata_parse_error("formdata format is invalid: cannot find 'name_text_start'.");
        }

        // std::ofstream fs("test.txt", std::ios::out | std::ios::binary);
        // fs << std::string_view(body.begin() + sec_begin, sec_end - sec_begin);
        // fs.close();

        sec_begin += NAME_TEXT_START.size() + pos;
        data = body.substr(sec_begin, sec_end - sec_begin);
        pos = data.find(NAME_TEXT_END.data());
        if (sec_begin == std::string_view::npos) {
            throw http_formdata_parse_error("formdata format is invalid: cannot find 'name_text_end'.");
        }

        name = data.substr(0, pos);
        LOG_INFO() << "name is: " << name;

        sec_begin += NAME_TEXT_END.size() + pos;
        data = body.substr(sec_begin, sec_end - sec_begin);
        pos = data.find(CRLF_CRLF.data());
        if (sec_begin == std::string_view::npos) {
            throw http_formdata_parse_error("formdata format is invalid: cannot find 'CRLF_CRLF'.");
        }

        sec_begin += CRLF_CRLF.size() + pos;
        sec.begin = sec_begin;
        sec.end = sec_end - 2;
        sections[name] = sec;
    } while (true);

    // for (auto& [k, v] : sections) {
    //     LOG_INFO() << k << " " << v.begin << " " << v.end;
    // }

    return sections;
}
