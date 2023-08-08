add_executable(ConfigTest ConfigTest.cpp)
target_link_libraries(ConfigTest base gtest pthread)

add_executable(LoggerTest LoggerTest.cpp)
target_link_libraries(LoggerTest base gtest pthread)

add_executable(FileManagerTest FileManagerTest.cpp)
target_link_libraries(FileManagerTest base gtest pthread)

add_executable(FileTransmissionTest FileTransmissionTest.cpp)
target_link_libraries(FileTransmissionTest base server gtest pthread crypto)

add_executable(LoginManagerTest LoginManagerTest.cpp)
target_link_libraries(LoginManagerTest base server gtest pthread crypto)

add_executable(FileManageRequestTest FileManageRequestTest.cpp)
target_link_libraries(FileManageRequestTest base server gtest pthread crypto)
