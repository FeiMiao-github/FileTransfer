#pragma once

#include <boost/beast.hpp>
#include "../comm/def.h"

FT_SERVER_NSP_START

std::string_view mime_type(std::string_view path);

FT_SERVER_NSP_END
