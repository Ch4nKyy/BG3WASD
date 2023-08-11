
using namespace DKUtil::Alias;

namespace
{
	void* Search(uintptr_t a_base = 0)
	{
		return dku::Hook::Assembly::search_pattern<"38 05 ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 48 8B ?? ?? 8B 05 ?? ?? ?? ?? 39 81 ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 8B">(a_base);
	}

	void Patches_Commit()
	{
		static constexpr uint8_t data[0xC] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

		auto firstMatch = Search();
		auto addr1 = AsAddress(firstMatch);

		auto secondMatch = Search(addr1 + 1);
		auto addr2 = AsAddress(secondMatch);

		if (addr1 && addr2) {
			dku::Hook::WriteImm(addr1, data);
			INFO("patch 1 committed : {:X}", addr1)

			dku::Hook::WriteImm(addr2, data);
			INFO("patch 2 committed : {:X}", addr2)
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

		Patches_Commit();
	}

	return TRUE;
}
