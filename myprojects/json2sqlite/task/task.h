#ifndef TASK_H_
#define TASK_H_

#include <string>
#include <vector>
#include <memory>

namespace json2sqlite {
	enum TaskType {
		TASK_NONE = -1,

		TASK_JSON_TO_SQLITE = 0,
	};
	class BasicTask {
		TaskType type_;
	public:
		BasicTask();
		BasicTask(TaskType type);
		~BasicTask();
	};

	class Virus;
	class Json2SqliteTask : public BasicTask {
	public:
		std::string json_path_;
		std::string database_path_;
		bool append_;
		std::vector<std::shared_ptr<Virus>> virus_;
	public:
		Json2SqliteTask();
		~Json2SqliteTask();
	};
}

#endif