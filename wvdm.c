#define WIN32_LEAN_AND_MEAN
#define INITGUID
#include <windows.h>
#include <objbase.h>
#include <servprov.h>
#include <objectarray.h>
#include <dwmapi.h>
#include "config.h"

/* undocumented COM CLSID/IID for Windows Virtual Desktop */
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

#define HOTKEY_ID_BASE      100
#define HOTKEY_ID_MOVE      200
#define HOTKEY_ID_PREV      300
#define HOTKEY_ID_NEXT      301
#define HOTKEY_ID_MOVE_PREV 400
#define HOTKEY_ID_MOVE_NEXT 401
#define HOTKEY_ID_EXIT      500
#define HOTKEY_ID_NEW       600
#define HOTKEY_ID_REMOVE    601
#define HOTKEY_ID_LAST      700

#define ADJ_PREV            0
#define ADJ_NEXT            1

typedef struct IVirtualDesktopVtbl IVirtualDesktopVtbl;
typedef struct IVirtualDesktop {
	const IVirtualDesktopVtbl *lpVtbl;
} IVirtualDesktop;

/* vtable layout for Windows 11 24H2
 * source: github.com/MScholtes/VirtualDesktop/blob/master/
 *         VirtualDesktop11-24H2.cs
 * slots 0-2: IUnknown
 * slot  3: IsViewVisible (unused)
 * slot  4: GetId
 * slot  5: GetName (unused)
 * slot  6: unused */
struct IVirtualDesktopVtbl {
	HRESULT (STDMETHODCALLTYPE *QueryInterface)(IVirtualDesktop*, REFIID, void**);
	ULONG   (STDMETHODCALLTYPE *AddRef)(IVirtualDesktop*);
	ULONG   (STDMETHODCALLTYPE *Release)(IVirtualDesktop*);
	void *pad3;
	HRESULT (STDMETHODCALLTYPE *GetId)(IVirtualDesktop*, GUID*);
	void *pad5;
	void *pad6;
};

typedef struct IVirtualDesktopManagerInternalVtbl
	IVirtualDesktopManagerInternalVtbl;
typedef struct IVirtualDesktopManagerInternal {
	const IVirtualDesktopManagerInternalVtbl *lpVtbl;
} IVirtualDesktopManagerInternal;

/* vtable layout for Windows 11 24H2
 * source: github.com/MScholtes/VirtualDesktop/blob/master/
 *         VirtualDesktop11-24H2.cs
 * slots 0-2: IUnknown
 * slot  3: GetCount
 * slot  4: MoveViewToDesktop
 * slot  5: CanViewMoveToDesktop (unused)
 * slot  6: GetCurrentDesktop
 * slot  7: GetDesktops
 * slot  8: EnsureViewIsVisible (unused)
 * slot  9: SwitchDesktop
 * slot 10: IsViewOnDesktop (unused)
 * slot 11: CreateDesktop
 * slot 12: varies by build (unused)
 * slot 13: RemoveDesktop(manager, fallback, toRemove) */
