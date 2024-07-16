#pragma once
#include <ntifs.h>

#define DEBUG_LOG( format, ... ) DbgPrintEx( DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[ FltMgrHookKD ] " format "\n", ##__VA_ARGS__ )


EXTERN_C NTSTATUS SeLocateProcessImageName(PEPROCESS Process, PUNICODE_STRING* ProcessName);

EXTERN_C POBJECT_TYPE* IoDriverObjectType;

EXTERN_C NTSTATUS ObReferenceObjectByName(PUNICODE_STRING ObjectPath,
	ULONG Attributes,
	PACCESS_STATE PassedAccessState,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE ObjectType,
	KPROCESSOR_MODE AccessMode,
	PVOID ParseContext,
	PVOID* ObjectPtr);