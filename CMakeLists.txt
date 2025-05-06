cmake_minimum_required(VERSION 3.16)
project(FileTransfer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置 Boost 配置
set(Boost_USE_STATIC_LIBS ON)

# 设置Debug标志
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_BUILD_TYPE Debug)

set(CLIENT_EXE_NAME FT_Client)
set(SERVER_EXE_NAME FT_Server)

# enable cros
add_compile_definitions(FT_SERVER_ENABLE_CROS)

# 选项开关
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_GUI "Enable GUI support" OFF)

# 复制资源文件
add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
    COMMENT "Copying resources..."
    VERBATIM)

# 查找 Boost 库（如果需要）
find_package(Boost
REQUIRED
COMPONENTS
    program_options system filesystem log log_setup 
    # json
    url
)

aux_source_directory(src/client CLIENT_SOURCES)
aux_source_directory(src/server SERVER_SOURCES)
aux_source_directory(src/server/api SERVER_SOURCES)
aux_source_directory(src/server/exceptions SERVER_SOURCES)
aux_source_directory(src/server/handler SERVER_SOURCES)
aux_source_directory(src/server/response_factory SERVER_SOURCES)
aux_source_directory(src/comm COMMON_SOURCES)

# 添加可执行文件
add_executable(${SERVER_EXE_NAME} ${SERVER_SOURCES} ${COMMON_SOURCES})
add_executable(${CLIENT_EXE_NAME} ${CLIENT_SOURCES} ${COMMON_SOURCES})

# 链接 Boost 库
target_link_libraries(${SERVER_EXE_NAME}
    PRIVATE Boost::program_options Boost::filesystem Boost::log Boost::log_setup 
    # Boost::json
    Boost::url
)
target_link_libraries(${CLIENT_EXE_NAME}
    PRIVATE  Boost::program_options Boost::filesystem Boost::log Boost::log_setup
    # Boost::json
    Boost::url
)

# 添加资源依赖
add_dependencies(${SERVER_EXE_NAME} copy_resources)
add_dependencies(${CLIENT_EXE_NAME} copy_resources)

# 安装规则
set(CMAKE_INSTALL_PREFIX .)
install(TARGETS ${CLIENT_EXE_NAME} ${SERVER_EXE_NAME} DESTINATION .)
install(DIRECTORY ${CMAKE_BINARY_DIR}/res DESTINATION .)
