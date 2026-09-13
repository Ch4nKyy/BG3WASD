#include "FeatureSet.hpp"
#include "Hooks/ActiveCharacterChangedCavehook.hpp"
#include "Hooks/AfterChangingKeybindInMenuHook.hpp"
#include "Hooks/AfterInitialLoadInputConfigHook.hpp"
#include "Hooks/AutoRotateUnlockPatch.hpp"
#include "Hooks/BlockAnalogStickSelection2Patch.hpp"
#include "Hooks/BlockAnalogStickSelectionPatch.hpp"
#include "Hooks/BlockCancelActionStoppingMovementPatch.hpp"
#include "Hooks/BlockEdgePanningHook.hpp"
#include "Hooks/BlockHoldInteractMovePatch.hpp"
#include "Hooks/BlockInteractMovePatch.hpp"
#include "Hooks/CameraRotationOffHook.hpp"
#include "Hooks/CameraRotationOnHook.hpp"
#include "Hooks/ChangeRotationCursorPatch.hpp"
#include "Hooks/ConcatInputconfigPathHook.hpp"
#include "Hooks/ContextMenuClosedHook.hpp"
#include "Hooks/ContextMenuOpenedHook.hpp"
#include "Hooks/DecideMoveUpdaterHook.hpp"
#include "Hooks/DialogStatusChangedHook.hpp"
#include "Hooks/ExecuteCameraCommandHook.hpp"
#include "Hooks/ExecuteCastAbilityCommandHook.hpp"
#include "Hooks/ExecuteOpenContextMenuCommandHook.hpp"
#include "Hooks/FTBEndHook.hpp"
#include "Hooks/FTBStartHook.hpp"
#include "Hooks/HandleCameraCommandCavehook.hpp"
#include "Hooks/HandleCharacterCommandCavehook.hpp"
#include "Hooks/InsideUpdateInteractMoveCavehook.hpp"
#include "Hooks/IsInControllerModeGlobalRef.hpp"
#include "Hooks/LoadInputConfigFunctionRef.hpp"
#include "Hooks/LootWindowClosedHook.hpp"
#include "Hooks/LootWindowOpenedHook.hpp"
#include "Hooks/MenuClosedCavehook.hpp"
#include "Hooks/MenuOpenedCavehook.hpp"
#include "Hooks/OverrideMovementInputHook.hpp"
#include "Hooks/PhotomodeClosedCavehook.hpp"
#include "Hooks/PhotomodeOpenedCavehook.hpp"
#include "Hooks/PollEventHook.hpp"
#include "Hooks/PreventClickingActiveCharacterHook.hpp"
#include "Hooks/PreventClickingCameraOccludersCavehook.hpp"
#include "Hooks/PreventLosingChaseCamTargetHook.hpp"
#include "Hooks/SDL_CreateSystemCursorHook.hpp"
#include "Hooks/SDL_GetWindowGrabHook.hpp"
#include "Hooks/SetVirtualCursorPosHook.hpp"
#include "Hooks/TickInputSystemHook.hpp"
#include "Hooks/UpdateCameraHook.hpp"
#include "Hooks/WASDUnlockPatch.hpp"
#include "Hooks/WindowGainFocusCavehook.hpp"
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
        dku::Hook::Trampoline::AllocTrampoline(1 << 12);

        VersionInfo::Print(a_hModule);

        FeatureSet core{ "WASD Core" };
        core.AddRequiredHook(WASDUnlockPatch::Get());
        core.AddRequiredHook(UpdateCameraHook::Get());
        core.AddRequiredHook(OverrideMovementInputHook::Get());
        core.AddRequiredHook(IsInControllerModeGlobalRef::Get());
        core.AddRequiredHook(LoadInputConfigFunctionRef::Get());
        core.AddRequiredHook(AfterChangingKeybindInMenuHook::Get());
        core.AddRequiredHook(AfterInitialLoadInputConfigHook::Get());
        core.AddRequiredHook(ConcatInputconfigPathHook::Get());
        core.AddRequiredHook(BlockAnalogStickSelectionPatch::Get());
        core.AddRequiredHook(BlockAnalogStickSelection2Patch::Get());
        core.AddRequiredHook(ContextMenuClosedHook::Get());
        core.AddRequiredHook(ContextMenuOpenedHook::Get());
        core.AddRequiredHook(MenuClosedCavehook::Get());
        core.AddRequiredHook(MenuOpenedCavehook::Get());
        core.AddRequiredHook(DialogStatusChangedHook::Get());
        core.AddRequiredHook(HandleCharacterCommandCavehook::Get());
        core.AddRequiredHook(HandleCameraCommandCavehook::Get());

        FeatureSet unclickable_character{ "Unclickable active character" };
        unclickable_character.AddRequiredHook(ActiveCharacterChangedCavehook::Get());
        unclickable_character.AddRequiredHook(PreventClickingActiveCharacterHook::Get());
        unclickable_character.AddRequiredHook(PreventClickingCameraOccludersCavehook::Get());
        core.AddSubFeatureSet(unclickable_character);

        FeatureSet autorotate{ "Auto rotate camera to follow character if enabled" };
        autorotate.AddRequiredHook(AutoRotateUnlockPatch::Get());
        core.AddSubFeatureSet(autorotate);

        FeatureSet toggle_on_ftb{ "Auto toggling movement mode on FTB" };
        toggle_on_ftb.AddRequiredHook(FTBStartHook::Get());
        toggle_on_ftb.AddRequiredHook(FTBEndHook::Get());
        core.AddSubFeatureSet(toggle_on_ftb);

        FeatureSet camera_rotation{ "Extended camera rotation features" };
        camera_rotation.AddRequiredHook(SetVirtualCursorPosHook::Get());
        camera_rotation.AddRequiredHook(SDL_CreateSystemCursorHook::Get());
        camera_rotation.AddRequiredHook(SDL_GetWindowGrabHook::Get());
        camera_rotation.AddRequiredHook(CameraRotationOnHook::Get());
        camera_rotation.AddRequiredHook(CameraRotationOffHook::Get());
        camera_rotation.AddRequiredHook(ExecuteCastAbilityCommandHook::Get());
        camera_rotation.AddRequiredHook(PollEventHook::Get());
        camera_rotation.AddRequiredHook(TickInputSystemHook::Get());
        camera_rotation.AddRequiredHook(ChangeRotationCursorPatch::Get());
        camera_rotation.AddRequiredHook(ExecuteCameraCommandHook::Get());
        camera_rotation.AddRequiredHook(WindowGainFocusCavehook::Get());
        camera_rotation.AddRequiredHook(BlockCancelActionStoppingMovementPatch::Get());
        camera_rotation.AddRequiredHook(PhotomodeClosedCavehook::Get());
        camera_rotation.AddRequiredHook(PhotomodeOpenedCavehook::Get());
        camera_rotation.AddRequiredHook(BlockEdgePanningHook::Get());
        camera_rotation.AddRequiredHook(PreventLosingChaseCamTargetHook::Get());
        camera_rotation.AddRequiredHook(LootWindowOpenedHook::Get());
        camera_rotation.AddRequiredHook(LootWindowClosedHook::Get());
        camera_rotation.AddRequiredHook(ExecuteOpenContextMenuCommandHook::Get());
        core.AddSubFeatureSet(camera_rotation);

        FeatureSet interactmove_blocker{ "InteractMove Blocker" };
        interactmove_blocker.AddRequiredHook(DecideMoveUpdaterHook::Get());
        interactmove_blocker.AddRequiredHook(InsideUpdateInteractMoveCavehook::Get());
        interactmove_blocker.AddRequiredHook(BlockInteractMovePatch::Get());
        interactmove_blocker.AddRequiredHook(BlockHoldInteractMovePatch::Get());
        core.AddSubFeatureSet(interactmove_blocker);

        auto [core_is_valid, errors] = core.EnableRecursively();

        auto* settings = Settings::GetSingleton();
        settings->Load();

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