struct IVirtualDesktopManagerInternalVtbl {
	HRESULT (STDMETHODCALLTYPE *QueryInterface)(IVirtualDesktopManagerInternal*,
		REFIID, void**);
	ULONG   (STDMETHODCALLTYPE *AddRef)(IVirtualDesktopManagerInternal*);
	ULONG   (STDMETHODCALLTYPE *Release)(IVirtualDesktopManagerInternal*);
	HRESULT (STDMETHODCALLTYPE *GetCount)(IVirtualDesktopManagerInternal*, UINT*);
	HRESULT (STDMETHODCALLTYPE *MoveViewToDesktop)(IVirtualDesktopManagerInternal*,
		void*, IVirtualDesktop*);
	void *pad5;
	HRESULT (STDMETHODCALLTYPE *GetCurrentDesktop)(IVirtualDesktopManagerInternal*,
		IVirtualDesktop**);
	HRESULT (STDMETHODCALLTYPE *GetDesktops)(IVirtualDesktopManagerInternal*,
		IObjectArray**);
	void *pad8;
	HRESULT (STDMETHODCALLTYPE *SwitchDesktop)(IVirtualDesktopManagerInternal*,
		IVirtualDesktop*);
	void *pad10;
	HRESULT (STDMETHODCALLTYPE *CreateDesktop)(IVirtualDesktopManagerInternal*,
		IVirtualDesktop**);
	void *pad12;
	HRESULT (STDMETHODCALLTYPE *RemoveDesktop)(IVirtualDesktopManagerInternal*,
		IVirtualDesktop*, IVirtualDesktop*);
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
	HRESULT (STDMETHODCALLTYPE *QueryInterface)(IApplicationViewCollection*,
		REFIID, void**);
	ULONG   (STDMETHODCALLTYPE *AddRef)(IApplicationViewCollection*);
	ULONG   (STDMETHODCALLTYPE *Release)(IApplicationViewCollection*);
	void *pad3;
	void *pad4;
	void *pad5;
	HRESULT (STDMETHODCALLTYPE *GetViewForHwnd)(IApplicationViewCollection*, HWND,
		IApplicationView**);
};

static IServiceProvider               *g_provider = NULL;
static IVirtualDesktopManagerInternal *g_manager = NULL;
static IApplicationViewCollection     *g_view_collection = NULL;
static GUID                            g_last_desktop_guid = {0};
static BOOL                            g_have_last = FALSE;
static int                             g_hotkey_failures = 0;
static const WCHAR                     kVersion[] = L"wvdm 1.0";

/* --- GUID helpers --- */

/* avoid CRT memcmp pulled in by IsEqualGUID macro */
static BOOL guid_equal(const GUID *a, const GUID *b) {
	return a->Data1 == b->Data1 && a->Data2 == b->Data2
		&& a->Data3 == b->Data3 && a->Data4[0] == b->Data4[0]
		&& a->Data4[1] == b->Data4[1] && a->Data4[2] == b->Data4[2]
		&& a->Data4[3] == b->Data4[3] && a->Data4[4] == b->Data4[4]
		&& a->Data4[5] == b->Data4[5] && a->Data4[6] == b->Data4[6]
		&& a->Data4[7] == b->Data4[7];
}

static BOOL get_desktop_guid(IVirtualDesktop *desktop, GUID *guid) {
	return SUCCEEDED(desktop->lpVtbl->GetId(desktop, guid));
}

/* fetch the GUID of the desktop we are on; FALSE on any failure */
static BOOL get_current_desktop_guid(GUID *guid) {
	IVirtualDesktop *current_desktop = NULL;
	if (FAILED(g_manager->lpVtbl->GetCurrentDesktop(g_manager,
		&current_desktop)) || !current_desktop) {
		return FALSE;
	}
	BOOL ok = get_desktop_guid(current_desktop, guid);
	current_desktop->lpVtbl->Release(current_desktop);
	return ok;
}

/* --- COM bootstrap --- */

/* resolve IServiceProvider from CLSID_ImmersiveShell,
 * then query IVirtualDesktopManagerInternal from it */
static HRESULT ensure_manager(void) {
	if (g_manager) {
		return S_OK;
	}

	/* reuse an existing provider; a fresh CoCreateInstance would
	 * overwrite g_provider and leak the previous reference */
	if (!g_provider) {
		HRESULT hr = CoCreateInstance(&CLSID_ImmersiveShell, NULL,
			CLSCTX_LOCAL_SERVER, &IID_IServiceProvider, (void**)&g_provider);
		if (FAILED(hr)) {
			return hr;
		}
	}

	HRESULT hr = g_provider->lpVtbl->QueryService(g_provider,
		&CLSID_VirtualDesktopManagerInternal,
		&IID_IVirtualDesktopManagerInternal,
		(void**)&g_manager);
	if (FAILED(hr)) {
		/* drop the provider so a later retry can recreate it */
		g_provider->lpVtbl->Release(g_provider);
		g_provider = NULL;
	}
	return hr;
}

