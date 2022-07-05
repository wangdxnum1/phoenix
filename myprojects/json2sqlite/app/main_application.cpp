#include "main_application.h"

#include <iostream>

#include "gflags/gflags.h"

#include "extension/file_util/path_util.h"
#include "extension/at_exit_manager.h"
#include "extension/strings/string_util.h"
#include "extension/thread/thread_manager.h"
#include "extension/file_util/utf8_file_util.h"

#include "log/mylog.h"

#include "task/task.h"
#include "virus/virus_parser.h"
#include "virus/virus_database_generator.h"

DEFINE_string(json, "", "input json file path");
DEFINE_string(database, "json2sqlite3.db", "out put sqlite database path");
DEFINE_bool(append, false, "append json data to database,if not add this cmd,it will overrite database data");

static bool ValidateJsonFilePath(const char *flag, const std::string& value)
{
	if (value.empty()) return false;

	return true;
}

static bool ValidateAppend(const char *flag, bool value)
{
	return true;
}

enum ThreadIdentifier {
	THREAD_MAIN_ID = 0,

	THREAD_JSON_PARSE_ID = 100,

	THREAD_SQLITE3_ID = 200
};


namespace json2sqlite {
	int MainApplication::Run(int argc, char* argv[]) {		
		HandleCommandLine(argc, argv);

		// start event loop
		// 模拟UI程序，主线程开始事件循环，主要是用来学习这个线程模型
		RunOnCurrentThreadWithLoop();

		return 0;
	}

	bool MainApplication::HandleCommandLine(int argc, char* argv[]) {
		gflags::SetVersionString("1.0.0");
		gflags::RegisterFlagValidator(&FLAGS_json, &ValidateJsonFilePath);
		gflags::RegisterFlagValidator(&FLAGS_append, &ValidateAppend);

		google::ParseCommandLineFlags(&argc, &argv, true);

		return true;
	}

	void MainApplication::Init() {
		NS_EXTENSION::ThreadManager::RegisterThread(THREAD_MAIN_ID);

		// set current work directory
		std::wstring dir = NS_EXTENSION::GetCurrentModuleDirectory2();
		NS_EXTENSION::SetCurrentWorkDirctory(dir);

		// init log
		std::wstring logpath(dir);
		logpath = NS_EXTENSION::PathAppendComponent(logpath, L"json2sqlite.log");
		MyLog::GetInstance()->Init(NS_EXTENSION::UTF16ToUTF8(logpath), "json2sqlite", spdlog::level::debug);

		LOG_INFO("[json2sqlite] main start");

		// start work threads
		StartWorkThreads();

		// post a json parse task to json parse thread to start work
		PostJson2SqliteTask();

		__super::Init();
	}

	void MainApplication::CleanUp() {
		LOG_INFO("[json2sqlite] main start");

		NS_EXTENSION::ThreadManager::UnregisterThread();

		__super::CleanUp();
	}

	void MainApplication::RunOnCurrentThreadWithLoop() {
		NS_EXTENSION::AtExitManager at_exit = NS_EXTENSION::AtExitManagerAdeptor::GetAtExitManager();
		//NS_EXTENSION::ThreadManager::CreateFrameworkThread(THREAD_MAIN_ID, "main");

		AttachCurrentThreadWithLoop(base::MessageLoop::Type::TYPE_DEFAULT);
	}

	void MainApplication::StartWorkThreads() {
		std::once_flag my_flag;
		std::call_once(my_flag, [this]() {
			json_parse_thread_ = std::make_shared<NS_EXTENSION::FrameworkThread>("json_parse_thread");
			json_parse_thread_->RegisterInitCallback([&]() {
				LOG_INFO("json_parse_thread init");
				NS_EXTENSION::ThreadManager::RegisterThread(THREAD_JSON_PARSE_ID);
			});
			json_parse_thread_->RegisterCleanupCallback([&]() {
				LOG_INFO("json_parse_thread end");
				NS_EXTENSION::ThreadManager::UnregisterThread();
			});

			json_parse_thread_->Start();
			json_parse_thread_->WaitUntilThreadStarted();

			sqlite3_thread_ = std::make_shared<NS_EXTENSION::FrameworkThread>("sqlite3_thread");
			sqlite3_thread_->RegisterInitCallback([&]() {
				LOG_INFO("sqlite3_thread init");
				NS_EXTENSION::ThreadManager::RegisterThread(THREAD_SQLITE3_ID);
			});
			sqlite3_thread_->RegisterCleanupCallback([&]() {
				LOG_INFO("sqlite3_thread end");
				NS_EXTENSION::ThreadManager::UnregisterThread();
			});

			sqlite3_thread_->Start();
			sqlite3_thread_->WaitUntilThreadStarted();
		});
	}

	void MainApplication::PostJson2SqliteTask(){
		std::shared_ptr<Json2SqliteTask> task = std::make_shared<Json2SqliteTask>();
		task->json_path_ = FLAGS_json;
		task->database_path_ = FLAGS_database;
		task->append_ = FLAGS_append;

		// post task to json parse thread
		NS_EXTENSION::ThreadManager::PostTask(THREAD_JSON_PARSE_ID, [this, task]() {
			// code will optimize, now just use lambda
			HandleJson2SqliteTask(task);
		});
	}

	void MainApplication::HandleJson2SqliteTask(std::shared_ptr<Json2SqliteTask> task) {
		LOG_INFO("{}, {}, {}", task->json_path_, task->database_path_, task->append_);

		// read data
		std::string content;
		if (!NS_EXTENSION::ReadFileToString(NS_EXTENSION::ASCIIToUTF16(task->json_path_), content)) {
			LOG_ERROR("HandleJson2SqliteTask read json file fail");
			return;
		}

		// parse data
		if (!VirusParser::GetInstance()->ParseVirus(content, task->virus_)) {
			LOG_ERROR("HandleJson2SqliteTask parse virus fail");
			return;
		}
	
		// then post data to sqlite database thread to insert to database
		NS_EXTENSION::ThreadManager::PostTask(THREAD_SQLITE3_ID, [this, task]() {
			SaveVirusToSqlite(task);
		});
	}

	void MainApplication::SaveVirusToSqlite(std::shared_ptr<Json2SqliteTask> task) {
		if (!task->append_) {
			// this menas create a new database, then remove the old on if exist
			if(NS_EXTENSION::FilePathIsExist(NS_EXTENSION::ASCIIToUTF16(task->database_path_), false)){
				NS_EXTENSION::DeleteFile(NS_EXTENSION::ASCIIToUTF16(task->database_path_));
			}
		}

		bool ret = VirusDatabaseGenerator::GetInstance()->GenerateVirusDatabase(task->database_path_, task->virus_);

		// notify main thread to display result
		// then post data to sqlite database thread to insert to database
		NS_EXTENSION::ThreadManager::PostTask(THREAD_MAIN_ID, [this, ret]() {
			LOG_INFO("save virus {}", ret ? "success" : "fail");
		});
	}

	MainApplication::MainApplication(const std::string& name) : NS_EXTENSION::FrameworkThread(name) {

	}

	MainApplication::~MainApplication() {

	}
}