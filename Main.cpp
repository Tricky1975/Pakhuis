#include <SlyvQCol.hpp>

#include "GlobConfig.hpp"
#include "Project.hpp"
#include "BackUp.hpp"

using namespace Slyvina;
using namespace Pakhuis;
using namespace Units;

int main(int c, char** a) {
	auto CD{ CurrentDir() };
	QCol->LGreen("Pakhuis\n\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	QCol->Doing("Build", TrSPrintF("%s; %s", __DATE__, __TIME__));
	cout << "\n\n\n";
	FlagConfig _cli{};
	AddFlag_Bool(_cli, "-TotalRedo", false);
	CliConfig = ParseArg(c, a, _cli);
	ScanVolumes();
	DoVolume();
	ChangeDir(CD);
	QCol->Reset();
}