/* get IApplicationViewCollection from the same provider
 * — needed for GetViewForHwnd in move operations */
static HRESULT ensure_view_collection(void) {
	if (g_view_collection) {
		return S_OK;
	}

	if (FAILED(ensure_manager())) {
		return E_FAIL;
	}

	return g_provider->lpVtbl->QueryService(g_provider,
		&IID_IApplicationViewCollection,
		&IID_IApplicationViewCollection,
		(void**)&g_view_collection);
}

/* --- desktop lookup --- */

static IObjectArray *get_desktops(void) {
	IObjectArray *desktops = NULL;
	if (FAILED(g_manager->lpVtbl->GetDesktops(g_manager, &desktops))
		|| !desktops) {
		return NULL;
	}
	return desktops;
}

/* index of the desktop whose GUID matches, or -1; also reports the desktop
 * count so callers avoid a second GetCount round-trip */
static int get_current_desktop_index(IObjectArray *desktops, GUID *cur_guid,
	UINT *out_count) {
	UINT count = 0;
	if (FAILED(desktops->lpVtbl->GetCount(desktops, &count))) {
		return -1;
	}
	*out_count = count;

	for (UINT i = 0; i < count; i++) {
		IVirtualDesktop *desktop = NULL;
		if (FAILED(desktops->lpVtbl->GetAt(desktops, i, &IID_IVirtualDesktop,
			(void**)&desktop)) || !desktop) {
			continue;
		}
		GUID guid;
		BOOL ok = get_desktop_guid(desktop, &guid);
		desktop->lpVtbl->Release(desktop);
		if (!ok) {
			continue;
		}
		if (guid_equal(&guid, cur_guid)) {
			return (int)i;
		}
	}
	return -1;
}

static IVirtualDesktop *get_desktop_at(int index) {
	IObjectArray *desktops = get_desktops();
	if (!desktops) {
		return NULL;
	}

	IVirtualDesktop *desktop = NULL;
	HRESULT hr = desktops->lpVtbl->GetAt(desktops, (UINT)index,
		&IID_IVirtualDesktop, (void**)&desktop);
	desktops->lpVtbl->Release(desktops);
	return SUCCEEDED(hr) ? desktop : NULL;
}

static IVirtualDesktop *get_desktop_by_guid(GUID *guid) {
	IObjectArray *desktops = get_desktops();
	if (!desktops) {
		return NULL;
	}

	UINT count = 0;
	if (FAILED(desktops->lpVtbl->GetCount(desktops, &count))) {
		desktops->lpVtbl->Release(desktops);
		return NULL;
	}

	IVirtualDesktop *desktop = NULL;
	for (UINT i = 0; i < count; i++) {
		IVirtualDesktop *d = NULL;
		GUID d_guid;
		if (FAILED(desktops->lpVtbl->GetAt(desktops, i, &IID_IVirtualDesktop,
			(void**)&d)) || !d) {
			continue;
		}
		if (get_desktop_guid(d, &d_guid) && guid_equal(&d_guid, guid)) {
			desktop = d;
			break;
		}
		d->lpVtbl->Release(d);
	}

	desktops->lpVtbl->Release(desktops);
	return desktop;
}

static IVirtualDesktop *get_adjacent_desktop(UINT direction) {
	GUID cur_guid;
	if (!get_current_desktop_guid(&cur_guid)) {
		return NULL;
	}

	IObjectArray *desktops = get_desktops();
	if (!desktops) {
		return NULL;
	}

	UINT count = 0;
	int idx = get_current_desktop_index(desktops, &cur_guid, &count);
	if (idx < 0) {
		desktops->lpVtbl->Release(desktops);
		return NULL;
	}
	int last = (int)count - 1;

	if (!WRAP_ADJACENT) {
		if ((direction == ADJ_PREV && idx == 0)
			|| (direction == ADJ_NEXT && idx == last)) {
			desktops->lpVtbl->Release(desktops);
			return NULL;
		}
	}

	int desktop_idx = (direction == ADJ_PREV)
		? (idx == 0 ? last : idx - 1)
		: (idx == last ? 0 : idx + 1);

	IVirtualDesktop *adjacent_desktop = NULL;
	HRESULT hr = desktops->lpVtbl->GetAt(desktops, (UINT)desktop_idx,
		&IID_IVirtualDesktop, (void**)&adjacent_desktop);
	desktops->lpVtbl->Release(desktops);
	return SUCCEEDED(hr) ? adjacent_desktop : NULL;
}

