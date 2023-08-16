
using namespace DKUtil::Alias;

namespace
{
	void Patches_Commit()
	{
		static constexpr uint8_t data[0x6] = { 0x31, 0xC0, 0xFF, 0xC0, 0x90, 0x90 };

		auto match1 = dku::Hook::Assembly::search_pattern<
			"38 ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 32 C0 48 8B ?? ?? ?? 48 8B ?? ?? ?? 0F ?? ?? ?? ?? 48 ?? ?? ?? 5F C3">();
		auto addr1 = AsAddress(match1);

		if (addr1) {
			dku::Hook::WriteImm(addr1, data);
			INFO("WASD unlocked : {:X}", addr1);
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
