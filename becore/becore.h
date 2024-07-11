#pragma once
#ifdef _WIN32
#ifdef BECORE_EXPORTS
#define BECORE_DLL_API __declspec(dllexport)
#else
#define BECORE_DLL_API __declspec(dllimport)
#endif
#else
#define BECORE_EXPORTS
#endif
