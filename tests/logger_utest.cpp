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
    extern void log_error_callback_default(void);
    extern void log_form_debug_message(const char* message, logger_level level, const char* file, const char* func, const int line);
    extern void log_date_update(void);
}

const char* test_log_file = "log.txt";

class TestLoggerFix : public ::testing::Test
{
protected:
	void SetUp()
	{
        log_buffers_init();
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

TEST(TestLogger, UpdateLogDate)
{
    log_obj.err_message = (char*)malloc(32);
    log_date_update();
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Date buffer uninitialized\n");
    delete(log_obj.err_message);
}

// TEST_F(TestLoggerFix, UpdateLogDate)
// {
//     time_t t = time(NULL);
//     struct tm tm = *localtime(&t);
//     std::snprintf(log_obj.date_buf, 32, "%.2i.%.2i.%i %.2i:%.2i:%.2i",
//                              tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
//     log_date_update();
// }

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
    logger_settings settings= {
        .out_type = LOGOUT_FILE,
        .output.file_name = test_log_file,
    };

    ASSERT_EQ(log_init(&settings), LOGERR_LOGUNKNOWNTYPE);
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Unknown logger type\n");
    log_destruct();
}

TEST(TestLogger, InitLoggerErrOutputType) 
{
    logger_settings settings= {
        .type = LOGTYPE_DEBUG,
    };

    ASSERT_EQ(log_init(&settings), LOGERR_LOGUNKNOWNOUTTYPE);
    EXPECT_STREQ(log_get_internal_error(), "LOGGER_ERROR::Unknown logger output type\n");
    log_destruct();
}

TEST(TestLogger, InitLogger) 
{
    logger_settings settings= {
        .type = LOGTYPE_DEBUG,
        .out_type = LOGOUT_FILE,
        .output.file_name = test_log_file,      
    };

    ASSERT_EQ(log_init(&settings), LOGERR_NOERR);
    EXPECT_STREQ(log_get_internal_error(), "");
    log_destruct();
    remove(test_log_file);
}
