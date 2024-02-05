#include <gtest/gtest.h>

extern "C" {
    #include "logger.h"
}

class LoggerTests : public ::testing::Test
{
public:
    logger_error err;
protected:
	void SetUp()
	{
	}
	void TearDown()
	{
		log_destruct();
	}
};

const char *test_file = "log.txt";

TEST_F(LoggerTests, DebugLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_debug("debug message");
	ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LoggerTests, InfoLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_info("info message");
	ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LoggerTests, WarningLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_warning("warning message");
	ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LoggerTests, ErrorLogDebug)
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    log_error("error message");
	ASSERT_STREQ(log_get_internal_error(), "");
}

TEST_F(LoggerTests, InfoLogProduct)
{
	logger_settings settings;
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
	// cut off the date
	ASSERT_STREQ(&buf[19], "::INFO::info message\n");
}

TEST_F(LoggerTests, WarningLogProduct)
{
	logger_settings settings;
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
	// cut off the date
	ASSERT_STREQ(&buf[19], "::WARNING::warning message\n");
}

TEST_F(LoggerTests, ErrorLogProduct)
{
	logger_settings settings;
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
	// cut off the date and callstack
	ASSERT_STREQ(&buf[19], "::ERROR::error message\n");
}