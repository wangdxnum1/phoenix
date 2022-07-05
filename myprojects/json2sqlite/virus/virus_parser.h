#ifndef VIRUS_PARSER_H_
#define VIRUS_PARSER_H_

#include <string>
#include <vector>
#include "base/macros.h"

#include "extension/memory/singleton.h"

namespace json2sqlite {
	class Virus;
	using VirusPtr = std::shared_ptr<Virus>;

	class VirusParser : public NS_EXTENSION::Singleton<VirusParser, false> {
	//SingletonHideConstructor(VirusParser);
	public:
		bool ParseVirus(const std::string& content, std::vector<VirusPtr> &virus);
	public:
		VirusParser();
		~VirusParser();
	//private:
	//	DISALLOW_COPY_AND_ASSIGN(VirusParser);
	};
}

#endif