set(base_SRCS
    Config.cpp
    FileManager.cpp
    Logger.cpp
    )

add_library(base ${base_SRCS})
target_link_libraries(base jsoncpp)

install(TARGETS base DESTINATION lib)
