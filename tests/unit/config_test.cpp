#include <gtest/gtest.h>
#include "config/configmanager.h"

namespace {

// Test fixture for ConfigManager
class ConfigManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test environment
    }

    void TearDown() override {
        // Clean up test environment
    }
};

// Test ConfigManager initialization
TEST_F(ConfigManagerTest, Initialization) {
    ITD::ConfigManager config;
    EXPECT_FALSE(config.GetConfigFilePath().empty());
}

// Test string values
TEST_F(ConfigManagerTest, StringValues) {
    ITD::ConfigManager config;
    
    // Set a string value
    config.SetString("test", "key", "value");
    
    // Get the string value
    EXPECT_EQ(config.GetString("test", "key"), "value");
    
    // Get a string with default
    EXPECT_EQ(config.GetString("test", "nonexistent", "default"), "default");
}

// Test integer values
TEST_F(ConfigManagerTest, IntegerValues) {
    ITD::ConfigManager config;
    
    // Set an integer value
    config.SetInt("test", "key", 42);
    
    // Get the integer value
    EXPECT_EQ(config.GetInt("test", "key"), 42);
    
    // Get an integer with default
    EXPECT_EQ(config.GetInt("test", "nonexistent", 100), 100);
}

// Test boolean values
TEST_F(ConfigManagerTest, BooleanValues) {
    ITD::ConfigManager config;
    
    // Set a boolean value
    config.SetBool("test", "key", true);
    
    // Get the boolean value
    EXPECT_TRUE(config.GetBool("test", "key"));
    
    // Get a boolean with default
    EXPECT_FALSE(config.GetBool("test", "nonexistent", false));
}

} // namespace 