#include "PreventClickingCameraOccludersCavehook.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Camera.hpp"

// The hook overrides a lea and a call instruction, because the call instruction is so short that it cannot be hooked.
// The call instruction should be overriden, so not executed at all, but the lea instruction must be executed,
// because it prepares a function argument.
// The shadow space that DKUtil allocates is not necessary here, it messes up the arguments on the stack.
// So reverse it through add rsp and sub rsp.
struct PreventClickingCameraOccludersProlog : Xbyak::CodeGenerator
{
    PreventClickingCameraOccludersProlog()
    {
        lea(rdx, ptr[rdi + 0x0C]);
        add(rsp, 32);
    }
};

struct PreventClickingCameraOccludersEpilog : Xbyak::CodeGenerator
{
    PreventClickingCameraOccludersEpilog() { sub(rsp, 32); }
};

void PreventClickingCameraOccludersCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    PreventClickingCameraOccludersProlog prolog;
    prolog.ready();
    PreventClickingCameraOccludersEpilog epilog;
    epilog.ready();
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 7 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kSkipNOP);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

bool PreventClickingCameraOccludersCavehook::Func(int64_t* self, Vector3* start, Vector3* end,
    float* results, int32_t flags, int32_t mask1, int32_t mask2, int32_t option1, int32_t option2,
    int32_t option3, int64_t* options)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    auto vtable = *reinterpret_cast<void***>(self);
    auto OriginalFunc = reinterpret_cast<decltype(Func)*>(vtable[0x40 / 8]);

    // The threshold compares against zoom * pitch.
    // So if the camera is close, a steep 45 degree angle is required for objects to be clickable.
    // If the camera is far, a smaller angle is fine.
    // This hopefully avoids missclicks, but allows to click objects behind the character when appropriate.
    auto constexpr THRESHOLD = 300.0f;

    auto* camera = state->GetCamera();

    if ((!camera && state->prevented_camera_occluders_last_frame) ||
        (*settings->prevent_clicking_camera_occluders && state->IsCharacterMovementMode() &&
            camera && state->last_valid_camera_zoom * camera->currentPitch_164 < THRESHOLD))
    {
        state->prevented_camera_occluders_last_frame = true;
        Vector3 delta = *end - *start;
        const float length = delta.Length();
        Vector3 dir = delta / length;
        Vector3 start_copy = *start + dir * state->last_valid_camera_zoom;
        return OriginalFunc(self, &start_copy, end, results, flags, mask1, mask2, option1, option2,
            option3, options);
    }
    else
    {
        state->prevented_camera_occluders_last_frame = false;
        return OriginalFunc(self, start, end, results, flags, mask1, mask2, option1, option2,
            option3, options);
    }
}
