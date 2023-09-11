#pragma once

class VersionInfo
{
public:
    static void Print(HMODULE a_hModule);

public:
    static void PrintModVersion(HMODULE a_hModule);
    static void PrintGameVersion();
    static void PrintModuleVersion(std::wstring file_name);
};
