#define WIN32_LEAN_AND_MEAN
#define INITGUID
#include <windows.h>
#include <objbase.h>
#include <servprov.h>

static const GUID CLSID_ImmersiveShell =
    {0xC2F03A33, 0x21F5, 0x47FA, {0xB4, 0xBB, 0x15, 0x63, 0x62, 0xA2, 0xF2, 0x39}};

static const GUID CLSID_VirtualDesktopManagerInternal =
    {0xC5E0CDCA, 0x7B6E, 0x41B2, {0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B}};

static const GUID IID_IVirtualDesktopManagerInternal =
    {0x53F5CA0B, 0x158F, 0x4124, {0x90, 0x0C, 0x05, 0x71, 0x58, 0x06, 0x0B, 0x27}};

static const GUID IID_IVirtualDesktop =
    {0x3F07F4BE, 0xB107, 0x441A, {0xAF, 0x0F, 0x39, 0xD8, 0x25, 0x29, 0x07, 0x2C}};

static const GUID IID_IApplicationViewCollection =
    {0x1841C6D7, 0x4F9D, 0x42C0, {0xAF, 0x41, 0x87, 0x47, 0x53, 0x8F, 0x10, 0xE5}};

#define HOTKEY_ID_BASE    100
#define HOTKEY_ID_MOVE    200
#define HOTKEY_ID_PREV    300
#define HOTKEY_ID_NEXT    301
#define HOTKEY_ID_MOVE_PREV  400
#define HOTKEY_ID_MOVE_NEXT  401
#define HOTKEY_ID_EXIT       500
#define HOTKEY_ID_NEW        600
#define HOTKEY_ID_REMOVE     601

#define TD_OK_BUTTON      0x0001
#define TD_INFO_ICON      MAKEINTRESOURCEW(-3)

typedef struct IObjectArrayVtbl IObjectArrayVtbl;
typedef struct IObjectArray {
    const IObjectArrayVtbl *lpVtbl;
} IObjectArray;

struct IObjectArrayVtbl {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IObjectArray*, REFIID, void**);
    ULONG   (STDMETHODCALLTYPE *AddRef)(IObjectArray*);
    ULONG   (STDMETHODCALLTYPE *Release)(IObjectArray*);
    HRESULT (STDMETHODCALLTYPE *GetCount)(IObjectArray*, UINT*);
    HRESULT (STDMETHODCALLTYPE *GetAt)(IObjectArray*, UINT, REFIID, void**);
};

typedef struct IVirtualDesktopVtbl IVirtualDesktopVtbl;
typedef struct IVirtualDesktop {
    const IVirtualDesktopVtbl *lpVtbl;
} IVirtualDesktop;

struct IVirtualDesktopVtbl {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IVirtualDesktop*, REFIID, void**);
    ULONG   (STDMETHODCALLTYPE *AddRef)(IVirtualDesktop*);
    ULONG   (STDMETHODCALLTYPE *Release)(IVirtualDesktop*);
    HRESULT (STDMETHODCALLTYPE *IsViewVisible)(IVirtualDesktop*, void*, BOOL*);
    HRESULT (STDMETHODCALLTYPE *GetId)(IVirtualDesktop*, GUID*);
    HRESULT (STDMETHODCALLTYPE *GetName)(IVirtualDesktop*, void**);
    HRESULT (STDMETHODCALLTYPE *GetWallpaper)(IVirtualDesktop*, void**);
};

typedef struct IVirtualDesktopManagerInternalVtbl IVirtualDesktopManagerInternalVtbl;
typedef struct IVirtualDesktopManagerInternal {
    const IVirtualDesktopManagerInternalVtbl *lpVtbl;
} IVirtualDesktopManagerInternal;

