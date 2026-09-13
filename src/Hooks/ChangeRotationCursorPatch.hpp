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
        PatchHook(
            { search_pattern<"8B FB 4C 39 04 F9 74 ?? 48 8B 46 10 8B 1C B8 85 DB 79 ?? EB ?? 8B D3 "
                             "48 8B CE E8 ?? ?? ?? ?? 48 63 46 2C 3B D8 7D ?? 48 8B 4E 30">() },
            { 26 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static ChangeRotationCursorPatch& Get()
    {
        static ChangeRotationCursorPatch instance;
        return instance;
    }
};
