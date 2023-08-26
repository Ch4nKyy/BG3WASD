class ProcPtr
{
public:
    explicit ProcPtr(FARPROC ptr) : _ptr(ptr) {}

    template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
    operator T*() const
    {
        return reinterpret_cast<T*>(_ptr);
    }

private:
    FARPROC _ptr;
};

class DllHelper
{
public:
    explicit DllHelper(LPCWSTR filename) : _module(GetModuleHandle(filename)) {}

    ~DllHelper() {}

    ProcPtr operator[](LPCSTR proc_name) const
    {
        return ProcPtr(GetProcAddress(_module, proc_name));
    }

    static HMODULE _parent_module;

private:
    HMODULE _module;
};

using SDL_SetRelativeMouseMode_t = std::add_pointer_t<int(bool enabled)>;
using SDL_GetWindowSize_t = std::add_pointer_t<void(int64_t window, int* x, int* y)>;
using SDL_WarpMouseInWindow_t = std::add_pointer_t<void(int64_t window, int x, int y)>;

class SdlWrapper
{
    DllHelper _dll{ L"SDL2.dll" };

public:
    SDL_SetRelativeMouseMode_t SetRelativeMouseMode = _dll["SDL_SetRelativeMouseMode"];
    SDL_GetWindowSize_t GetWindowSize = _dll["SDL_GetWindowSize"];
    SDL_WarpMouseInWindow_t WarpMouseInWindow = _dll["SDL_WarpMouseInWindow"];
};
