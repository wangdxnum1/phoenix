#include "mylog.h"

#include "spdlog/sinks/rotating_file_sink.h"

bool MyLog::Init(const std::string &logPath, const std::string &log_name, spdlog::level::level_enum level)
{
	try {
		std::vector<spdlog::sink_ptr> sinks;
#ifdef WIN32
		sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
		sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
#endif
		sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logPath, kLogFileSize, kMaxFileCount));

#if _DEBUG
		//debug use. sync log.
		loger_ = std::make_shared<spdlog::logger>(log_name, begin(sinks), end(sinks));
		loger_->set_level(spdlog::level::trace);
		loger_->flush_on(spdlog::level::trace);
#else
		loger_ = std::make_shared<spdlog::async_logger>(log_name, begin(sinks), end(sinks), 1024, spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::seconds(2));
		loger_->set_level(level);
		loger_->flush_on(spdlog::level::critical);
#endif // _DEBUG

		spdlog::register_logger(loger_);
		spdlog::set_pattern("[%Y-%m-%d %T.%e] [%P:%t] [%n] [%l] %v");
	}
	catch (const spdlog::spdlog_ex& ex) {
		std::cout << "Log initialization failed: " << ex.what() << std::endl;
		return false;
	}

	return true;
}

void MyLog::SetLevel(spdlog::level::level_enum level)
{
	loger_->set_level(level);
}

std::shared_ptr<spdlog::logger> MyLog::GetLoger() {
	return loger_; 
}


MyLog::MyLog() {

}

MyLog::~MyLog() {
	
}