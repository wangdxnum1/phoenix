// json2sqlite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "app/main_application.h"

int main(int argc, char* argv[])
{
	json2sqlite::MainApplication app("main");
	app.Run(argc, argv);

    return 0;
}

