#include "StdAfx.h"
#include "ChromeTrayIcon.h"

#include "JSMethods.h"

#include "utils.h"

// Chrome_WidgetWin_1
// Chrome_WidgetWin_1
static const TCHAR *ChromeWidgetWinClass= _T("Chrome_WidgetWin_1");
static const TCHAR *ChromeWindowClass	= _T("Chrome_WindowImpl_0");
static const TCHAR *ChromeWidgetClass	= _T("Chrome_WidgetWin_0");

static LPCTSTR ChromeWindowClasses[]	= {ChromeWidgetWinClass, ChromeWidgetClass, ChromeWindowClass};

static const int ContexMenuItemTextMax	= 48;

CChromeTrayIcon::CChromeTrayIcon(void) : 
		m_hIcon(NULL), m_HotKeyIdBoss(0), m_HotKeyIdRestore(0),
		m_bChromeIsHidded(FALSE)
{
	InitializeCriticalSection(&m_csWindowsList);
}

CChromeTrayIcon::~CChromeTrayIcon(void)
{
	DeleteCriticalSection(&m_csWindowsList);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

BOOL CChromeTrayIcon::CreateTrayIcon(HINSTANCE hInstance)
{
	if(Create(NULL) == NULL)
	{
		DebugLog(_T("Failed to create the tray icon!\n GLE: %lu"), GetLastError());

		return FALSE;
	}

	ShowWindow(SW_HIDE);

	OptionsChanged();

	ReCreateTrayIcon();

	m_uTrayRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));;

	m_TrayPopup.Create();
	HICON hIcon = GetChromeWindowIcon();
	m_TrayPopup.SetIcon(hIcon);

	StartMonitoring();

	OptionsChanged();

	return TRUE;
}

BOOL CChromeTrayIcon::ReCreateTrayIcon()
{
	m_TrayIcon.Destroy();

	HICON hIcon = GetChromeWindowIcon();

	return m_TrayIcon.Create(m_hWnd, 1, hIcon, _T("Google Chrome"));
}

BOOL CChromeTrayIcon::DestroyTrayIcon()
{
	StopMonitoring();

	UnregisterHotKeys();

	EnterCriticalSection(&m_csWindowsList);

	for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
	{
		ShowChromeWindow(m_ChromeWindows[i].hWnd);
	}

	m_ChromeWindows.clear();

	LeaveCriticalSection(&m_csWindowsList);

	if(IsWindow())
	{
		DestroyWindow();
	}

	m_TrayIcon.Destroy();

	m_TrayPopup.DestroyWindow();

	if(m_hIcon != NULL)
	{
		DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}

	return TRUE;
}

