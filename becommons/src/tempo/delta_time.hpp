#pragma once
#include "becommons.hpp"
namespace Bubble {
	namespace Tempo {
		extern BECOMMONS_DLL_API double delta_time;
		void BECOMMONS_DLL_API iniDT();
		void BECOMMONS_DLL_API endDT();
	}
}