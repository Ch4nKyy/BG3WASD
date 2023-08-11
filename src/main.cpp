
using namespace DKUtil::Alias;

namespace
{
	void Patch1_Commit()
	{
		auto* scan = dku::Hook::Assembly::search_pattern<
			"38 05 ?? ?? ?? ?? 0F 84 7D 10 00 00 48 8B 4E 68 8B 05 ?? ?? ?? ?? 39 81 40 01 00 00 0F 84 67 10 00 00 48 8B 99 B8 00 00 00 48 8B CB">();
		auto addr = AsAddress(scan);

		if (scan) {
			static constexpr uint8_t data[0xC] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
			dku::Hook::WriteImm(addr, data);
			INFO("patch 1 committed : {:X}", addr)
		}
	}

	void Patch2_Commit()
	{
		auto* scan = dku::Hook::Assembly::search_pattern<
			"38 05 ?? ?? ?? ?? 0F 84 44 01 00 00 48 8B 4B 68 8B 05 ?? ?? 64 04 39 81 40 01 00 00 0F 84 2E 01 00 00 48 8B B9 B8 00 00 00 48 8B CF E8 ?? E5 95 FF">();
		auto addr = AsAddress(scan);

		if (scan) {
			static constexpr uint8_t data[0xC] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
			dku::Hook::WriteImm(addr, data);
			INFO("patch 2 committed : {:X}", addr)
		}
	}
}

BOOL APIENTRY DllMain(HMODULE a_hModule, DWORD a_ul_reason_for_call, LPVOID a_lpReserved)
{
	if (a_ul_reason_for_call == DLL_PROCESS_ATTACH) {
#ifndef NDEBUG
		while (!IsDebuggerPresent()) {
			Sleep(100);
		}
#endif

		// stuff
		dku::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));

		INFO("game type : {}", dku::Hook::GetProcessName())

		Patch1_Commit();
		Patch2_Commit();
	}

	return TRUE;
}
