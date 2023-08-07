set(
    server_SRCS
    FtpServer.cpp
)

add_library(server ${server_SRCS})
target_link_libraries(server base)
