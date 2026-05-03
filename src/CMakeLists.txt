file(GLOB CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
)

add_library(core ${CORE_SOURCES})

target_include_directories(core PUBLIC
    ${PROJECT_SOURCE_DIR}/include/core
)

target_link_libraries(core PUBLIC
    project_options
    project_warnings
)