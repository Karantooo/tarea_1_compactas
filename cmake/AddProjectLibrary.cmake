function(add_project_library lib_name)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES LIBRARIES)
    cmake_parse_arguments(LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT LIB_SOURCES)
        message(FATAL_ERROR "add_project_library(${lib_name}) requires SOURCES")
    endif()

    add_library(${lib_name} ${LIB_SOURCES})

    target_include_directories(${lib_name}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(${lib_name}
        PUBLIC
            project_warnings
            project_options
            ${LIB_LIBRARIES}
    )
endfunction()