struct IVirtualDesktopManagerInternalVtbl {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IVirtualDesktopManagerInternal*, REFIID, void**);
    ULONG   (STDMETHODCALLTYPE *AddRef)(IVirtualDesktopManagerInternal*);
    ULONG   (STDMETHODCALLTYPE *Release)(IVirtualDesktopManagerInternal*);
    HRESULT (STDMETHODCALLTYPE *GetCount)(IVirtualDesktopManagerInternal*, UINT*);
    HRESULT (STDMETHODCALLTYPE *MoveViewToDesktop)(IVirtualDesktopManagerInternal*, void*, IVirtualDesktop*);
    HRESULT (STDMETHODCALLTYPE *CanMoveViewBetweenDesktops)(IVirtualDesktopManagerInternal*, void*, BOOL*);
    HRESULT (STDMETHODCALLTYPE *GetCurrentDesktop)(IVirtualDesktopManagerInternal*, IVirtualDesktop**);
    HRESULT (STDMETHODCALLTYPE *GetDesktops)(IVirtualDesktopManagerInternal*, IObjectArray**);
    HRESULT (STDMETHODCALLTYPE *GetAdjacentDesktop)(IVirtualDesktopManagerInternal*, IVirtualDesktop*, UINT, IVirtualDesktop**);
    HRESULT (STDMETHODCALLTYPE *SwitchDesktop)(IVirtualDesktopManagerInternal*, IVirtualDesktop*);
    HRESULT (STDMETHODCALLTYPE *SwitchDesktopAndMoveForegroundView)(IVirtualDesktopManagerInternal*, IVirtualDesktop*);
    HRESULT (STDMETHODCALLTYPE *CreateDesktop)(IVirtualDesktopManagerInternal*, IVirtualDesktop**);
    HRESULT (STDMETHODCALLTYPE *MoveDesktop)(IVirtualDesktopManagerInternal*, IVirtualDesktop*, UINT);
    HRESULT (STDMETHODCALLTYPE *RemoveDesktop)(IVirtualDesktopManagerInternal*, IVirtualDesktop*, IVirtualDesktop*);
    HRESULT (STDMETHODCALLTYPE *FindDesktop)(IVirtualDesktopManagerInternal*, const GUID*, IVirtualDesktop**);
    HRESULT (STDMETHODCALLTYPE *GetDesktopSwitchIncludeExcludeViews)(IVirtualDesktopManagerInternal*, IVirtualDesktop*, IObjectArray*, IObjectArray*);
    HRESULT (STDMETHODCALLTYPE *SetName)(IVirtualDesktopManagerInternal*, IVirtualDesktop*, const WCHAR*);
    HRESULT (STDMETHODCALLTYPE *SetWallpaper)(IVirtualDesktopManagerInternal*, IVirtualDesktop*, const WCHAR*);
    HRESULT (STDMETHODCALLTYPE *UpdateWallpaperForAll)(IVirtualDesktopManagerInternal*, const WCHAR*);
};

typedef struct IApplicationViewVtbl IApplicationViewVtbl;
typedef struct IApplicationView {
    const IApplicationViewVtbl *lpVtbl;
} IApplicationView;

struct IApplicationViewVtbl {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IApplicationView*, REFIID, void**);
    ULONG   (STDMETHODCALLTYPE *AddRef)(IApplicationView*);
    ULONG   (STDMETHODCALLTYPE *Release)(IApplicationView*);
};

typedef struct IApplicationViewCollectionVtbl IApplicationViewCollectionVtbl;
typedef struct IApplicationViewCollection {
    const IApplicationViewCollectionVtbl *lpVtbl;
} IApplicationViewCollection;

struct IApplicationViewCollectionVtbl {
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IApplicationViewCollection*, REFIID, void**);
    ULONG   (STDMETHODCALLTYPE *AddRef)(IApplicationViewCollection*);
    ULONG   (STDMETHODCALLTYPE *Release)(IApplicationViewCollection*);
    HRESULT (STDMETHODCALLTYPE *GetViews)(IApplicationViewCollection*, IObjectArray**);
    HRESULT (STDMETHODCALLTYPE *GetViewsByZOrder)(IApplicationViewCollection*, IObjectArray**);
    HRESULT (STDMETHODCALLTYPE *GetViewsByAppUserModelID)(IApplicationViewCollection*, const WCHAR*, IObjectArray**);
    HRESULT (STDMETHODCALLTYPE *GetViewForHwnd)(IApplicationViewCollection*, HWND, IApplicationView**);
};

static IServiceProvider              *g_provider = NULL;
static IVirtualDesktopManagerInternal *g_manager = NULL;
static IApplicationViewCollection    *g_view_collection = NULL;

static HRESULT ensure_manager(void)
{
    if (g_manager) return S_OK;

    HRESULT hr = CoCreateInstance(&CLSID_ImmersiveShell, NULL, CLSCTX_LOCAL_SERVER,
                                  &IID_IServiceProvider, (void**)&g_provider);
    if (FAILED(hr)) return hr;

    hr = g_provider->lpVtbl->QueryService(g_provider,
        &CLSID_VirtualDesktopManagerInternal,
        &IID_IVirtualDesktopManagerInternal,
        (void**)&g_manager);
    return hr;
}

static HRESULT ensure_view_collection(void)
{
    if (g_view_collection) return S_OK;

    HRESULT hr = ensure_manager();
    if (FAILED(hr)) return hr;

    hr = g_provider->lpVtbl->QueryService(g_provider,
        &IID_IApplicationViewCollection,
        &IID_IApplicationViewCollection,
        (void**)&g_view_collection);
    return hr;
}

