cmake_minimum_required(VERSION 3.20)

# 启用 vcpkg 清单模式
set(VCPKG_MANIFEST_MODE ON)
set(VCPKG_MANIFEST_INSTALL ON)

project(ets2-telemetry-plugin VERSION 1.0.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows 特定设置
if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 查找依赖包
find_package(httplib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# 添加源文件
set(SOURCES
    ets2-telemetry/src/ets2-telemetry.cpp
    ets2-telemetry/src/sharedmemory.cpp
    ets2-telemetry/src/scs_config_handlers.cpp
    ets2-telemetry/src/web_api.cpp
)

# 添加头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}/ets2-telemetry/inc
    ${PROJECT_SOURCE_DIR}/scs_sdk_1_5
)

# 创建动态库目标
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# 链接依赖库
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    httplib::httplib
    nlohmann_json::nlohmann_json
)

# Windows 特定设置
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        WIN32_LEAN_AND_MEAN 
        NOMINMAX
        _WIN32_WINNT=0x0601
    )
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 复制配置文件
install(FILES 
    ${PROJECT_SOURCE_DIR}/ets2-telemetry/config.json
    DESTINATION bin
) 