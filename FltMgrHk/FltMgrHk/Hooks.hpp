#pragma once
#include <ntifs.h>
#include "Defines.hpp"
#include "HookUtils.hpp"

namespace Hooks
{
	VOID Initialize();
	VOID Deinitialize();
} // namespace Hooks