static void switch_to_desktop(int index)
{
    IObjectArray *desktops = NULL;
    IVirtualDesktop *desktop = NULL;
    HRESULT hr;

    hr = ensure_manager();
    if (FAILED(hr)) return;

    hr = g_manager->lpVtbl->GetDesktops(g_manager, &desktops);
    if (FAILED(hr) || !desktops) return;

    hr = desktops->lpVtbl->GetAt(desktops, (UINT)index, &IID_IVirtualDesktop, (void**)&desktop);
    if (FAILED(hr) || !desktop) {
        desktops->lpVtbl->Release(desktops);
        return;
    }

    g_manager->lpVtbl->SwitchDesktop(g_manager, desktop);

    desktop->lpVtbl->Release(desktop);
    desktops->lpVtbl->Release(desktops);
}

static void move_window_to_desktop(int index)
{
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return;

    HRESULT hr;
    IApplicationView *view = NULL;
    IObjectArray *desktops = NULL;
    IVirtualDesktop *desktop = NULL;

    hr = ensure_view_collection();
    if (FAILED(hr)) return;

    hr = g_view_collection->lpVtbl->GetViewForHwnd(g_view_collection, hwnd, &view);
    if (FAILED(hr) || !view) return;

    hr = g_manager->lpVtbl->GetDesktops(g_manager, &desktops);
    if (FAILED(hr) || !desktops) {
        view->lpVtbl->Release(view);
        return;
    }

    hr = desktops->lpVtbl->GetAt(desktops, (UINT)index, &IID_IVirtualDesktop, (void**)&desktop);
    if (FAILED(hr) || !desktop) {
        view->lpVtbl->Release(view);
        desktops->lpVtbl->Release(desktops);
        return;
    }

    g_manager->lpVtbl->MoveViewToDesktop(g_manager, (void*)view, desktop);
    g_manager->lpVtbl->SwitchDesktop(g_manager, desktop);

    desktop->lpVtbl->Release(desktop);
    desktops->lpVtbl->Release(desktops);
    view->lpVtbl->Release(view);
}

static int get_current_desktop_index(IObjectArray *desktops, GUID *cur_guid)
{
    UINT count = 0;
    HRESULT hr = desktops->lpVtbl->GetCount(desktops, &count);
    if (FAILED(hr)) return -1;

    for (UINT i = 0; i < count; i++) {
        IVirtualDesktop *d = NULL;
        GUID guid;
        hr = desktops->lpVtbl->GetAt(desktops, i, &IID_IVirtualDesktop, (void**)&d);
        if (FAILED(hr) || !d) continue;
        hr = d->lpVtbl->GetId(d, &guid);
        d->lpVtbl->Release(d);
        if (FAILED(hr)) continue;
        if (IsEqualGUID(&guid, cur_guid)) return (int)i;
    }
    return -1;
}

static void switch_to_adjacent_desktop(UINT direction)
{
    IVirtualDesktop *current = NULL;
    IVirtualDesktop *target = NULL;
    IObjectArray *desktops = NULL;
    GUID cur_guid;
    int idx;
    HRESULT hr;

    hr = ensure_manager();
    if (FAILED(hr)) return;

    hr = g_manager->lpVtbl->GetCurrentDesktop(g_manager, &current);
    if (FAILED(hr) || !current) return;

    hr = current->lpVtbl->GetId(current, &cur_guid);
    current->lpVtbl->Release(current);
    if (FAILED(hr)) return;

    hr = g_manager->lpVtbl->GetDesktops(g_manager, &desktops);
    if (FAILED(hr) || !desktops) return;

    idx = get_current_desktop_index(desktops, &cur_guid);
    if (idx < 0) { desktops->lpVtbl->Release(desktops); return; }

    UINT count = 0;
    desktops->lpVtbl->GetCount(desktops, &count);

    int target_idx = (direction == 0)
        ? (idx == 0 ? (int)count - 1 : idx - 1)
        : (idx == (int)count - 1 ? 0 : idx + 1);

    hr = desktops->lpVtbl->GetAt(desktops, (UINT)target_idx, &IID_IVirtualDesktop, (void**)&target);
    desktops->lpVtbl->Release(desktops);
    if (FAILED(hr) || !target) return;

    g_manager->lpVtbl->SwitchDesktop(g_manager, target);
    target->lpVtbl->Release(target);
}

