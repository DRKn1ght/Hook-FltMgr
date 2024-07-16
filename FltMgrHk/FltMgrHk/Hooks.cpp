#pragma once
#include "Hooks.hpp"

NTSTATUS NTAPI hk_FltMgrIoDispatcher(PDEVICE_OBJECT DeviceObject, PIRP Irp);
PDRIVER_DISPATCH o_FltMgrIoDispatcher = nullptr;

NTSTATUS NTAPI hk_FltMgrIoDispatcher(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PUNICODE_STRING ProcessName = nullptr;
	PIO_STACK_LOCATION IoStackLocation = IoGetCurrentIrpStackLocation(Irp);

	// Check if the stack location is null
	if (!IoStackLocation)
		goto EXIT;

	// Check if the request was made in kernel mode
	if (ExGetPreviousMode() == KernelMode)
		goto EXIT;

	PFILE_OBJECT FileObject = IoStackLocation->FileObject;

	// Check if the FileObject or its buffer is null
	if (!FileObject || !FileObject->FileName.Buffer)
		goto EXIT;

	// Get the name of the process that made the request
	if (!NT_SUCCESS(SeLocateProcessImageName(PsGetCurrentProcess(), &ProcessName)))
		goto EXIT;

	// Check if the process name or its buffer is null
	if (!ProcessName || !ProcessName->Buffer)
	{
		goto EXIT;
	}

	// Find the last '\' in the process path by starting from the end
	USHORT lastBackslashIndex = 0;
	USHORT length = ProcessName->Length / sizeof(WCHAR);

	for (USHORT i = length; i > 0; i--)
	{
		if (ProcessName->Buffer[i - 1] == L'\\')
		{
			lastBackslashIndex = i - 1;
			break; // Exit loop after finding the last backslash
		}
	}

	// Create a string with the process name without the path
	UNICODE_STRING shortProcessName;
	shortProcessName.Buffer = &ProcessName->Buffer[lastBackslashIndex + 1]; // Move to the next character after the backslash
	shortProcessName.Length = (ProcessName->Length - (lastBackslashIndex + 1) * sizeof(WCHAR));
	shortProcessName.MaximumLength = shortProcessName.Length;

	// Log the process name and the file being written to
	DEBUG_LOG("Process => (%S) requested writing in file => (%S)", shortProcessName.Buffer, FileObject->FileName.Buffer);

EXIT:
	// Free the allocated memory for the process name
	if (ProcessName)
		ExFreePool(ProcessName);

	// Return the original dispatcher function
	return o_FltMgrIoDispatcher(DeviceObject, Irp);
}

VOID Hooks::Deinitialize()
{
	HookUtils::UnhookIoDispatcher(L"\\FileSystem\\FltMgr", IRP_MJ_WRITE, o_FltMgrIoDispatcher);
}

VOID Hooks::Initialize()
{
	HookUtils::HookIoDispatcher(L"\\FileSystem\\FltMgr", IRP_MJ_WRITE, hk_FltMgrIoDispatcher, (PVOID*)&o_FltMgrIoDispatcher);
}