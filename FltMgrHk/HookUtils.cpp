#include "HookUtils.hpp"


#include "HookUtils.hpp"

bool HookUtils::HookIoDispatcher(const wchar_t* DriverName, unsigned int MajorFunctionIndex, PVOID HookFunction, PVOID* OriginalFunction)
{
	UNICODE_STRING DriverNameUniStr;
	PDRIVER_OBJECT TargetDriver = nullptr;

	// Initializes a string for the DriverName as unicode.
	RtlInitUnicodeString(&DriverNameUniStr, DriverName);

	// Obtains a reference to the driver by DriverName
	NTSTATUS Status = ObReferenceObjectByName(&DriverNameUniStr, 0, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&TargetDriver);

	// Failed to find the driver object by name, perhaps the driver is not loaded yet or the name is incorrect.
	if (!NT_SUCCESS(Status))
		return false;

	// Assign the original function to the original dispatcher.
	if (OriginalFunction)
		*OriginalFunction = TargetDriver->MajorFunction[MajorFunctionIndex];

	// Change the original dispatcher pointed to by MajorFunction[MajorFunctionIndex] to our hook function.
	TargetDriver->MajorFunction[MajorFunctionIndex] = (PDRIVER_DISPATCH)HookFunction;

	// Dereference the object to avoid reference count leaks.
	ObDereferenceObject(TargetDriver);

	if (OriginalFunction)
	{
		DEBUG_LOG("[HookIoDispatcher] Successfully hooked!, DriverName: (%S), MajorFunctionIndex: (%i), HookFunction: (%p), OriginalFunction: (%p)",
			DriverName,
			MajorFunctionIndex,
			HookFunction,
			*OriginalFunction);
	}
	else
	{
		DEBUG_LOG("[HookIoDispatcher] Successfully unhooked!, DriverName: (%S), MajorFunctionIndex: (%i), OriginalFunction: (%p)",
			DriverName,
			MajorFunctionIndex,
			HookFunction);
	}

	return true;

}

bool HookUtils::UnhookIoDispatcher(const wchar_t* DriverName, unsigned int MajorFunctionIndex, PVOID OriginalFunction)
{
	return HookIoDispatcher(DriverName, MajorFunctionIndex, OriginalFunction, nullptr);
}