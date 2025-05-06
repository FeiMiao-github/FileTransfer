#include "../../comm/def.h"
#include <map>
#include <string_view>

FT_SERVER_NSP_START

class formdata_parser {
public:
    struct section_t {
        std::string_view::size_type begin;
        std::string_view::size_type end;
    };

    static constexpr std::string_view CONTENT_DISPOSITION_TEXT = "Content-Disposition: form-data; ";
    static constexpr std::string_view NAME_TEXT_START = "name=\"";
    static constexpr std::string_view NAME_TEXT_END = "\"";

    static constexpr std::string_view CRLF_CRLF = "\r\n\r\n";

    std::map<std::string, section_t> parse(const std::string_view body, const std::string_view boundary) const;
};

FT_CLIENT_NSP_END