/* --- switch core --- */

/* the window the shell leaves topmost and uncloaked on the new desktop is the
 * one that should take the foreground; activating it directly is what stops
 * the taskbar flash — the desktop-name label never displays for a programmatic
 * switch, only Explorer's own input paths show it */

/* shell chrome would match the activation filter; both taskbar classes can be
 * top-level windows on Windows 11, and the desktop hosts are always present */
static BOOL is_shell_chrome(HWND hwnd) {
	WCHAR class_name[64];
	if (GetClassNameW(hwnd, class_name, 64) == 0) {
		return FALSE;
	}
	return lstrcmpW(class_name, L"Shell_TrayWnd") == 0
		|| lstrcmpW(class_name, L"XamlExplorerHostIslandWindow") == 0
		|| lstrcmpW(class_name, L"Progman") == 0
		|| lstrcmpW(class_name, L"WorkerW") == 0;
}

static BOOL CALLBACK activation_enum_cb(HWND hwnd, LPARAM lparam) {
	HWND *target = (HWND*)lparam;
	if (is_shell_chrome(hwnd)) {
		return TRUE;
	}
	/* alt-tab rules: visible, unowned, not a tool window; cloaked windows are
	 * minimized or on another desktop */
	if (!IsWindowVisible(hwnd) || GetWindow(hwnd, GW_OWNER)) {
		return TRUE;
	}
	if (GetWindowLongPtrW(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) {
		return TRUE;
	}
	DWORD cloaked = 0;
	if (FAILED(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &cloaked,
		sizeof(cloaked))) || cloaked != 0) {
		return TRUE;
	}
	*target = hwnd;
	return FALSE;
}

/* the first window in Z-order to pass the filter is the shell's candidate for
 * the new desktop's active window */
static HWND find_activation_target(void) {
	HWND target = NULL;
	EnumWindows(activation_enum_cb, (LPARAM)&target);
	return target;
}

/* SetForegroundWindow from a background process is blocked by the foreground
 * lock; attaching our input queue to the target's thread first is the
 * documented way to bypass it */
static void foreground_activate(HWND target) {
	DWORD target_thread = 0;
	GetWindowThreadProcessId(target, &target_thread);
	if (!target_thread) {
		return;
	}
	AttachThreadInput(target_thread, GetCurrentThreadId(), TRUE);
	SetForegroundWindow(target);
	AttachThreadInput(target_thread, GetCurrentThreadId(), FALSE);
}

/* switch and activate the new desktop's active window; activating the window
 * explicitly is what prevents the taskbar flash */
static void switch_desktop(IVirtualDesktop *desktop) {
	g_manager->lpVtbl->SwitchDesktop(g_manager, desktop);
	HWND target = find_activation_target();
	if (!target) {
		/* empty desktop: hand focus to the shell desktop as the natural
		 * nothing-focused state */
		target = GetShellWindow();
	}
	if (target) {
		foreground_activate(target);
	}
}

/* switch to the given desktop, remembering the one we leave */
static void switch_to(IVirtualDesktop *desktop) {
	GUID d_guid;
	if (!get_desktop_guid(desktop, &d_guid)) {
		/* cannot identify the target; switch without bookkeeping */
		switch_desktop(desktop);
		return;
	}

	/* a hotkey for the desktop we are already on is a no-op: no switch, no
	 * focus churn, and no last-desktop bookkeeping */
	GUID cur_guid;
	if (get_current_desktop_guid(&cur_guid)) {
		if (guid_equal(&d_guid, &cur_guid)) {
			return;
		}
		/* record the desktop we leave, keeping the toggle working */
		g_last_desktop_guid = cur_guid;
		g_have_last = TRUE;
	}

	switch_desktop(desktop);
}

