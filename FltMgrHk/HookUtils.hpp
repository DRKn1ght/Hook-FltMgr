#pragma once
#include <ntifs.h>
#include "Defines.hpp"

namespace HookUtils
{
	bool HookIoDispatcher(const wchar_t* DriverName, unsigned int MajorFunctionIndex, PVOID HookFunction, PVOID* OriginalFunction);
	bool UnhookIoDispatcher(const wchar_t* DriverName, unsigned int MajorFunctionIndex, PVOID OriginalFunction);
} // namespace HookUtils