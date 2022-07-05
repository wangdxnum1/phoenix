#include "task.h"

namespace json2sqlite {
	BasicTask::BasicTask():type_(TASK_NONE) {

	}

	BasicTask::BasicTask(TaskType type):type_(type) {

	}


	BasicTask::~BasicTask() {

	}

	Json2SqliteTask::Json2SqliteTask():BasicTask(TASK_JSON_TO_SQLITE){

	}

	Json2SqliteTask::~Json2SqliteTask() {

	}
}