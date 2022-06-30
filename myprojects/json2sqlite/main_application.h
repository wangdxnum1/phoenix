#ifndef MAIN_APP_H_
#define MAIN_APP_H_

#include <memory>
#include "base/macros.h"

namespace json2sqlite {
	class MainApplication : public std::enable_shared_from_this<MainApplication> {
	public:
		int Run(int argc, char* argv[]);
	private:
		bool HandleCommandLine(int argc, char* argv[]);
	public:
		MainApplication();
		~MainApplication();
	private:
		DISALLOW_COPY_AND_ASSIGN(MainApplication);
	};
}

#endif
