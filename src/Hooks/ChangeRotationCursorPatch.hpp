#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct ChangeRotationCursorPatchAsm : Xbyak::CodeGenerator
{
    ChangeRotationCursorPatchAsm()
    {
        db(0x48);
        db(0x8D);
        db(0x05);
        db(0x68);
        db(0xF9);
        db(0x6D);
        db(0x04);
    }
};

// Caller: UpdateCursor
// Target: Line that gets the rotation cursor.
// Thread: GameThread
// Called when: camera rotation is active.
// Purpose: Change the rotation cursor from the usual icon to the system cursor,
//          which we overrode with a crosshair in SDL_CreateSystemCursorHook.
class ChangeRotationCursorPatch : public PatchHook
{
public:
    ChangeRotationCursorPatch() :
        PatchHook({ search_pattern<"48 8D 05 08 FA 6D 04">() }, { 0 },
            std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static ChangeRotationCursorPatch& Get()
    {
        static ChangeRotationCursorPatch instance;
        return instance;
    }
};
