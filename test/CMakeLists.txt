add_executable(ConfigTest ConfigTest.cpp)
target_link_libraries(ConfigTest base gtest pthread)


add_executable(LoggerTest LoggerTest.cpp)
target_link_libraries(LoggerTest base gtest pthread)


add_executable(FileManagerTest FileManagerTest.cpp)
target_link_libraries(FileManagerTest base gtest pthread)




