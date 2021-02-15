#include "main.h"

uint32_t layout_change_mode = 1;

std::map<wchar_t, wchar_t> rus_table =
{
	// Russian to English
	{L'¨', L'~'}, {L'\"', L'@'}, {L'¹', L'#'}, {L';', L'$'},
	{L':', L'^'}, {L'?', L'&'}, {L'É', L'Q'}, {L'Ö', L'W'},
	{L'Ó', L'E'}, {L'Ê', L'R'}, {L'Å', L'T'}, {L'Í', L'Y'},
	{L'Ã', L'U'}, {L'Ø', L'I'}, {L'Ù', L'O'}, {L'Ç', L'P'},
	{L'Õ', L'{'}, {L'Ú', L'}'}, {L'/', L'|'}, {L'Ô', L'A'},
	{L'Û', L'S'}, {L'Â', L'D'}, {L'À', L'F'}, {L'Ï', L'G'},
	{L'Ð', L'H'}, {L'Î', L'J'}, {L'Ë', L'K'}, {L'Ä', L'L'},
	{L'Æ', L':'}, {L'Ý', L'\"'}, {L'ß', L'Z'}, {L'×', L'X'},
	{L'Ñ', L'C'}, {L'Ì', L'V'}, {L'È', L'B'}, {L'Ò', L'N'},
	{L'Ü', L'M'}, {L'Á', L'<'}, {L'Þ', L'>'}, {L',', L'?'},
	{L'¸', L'`'}, {L'é', L'q'}, {L'ö', L'w'}, {L'ó', L'e'},
	{L'ê', L'r'}, {L'å', L't'}, {L'í', L'y'}, {L'ã', L'u'},
	{L'ø', L'i'}, {L'ù', L'o'}, {L'ç', L'p'}, {L'õ', L'['},
	{L'ú', L']'}, {L'ô', L'a'}, {L'û', L's'}, {L'â', L'd'},
	{L'à', L'f'}, {L'ï', L'g'}, {L'ð', L'h'}, {L'î', L'j'},
	{L'ë', L'k'}, {L'ä', L'l'}, {L'æ', L';'}, {L'ý', L'\''},
	{L'ÿ', L'z'}, {L'÷', L'x'}, {L'ñ', L'c'}, {L'ì', L'v'},
	{L'è', L'b'}, {L'ò', L'n'}, {L'ü', L'm'}, {L'á', L','},
	{L'þ', L'.'}, {L'.', L'/'}
};

std::map<wchar_t, wchar_t> eng_table =
{
	// English to Russian
	{L'~', L'¨'}, {L'@', L'\"'}, {L'#', L'¹'}, {L'$', L';'},
	{L'^', L':'}, {L'&', L'?'}, {L'Q', L'É'}, {L'W', L'Ö'},
	{L'E', L'Ó'}, {L'R', L'Ê'}, {L'T', L'Å'}, {L'Y', L'Í'},
	{L'U', L'Ã'}, {L'I', L'Ø'}, {L'O', L'Ù'}, {L'P', L'Ç'},
	{L'{', L'Õ'}, {L'}', L'Ú'}, {L'|', L'/'}, {L'A', L'Ô'},
	{L'S', L'Û'}, {L'D', L'Â'}, {L'F', L'À'}, {L'G', L'Ï'},
	{L'H', L'Ð'}, {L'J', L'Î'}, {L'K', L'Ë'}, {L'L', L'Ä'},
	{L':', L'Æ'}, {L'\"', L'Ý'}, {L'Z', L'ß'}, {L'X', L'×'},
	{L'C', L'Ñ'}, {L'V', L'Ì'}, {L'B', L'È'}, {L'N', L'Ò'},
	{L'M', L'Ü'}, {L'<', L'Á'}, {L'>', L'Þ'}, {L'?', L','},
	{L'`', L'¸'}, {L'q', L'é'}, {L'w', L'ö'}, {L'e', L'ó'},
	{L'r', L'ê'}, {L't', L'å'}, {L'y', L'í'}, {L'u', L'ã'},
	{L'i', L'ø'}, {L'o', L'ù'}, {L'p', L'ç'}, {L'[', L'õ'},
	{L']', L'ú'}, {L'a', L'ô'}, {L's', L'û'}, {L'd', L'â'},
	{L'f', L'à'}, {L'g', L'ï'}, {L'h', L'ð'}, {L'j', L'î'},
	{L'k', L'ë'}, {L'l', L'ä'}, {L';', L'æ'}, {L'\'', L'ý'},
	{L'z', L'ÿ'}, {L'x', L'÷'}, {L'c', L'ñ'}, {L'v', L'ì'},
	{L'b', L'è'}, {L'n', L'ò'}, {L'm', L'ü'}, {L',', L'á'},
	{L'.', L'þ'}, {L'/', L'.'}
};

void ChangeLayout(wchar_t *in, wchar_t break_symbol = L'\0', bool reverse = false)
{
	bool now_russian = true;
	do
	{
		if (*in == break_symbol)
			return;

		if (reverse)
		{
			if (*in >= L'À' && *in <= L'ÿ')
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
			if (input_box->m_pwszBuffer[1] >= L'À' && input_box->m_pwszBuffer[1] <= L'ÿ')
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
			if (layout_change_mode < 0 || layout_change_mode > 3)
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
