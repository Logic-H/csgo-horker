add_executable(horker
        main.cpp
        memorymanager.cpp
        interfaces/iclient.cpp
        features/fglow.cpp
        features/fvisual.cpp
)

target_link_libraries(horker
        Threads::Threads
        )

set_property(TARGET horker PROPERTY CXX_STANDARD 11)

