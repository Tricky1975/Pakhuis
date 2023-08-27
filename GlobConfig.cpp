#include <SlyvAsk.hpp>
#include <SlyvArgParse.hpp>
#include <SlyvDirry.hpp>
#include <SlyvDir.hpp>
#include <SlyvQCol.hpp>
#include <SlyvTime.hpp>
#include <SlyvStream.hpp>
#include <SlyvVolumes.hpp>

#include "GlobConfig.hpp"

namespace Slyvina {
	using namespace Units;
	namespace Pakhuis {
		ParsedArg CliConfig{};
		GINIE _GlobalConfig{ nullptr };
			
		Units::GINIE GlobalConfig() {
			if (!_GlobalConfig) {
				if (!FileExists(GlobConfigFile())) {
					QCol->Doing("Creating", GlobConfigFile());
					SaveString(GlobConfigFile(), "[Creation]\nCreated=" + CurrentDate() + "; " + CurrentTime());
				}
				QCol->Doing("Reading", GlobConfigFile());
				_GlobalConfig = LoadUGINIE(GlobConfigFile(), GlobConfigFile(), "Global config for Pakhuis!");
			}
			return _GlobalConfig;
		}

		std::string GlobConfigDir() { return Dirry("$AppSupport$"); }
		std::string GlobConfigFile() { return GlobConfigDir() + "/Pakhuis.glob.ini"; }

		std::string BackupProject() {
			std::string ret{""};
			if (CliConfig.arguments.size())
				ret = AVolPath(CliConfig.arguments[0]);
			else
				ret = AVolPath(Ask(GlobalConfig(), "Default", "Project", "No project given. What project file to use by default?"));
			auto d = ExtractDir(ret);
			if (!DirectoryExists(d)) {
				QCol->Doing("Creating dir", d);
				MakeDir(d);
			}
			if (!FileExists(ret)) {
				QCol->Doing("Creating", ret);
				SaveString(ret, "[Creation]\nCreated=" + CurrentDate() + "; " + CurrentTime());
			}
			return ret;
		}
	}
}