BOOL CChromeTrayIcon::SetTrayIcon(LPCTSTR lpszIconPath)
{	
	BOOL bResult	= FALSE;

	if(m_hIcon != NULL)
	{
		DestroyIcon(m_hIcon);
	}

	if(lpszIconPath == NULL || lpszIconPath[0] == '\0')
	{
		m_hIcon = GetChromeWindowIcon();
	}
	else
	{
		m_hIcon = (HICON)LoadImage(NULL, lpszIconPath, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

		if(m_hIcon == NULL)
		{
			m_hIcon = GetChromeWindowIcon();
		}
	}

	return m_TrayIcon.SetIcon(m_hIcon);
}

BOOL CChromeTrayIcon::PopupNotify(LPCTSTR lpszTitle, LPCTSTR lpszText)
{
	m_TrayPopup.PopupWindow(lpszTitle, lpszText);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
LRESULT CChromeTrayIcon::OnTrayMouseCommand(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	TrayAction action = Nothing;

	switch(uMsg)
	{
	case WM_TRAY_LCLICK:
		{
			action = m_options.actLClick;
		}
		break;

	case WM_TRAY_LDBLCLICK:
		{
			action = m_options.actLDblClick;
		}
		break;

	case WM_TRAY_RCLICK:
		{
			action = m_options.actRClick;
		}
		break;

	case WM_TRAY_RDBLCLICK:
		{
			action = m_options.actRDblClick;
		}
		break;
	}

	switch(action)
	{
	case Nothing:
		{

		}
		break;

	case Restore:
		{
			RestoreAllChromeWindows();
		}
		break;

	case ContextMenu:
		{
			ShowContextMenu();
		}
		break;

	case NewTab:
		{
			BOOL bDummy;
			OnNewTab(0, 0, 0, bDummy);
		}
		break;

	case NewWindow:
		{
			BOOL bDummy;
			OnNewWnd(0, 0, 0, bDummy);
		}
		break;
	}

	return 0;
}

LRESULT CChromeTrayIcon::OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(wParam == (WPARAM)m_HotKeyIdBoss)	
	{
		OnBossKeyPressed();
	}
	else if(wParam == (WPARAM)m_HotKeyIdRestore)
	{
		OnRestoreByKeyPressed();
	}

	return 0;
}

LRESULT CChromeTrayIcon::OnOptions(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HWND hChromeWindow = FindVisibleChromeWindow();

	if(hChromeWindow == NULL)
	{
		if(m_ChromeWindows.size() != 0)
		{
			hChromeWindow = FindVisibleChromeWindow();
		}
	}

	if(hChromeWindow != NULL)
	{
		ShowChromeWindow(hChromeWindow);
	}

	CJSMethods::ShowOptions();

	return 0;
}

LRESULT CChromeTrayIcon::OnNewTab(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HWND hChromeWindow = FindVisibleChromeWindow();

	if(hChromeWindow == NULL)
	{
		if(m_ChromeWindows.size() != 0)
		{
			hChromeWindow = FindVisibleChromeWindow();
		}
	}

	if(hChromeWindow != NULL)
	{
		ShowChromeWindow(hChromeWindow);
	}

	CJSMethods::NewTab();

	return 0;
}

LRESULT CChromeTrayIcon::OnNewWnd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CJSMethods::NewWindow();

	return 0;
}

