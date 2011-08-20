/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "GlobalHotkeysPlugin.h"
#include "PluginSettings.h"

extern HANDLE dllHandle;

HWND hWd = 0;

GlobalHotkeysPlugin::GlobalHotkeysPlugin() 
	: m_mainWindow(), m_settingsDialog(IDD_GH_DIALOG)
{
	SetResourceHandle((HINSTANCE)dllHandle);
	
	m_mainWindow.Create();
}

GlobalHotkeysPlugin::~GlobalHotkeysPlugin()
{
	m_mainWindow.DestroyWindow();

	::PostQuitMessage(0);
}

void GlobalHotkeysWnd::PreRegisterClass(WNDCLASS &wc)
{
	m_wc.style			= CS_HREDRAW|CS_VREDRAW;
	m_wc.lpfnWndProc	= CWnd::StaticWindowProc;
	m_wc.cbClsExtra		= 0;
	m_wc.cbWndExtra		= 0;
	m_wc.hInstance		= GetApp()->GetInstanceHandle();
	m_wc.hIcon			= NULL;
	m_wc.hCursor		= LoadCursor (NULL, IDC_ARROW);
	m_wc.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	m_wc.lpszMenuName	= NULL;
	m_wc.lpszClassName  = TEXT("GlobalHotkeysClass");
}

void GlobalHotkeysWnd::PreCreate(CREATESTRUCT &cs)
{
	m_cs.cx             = CW_USEDEFAULT;
	m_cs.cy             = CW_USEDEFAULT;
	m_cs.dwExStyle      = WS_EX_TRANSPARENT;
	m_cs.hInstance      = GetApp()->GetInstanceHandle();
	m_cs.hMenu          = NULL;
	m_cs.hwndParent     = NULL;
	m_cs.lpCreateParams = cs.lpCreateParams;
	m_cs.lpszClass      = cs.lpszClass;
	m_cs.lpszName       = TEXT("Global Hotkeys");
	m_cs.style          = WS_POPUP;
	m_cs.x              = CW_USEDEFAULT;
	m_cs.y              = CW_USEDEFAULT;
}

LRESULT GlobalHotkeysWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		OnCreate(hWnd);
		break;
	case WM_HOTKEY:
		OnHotkey(wParam, lParam);
		break;
	case WM_DESTROY:
		OnDestroy(hWnd);
		break;
	}

	// Pass unhandled messages on to parent DialogProc
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}


//
// Register global hotkeys
//
void GlobalHotkeysWnd::OnCreate(HWND hWnd)
{
	hWd = hWnd;

	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		unsigned int modifiers = (iter->second->GetAlt() ? MOD_ALT : 0) | 
			                     (iter->second->GetControl() ? MOD_CONTROL : 0) | 
			                     (iter->second->GetShift() ? MOD_SHIFT : 0) | 
			                     (iter->second->GetWin() ? MOD_WIN : 0);
		RegisterHotKey(hWnd, iter->first, modifiers, iter->second->GetKeyCode());
	}
}

//
// Handle global hotkeys
//
void GlobalHotkeysWnd::OnHotkey(WPARAM wParam, LPARAM lParam)
{
	std::map<const unsigned int, Hotkey*>& hotkeys = *PluginSettings::Instance()->GetHotkeys();
	hotkeys[wParam]->PerformAction();
}

//
// Unregister global hotkeys
//
void GlobalHotkeysWnd::OnDestroy(HWND hWnd)
{
	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		UnregisterHotKey(hWnd, iter->first);
		delete iter->second;
		iter->second = 0;
	}
}

