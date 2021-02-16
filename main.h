#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <string>
#include <unordered_map>

#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/libMinHook.x86.lib")

typedef void(__fastcall* hookedProcessInput_t)(void*, DWORD);

#pragma pack(push, 1)

// DXUT
struct stDXUTEditBox
{
	uint8_t		stDXUTControl[0x4D];

	wchar_t*	m_pwszBuffer;   // Buffer to hold text
	int			m_nBufferSize;	// Size of the buffer allocated, in characters
};

// SAMP
typedef void(__cdecl *CMDPROC)(PCHAR);
struct stInputInfo
{
	void									*pD3DDevice;
	void									*pDXUTDialog;
	stDXUTEditBox							*pDXUTEditBox;
	CMDPROC									pCMDs[144];
	char									szCMDNames[144][33];
	int										iCMDCount;
	int										iInputEnabled;
	char									szInputBuffer[129];
	char									szRecallBufffer[10][129];
	char									szCurrentBuffer[129];
	int										iCurrentRecall;
	int										iTotalRecalls;
	CMDPROC									pszDefaultCMD;
};

#pragma pack(pop)

#endif