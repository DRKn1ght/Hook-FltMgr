#include <ntifs.h>
#include "Hooks.hpp"

VOID DriverUnload(PDRIVER_OBJECT Driver)
{
	UNREFERENCED_PARAMETER(Driver);

	// Unhook all previously hooked routines
	Hooks::Deinitialize();
}



EXTERN_C NTSTATUS DriverEntry(PDRIVER_OBJECT Driver, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	Driver->DriverUnload = &DriverUnload;

	// Hook the routines
	Hooks::Initialize();

	BOOLEAN NeedLogReinitialization;

	return STATUS_SUCCESS;
}