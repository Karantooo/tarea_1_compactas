function(target_enable_build_type_options target_name)
    if(MSVC)
        target_compile_options(${target_name} INTERFACE
            $<$<CONFIG:Debug>:/Zi>
            $<$<CONFIG:Release>:/O2>
        )
    else()
        target_compile_options(${target_name} INTERFACE
            $<$<CONFIG:Debug>:-O0;-g3;-fno-omit-frame-pointer>
            $<$<CONFIG:Release>:-O3;-march=native;-mtune=native>
        )
    endif()
endfunction()
