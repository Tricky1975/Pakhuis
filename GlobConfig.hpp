#pragma once
#include <SlyvGINIE.hpp>
#include <SlyvArgParse.hpp>


namespace Slyvina {
	namespace Pakhuis {
		extern Units::ParsedArg CliConfig;

		Units::GINIE GlobalConfig();

		std::string GlobConfigDir();
		std::string GlobConfigFile();

		std::string BackupProject();

	}
}