static void move_window_to_adjacent_desktop(UINT direction)
{
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return;

    IVirtualDesktop *current = NULL;
    IVirtualDesktop *target = NULL;
    IObjectArray *desktops = NULL;
    IApplicationView *view = NULL;
    GUID cur_guid;
    int idx;
    HRESULT hr;

    hr = ensure_manager();
    if (FAILED(hr)) return;

    hr = ensure_view_collection();
    if (FAILED(hr)) return;

    hr = g_view_collection->lpVtbl->GetViewForHwnd(g_view_collection, hwnd, &view);
    if (FAILED(hr) || !view) return;

    hr = g_manager->lpVtbl->GetCurrentDesktop(g_manager, &current);
    if (FAILED(hr) || !current) {
        view->lpVtbl->Release(view);
        return;
    }

    hr = current->lpVtbl->GetId(current, &cur_guid);
    current->lpVtbl->Release(current);
    if (FAILED(hr)) {
        view->lpVtbl->Release(view);
        return;
    }

    hr = g_manager->lpVtbl->GetDesktops(g_manager, &desktops);
    if (FAILED(hr) || !desktops) {
        view->lpVtbl->Release(view);
        return;
    }

    idx = get_current_desktop_index(desktops, &cur_guid);
    if (idx < 0) {
        desktops->lpVtbl->Release(desktops);
        view->lpVtbl->Release(view);
        return;
    }

    UINT count = 0;
    desktops->lpVtbl->GetCount(desktops, &count);

    int target_idx = (direction == 0)
        ? (idx == 0 ? (int)count - 1 : idx - 1)
        : (idx == (int)count - 1 ? 0 : idx + 1);

    hr = desktops->lpVtbl->GetAt(desktops, (UINT)target_idx, &IID_IVirtualDesktop, (void**)&target);
    desktops->lpVtbl->Release(desktops);
    if (FAILED(hr) || !target) {
        view->lpVtbl->Release(view);
        return;
    }

    g_manager->lpVtbl->MoveViewToDesktop(g_manager, (void*)view, target);
    g_manager->lpVtbl->SwitchDesktop(g_manager, target);

    target->lpVtbl->Release(target);
    view->lpVtbl->Release(view);
}

static void create_virtual_desktop(void)
{
    IVirtualDesktop *desktop = NULL;
    HRESULT hr = ensure_manager();
    if (FAILED(hr)) return;

    hr = g_manager->lpVtbl->CreateDesktop(g_manager, &desktop);
    if (FAILED(hr) || !desktop) return;

    g_manager->lpVtbl->SwitchDesktop(g_manager, desktop);
    desktop->lpVtbl->Release(desktop);
}

