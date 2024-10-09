#pragma comment(lib, "wevtapi.lib")

#include <stdio.h>
#include <windows.h>
#include <winevt.h>

#define WriteData_MASK 0x2
#define AppendData_MASK 0x4
#define DeleteChild_MASK 0x40
#define DELETE_MASK 0x10000
#define READ_CONTROL_MASK 0x20000

int main() {
	EVT_HANDLE results = EvtQuery(NULL, L"Security", L"Event/System[EventID=5145]", EvtQueryForwardDirection);
	PWSTR valuesToRender[] = {
		L"Event/System/TimeCreated/@SystemTime",
		L"Event/EventData/Data[@Name=\"IpAddress\"]",
		L"Event/EventData/Data[@Name=\"ShareName\"]",
		L"Event/EventData/Data[@Name=\"RelativeTargetName\"]",
		L"Event/EventData/Data[@Name=\"AccessMask\"]",
	};
	DWORD totalValuesToRender = sizeof(valuesToRender) / sizeof(PWSTR);
	EVT_HANDLE renderContext = EvtCreateRenderContext(totalValuesToRender, valuesToRender, EvtRenderContextValues);
	EVT_HANDLE event = NULL;
	DWORD evtNextReturned = 0;
	while (EvtNext(results, 1, &event, INFINITE, 0, &evtNextReturned)) {
		PEVT_VARIANT values = NULL;
		DWORD valuesSize = 0;
		EvtRender(renderContext, event, EvtRenderEventValues, valuesSize, values, &valuesSize, NULL);
		values = (PEVT_VARIANT) malloc(valuesSize);
		EvtRender(renderContext, event, EvtRenderEventValues, valuesSize, values, &valuesSize, NULL);
		UINT32 accessMask = values[4].UInt32Val;
		if (!(accessMask & (WriteData_MASK | AppendData_MASK | DeleteChild_MASK | DELETE_MASK | READ_CONTROL_MASK))) {
			free(values);
			EvtClose(event);
			continue;
		}
		ULONGLONG fileTimeLong = values[0].FileTimeVal;
		FILETIME fileTime;
		fileTime.dwHighDateTime = (DWORD) (fileTimeLong >> 32);
		fileTime.dwLowDateTime = (DWORD) fileTimeLong;
		FileTimeToLocalFileTime(&fileTime, &fileTime);
		SYSTEMTIME systemTime;
		FileTimeToSystemTime(&fileTime, &systemTime);
		WCHAR access[256] = {'\0'};
		if (accessMask & READ_CONTROL_MASK) wcscat(access, L"Read ");
		if (accessMask & WriteData_MASK) wcscat(access, L"Write ");
		if (accessMask & AppendData_MASK) wcscat(access, L"Append ");
		if (accessMask & DeleteChild_MASK) wcscat(access, L"Delete ");
		if (accessMask & DELETE_MASK) wcscat(access, L"Delete ");
		access[wcslen(access) - 1] = '\0';
		wprintf(
			L"%02d-%02d-%02d %02d:%02d:%02d\t%s\t%s\\%s\t%s\t0x%08X\n",
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond,
			values[1].StringVal,
			values[2].StringVal, values[3].StringVal,
			access,
			accessMask
		);
		free(values);
		EvtClose(event);
	}
	EvtClose(renderContext);
	EvtClose(results);
	return 0;
}
