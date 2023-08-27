#include <SlyvAsk.hpp>
#include <SlyvRandom.hpp>
#include <SlyvDir.hpp>
#include <SlyvVolumes.hpp>
#include <SlyvConInput.hpp>

#include "BackUp.hpp"
#include "Project.hpp"
#include "GlobConfig.hpp"

using namespace std;

namespace Slyvina {
	using namespace Units;
	namespace Pakhuis {
		static string PackExe{};
		static string PackNew{}; 
		static string PackUpd{};
		static string PackExt{};
		static string PackRecurse{};


		inline void AskPack(){
			if (PackExe.size()) return;
			PackExe = Ask(PrjConfig(), "Packer", "Exe_" + Platform(), "Packer Exe:");
			PackNew = Ask(PrjConfig(), "Packer", "New", "Parameters new archive: ");
			PackUpd = Ask(PrjConfig(), "Packer", "Update", "Parameters update:");		
			PackExt = Ask(PrjConfig(), "Packer", "Extension", "Packer extension:");
			PackRecurse = Ask(PrjConfig(), "Packer", "Recurse", "Recurse flag:");
		}



		void DoVolume(std::string Vol,std::string Process) {
			auto& Vols{ GetVolumes() };
			QCol->Doing("Volume", Vol+" "+Process);
			AskPack();
			static auto SReset{ 0 };
			auto VolTag = "VOL::" + Vol;
			auto VReset = AskInt(PrjConfig(), VolTag, "Reset", "Volume ROOT reset value: ", Rand.Get(75, 125));
			auto VCount = PrjConfig()->IntValue(VolTag, "Count");
			auto VArchv = string("Pakhuis____"); if (ExtractDir(Vol).size()) VArchv += "AN____" + ChReplace(ChReplace(Ask(PrjConfig(), VolTag, "Alternate_Name", "Alternate name for this volume: ", ChReplace(ChReplace(Vol,'/','_'),':','_')), '/', '_'), ':', '_'); else VArchv += ChReplace(Vol, ':', '_');
			auto VAFile = BackUpDir() + "/" + VArchv + "." + PackExt;
			auto vMax = std::max(100,VReset);
			if (CliConfig.bool_flags["-TOTALREDO"]) VCount = 0;
			if (!FileExists(VAFile)) VCount = 0;
			auto Commando{ PackExe+" "};
			if (VCount <= 0) {
				if (FileExists(VAFile)) FileDelete(VAFile);
				Commando += PackNew;
			} else Commando += PackUpd;
			Commando = StReplace(Commando, "<archive>", "\"" + VAFile + "\"");
			ChangeDir(AVolPath(Vol));
			QCol->Doing("Executing", Commando);
			QCol->Reset();
			system(Commando.c_str());
			if (VCount <= 0) {
				PrjConfig()->Value(VolTag, "Count", VReset + (SReset++));
			}  else
				PrjConfig()->Value(VolTag, "Count", VCount - 1);
			auto Directories = FileList(AVolPath(Vol), DirWant::Directories);
			for (auto& D : *Directories) {
				auto WhatToDo{ PrjConfig()->IntValue(VolTag,"ZZ_Dir::"+D) };
				while(WhatToDo<1 || WhatToDo>3) {
					QCol->Red("Note!\n");
					QCol->Yellow("The directory ");
					QCol->Cyan(D);
					QCol->Yellow(" in volume ");
					QCol->Cyan(Vol);
					QCol->Yellow(" has not been encountered before!\nWhat am I suppsed to do with it?\n");
					QCol->LGreen("\t1. "); QCol->LMagenta("Treat as normal dir\n");
					QCol->LGreen("\t2. "); QCol->LMagenta("Volumize\n");
					QCol->LGreen("\t3. "); QCol->LMagenta("Ignore\n");
					QCol->Yellow("Tell me: "); QCol->Cyan("");
					WhatToDo = ToInt(ReadLine());
					PrjConfig()->Value(VolTag, "ZZ_Dir::" + D, WhatToDo);
					if (WhatToDo == 2) {
						Vols.push_back(Vol + "/" + D);
						PrjConfig()->AddNew("Volumized Dirs", "Volumized", Vol + "/" + D, true);
					}
				}
				if (WhatToDo == 1) {
					QCol->Doing("Volume", Vol, "\t");
					QCol->Doing("Directory", D);
					auto DirTag = "VOL::" + Vol + "::" + D;
					auto DReset = AskInt(PrjConfig(), DirTag, "Reset", "Volume " + D + " (" + Vol + ") reset value : ", Rand.Get((vMax / 4) * 3, vMax));
					auto DCount = PrjConfig()->IntValue(DirTag, "Count");
					vMax = std::max(100, DReset);
					//auto DArchv = string("Pakhuis____"); if (ExtractDir(Vol).size()) DArchv += "AN____" + ChReplace(ChReplace(Ask(PrjConfig(), VolTag, "AlternateName", "Alternate name for this volume: ", StripAll(Vol)), '/', '_'), ':', '_'); else VArchv += ChReplace(Vol, ':', '_');;  DArchv += "_______" + D;
					auto DAFile = StripExt(VAFile) + "___DIR___" + D + "." + PackExt;
					//auto DAFile = BackUpDir() + "/" + DArchv + "." + PackExt;
					Commando = PackExe + " ";
					if (DCount <= 0) {
						if (FileExists(DAFile)) FileDelete(DAFile);
						Commando += PackNew;
					} else Commando += PackUpd;
					Commando += " " + PackRecurse;
					Commando = StReplace(Commando, "<archive>", "\"" + DAFile + "\"");
					ChangeDir(AVolPath(Vol + "/" + D));
					QCol->Doing("Executing", Commando);
					QCol->Reset();
					system(Commando.c_str());
					if (DCount <= 0)
						PrjConfig()->Value(DirTag, "Count", abs(DReset + (SReset++)));
					else
						PrjConfig()->Value(DirTag, "Count", DCount - 1);
				}
			}			
		}

		void DoVolume() {
			auto& Vols{ GetVolumes() };
			for (size_t i = 0; i < Vols.size(); i++) {
				DoVolume(Vols[i], TrSPrintF("(%9d/%9d)", i, Vols.size()));
			}

		}
	}
}
