set(
    server_SRCS
    FileTransmission.cpp
    FileManageRequest.cpp
    LoginManager.cpp
    HandlerFactory.cpp
    FtpServer.cpp
)

add_library(server ${server_SRCS})
target_link_libraries(server base crypto)
