#include "main.h"

uint32_t layout_change_mode = 1;

std::map<wchar_t, wchar_t> rus_table =
{
	// Russian to English
	{L'Ё', L'~'}, {L'\"', L'@'}, {L'№', L'#'}, {L';', L'$'},
	{L':', L'^'}, {L'?', L'&'}, {L'Й', L'Q'}, {L'Ц', L'W'},
	{L'У', L'E'}, {L'К', L'R'}, {L'Е', L'T'}, {L'Н', L'Y'},
	{L'Г', L'U'}, {L'Ш', L'I'}, {L'Щ', L'O'}, {L'З', L'P'},
	{L'Х', L'{'}, {L'Ъ', L'}'}, {L'/', L'|'}, {L'Ф', L'A'},
	{L'Ы', L'S'}, {L'В', L'D'}, {L'А', L'F'}, {L'П', L'G'},
	{L'Р', L'H'}, {L'О', L'J'}, {L'Л', L'K'}, {L'Д', L'L'},
	{L'Ж', L':'}, {L'Э', L'\"'}, {L'Я', L'Z'}, {L'Ч', L'X'},
	{L'С', L'C'}, {L'М', L'V'}, {L'И', L'B'}, {L'Т', L'N'},
	{L'Ь', L'M'}, {L'Б', L'<'}, {L'Ю', L'>'}, {L',', L'?'},
	{L'ё', L'`'}, {L'й', L'q'}, {L'ц', L'w'}, {L'у', L'e'},
	{L'к', L'r'}, {L'е', L't'}, {L'н', L'y'}, {L'г', L'u'},
	{L'ш', L'i'}, {L'щ', L'o'}, {L'з', L'p'}, {L'х', L'['},
	{L'ъ', L']'}, {L'ф', L'a'}, {L'ы', L's'}, {L'в', L'd'},
	{L'а', L'f'}, {L'п', L'g'}, {L'р', L'h'}, {L'о', L'j'},
	{L'л', L'k'}, {L'д', L'l'}, {L'ж', L';'}, {L'э', L'\''},
	{L'я', L'z'}, {L'ч', L'x'}, {L'с', L'c'}, {L'м', L'v'},
	{L'и', L'b'}, {L'т', L'n'}, {L'ь', L'm'}, {L'б', L','},
	{L'ю', L'.'}, {L'.', L'/'}
};

std::map<wchar_t, wchar_t> eng_table =
{
	// English to Russian
	{L'~', L'Ё'}, {L'@', L'\"'}, {L'#', L'№'}, {L'$', L';'},
	{L'^', L':'}, {L'&', L'?'}, {L'Q', L'Й'}, {L'W', L'Ц'},
	{L'E', L'У'}, {L'R', L'К'}, {L'T', L'Е'}, {L'Y', L'Н'},
	{L'U', L'Г'}, {L'I', L'Ш'}, {L'O', L'Щ'}, {L'P', L'З'},
	{L'{', L'Х'}, {L'}', L'Ъ'}, {L'|', L'/'}, {L'A', L'Ф'},
	{L'S', L'Ы'}, {L'D', L'В'}, {L'F', L'А'}, {L'G', L'П'},
	{L'H', L'Р'}, {L'J', L'О'}, {L'K', L'Л'}, {L'L', L'Д'},
	{L':', L'Ж'}, {L'\"', L'Э'}, {L'Z', L'Я'}, {L'X', L'Ч'},
	{L'C', L'С'}, {L'V', L'М'}, {L'B', L'И'}, {L'N', L'Т'},
	{L'M', L'Ь'}, {L'<', L'Б'}, {L'>', L'Ю'}, {L'?', L','},
	{L'`', L'ё'}, {L'q', L'й'}, {L'w', L'ц'}, {L'e', L'у'},
	{L'r', L'к'}, {L't', L'е'}, {L'y', L'н'}, {L'u', L'г'},
	{L'i', L'ш'}, {L'o', L'щ'}, {L'p', L'з'}, {L'[', L'х'},
	{L']', L'ъ'}, {L'a', L'ф'}, {L's', L'ы'}, {L'd', L'в'},
	{L'f', L'а'}, {L'g', L'п'}, {L'h', L'р'}, {L'j', L'о'},
	{L'k', L'л'}, {L'l', L'д'}, {L';', L'ж'}, {L'\'', L'э'},
	{L'z', L'я'}, {L'x', L'ч'}, {L'c', L'с'}, {L'v', L'м'},
	{L'b', L'и'}, {L'n', L'т'}, {L'm', L'ь'}, {L',', L'б'},
	{L'.', L'ю'}, {L'/', L'.'}
};

