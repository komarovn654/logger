#include <gtest/gtest.h>

extern "C" {
    #include "logger.h"

    extern struct logger_src {
        logger_output out_type;
        FILE* out_stream;

        char* date_buf;
        char* message_buf;
        void* backtrace_buf;

        char* err_message;
        void (*error_callback)(void);

        void (*writer)(void);
        void (*message_former)(const char* message, logger_level level, const char* file, const char* func, const int line);
    } log_obj;
    extern logger_error log_buffers_init(void);
    extern void log_write_std(void);
    extern void log_write_file(void);
    extern void log_error_callback_default(void);
    extern void log_form_debug_message(const char* message, logger_level level, const char* file, const char* func, const int line);
    extern void log_date_update(void);
    extern void log_error_callback_default(void);
    extern void log_write_int_err(const char* message, ...);
    extern logger_error log_set_out_file(const char* file_name);
    extern void log_form_product_message(const char* message, logger_level level, const char* file, const char* func, const int line);
}

const char* test_log_file = "log.txt";

class TestLoggerFix : public ::testing::Test
{
protected:
	void SetUp()
	{
        log_buffers_init();
        log_obj.error_callback = log_error_callback_default;
	}
	void TearDown()
	{
		log_destruct();
	}
};

TEST_F(TestLoggerFix, InternalBuffersInit) 
{
    ASSERT_EQ(log_buffers_init(), LOGERR_NOERR);
    EXPECT_TRUE(log_obj.date_buf != NULL);
    EXPECT_TRUE(log_obj.message_buf != NULL);
    EXPECT_TRUE(log_obj.backtrace_buf != NULL);
    EXPECT_TRUE(log_obj.err_message != NULL);
}

TEST_F(TestLoggerFix, InternalBuffersDestruct) 
{
    log_destruct();
    EXPECT_TRUE(log_obj.date_buf == NULL);
    EXPECT_TRUE(log_obj.message_buf == NULL);
    EXPECT_TRUE(log_obj.backtrace_buf == NULL);
    EXPECT_TRUE(log_obj.err_message == NULL);
}

TEST_F(TestLoggerFix, GetInternalError) 
{
    std::snprintf(log_obj.err_message, 27, "there is an internal error");
    EXPECT_STREQ(log_get_internal_error(), "there is an internal error");
}

TEST_F(TestLoggerFix, WriteInternalError) 
{
    const char expect[] = "there is an internal error 15";
    log_write_int_err("there is an internal error %d", 15);
    EXPECT_STREQ(log_obj.err_message, expect);
}

TEST(TestLogger, UpdateDateInitErr)
{
    log_obj.err_message = new(char[128]);
    log_date_update();
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Date buffer uninitialized\n");
    delete(log_obj.err_message);
}

TEST_F(TestLoggerFix, UpdateLogDate)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char* expect = new(char[32]);
    std::snprintf(expect, 32, "%.2i.%.2i.%i %.2i:%.2i:%.2i",
                             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    log_date_update();
    EXPECT_STREQ(expect, log_obj.date_buf);
}

TEST_F(TestLoggerFix, WriteFile)
{
    const char test_string[] = "hello world!\n";
    char read_buf[32] = {0};
    FILE* f = fopen(test_log_file, "w");
    log_obj.out_stream = f;
    snprintf(log_obj.message_buf, 32, test_string);
    log_write_file();
    fclose(f);

    f = fopen(test_log_file, "r");
    fread((void*)read_buf, sizeof(char), 32, f);
    printf("%s\n", read_buf);
    EXPECT_STREQ(read_buf, test_string);
    remove(test_log_file);
}

TEST_F(TestLoggerFix, SetOutFile)
{
    FILE * file;
    file = fopen(test_log_file, "r");
    ASSERT_TRUE(file == NULL);

    EXPECT_EQ(log_set_out_file(test_log_file), LOGERR_NOERR);
    EXPECT_EQ(log_obj.writer, log_write_file);
    fclose(log_obj.out_stream);

    file = fopen(test_log_file, "r");
    ASSERT_TRUE(file != NULL);
    fclose(file);
    remove(test_log_file);
}

TEST_F(TestLoggerFix, RefreshBacktrace)
{
    char expect[LOG_BACKTRACE_BUF_SIZE] = { 0 };
    memset(log_obj.backtrace_buf, 1, LOG_BACKTRACE_BUF_SIZE);
    __log_refresh_backtrace_buf();
    EXPECT_STREQ((char *)log_obj.backtrace_buf, expect);
}

TEST_F(TestLoggerFix, FormDebugMessage)
{
    char expect[256];
    log_date_update();
    snprintf(expect, 256, "%s::%s::%s::%s::%i::%s\n", log_obj.date_buf, 
        "INFO", "file", "func", 3, "message");

    log_form_debug_message("message", LOGLEVEL_INFO, "file", "func", 3);
    EXPECT_STREQ(expect, log_obj.message_buf);
}