/* --- move core --- */

/* move the foreground window to the given desktop, switching after (optional);
 * takes ownership of the desktop reference and releases it on every path */
static void move_window_to(IVirtualDesktop *desktop) {
	HWND hwnd = GetForegroundWindow();
	if (!hwnd) {
		desktop->lpVtbl->Release(desktop);
		return;
	}

	if (FAILED(ensure_view_collection())) {
		desktop->lpVtbl->Release(desktop);
		return;
	}

	IApplicationView *view = NULL;
	if (FAILED(g_view_collection->lpVtbl->GetViewForHwnd(g_view_collection,
		hwnd, &view)) || !view) {
		desktop->lpVtbl->Release(desktop);
		return;
	}

	/* MoveViewToDesktop(this, view, desktop); only switch after a successful
	 * move, so a failure does not strand the window on the old desktop */
	if (SUCCEEDED(g_manager->lpVtbl->MoveViewToDesktop(g_manager, (void*)view,
		desktop)) && SWITCH_AFTER_ACTION) {
		switch_to(desktop);
	}

	desktop->lpVtbl->Release(desktop);
	view->lpVtbl->Release(view);
}

/* --- actions (dispatched from wvdm_main) --- */

static void switch_to_desktop(int index) {
	if (FAILED(ensure_manager())) {
		return;
	}
	IVirtualDesktop *desktop = get_desktop_at(index);
	if (!desktop) {
		return;
	}
	switch_to(desktop);
	desktop->lpVtbl->Release(desktop);
}

static void move_window_to_desktop(int index) {
	IVirtualDesktop *desktop = get_desktop_at(index);
	if (!desktop) {
		return;
	}
	move_window_to(desktop);
}

static void switch_to_adjacent_desktop(UINT direction) {
	if (FAILED(ensure_manager())) {
		return;
	}
	IVirtualDesktop *adjacent_desktop = get_adjacent_desktop(direction);
	if (!adjacent_desktop) {
		return;
	}
	switch_to(adjacent_desktop);
	adjacent_desktop->lpVtbl->Release(adjacent_desktop);
}

static void move_window_to_adjacent_desktop(UINT direction) {
	IVirtualDesktop *adjacent_desktop = get_adjacent_desktop(direction);
	if (!adjacent_desktop) {
		return;
	}
	move_window_to(adjacent_desktop);
}

static void create_virtual_desktop(void) {
	if (FAILED(ensure_manager())) {
		return;
	}

	/* no numbered hotkey above MAX_DESKTOPS; no room for an extra desktop */
	UINT count = 0;
	if (FAILED(g_manager->lpVtbl->GetCount(g_manager, &count))
		|| count >= MAX_DESKTOPS) {
		return;
	}

	IVirtualDesktop *desktop = NULL;
	if (FAILED(g_manager->lpVtbl->CreateDesktop(g_manager, &desktop))
		|| !desktop) {
		return;
	}

	if (SWITCH_AFTER_ACTION) {
		switch_to(desktop);
	}

	desktop->lpVtbl->Release(desktop);
}

