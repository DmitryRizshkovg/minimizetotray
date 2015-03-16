#pragma once

typedef struct ChromeTab
{
	int		nId;
	wstring	strTitle;
}
ChromeTab;

typedef struct ChromeWindow
{
	int		nId;
	HWND	hWnd;

	BOOL	bWasVisibleBeforeBoss;
}
ChromeWindow;

typedef enum TrayAction {Nothing = 0, ContextMenu, Restore, NewTab, NewWindow};

typedef struct ChromeTrayIconOptions
{
	ChromeTrayIconOptions()
	{
		bHideTrayIfNowWindows		= TRUE;

		bMinimizeOnLeftButton		= TRUE;
		bMinimizeOnRightButton		= FALSE;
		
		actLClick					= Nothing;
		actRClick					= ContextMenu;
		actLDblClick				= Restore;
		actRDblClick				= Nothing;

		bShowNewWindow				= FALSE;
		bShowNewTab					= FALSE;
		bShowOptions				= TRUE;
		bShowFavorites				= FALSE;

		bEnableBossKey				= FALSE;
		wBossKey					= 0;
		wBossModifier				= 0;
		bBossHideTrayIcon			= FALSE;

		bEnableRestoreByKey			= FALSE;
		wRestoreByKeyKey			= 0;
		wRestoreByKeyModifier		= 0;
	}

	BOOL		bHideTrayIfNowWindows;

	BOOL		bMinimizeOnLeftButton;
	BOOL		bMinimizeOnRightButton;

	TrayAction	actLClick;
	TrayAction	actRClick;
	TrayAction	actLDblClick;
	TrayAction	actRDblClick;

	BOOL		bShowNewWindow;
	BOOL		bShowNewTab;
	BOOL		bShowOptions;
	BOOL		bShowFavorites;
	wstring		strFavorites;

	BOOL		bEnableBossKey;
	WORD		wBossKey;
	WORD		wBossModifier;
	BOOL		bBossHideTrayIcon;

	BOOL		bEnableRestoreByKey;
	WORD		wRestoreByKeyKey;
	WORD		wRestoreByKeyModifier;
}
ChromeTrayIconOptions;

typedef struct ChromeTrayIconLanguage
{
	ChromeTrayIconLanguage()
	{
		strOptions		= _T("Options");
		strNewTab		= _T("New Tab");
		strNewWindow	= _T("New Window");
		strFavorites	= _T("Favorites");
	}

	wstring strOptions;
	wstring strNewTab;
	wstring strNewWindow;
	wstring strFavorites;
}
ChromeTrayIconLanguage;