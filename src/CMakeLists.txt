add_executable(horker
    main.cpp
    config.cpp
    process.cpp
    helper.cpp
    engine.cpp
    features/faim.cpp
    features/fbase.cpp
    features/fglow.cpp
    features/fvisual.cpp
    sdk/cbaseentity.cpp
    sdk/centitylist.cpp
)

target_link_libraries(horker
    ${X11_LIBRARIES}
    Threads::Threads
)

set_property(TARGET horker PROPERTY CXX_STANDARD 11)

