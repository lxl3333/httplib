#include "../src/base/Config.h"
#include <gtest/gtest.h>

TEST(ConfigTest, TestConstructor) {
    Config config("test_config.json");
    EXPECT_EQ(config.address(), "localhost:8080");
    EXPECT_EQ(config.run_user(), "nobody");
    EXPECT_EQ(config.concurrency_limit(), 5);
    EXPECT_EQ(config.keepalive_limit(), 10);
    auto handlers = config.handlers();
    ASSERT_EQ(handlers.size(), 2);
    EXPECT_EQ(handlers[0].url, "/data");
    EXPECT_EQ(handlers[0].type, "data");
    EXPECT_EQ(handlers[0].dir, "/var/test/httplib/data");
    EXPECT_EQ(handlers[1].url, "/data");
    EXPECT_EQ(handlers[1].type, "");
    EXPECT_EQ(handlers[1].dir, "");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
