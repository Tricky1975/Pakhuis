#pragma once
#include <SlyvGINIE.hpp>
namespace Slyvina {
	namespace Pakhuis {

		Units::GINIE PrjConfig();

		void ScanVolumes();
		std::vector<std::string>& GetVolumes();
		std::string BackUpDir();
	}
}
