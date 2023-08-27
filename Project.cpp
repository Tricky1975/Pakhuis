#include <string>
#include <vector>
#include <SlyvAsk.hpp>
#include <SlyvVolumes.hpp>
#include <SlyvQCol.hpp>
#include "GlobConfig.hpp"
#include "Project.hpp"


using namespace std;
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Pakhuis {
		static vector<string> _Volumes{};
		static GINIE _PrjConfig{nullptr};

		Units::GINIE PrjConfig() {
			if (!_PrjConfig) {
				QCol->Doing("Reading", BackupProject());
				_PrjConfig = LoadGINIE(BackupProject(), BackupProject(), "Pakhuis Backup Project");
			}
			return _PrjConfig;
		}

		void ScanVolumes() {
			QCol->Doing("Scanning", "Raw volumes");
			_Volumes.clear();
			auto raw{ Volumes() };
			for (auto v : *raw) {
				//cout << v.first << " = "<<v.second<< endl; //debug
				if (Yes(PrjConfig(), "Raw Volumes", v.first, TrSPrintF("Found volume \"%s\" (%s)!\nInclude it", v.first.c_str(), v.second.c_str()))) {
					QCol->LGreen("=> ");
					QCol->LMagenta(v.first+"\n");
					_Volumes.push_back(v.first + ":");
				}
			}
			QCol->Doing("Checking", "Volumized dirs");
			auto vdirs{ PrjConfig()->List("Volumized Dirs","Volumized") };
			for (auto v : *vdirs) {
				if (DirectoryExists(AVolPath(v))) {
					QCol->LGreen("=> ");
					QCol->LMagenta(v+"\n");
					_Volumes.push_back(v);
				} else {
					QCol->Red("X  ");
					QCol->Yellow(v + "\n");
				}
			}
		}
		std::vector<std::string>& GetVolumes() {
			if (!_Volumes.size()) ScanVolumes();
			return _Volumes;
		}
		std::string BackUpDir() {
			auto ret = Ask(PrjConfig(), "Directories", "Backup", "Directory for Backups:", ExtractDir(BackupProject()));
			if (!DirectoryExists(ret)) {
				QCol->Doing("Creating dir", ret);
				MakeDir(ret);
			}
			return ret;
		}
	}
}