LRESULT CChromeTrayIcon::OnFavorites(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	map<DWORD, wstring>::iterator it = m_Favorites.find(wID);

	if(it == m_Favorites.end())
	{
		return 0;
	}

	HWND hChromeWindow = FindVisibleChromeWindow();

	if(hChromeWindow == NULL)
	{
		if(m_ChromeWindows.size() != 0)
		{
			hChromeWindow = FindVisibleChromeWindow();
		}
	}

	if(hChromeWindow != NULL)
	{
		ShowChromeWindow(hChromeWindow);
	}

	CJSMethods::OpenUrl((*it).second);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CChromeTrayIcon::ShowChromeWindow(HWND hWnd)
{
	if(::IsWindow(hWnd) == FALSE)
	{
		return;
	}

	if(::IsWindowVisible(hWnd) == FALSE)
	{
		::ShowWindow(hWnd, SW_SHOW);
	}

	if(::IsIconic(hWnd))
	{
		::ShowWindow(hWnd, SW_RESTORE);
	}

	::SetForegroundWindow(hWnd);
	::SetActiveWindow(hWnd);

	ShowHideTrayIcon();
}

void CChromeTrayIcon::HideChromeWindow(HWND hWnd)
{
	BOOL	bFound			= FALSE;

	TCHAR	szTooltip[255]	= {0};

	EnterCriticalSection(&m_csWindowsList);

	for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
	{
		if(hWnd == m_ChromeWindows[i].hWnd)
		{
			bFound = TRUE;
			break;
		}
	}

	if(bFound == FALSE)
	{
		ChromeWindow newWnd = {-1, hWnd};
		m_ChromeWindows.push_back(newWnd);
	}

	LeaveCriticalSection(&m_csWindowsList);

	UpdateIconTooltip(hWnd);

	::ShowWindow(hWnd, SW_HIDE);

	ShowHideTrayIcon();
}

BOOL CChromeTrayIcon::OptionsChanged()
{
	if(CJSMethods::GetOptions(m_options) == false)
	{
		return FALSE;
	}

	if(m_options.bHideTrayIfNowWindows == FALSE)
	{
		if(m_TrayIcon.IsIconVisible() == FALSE)
		{
			m_TrayIcon.ShowIcon();
		}
	}
	else
	{
		if(m_TrayIcon.IsIconVisible() && m_ChromeWindows.empty())
		{
			m_TrayIcon.HideIcon();
		}
	}

	UnregisterHotKeys();
	RegisterHotKeys();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

BOOL CChromeTrayIcon::StartMonitoring()
{
	m_mouseMonitor.SetCallBack(this);
	m_mouseMonitor.Start();

	return TRUE;
}

BOOL CChromeTrayIcon::StopMonitoring()
{
	m_mouseMonitor.Stop();

	return TRUE;
}

BOOL CChromeTrayIcon::OnMouseButtonUp(const HWND &targetWindow, const DWORD &mouseButton, const POINT &coord)
{
	TCHAR windowClass[255] = {0};

	if(GetClassName(targetWindow, windowClass, _countof(windowClass)) == 0)
	{
		return FALSE;
	}

	BOOL isChromeWindow = FALSE;

	for(size_t i = 0; i < _countof(ChromeWindowClasses); ++i)
	{
		if(_wcsicmp(ChromeWindowClasses[i], windowClass) == 0)
		{
			isChromeWindow = TRUE;

			break;
		}
	}

	if(isChromeWindow == FALSE)
	{
		DebugLog(_T("This is not Chrome window. Window class: %s\n"), windowClass);

		return FALSE;
	}

	switch(mouseButton)
	{
	case VK_LBUTTON:
		{
			if(m_options.bMinimizeOnLeftButton)
			{
				HideChromeWindow(targetWindow);
			}
		}
		break;

	case VK_RBUTTON:
		{
			if(m_options.bMinimizeOnRightButton)
			{
				HideChromeWindow(targetWindow);
			}
		}
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CChromeTrayIcon::RestoreAllChromeWindows()
{
	HWND hWnd;

	size_t nRestoredWindows = 0;

	EnterCriticalSection(&m_csWindowsList);

	for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
	{
		hWnd = m_ChromeWindows[i].hWnd;

		if(::IsWindow(hWnd) == FALSE)
		{
			m_ChromeWindows.erase(m_ChromeWindows.begin() + i);

			if(i != 0)
			{
				--i;
			}

			continue;
		}

		if(::IsWindowVisible(hWnd) == FALSE)
		{
			ShowChromeWindow(hWnd);
			++nRestoredWindows;
		}
	}

	LeaveCriticalSection(&m_csWindowsList);

	if(nRestoredWindows == 0 && m_ChromeWindows.size() != 0)
	{
		HWND hWnd = FindVisibleChromeWindow();

		if(hWnd != NULL)
		{
			ShowChromeWindow(hWnd);
		}
	}

	UpdateIconTooltip(NULL);
}

void CChromeTrayIcon::ShowContextMenu()
{
	CJSMethods::GetLanguage(m_language);

	TCHAR szWindowName[255] = {0};

	if(m_TrayMenu.m_hMenu != NULL)
	{
		m_TrayMenu.DestroyMenu();
	}

	m_TrayMenu.CreatePopupMenu();
	
	if(m_options.bShowOptions)
	{
		m_TrayMenu.AppendMenu(MF_STRING, TRAY_OPTIONS_COMMAND, m_language.strOptions.c_str());
	}

	if(m_options.bShowOptions && (m_options.bShowNewWindow || m_options.bShowNewTab))
	{
		m_TrayMenu.AppendMenu(MF_SEPARATOR, TRAY_OPTIONS_COMMAND, _T(""));
	}

	if(m_options.bShowNewWindow)
	{
		m_TrayMenu.AppendMenu(MF_STRING, TRAY_NEW_WND_COMMAND, m_language.strNewWindow.c_str());
	}

	if(m_options.bShowNewTab)
	{
		m_TrayMenu.AppendMenu(MF_STRING, TRAY_NEW_TAB_COMMAND, m_language.strNewTab.c_str());
	}

	if(m_options.bShowFavorites && m_options.strFavorites.empty() == FALSE)
	{
		if(m_options.bShowOptions || m_options.bShowNewWindow || m_options.bShowNewTab)
		{
			m_TrayMenu.AppendMenu(MF_SEPARATOR, TRAY_OPTIONS_COMMAND, _T(""));
		}

		m_Favorites.clear();

		vector<wstring> favorites;
		vector<wstring> favoriteItem;

		CMenu favoritesMenu;
		favoritesMenu.CreatePopupMenu();

		DWORD dwCmdId;

		if(SplitString(m_options.strFavorites, favorites, _T("\n")))
		{
			for(size_t i = 0; i < favorites.size(); ++i)
			{
				if(SplitString(favorites[i], favoriteItem, _T(" http")))
				{
					if(favoriteItem.size() == 2)
					{
						dwCmdId = TRAY_FAVORITES_COMMAND + m_Favorites.size();

						favoritesMenu.AppendMenu(MF_STRING, dwCmdId, favoriteItem[0].c_str());
						m_Favorites[dwCmdId] = favoriteItem[1].c_str();
					}
				}
			}
		}

		m_TrayMenu.AppendMenu(MF_POPUP | MF_STRING, (UINT_PTR)favoritesMenu.m_hMenu, m_language.strFavorites.c_str());
	}

	BOOL	bNeedToAddSeparator	= TRUE;
	vector<ChromeTab> tabs;
	HWND	hWnd				= NULL;
	HWND	hChildWindow		= NULL;

	EnterCriticalSection(&m_csWindowsList);

	for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
	{
		hWnd = m_ChromeWindows[i].hWnd;

		if(::IsWindow(hWnd) == FALSE)
		{
			m_ChromeWindows.erase(m_ChromeWindows.begin() + i);

			if(i != 0)
			{
				--i;
			}
			continue;
		}

		if(::IsWindowVisible(hWnd) == FALSE)
		{
			hChildWindow = NULL;

			for(size_t j = 0; j < _countof(ChromeWindowClasses); ++j)
			{
				hChildWindow = FindWindowEx(hWnd, NULL, ChromeWindowClasses[j], NULL);

				if(hChildWindow != NULL)
				{
					::GetWindowText(hChildWindow, szWindowName, _countof(szWindowName));
					break;
				}
			}

			if(hChildWindow == NULL)
			{
				::GetWindowText(hWnd, szWindowName, _countof(szWindowName));
			}

			if(wcslen(szWindowName) > ContexMenuItemTextMax)
			{
				szWindowName[ContexMenuItemTextMax] = '\0';
				wcscat_s(szWindowName, _T("..."));
			}

			tabs.clear();
			
			if(tabs.size() == 0)
			{
				if(bNeedToAddSeparator)
				{
					m_TrayMenu.AppendMenu(MF_SEPARATOR, TRAY_OPTIONS_COMMAND, _T(""));
					bNeedToAddSeparator = FALSE;
				}

				m_TrayMenu.AppendMenu(MF_STRING, TRAY_MENU_COMMAND + 100 * i, szWindowName);
			}
			else
			{
				ChromeTab tab;

				CMenu subMenu;
				subMenu.CreatePopupMenu();

				for(size_t k = 0; k < tabs.size(); ++k)
				{
					tab = tabs[k];

					DebugLog(_T("Tab title: %s"), tab.strTitle.c_str());

					if(tab.strTitle.size() > ContexMenuItemTextMax)
					{
						tab.strTitle = tab.strTitle.substr(ContexMenuItemTextMax);
						tab.strTitle += _T("...");
					}

					subMenu.AppendMenu(MF_STRING, TRAY_MENU_COMMAND + 100 * i + tab.nId, tab.strTitle.c_str());
				}

				m_TrayMenu.AppendMenu(MF_POPUP | MF_STRING, (UINT_PTR)subMenu.m_hMenu, szWindowName);
			}
		}
	}

	LeaveCriticalSection(&m_csWindowsList);

	if(m_TrayMenu.GetMenuItemCount() == 0)
	{
		return;
	}

	POINT pt;
	GetCursorPos(&pt);	

	SetForegroundWindow(m_hWnd);

	int nMenuItem = ::TrackPopupMenu(m_TrayMenu, TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, m_hWnd, NULL);

	PostMessage(WM_NULL, 0, 0);

	if(nMenuItem >= TRAY_MENU_COMMAND)
	{
		int nIndex = nMenuItem - TRAY_MENU_COMMAND;

		int nWindow = (int)(nIndex / 100);

		ShowChromeWindow(m_ChromeWindows[nWindow].hWnd);

		UpdateIconTooltip(NULL);
	}
	else
	{
		::SendMessage(m_hWnd, WM_COMMAND, nMenuItem, 0);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

HICON CChromeTrayIcon::GetChromeWindowIcon()
{
	HICON hIcon = NULL;

	HWND hChromeWnd = FindWindow(ChromeWidgetWinClass, NULL);

	if(hChromeWnd == NULL)
	{
		hChromeWnd = FindWindow(ChromeWindowClass, NULL);

		if(hChromeWnd == NULL)
		{
			hChromeWnd = FindWindow(ChromeWidgetClass, NULL);
		}
	}

	if(hChromeWnd == NULL)
	{
		return hIcon;
	}

	if(hIcon = (HICON)SendMessage(hChromeWnd, WM_GETICON, ICON_SMALL, 0))
	{
		hIcon = (HICON)CopyImage(hIcon, IMAGE_ICON, 16, 16, LR_DEFAULTSIZE);

		return hIcon;
	}
	else if (hIcon = (HICON)GetClassLong(hChromeWnd, GCL_HICON))
	{
		hIcon = (HICON)CopyImage(hIcon, IMAGE_ICON, 16, 16, LR_DEFAULTSIZE);

		return hIcon;
	}

	return hIcon;
}

HWND CChromeTrayIcon::FindVisibleChromeWindow()
{
	HWND hChromeWindow = NULL;

	for(size_t i = 0; i < _countof(ChromeWindowClasses); ++i)
	{
		hChromeWindow = FindWindowEx(NULL, NULL, ChromeWindowClasses[i], NULL);

		while(hChromeWindow != NULL && FindWindowEx(hChromeWindow, NULL, ChromeWindowClasses[i], NULL) == NULL)
		{
			hChromeWindow = FindWindowEx(NULL, hChromeWindow, ChromeWindowClasses[i], NULL);
		}

		if(hChromeWindow != NULL)
		{
			return hChromeWindow;
		}
	}

	return NULL;
}

BOOL CALLBACK EnumChromeWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR windowClass[1024] = {0};

	if(GetClassName(hwnd, windowClass, _countof(windowClass)) == 0)
	{
		return TRUE;
	}

	for(size_t i = 0; i < _countof(ChromeWindowClasses); ++i)
	{
		if(_wcsicmp(windowClass, ChromeWindowClasses[i]) == 0)
		{
			//for(size_t j = 0; j < _countof(ChromeWindowClasses); ++j)
			{
				//HWND chromeWindow = FindWindowEx(hwnd, NULL, ChromeWindowClasses[j], NULL);

				//chromeWindow = FindWindowEx(NULL, chromeWindow, ChromeWindowClasses[i], NULL);

				//if(chromeWindow != NULL)
				if(GetParent(hwnd) == NULL)
				{
					ChromeWindow newWnd = {-1, hwnd};

					reinterpret_cast<vector<ChromeWindow>*>(lParam)->push_back(newWnd);

					break;
				}
			}

			break;
		}
	}

	return TRUE;
}

BOOL CChromeTrayIcon::FindAllChromeWindows(vector<ChromeWindow> &windows)
{
	EnumWindows(EnumChromeWindowsProc, reinterpret_cast<LPARAM>(&windows));

	return TRUE;
}

BOOL CChromeTrayIcon::RegisterHotKey(LPCTSTR atomName, ATOM &id, WORD key, WORD modifier)
{
	if(id == 0)
	{
		id = GlobalAddAtom(atomName);
	}
	else
	{
		UnregisterHotKey(id);
	}

	return ::RegisterHotKey(m_hWnd, id, modifier, key);
}

BOOL CChromeTrayIcon::RegisterHotKeys()
{
	if(m_options.bEnableBossKey)
	{
		RegisterHotKey(_T("chromeTrayHotKeyBoss"), m_HotKeyIdBoss, m_options.wBossKey, m_options.wBossModifier);
	}

	if(m_options.bEnableRestoreByKey)
	{
		RegisterHotKey(_T("chromeTrayHotKeyRestore"), m_HotKeyIdRestore, m_options.wRestoreByKeyKey, m_options.wRestoreByKeyModifier);
	}

	return TRUE;
}

BOOL CChromeTrayIcon::UnregisterHotKey(ATOM &id)
{
	if(id != 0)
	{
		::UnregisterHotKey(m_hWnd, (int)id);
		GlobalDeleteAtom(id);
		id = 0;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void CChromeTrayIcon::UnregisterHotKeys()
{
	UnregisterHotKey(m_HotKeyIdBoss);
	UnregisterHotKey(m_HotKeyIdRestore);
}

void CChromeTrayIcon::UpdateIconTooltip(HWND hLastWnd)
{
	TCHAR szTooltip[255] = {0};

	if(hLastWnd == NULL)
	{
		EnterCriticalSection(&m_csWindowsList);

		for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
		{
			hLastWnd = m_ChromeWindows[i].hWnd;

			if(::IsWindowVisible(hLastWnd) == FALSE)
			{
				::GetWindowText(hLastWnd, szTooltip, _countof(szTooltip));
				break;
			}
		}

		LeaveCriticalSection(&m_csWindowsList);
	}
	else
	{
		::GetWindowText(hLastWnd, szTooltip, _countof(szTooltip));
	}

	if(wcslen(szTooltip) == 0)
	{
		wcscpy_s(szTooltip, _T("Google Chrome"));
	}

	m_TrayIcon.SetTooltip(szTooltip);
}

void CChromeTrayIcon::ShowHideTrayIcon()
{
	BOOL hiddenWindows = FALSE;

	for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
	{
		if(::IsWindow(m_ChromeWindows[i].hWnd))
		{
			if(::IsWindowVisible(m_ChromeWindows[i].hWnd) == FALSE)
			{
				hiddenWindows = TRUE;
				break;
			}
		}
	}

	if(m_options.bHideTrayIfNowWindows == TRUE)
	{
		if(hiddenWindows == TRUE)
		{
			if(m_TrayIcon.IsIconVisible() == FALSE)
			{
				m_TrayIcon.ShowIcon();
			}
		}
		else
		{
			if(m_TrayIcon.IsIconVisible() == TRUE)
			{
				m_TrayIcon.HideIcon();
			}
		}
	}
	else
	{
		if(m_TrayIcon.IsIconVisible() == FALSE)
		{
			m_TrayIcon.ShowIcon();
		}
	}
}

void CChromeTrayIcon::OnBossKeyPressed()
{
	if(m_bChromeIsHidded)
	{
		m_bChromeIsHidded = FALSE;

		EnterCriticalSection(&m_csWindowsList);

		for(vector<ChromeWindow>::iterator it = m_ChromeWindows.begin(); it != m_ChromeWindows.end(); ++it)
		{
			if((*it).bWasVisibleBeforeBoss == TRUE)
			{
				ShowChromeWindow((*it).hWnd);
			}
		}

		LeaveCriticalSection(&m_csWindowsList);

		ShowHideTrayIcon();
		UpdateIconTooltip(NULL);
	}
	else
	{
		m_bChromeIsHidded = TRUE;

		HWND hWnd;

		EnterCriticalSection(&m_csWindowsList);

		m_ChromeWindows.clear();

		FindAllChromeWindows(m_ChromeWindows);

		for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
		{
			hWnd = m_ChromeWindows[i].hWnd;

			if(::IsWindowVisible(hWnd))
			{
				m_ChromeWindows[i].bWasVisibleBeforeBoss = TRUE;

				HideChromeWindow(hWnd);
			}
			else
			{
				m_ChromeWindows[i].bWasVisibleBeforeBoss = FALSE;
			}
		}

		LeaveCriticalSection(&m_csWindowsList);

		if(m_options.bBossHideTrayIcon)
		{
			m_TrayIcon.HideIcon();
		}
	}
}

void CChromeTrayIcon::OnRestoreByKeyPressed()
{
	if(m_bChromeIsHidded)
	{
		return;
	}

	EnterCriticalSection(&m_csWindowsList);

	for(size_t i = 0; i < m_ChromeWindows.size(); ++i)
	{
		HWND hWnd = m_ChromeWindows[i].hWnd;

		if(::IsWindowVisible(hWnd) == FALSE)
		{
			ShowChromeWindow(hWnd);
		}
	}

	LeaveCriticalSection(&m_csWindowsList);

	UpdateIconTooltip(NULL);
}