void ChangeLayout(wchar_t *in, wchar_t break_symbol = L'\0', bool reverse = false)
{
	bool now_russian = true;
	do
	{
		if (*in == break_symbol)
			break;

		if (reverse)
		{
			if (*in >= L'А' && *in <= L'я')
				now_russian = true;
			else
				if (*in >= L'A' && *in <= L'z')
					now_russian = false;
		}

		auto &table = now_russian ? rus_table : eng_table;

		auto result = table.find(*in);
		if (result != table.end())
			*in = result->second;

	} while (*in++ != L'\0');
}

hookedProcessInput_t orig_ProcessInput;
void __fastcall hooked_ProcessInput(void* _this, DWORD edx)
{
	if (layout_change_mode != 0)
	{
		stDXUTEditBox* input_box = reinterpret_cast<stInputInfo*>(_this)->pDXUTEditBox;

		if (input_box->m_nBufferSize > 2 && input_box->m_pwszBuffer[0] == L'.')
		{
			if (input_box->m_pwszBuffer[1] >= L'А' && input_box->m_pwszBuffer[1] <= L'я')
			{
				ChangeLayout(input_box->m_pwszBuffer, ((layout_change_mode == 3) ? L' ' : L'\0'), (layout_change_mode == 1));
			}
		}
	}
	return orig_ProcessInput(_this, edx);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);

			DWORD dwSAMPAddr = (DWORD)GetModuleHandle("samp.dll");
			if (dwSAMPAddr == NULL)
			{
				MessageBoxA(NULL, "SA-MP was not loaded.", "CommandLayoutChanger.asi", MB_OK | MB_ICONERROR);
				return false;
			}

			IMAGE_NT_HEADERS *ntheader = reinterpret_cast<IMAGE_NT_HEADERS*>(dwSAMPAddr + reinterpret_cast<IMAGE_DOS_HEADER*>(dwSAMPAddr)->e_lfanew);
			uintptr_t ep = ntheader->OptionalHeader.AddressOfEntryPoint;
			switch (ep)
			{
			case 0x31DF13:	orig_ProcessInput = (hookedProcessInput_t)(dwSAMPAddr + 0x65D30); break; // R1
			case 0xCC4D0:	orig_ProcessInput = (hookedProcessInput_t)(dwSAMPAddr + 0x69260); break; // R3
			case 0xCBCB0:	orig_ProcessInput = (hookedProcessInput_t)(dwSAMPAddr + 0x69990); break; // R4
			default:
				{
					MessageBoxA(NULL, "Unknown SA-MP version.", "CommandLayoutChanger.asi", MB_OK | MB_ICONERROR);
					return false;
				}
				break;
			}

			layout_change_mode = GetPrivateProfileInt("main", "change-mode", 1, ".\\CommandLayoutChanger.ini");
			if (layout_change_mode > 3)
				layout_change_mode = 1;

			WritePrivateProfileString("main", "change-mode", std::to_string(layout_change_mode).c_str(), ".\\CommandLayoutChanger.ini");

			MH_Initialize();
			MH_CreateHookEx((void*)orig_ProcessInput, &hooked_ProcessInput, &orig_ProcessInput);
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			MH_RemoveHook((void*)orig_ProcessInput);
			MH_Uninitialize();
		}
		break;
	}
	return true;
}