static void remove_virtual_desktop(void) {
	if (FAILED(ensure_manager())) {
		return;
	}

	IVirtualDesktop *current_desktop = NULL;
	if (FAILED(g_manager->lpVtbl->GetCurrentDesktop(g_manager,
		&current_desktop)) || !current_desktop) {
		return;
	}

	IVirtualDesktop *fallback_desktop = get_adjacent_desktop(ADJ_NEXT);
	if (!fallback_desktop) {
		fallback_desktop = get_adjacent_desktop(ADJ_PREV);
	}
	if (!fallback_desktop) {
		current_desktop->lpVtbl->Release(current_desktop);
		return;
	}

	/* with WRAP_ADJACENT and a single desktop the fallback is that same
	 * desktop; Windows will not remove the last one, so bail out */
	GUID current_guid;
	GUID fallback_guid;
	if (get_desktop_guid(current_desktop, &current_guid)
		&& get_desktop_guid(fallback_desktop, &fallback_guid)
		&& guid_equal(&fallback_guid, &current_guid)) {
		current_desktop->lpVtbl->Release(current_desktop);
		fallback_desktop->lpVtbl->Release(fallback_desktop);
		return;
	}

	switch_to(fallback_desktop);
	/* RemoveDesktop(this, fallbackDesktop, desktopToRemove) */
	if (FAILED(g_manager->lpVtbl->RemoveDesktop(g_manager, fallback_desktop,
		current_desktop))) {
		/* removal failed (e.g. a window is still pinned); go back to the
		 * desktop we were on instead of leaving the user on the fallback */
		switch_to(current_desktop);
	}

	current_desktop->lpVtbl->Release(current_desktop);
	fallback_desktop->lpVtbl->Release(fallback_desktop);
}

static void switch_to_last_desktop(void) {
	if (FAILED(ensure_manager())) {
		return;
	}
	if (!g_have_last) {
		return;
	}

	IVirtualDesktop *last_desktop = get_desktop_by_guid(&g_last_desktop_guid);
	if (!last_desktop) {
		/* the stored desktop no longer exists; clear the stale state */
		g_have_last = FALSE;
		return;
	}

	/* switch_to records the desktop we leave, keeping the toggle working */
	switch_to(last_desktop);
	last_desktop->lpVtbl->Release(last_desktop);
}

/* --- command line and message loop --- */

static void print_help(void) {
	static const WCHAR *help =
		L"wvdm - windows virtual desktop manager\n"
		L"\n"
		L"  --help, -h        show this message\n"
		L"  --version, -v     show version\n"
		L"\n"
		L"Hotkeys:\n"
		L"  Ctrl+Alt+1..9               switch to desktop N\n"
		L"  Ctrl+Alt+Shift+1..9         move window to desktop N\n"
		L"  Ctrl+Alt+Left/Right          previous/next desktop\n"
		L"  Ctrl+Alt+Shift+Left/Right    move window to adjacent\n"
		L"  Ctrl+Alt+Space             toggle last desktop\n"
		L"  Ctrl+Alt+N                 create new desktop\n"
		L"  Ctrl+Alt+W                 close current desktop\n"
		L"  Ctrl+Alt+Q                 quit\n";
	MessageBoxW(NULL, help, L"wvdm", MB_OK);
}

/* count registration failures so the user learns about binding conflicts */
static void register_hotkey(int id, UINT modifiers, UINT key) {
	if (!RegisterHotKey(NULL, id, modifiers, key)) {
		g_hotkey_failures++;
	}
}