TEST_F(TestLoggerFix, FormDebugMessageErr)
{
    char overflow[2049] = { 0 };
    memset(overflow, 1, 2049);

    log_form_debug_message(overflow, LOGLEVEL_INFO, "file", "func", 3);
    EXPECT_STREQ("LOGGER_ERROR::Message buffer overflow\n", log_obj.err_message);
}

TEST_F(TestLoggerFix, FormProductMessage)
{
    char expect[256];
    log_date_update();
    snprintf(expect, 256, "%s::%s::%s\n", log_obj.date_buf, "INFO", "message");

    log_form_product_message("message", LOGLEVEL_INFO, "file", "func", 3);
    EXPECT_STREQ(expect, log_obj.message_buf);
}

TEST_F(TestLoggerFix, FormProductMessageErr)
{
    char overflow[2049] = { 0 };
    memset(overflow, 1, 2049);

    log_form_product_message(overflow, LOGLEVEL_INFO, "file", "func", 3);
    EXPECT_STREQ("LOGGER_ERROR::Message buffer overflow\n", log_obj.err_message);
}

TEST(TestLogger, InitDefautlLogger) 
{
    ASSERT_EQ(log_init_default(), LOGERR_NOERR);
    EXPECT_TRUE(log_obj.out_type == LOGOUT_STREAM);
    EXPECT_TRUE(log_obj.out_stream == stderr);
    EXPECT_TRUE(log_obj.writer == log_write_std);
    EXPECT_TRUE(log_obj.error_callback == log_error_callback_default);
    EXPECT_TRUE(log_obj.message_former == log_form_debug_message);
    log_destruct();
}

TEST(TestLogger, InitLoggerNoSettings) 
{
    ASSERT_EQ(log_init(NULL), LOGERR_NOERR);
    EXPECT_TRUE(log_obj.out_type == LOGOUT_STREAM);
    EXPECT_TRUE(log_obj.out_stream == stderr);
    EXPECT_TRUE(log_obj.writer == log_write_std);
    EXPECT_TRUE(log_obj.error_callback == log_error_callback_default);
    EXPECT_TRUE(log_obj.message_former == log_form_debug_message);
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_WARNING::Empty settings, setting default\n");
    log_destruct();
}

TEST(TestLogger, InitLoggerErrType) 
{
    logger_settings settings;
    memset(&settings, 0, sizeof(logger_settings));
    settings.out_type = LOGOUT_FILE;
    settings.output.file_name = test_log_file;
    
    ASSERT_EQ(log_init(&settings), LOGERR_LOGUNKNOWNTYPE);
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Unknown logger type\n");
    log_destruct();
    remove(test_log_file);
}

TEST(TestLogger, InitLoggerErrOutputType) 
{
    logger_settings settings;
    memset(&settings, 0, sizeof(logger_settings));
    settings.type = LOGTYPE_DEBUG;

    ASSERT_EQ(log_init(&settings), LOGERR_LOGUNKNOWNOUTTYPE);
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Unknown logger output type\n");
    log_destruct();
}

TEST(TestLogger, InitLoggerErrFile) 
{
    logger_settings settings;
    memset(&settings, 0, sizeof(logger_settings));
    settings.type = LOGTYPE_DEBUG;
    settings.out_type = LOGOUT_FILE;

    ASSERT_EQ(log_init(&settings), LOGERR_LOGFILECREATE);
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Unable to create/open log file\n");
    log_destruct();
}

TEST(TestLogger, InitLogger) 
{
    logger_settings settings;
    memset(&settings, 0, sizeof(logger_settings));
    settings.type = LOGTYPE_DEBUG;
    settings.out_type = LOGOUT_FILE;
    settings.output.file_name = test_log_file;

    ASSERT_EQ(log_init(&settings), LOGERR_NOERR);
    EXPECT_STREQ(log_get_internal_error(), "");
    log_destruct();
    remove(test_log_file);
}

TEST_F(TestLoggerFix, Log)
{
    log_obj.message_former = log_form_debug_message; 
    log_obj.writer = log_write_file;
    log_obj.out_stream = fopen(test_log_file, "w");

    char expect[256];
    log_date_update();
    int len = snprintf(expect, 256, "%s::%s::%s::%s::%i::%s\n", log_obj.date_buf, 
        "INFO", "file", "func", 2, "message");
    
    __log_log("message", LOGLEVEL_INFO, "file", "func", 2);
    fclose(log_obj.out_stream);

    char buf[256];
    memset(buf, 0, 256);
    FILE *f = fopen(test_log_file, "r");
    fread(buf, sizeof(char), len, f);

    EXPECT_STREQ(expect, buf);
    remove(test_log_file);
}

TEST(TestLogger, LogErr)
{
    log_obj.err_message = new(char[256]);
    __log_log("message", LOGLEVEL_INFO, "file", "func", 2);
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Message buffer uninitialized\n");
    delete(log_obj.err_message);
}