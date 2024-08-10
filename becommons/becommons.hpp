#pragma once
#ifdef _WIN32
#ifdef BECOMMONS_EXPORTS
#define BECOMMONS_DLL_API __declspec(dllexport)
#else
#define BECOMMONS_DLL_API __declspec(dllimport)
#endif
#else
#define BECOMMONS_EXPORTS
#endif
