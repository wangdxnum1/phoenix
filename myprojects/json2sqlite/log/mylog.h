#ifndef MYLOG_H_
#define MYLOG_H_

#include <memory>
#include <string>
#include <iostream>

#include "extension/memory/singleton.h"

#include "spdlog/spdlog.h"

namespace{
	const static uint32_t kLogFileSize = 1024 * 1024 * 20; //20M 字节
	const static uint32_t kMaxFileCount = 5; //最大日志文件数量
}

class MyLog
{
public:
	SINGLETON_DEFINE(MyLog);
public:
	bool Init(const std::string &logPath, const std::string &log_name, spdlog::level::level_enum level);
	void SetLevel(spdlog::level::level_enum level);

	std::shared_ptr<spdlog::logger> GetLoger();
public:
	MyLog();
	~MyLog();

private:
	std::shared_ptr<spdlog::logger> loger_;
};

#define LOGGER (MyLog::GetInstance()->GetLoger())

#define LOG_TRACE(...)		(LOGGER->trace(__VA_ARGS__))
#define LOG_DEBUG(...)		(LOGGER->debug(__VA_ARGS__))
#define LOG_INFO(...)		(LOGGER->info(__VA_ARGS__))
#define LOG_ERROR(...)		(LOGGER->error(__VA_ARGS__))
#define LOG_WARN(...)		(LOGGER->warn(__VA_ARGS__))
#define LOG_CRITICAL(...)	(LOGGER->critical(__VA_ARGS__))

#endif // !MYLOG_H_