static int wvdm_main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	(void)hInstance;
	(void)hPrevInstance;
	(void)nCmdShow;

	if (lpCmdLine && lpCmdLine[0]) {
		/* lpCmdLine may start with the space separating it from the exe name */
		LPSTR arg = lpCmdLine;
		while (*arg == ' ' || *arg == '\t') {
			arg++;
		}
		if (lstrcmpiA(arg, "--help") == 0 || lstrcmpiA(arg, "-h") == 0) {
			print_help();
			return 0;
		}
		if (lstrcmpiA(arg, "--version") == 0 || lstrcmpiA(arg, "-v") == 0) {
			MessageBoxW(NULL, kVersion, L"wvdm", MB_OK);
			return 0;
		}
		print_help();
		return 1;
	}

	/* one instance per logon session; a second launch exits silently */
	HANDLE single_mutex = CreateMutex(NULL, FALSE, "Local\\wvdm");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 0;
	}

	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		CloseHandle(single_mutex);
		return 1;
	}

	for (int i = 0; i < MAX_DESKTOPS; i++) {
		register_hotkey(HOTKEY_ID_BASE + i, MOD_SWITCH, KEY_SWITCH(i));
		register_hotkey(HOTKEY_ID_MOVE + i, MOD_MOVE, KEY_MOVE(i));
	}

	register_hotkey(HOTKEY_ID_PREV, MOD_PREV, KEY_PREV);
	register_hotkey(HOTKEY_ID_NEXT, MOD_NEXT, KEY_NEXT);
	register_hotkey(HOTKEY_ID_MOVE_PREV, MOD_MOVE_PREV, KEY_MOVE_PREV);
	register_hotkey(HOTKEY_ID_MOVE_NEXT, MOD_MOVE_NEXT, KEY_MOVE_NEXT);
	register_hotkey(HOTKEY_ID_EXIT, MOD_EXIT, KEY_EXIT);
	register_hotkey(HOTKEY_ID_NEW, MOD_NEW, KEY_NEW);
	register_hotkey(HOTKEY_ID_REMOVE, MOD_REMOVE, KEY_REMOVE);
	register_hotkey(HOTKEY_ID_LAST, MOD_LAST, KEY_LAST);

	if (g_hotkey_failures) {
		MessageBoxW(NULL, L"Some hotkeys failed to register", L"wvdm",
			MB_OK | MB_ICONWARNING);
	}

	/* GetMessage returns 0 on WM_QUIT and -1 on error; loop on positive */
	int msg_ret = 0;
	MSG msg = {0};
	while ((msg_ret = GetMessage(&msg, NULL, 0, 0)) > 0) {
		if (msg.message == WM_HOTKEY) {
			UINT id = (UINT)msg.wParam;
			if (id == HOTKEY_ID_EXIT) {
				/* graceful teardown can block on the shell COM release or
				 * CoUninitialize, leaving the process in the task manager;
				 * hard-exit instead — process death reclaims hotkeys and
				 * COM state */
				TerminateProcess(GetCurrentProcess(), 0);
			}
			if (id >= HOTKEY_ID_BASE && id < HOTKEY_ID_BASE + MAX_DESKTOPS) {
				switch_to_desktop((int)(id - HOTKEY_ID_BASE));
			} else if (id >= HOTKEY_ID_MOVE && id < HOTKEY_ID_MOVE + MAX_DESKTOPS) {
				move_window_to_desktop((int)(id - HOTKEY_ID_MOVE));
			} else if (id == HOTKEY_ID_PREV) {
				switch_to_adjacent_desktop(ADJ_PREV);
			} else if (id == HOTKEY_ID_NEXT) {
				switch_to_adjacent_desktop(ADJ_NEXT);
			} else if (id == HOTKEY_ID_MOVE_PREV) {
				move_window_to_adjacent_desktop(ADJ_PREV);
			} else if (id == HOTKEY_ID_MOVE_NEXT) {
				move_window_to_adjacent_desktop(ADJ_NEXT);
			} else if (id == HOTKEY_ID_NEW) {
				create_virtual_desktop();
			} else if (id == HOTKEY_ID_REMOVE) {
				remove_virtual_desktop();
			} else if (id == HOTKEY_ID_LAST) {
				switch_to_last_desktop();
			}
		}
	}

	/* the EXIT hotkey terminates the process in the message loop above;
	 * this return only covers a GetMessage error (-1) or WM_QUIT (0) */
	return msg_ret < 0;
}

/* no CRT: returning from the entry point never terminates the process, and
 * ExitProcess can block on ole32 teardown after shell COM use, leaving a
 * lingering process; hard-terminate instead */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow) {
	TerminateProcess(GetCurrentProcess(), wvdm_main(hInstance, hPrevInstance,
		lpCmdLine, nCmdShow));
	/* TerminateProcess on the current process never returns; this return is
	 * unreachable, but satisfies gcc's -Wreturn-type check */
	return 0;
}
