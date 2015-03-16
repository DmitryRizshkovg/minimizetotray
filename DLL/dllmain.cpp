// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "stubs.h"

#include "DebugLog.h"

#include "ChromeTrayIcon.h"
#include "ScriptableNPObject.h"
#include "JSMethods.h"

static NPNetscapeFuncs *g_BrowserFuncs	= NULL;
static NPPluginFuncs *g_PluginFuncs		= NULL;

#pragma data_seg("Shared")
HINSTANCE g_hInstance = NULL;
#pragma data_seg()

CChromeTrayIcon	g_ChromeTrayIcon;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DebugLog(_T("In DllMain. Reason: %lu"), ul_reason_for_call);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			g_hInstance = hModule;
		}
		break;

	case DLL_THREAD_ATTACH:
		{

		}
		break;

	case DLL_THREAD_DETACH:
		{

		}
		break;

	case DLL_PROCESS_DETACH:
		{
		}
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

NPError SetWindow(NPP instance, NPWindow *window) 
{
	DebugLog(_T("SetWindow called, window handle: 0x%X"), (DWORD)window->window);

	return NPERR_NO_ERROR;
}

NPError Destroy(NPP instance, NPSavedData **save) 
{
	g_ChromeTrayIcon.DestroyTrayIcon();

	return NPERR_NO_ERROR;
}

NPError GetValue(NPP instance, NPPVariable variable, void *value)
{
	switch (variable) 
	{
	case NPPVpluginScriptableNPObject: 
		{
			NPObject *listener = (NPObject*)g_BrowserFuncs->createobject(instance, &CScriptableNPObject::m_npClass);

			CJSMethods::RegisterMethods((CScriptableNPObject*)listener);

			*((NPObject**)value) = listener;

			if(g_ChromeTrayIcon.IsWindow() == FALSE)
			{
				g_ChromeTrayIcon.CreateTrayIcon(g_hInstance);				
			}
		}
		break;

	default: 
		{
			return NPERR_INVALID_PARAM;
		}
		break;
	}

	return NPERR_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

NPError WINAPI NP_GetEntryPoints(NPPluginFuncs* pFuncs)
{
	DebugLog(_T("NP_GetEntryPoints\n"));

	if (g_PluginFuncs != NULL) 
	{
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	g_PluginFuncs = pFuncs;

	pFuncs->newp			= StubNewInstance;
	pFuncs->destroy			= Destroy;
	pFuncs->setwindow		= SetWindow;
	pFuncs->newstream		= StubNewStream;
	pFuncs->destroystream	= StubDestroyStream;
	pFuncs->asfile			= StubStreamAsFile;
	pFuncs->writeready		= StubWriteReady;
	pFuncs->write			= StubWrite;
	pFuncs->print			= StubPrint;
	pFuncs->event			= StubHandleEvent;
	pFuncs->urlnotify		= StubURLNotify;
	pFuncs->getvalue		= GetValue;
	pFuncs->setvalue		= StubSetValue;

	return NPERR_NO_ERROR;
}

NPError WINAPI NP_Initialize(NPNetscapeFuncs *aNPNFuncs)
{
	DebugLog(_T("NP_Initialize\n"));

	if (g_BrowserFuncs != NULL) 
	{
		return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	g_BrowserFuncs = aNPNFuncs;

	CScriptableNPObject::m_pBrowserFuncs = g_BrowserFuncs;

	return NPERR_NO_ERROR;
}

NPError WINAPI NP_Shutdown(void)
{
	DebugLog(_T("NP_Shutdown\n"));

	BOOL bResult = g_ChromeTrayIcon.DestroyTrayIcon();

	DebugLog(_T("DestroyTrayIcon returned: %d"), bResult);

	return NPERR_NO_ERROR;
}