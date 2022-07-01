#include "main_application.h"

#include <iostream>

#include "gflags/gflags.h"

#include "extension/file_util/path_util.h"
#include "extension/at_exit_manager.h"
#include "extension/strings/string_util.h"

#include "log/mylog.h"

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


//DEFINE_string(host, "localhost", "Server host address");
//DEFINE_int32(port, 8080, "Server port");

namespace json2sqlite {
	int MainApplication::Run(int argc, char* argv[]) {		
		HandleCommandLine(argc, argv);

		// start event loop
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
		// set current work directory
		std::wstring dir = NS_EXTENSION::GetCurrentModuleDirectory2();
		NS_EXTENSION::SetCurrentWorkDirctory(dir);

		// init log
		std::wstring logpath(dir);
		logpath = NS_EXTENSION::PathAppendComponent(logpath, L"json2sqlite.log");
		MyLog::GetInstance()->Init(NS_EXTENSION::UTF16ToUTF8(logpath), "json2sqlite", spdlog::level::debug);

		LOG_INFO("[json2sqlite] main start");

	}

	void MainApplication::CleanUp() {
		LOG_INFO("[json2sqlite] main start");
	}

	void MainApplication::RunOnCurrentThreadWithLoop() {
		NS_EXTENSION::AtExitManager at_exit = NS_EXTENSION::AtExitManagerAdeptor::GetAtExitManager();
		AttachCurrentThreadWithLoop(base::MessageLoop::Type::TYPE_DEFAULT);
	}

	void MainApplication::ResetCurrentWorkDirectory() {

	}

	MainApplication::MainApplication(const std::string& name) : NS_EXTENSION::FrameworkThread(name) {

	}

	MainApplication::~MainApplication() {

	}
}