static void remove_virtual_desktop(void)
{
    IVirtualDesktop *current = NULL;
    IVirtualDesktop *fallback = NULL;
    IObjectArray *desktops = NULL;
    GUID cur_guid;
    int idx;
    UINT count;
    HRESULT hr;

    hr = ensure_manager();
    if (FAILED(hr)) return;

    hr = g_manager->lpVtbl->GetDesktops(g_manager, &desktops);
    if (FAILED(hr) || !desktops) return;

    hr = desktops->lpVtbl->GetCount(desktops, &count);
    if (FAILED(hr) || count <= 1) {
        desktops->lpVtbl->Release(desktops);
        return;
    }
    desktops->lpVtbl->Release(desktops);

    hr = g_manager->lpVtbl->GetCurrentDesktop(g_manager, &current);
    if (FAILED(hr) || !current) return;

    hr = g_manager->lpVtbl->GetDesktops(g_manager, &desktops);
    if (FAILED(hr) || !desktops) {
        current->lpVtbl->Release(current);
        return;
    }

    hr = current->lpVtbl->GetId(current, &cur_guid);
    if (FAILED(hr)) {
        current->lpVtbl->Release(current);
        desktops->lpVtbl->Release(desktops);
        return;
    }

    idx = get_current_desktop_index(desktops, &cur_guid);
    if (idx < 0) {
        current->lpVtbl->Release(current);
        desktops->lpVtbl->Release(desktops);
        return;
    }

    hr = desktops->lpVtbl->GetCount(desktops, &count);

    int target_idx = (idx == (int)count - 1) ? idx - 1 : idx + 1;

    hr = desktops->lpVtbl->GetAt(desktops, (UINT)target_idx, &IID_IVirtualDesktop, (void**)&fallback);
    desktops->lpVtbl->Release(desktops);
    if (FAILED(hr) || !fallback) {
        current->lpVtbl->Release(current);
        return;
    }

    g_manager->lpVtbl->SwitchDesktop(g_manager, fallback);
    g_manager->lpVtbl->RemoveDesktop(g_manager, current, fallback);

    current->lpVtbl->Release(current);
    fallback->lpVtbl->Release(fallback);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr;
    MSG msg = {0};

    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) return 1;

    for (int i = 0; i < 9; i++) {
        RegisterHotKey(NULL, HOTKEY_ID_BASE + i, MOD_ALT, '1' + i);
        RegisterHotKey(NULL, HOTKEY_ID_MOVE + i, MOD_ALT | MOD_SHIFT, '1' + i);
    }

    RegisterHotKey(NULL, HOTKEY_ID_PREV, MOD_CONTROL | MOD_WIN, VK_LEFT);
    RegisterHotKey(NULL, HOTKEY_ID_NEXT, MOD_CONTROL | MOD_WIN, VK_RIGHT);
    RegisterHotKey(NULL, HOTKEY_ID_MOVE_PREV, MOD_CONTROL | MOD_WIN | MOD_SHIFT, VK_LEFT);
    RegisterHotKey(NULL, HOTKEY_ID_MOVE_NEXT, MOD_CONTROL | MOD_WIN | MOD_SHIFT, VK_RIGHT);
    RegisterHotKey(NULL, HOTKEY_ID_EXIT, MOD_ALT | MOD_CONTROL, 'Q');
    RegisterHotKey(NULL, HOTKEY_ID_NEW, MOD_CONTROL | MOD_ALT, 'N');
    RegisterHotKey(NULL, HOTKEY_ID_REMOVE, MOD_CONTROL | MOD_ALT, 'W');

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            UINT id = (UINT)msg.wParam;
            if (id == HOTKEY_ID_EXIT) break;
            if (id >= HOTKEY_ID_BASE && id < HOTKEY_ID_BASE + 9)
                switch_to_desktop((int)(id - HOTKEY_ID_BASE));
            else if (id >= HOTKEY_ID_MOVE && id < HOTKEY_ID_MOVE + 9)
                move_window_to_desktop((int)(id - HOTKEY_ID_MOVE));
            else if (id == HOTKEY_ID_PREV)
                switch_to_adjacent_desktop(0);
            else if (id == HOTKEY_ID_NEXT)
                switch_to_adjacent_desktop(1);
            else if (id == HOTKEY_ID_MOVE_PREV)
                move_window_to_adjacent_desktop(0);
            else if (id == HOTKEY_ID_MOVE_NEXT)
                move_window_to_adjacent_desktop(1);
            else if (id == HOTKEY_ID_NEW)
                create_virtual_desktop();
            else if (id == HOTKEY_ID_REMOVE)
                remove_virtual_desktop();
        }
    }

    for (int i = 0; i < 9; i++) {
        UnregisterHotKey(NULL, HOTKEY_ID_BASE + i);
        UnregisterHotKey(NULL, HOTKEY_ID_MOVE + i);
    }
    UnregisterHotKey(NULL, HOTKEY_ID_PREV);
    UnregisterHotKey(NULL, HOTKEY_ID_NEXT);
    UnregisterHotKey(NULL, HOTKEY_ID_MOVE_PREV);
    UnregisterHotKey(NULL, HOTKEY_ID_MOVE_NEXT);
    UnregisterHotKey(NULL, HOTKEY_ID_EXIT);
    UnregisterHotKey(NULL, HOTKEY_ID_NEW);
    UnregisterHotKey(NULL, HOTKEY_ID_REMOVE);

    if (g_view_collection) g_view_collection->lpVtbl->Release(g_view_collection);
    if (g_manager) g_manager->lpVtbl->Release(g_manager);
    if (g_provider) g_provider->lpVtbl->Release(g_provider);

    CoUninitialize();
    {
        typedef HRESULT (WINAPI *TDProc)(HWND, HINSTANCE, PCWSTR, PCWSTR, PCWSTR, int, PCWSTR, int*);
        TDProc td = NULL;
        HMODULE mod = LoadLibraryW(L"user32.dll");
        if (mod) {
            td = (TDProc)GetProcAddress(mod, "TaskDialog");
            FreeLibrary(mod);
        }
        if (!td) {
            mod = LoadLibraryW(L"comctl32.dll");
            if (mod) {
                td = (TDProc)GetProcAddress(mod, "TaskDialog");
                FreeLibrary(mod);
            }
        }
        if (td)
            td(NULL, NULL, L"vdmanager", L"vdmanager has exited.",
               NULL, TD_OK_BUTTON, TD_INFO_ICON, NULL);
        else
            MessageBoxW(NULL, L"vdmanager has exited.", L"vdmanager", MB_SETFOREGROUND | MB_SYSTEMMODAL | MB_ICONINFORMATION);
    }
    return 0;
}
