#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
//#include <string>
#include <unordered_map>

#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/minhook.x32.lib")

#pragma pack(push, 1)

struct CDXUTEditBox
{
	uint8_t		stDXUTControl[0x4D];

	wchar_t*	m_pwszBuffer;
	int			m_nBufferSize;
};

struct CInput
{
	typedef void(__cdecl *CMDPROC)(char*);

	class IDirect3DDevice9*	m_pDevice;
	class CDXUTDialog*		m_pGameUi;
	CDXUTEditBox*			m_pEditbox;
	CMDPROC					m_pCommandProc[144];
	char					m_szCommandName[144][33];
	int						m_nCommandCount;
	BOOL					m_bEnabled;
	char					m_szInput[129];
	char					m_szRecallBufffer[10][129];
	char					m_szCurrentBuffer[129];
	int						m_nCurrentRecall;
	int						m_nTotalRecall;
	CMDPROC					m_pDefaultCommand;

	typedef void(__thiscall *ProcessInput_t)(CInput*);
};

#pragma pack(pop)

#endif