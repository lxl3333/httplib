#include <gtest/gtest.h>
#include <sstream>
#include "../src/base/Logger.h"

TEST(LoggerTest, DebugLog) {
    std::stringstream log_stream;
    Logger logger(log_stream);

    logger.Debug("Debug message");

    EXPECT_TRUE(log_stream.str().find("[DEBUG] Debug message") != std::string::npos);
}

TEST(LoggerTest, InfoLog) {
    std::stringstream log_stream;
    Logger logger(log_stream);

    logger.Info("Info message");

    EXPECT_TRUE(log_stream.str().find("[INFO] Info message") != std::string::npos);
}

TEST(LoggerTest, WarningLog) {
    std::stringstream log_stream;
    Logger logger(log_stream);

    logger.Warning("Warning message");

    EXPECT_TRUE(log_stream.str().find("[WARNING] Warning message") != std::string::npos);
}

TEST(LoggerTest, ErrorLog) {
    std::stringstream log_stream;
    Logger logger(log_stream);

    logger.Error("Error message");

    EXPECT_TRUE(log_stream.str().find("[ERROR] Error message") != std::string::npos);
}

TEST(LoggerTest, UnknownLogLevel) {
    std::stringstream log_stream;
    Logger logger(log_stream);

    logger.Log(static_cast<Logger::LogLevel>(42), "Unknown message");

    EXPECT_TRUE(log_stream.str().find("[UNKNOWN] Unknown message") != std::string::npos);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
