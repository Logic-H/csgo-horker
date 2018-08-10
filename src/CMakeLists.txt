add_executable(horker
    features/faim.cpp
    features/fbase.cpp
    features/fglow.cpp
    features/fvisual.cpp
    sdk/centitylist.cpp
    config.cpp
    engine.cpp
    main.cpp
    offsets.cpp
    process.cpp
)

target_link_libraries(horker
    ${X11_LIBRARIES}
    Threads::Threads
)

set_property(TARGET horker PROPERTY CXX_STANDARD 11)

