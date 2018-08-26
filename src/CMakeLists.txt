if (NVCtrlLib_FOUND)
    set (USE_NVIDIA 1)
endif()

configure_file(defines.h.in defines.h @ONLY)

add_executable(horker
    features/faim.cpp
    features/fbase.cpp
    features/fglow.cpp
    features/fvisual.cpp
    sdk/centitylist.cpp
    hwctrl.cpp
    helper.cpp
    config.cpp
    engine.cpp
    main.cpp
    offsets.cpp
    process.cpp
)


target_link_libraries(horker
    ${X11_LIBRARIES}
    ${X11_XTest_LIB}
    ${NVCtrlLib_LIBRARY}
    Threads::Threads
)

target_include_directories(horker PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${NVCtrlLib_INCLUDE_DIR}
)
set_property(TARGET horker PROPERTY CXX_STANDARD 11)

