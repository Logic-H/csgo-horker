add_executable(horker
    main.cpp
    memorymanager.cpp
    interfaces/iengine.cpp
    interfaces/iclient.cpp
)

target_link_libraries(horker
    Threads::Threads
)

set_property(TARGET csgo-dumpy PROPERTY CXX_STANDARD 11)

