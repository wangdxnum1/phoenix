#ifndef VIRUS_DATABASE_GENERATOR_H_
#define VIRUS_DATABASE_GENERATOR_H_

#include <string>
#include <vector>

#include "extension/memory/singleton.h"

namespace json2sqlite {
	class Virus;
	using VirusPtr = std::shared_ptr<Virus>;

	class VirusDatabaseGenerator : public NS_EXTENSION::Singleton<VirusDatabaseGenerator, false> {
	public:
		bool GenerateVirusDatabase(const std::string &db, const std::vector<VirusPtr> &virus);
	public:
		VirusDatabaseGenerator();
		~VirusDatabaseGenerator();
	};
}

#endif