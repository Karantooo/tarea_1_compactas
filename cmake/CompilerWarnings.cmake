function(target_enable_strict_warnings target_name)
    if(MSVC)
        target_compile_options(${target_name} INTERFACE /W4 /permissive-)
    else()
        target_compile_options(${target_name} INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wfloat-equal
        )
    endif()
endfunction()
