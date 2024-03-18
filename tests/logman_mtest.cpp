#include <gtest/gtest.h>

extern "C" {
    #include "../src/logman_int.h"
}

const char *test_file = "log.txt";

class LogmanTests : public ::testing::Test
{
public:
    logman_error err;
protected:
    void SetUp()
    {
    }
    void TearDown()
    {
        log_destruct();
        
        remove(test_file);
    }
};

TEST_F(LogmanTests, DebugLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_debug("debug message");
    ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LogmanTests, InfoLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_info("info message");
    ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LogmanTests, WarningLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_warning("warning message");
    ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LogmanTests, ErrorLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_error("error message");
    ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LogmanTests, InfoLogProduct)
{
    logman_settings settings;
    settings.type = LOGTYPE_PRODUCT;
    settings.out_type = LOGOUT_FILE;
    settings.output.file_name = test_file;

    ASSERT_EQ(log_init(&settings), LOGERR_NOERR);
    log_info("info message");
    ASSERT_STREQ(log_get_internal_error(), "");
    log_destruct();

    FILE *f = fopen(test_file, "r");
    char buf[128];
    memset(buf, 0, 128);
    fread(buf, sizeof(char), 128, f);
    fclose(f);
    // cut off the date
    ASSERT_STREQ(&buf[19], "::INFO::info message\n");
}

TEST_F(LogmanTests, WarningLogProduct)
{
    logman_settings settings;
    settings.type = LOGTYPE_PRODUCT;
    settings.out_type = LOGOUT_FILE;
    settings.output.file_name = test_file;

    ASSERT_EQ(log_init(&settings), LOGERR_NOERR);
    log_warning("warning message");
    ASSERT_STREQ(log_get_internal_error(), "");
    log_destruct();

    FILE *f = fopen(test_file, "r");
    char buf[128];
    memset(buf, 0, 128);
    fread(buf, sizeof(char), 128, f);
    fclose(f);
    // cut off the date
    ASSERT_STREQ(&buf[19], "::WARNING::warning message\n");
}

TEST_F(LogmanTests, ErrorLogProduct)
{
    logman_settings settings;
    settings.type = LOGTYPE_PRODUCT;
    settings.out_type = LOGOUT_FILE;
    settings.output.file_name = test_file;

    ASSERT_EQ(log_init(&settings), LOGERR_NOERR);
    log_error("error message");
    ASSERT_STREQ(log_get_internal_error(), "");
    log_destruct();

    FILE *f = fopen(test_file, "r");
    char buf[128];
    memset(buf, 0, 128);
    fread(buf, sizeof(char), 42, f);
    fclose(f);
    // cut off the date and callstack
    ASSERT_STREQ(&buf[19], "::ERROR::error message\n");
}