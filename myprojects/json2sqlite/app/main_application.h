#ifndef MAIN_APP_H_
#define MAIN_APP_H_

#include <memory>
#include "base/macros.h"
#include "extension/thread/framework_thread.h"

namespace json2sqlite {
	class Json2SqliteTask;

	class MainApplication : public std::enable_shared_from_this<MainApplication>, 
		public NS_EXTENSION::FrameworkThread {
	public:
		int Run(int argc, char* argv[]);
	private:
		bool HandleCommandLine(int argc, char* argv[]);
	private:
		virtual void Init() override;
		virtual void CleanUp() override;
	private:
		void RunOnCurrentThreadWithLoop();
	private:
		void StartWorkThreads();
		void PostJson2SqliteTask();
		void HandleJson2SqliteTask(std::shared_ptr<Json2SqliteTask> task);
		void SaveVirusToSqlite(std::shared_ptr<Json2SqliteTask> task);
	public:
		MainApplication(const std::string& name);
		~MainApplication();
	private:
		DISALLOW_COPY_AND_ASSIGN(MainApplication);
	private:
		//std::shared_ptr<NS_EXTENSION::FrameworkThread> main_thread_;
		std::shared_ptr<NS_EXTENSION::FrameworkThread> json_parse_thread_;
		std::shared_ptr<NS_EXTENSION::FrameworkThread> sqlite3_thread_;
	};
}

#endif
