#include "FeatureSet.hpp"
#include "Hooks/AfterChangingKeybindInMenuHook.hpp"
#include "Hooks/AfterInitialLoadInputConfigHook.hpp"
#include "Hooks/BlockAnalogStickSelection2Patch.hpp"
#include "Hooks/BlockAnalogStickSelectionPatch.hpp"
#include "Hooks/BlockCameraInputCavehook.hpp"
#include "Hooks/BlockCancelActionStoppingMovementPatch.hpp"
#include "Hooks/BlockHoldInteractMovePatch.hpp"
#include "Hooks/BlockInteractMovePatch.hpp"
#include "Hooks/CallSpecificCommandFunctionPre2Cavehook.hpp"
#include "Hooks/CastOrCancelAbilityHook.hpp"
#include "Hooks/CheckCommandInputsHook.hpp"
#include "Hooks/CheckContextMenuOrCancelActionHook.hpp"
#include "Hooks/ConcatInputconfigPathHook.hpp"
#include "Hooks/DecideMoveUpdaterHook.hpp"
#include "Hooks/FTBEndHook.hpp"
#include "Hooks/FTBStartHook.hpp"
#include "Hooks/GetInputValueHook.hpp"
#include "Hooks/InsideUpdateInteractMoveCavehook.hpp"
#include "Hooks/IsInControllerModeHook.hpp"
#include "Hooks/LoadInputConfigHook.hpp"
#include "Hooks/PollEventHook.hpp"
#include "Hooks/ResetCursorRotateHook.hpp"
#include "Hooks/SDL_GetWindowGrabHook.hpp"
#include "Hooks/SetCursorRotateHook.hpp"
#include "Hooks/SetVirtualCursorPosHook.hpp"
#include "Hooks/UpdateCameraHook.hpp"
#include "Hooks/WASDUnlockPatch.hpp"
#include "Hooks/WindowGainFocusHook.hpp"
#include "InputHook.hpp"
#include "InputconfigPatcher.hpp"
#include "MessageBox.hpp"
#include "SDL.h"
#include "Settings.hpp"
#include "State.hpp"
#include "VersionInfo.hpp"

BOOL APIENTRY DllMain(HMODULE a_hModule, DWORD a_ul_reason_for_call, LPVOID a_lpReserved)
{
    if (a_ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
#ifndef NDEBUG
        while (!IsDebuggerPresent())
        {
            Sleep(100);
        }
#endif
        dku::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
        dku::Hook::Trampoline::AllocTrampoline(1 << 9);

        VersionInfo::Print(a_hModule);

        SDL_version linked;
        SDL_GetVersion(&linked);
        if (linked.major != 2 || linked.minor != 28)
        {
            FATAL(
                "SDL2.dll version mismatch. This usually means that you must re-install the mod, "
                "or at least the SDL2.dll!");
        }
        // With SDL 2.24 and spam-clicking Rotate I could bug the cursor so that it would
        // warp to the center of the screen.
        // Using SDL 2.28 and this Hint fixes this issue.
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");

        auto* settings = Settings::GetSingleton();
        settings->Load();

        FeatureSet core{ "WASD Core" };
        core.AddRequiredHook(WASDUnlockPatch::Get());
        core.AddRequiredHook(BlockCameraInputCavehook::Get());
        core.AddRequiredHook(UpdateCameraHook::Get());
        core.AddRequiredHook(GetInputValueHook::Get());
        core.AddRequiredHook(IsInControllerModeHook::Get());
        core.AddRequiredHook(LoadInputConfigHook::Get());
        core.AddRequiredHook(AfterChangingKeybindInMenuHook::Get());
        core.AddRequiredHook(AfterInitialLoadInputConfigHook::Get());
        core.AddRequiredHook(ConcatInputconfigPathHook::Get());
        core.AddRequiredHook(BlockAnalogStickSelectionPatch::Get());
        core.AddRequiredHook(BlockAnalogStickSelection2Patch::Get());

        FeatureSet toggle_on_ftb{ "Auto toggling movement mode on FTB" };
        toggle_on_ftb.AddRequiredHook(FTBStartHook::Get());
        toggle_on_ftb.AddRequiredHook(FTBEndHook::Get());
        core.AddSubFeatureSet(toggle_on_ftb);

        FeatureSet improved_mouselook{ "Improved mouselook" };
        improved_mouselook.AddRequiredHook(SetVirtualCursorPosHook::Get());
        improved_mouselook.AddRequiredHook(SDL_GetWindowGrabHook::Get());
        improved_mouselook.AddRequiredHook(SetCursorRotateHook::Get());
        improved_mouselook.AddRequiredHook(ResetCursorRotateHook::Get());
        improved_mouselook.AddRequiredHook(CheckContextMenuOrCancelActionHook::Get());
        improved_mouselook.AddRequiredHook(CastOrCancelAbilityHook::Get());
        improved_mouselook.AddRequiredHook(PollEventHook::Get());
        improved_mouselook.AddRequiredHook(CheckCommandInputsHook::Get());
        core.AddSubFeatureSet(improved_mouselook);

        FeatureSet interactmove_blocker{ "InteractMove Blocker" };
        interactmove_blocker.AddRequiredHook(DecideMoveUpdaterHook::Get());
        interactmove_blocker.AddRequiredHook(InsideUpdateInteractMoveCavehook::Get());
        interactmove_blocker.AddRequiredHook(BlockInteractMovePatch::Get());
        interactmove_blocker.AddRequiredHook(BlockHoldInteractMovePatch::Get());
        interactmove_blocker.AddRequiredHook(CallSpecificCommandFunctionPre2Cavehook::Get());
        core.AddSubFeatureSet(interactmove_blocker);

        auto [core_is_valid, errors] = core.EnableRecursively();

        if (core_is_valid)
        {
            InputHook::Enable(a_hModule);
        }

        if (!errors.empty())
        {
            errors.append("Your game version is not completely compatible with the mod version!\n");
            WARN(errors);
            MessageBox::Show(errors);
        }
        else
        {
            INFO("WASD Mod started without errors.");
        }
    }

    return TRUE;
}
