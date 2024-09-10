#include "BlockCameraInputCavehook.hpp"
#include "Addresses/IsInControllerMode.hpp"
#include "Settings.hpp"
#include "State.hpp"

struct BlockCameraInputProlog : Xbyak::CodeGenerator
{
    BlockCameraInputProlog()
    {
        push(rax);
        push(rbx);
        push(rcx);
        push(rdx);
    }
};

struct BlockCameraInputEpilog : Xbyak::CodeGenerator
{
    BlockCameraInputEpilog()
    {
        mov(r12b, al);
        mov(ptr[rdi + 324], r12b);
        pop(rdx);
        pop(rcx);
        pop(rbx);
        pop(rax);
    }
};

bool BlockCameraInputCavehook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(
        dku::Hook::Assembly::search_pattern<"44 88 A7 44 01 00 00">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("BlockCameraInputCavehook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void BlockCameraInputCavehook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        BlockCameraInputProlog prolog;
        prolog.ready();
        BlockCameraInputEpilog epilog;
        epilog.ready();
        handle = DKUtil::Hook::AddCaveHook(address, { 0, 7 }, FUNC_INFO(Func), &prolog, &epilog,
            DKUtil::Hook::HookFlag::kSkipNOP);
        handle->Enable();
        DEBUG("Hooked BlockCameraInputCavehook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Write 1 or 0 into rax. Then, write al into r12b. Then, write r12b into [rdi+324], which is the
// original behaviour of the hooked line. rdi is the camera object. 324 is "should_move".
//
// In the past, we also set the movement input to 0 (cam + 148 and cam + 152). This blocked
// camera movement in combat, when character movement mode was active. But not blocking it, is
// actually better. And we don't need to get the camera object.
bool BlockCameraInputCavehook::Func()
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (IsInControllerMode::Read())
    {
        return 1;
    }

    return !state->IsCharacterMovementMode();
}
