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

//const char GlobalHotkeysPlugin::version_str[] = "0.1.0";

GlobalHotkeysPlugin::GlobalHotkeysPlugin(HINSTANCE dllHandle) : m_settingsDialog(IDD_GH_DIALOG)
{
	SetResourceHandle(dllHandle);
	m_mainWindow.Create();
}

GlobalHotkeysPlugin::~GlobalHotkeysPlugin()
{
	m_mainWindow.Destroy();
	//::PostQuitMessage(0);
}

void GlobalHotkeysWnd::PreRegisterClass(WNDCLASS &wc)
{
	wc.style            = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc      = CWnd::StaticWindowProc;
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hInstance        = GetApp()->GetInstanceHandle();
	wc.hIcon            = NULL;
	wc.hCursor          = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground    = (HBRUSH) (COLOR_WINDOW+1);
	wc.lpszMenuName     = NULL;
	wc.lpszClassName    = TEXT("GlobalHotkeysClass");
}

void GlobalHotkeysWnd::PreCreate(CREATESTRUCT &cs)
{
	cs.hInstance      = GetApp()->GetInstanceHandle();
	cs.hMenu          = NULL;
	cs.hwndParent     = NULL;
	cs.lpCreateParams = NULL;
	cs.lpszName       = TEXT("Global Hotkeys");
	cs.style          = WS_POPUP;
	cs.dwExStyle      = WS_EX_TRANSPARENT;
	cs.x              = CW_USEDEFAULT;
	cs.y              = CW_USEDEFAULT;
	cs.cx             = CW_USEDEFAULT;
	cs.cy             = CW_USEDEFAULT;
}

LRESULT GlobalHotkeysWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_HOTKEY:
		OnHotkey(wParam, lParam);
		break;
	case WM_DESTROY:
		OnDestroy();
		break;
	}

	// Pass unhandled messages on to parent DialogProc
	return WndProcDefault(uMsg, wParam, lParam);
}

//
// Handle global hotkeys
//
void GlobalHotkeysWnd::OnHotkey(WPARAM wParam, LPARAM lParam)
{
	const std::vector<Action>& hotkeys = GlobalHotkeysPlugin::Instance().GetHotKeys();
	if(wParam >= 0 && wParam < hotkeys.size())
	{
		hotkeys[wParam].Perform();
	}
}

void GlobalHotkeysWnd::OnDestroy()
{
	GlobalHotkeysPlugin::Instance().UnregisterHotkeys();
}

bool GlobalHotkeysPlugin::RegisterHotkeys(const std::map<Action::Type, Hotkey>& hotkeys)
{
	UnregisterHotkeys();

	std::map<Action::Type, Hotkey>::const_iterator iter;
	for(iter = hotkeys.begin(); iter != hotkeys.end(); iter++)
	{
		Hotkey hotkey = iter->second;
		if(!hotkey.IsEmpty())
		{
			Action::Type action = iter->first;
			BYTE bMod = 0;
			// Map from WM_SETHOTKEY constants to RegisterHotKey values
			if(hotkey.GetModifiers() & HOTKEYF_CONTROL)
				bMod |= MOD_CONTROL;
			if(hotkey.GetModifiers() & HOTKEYF_SHIFT)
				bMod |= MOD_SHIFT;
			if(hotkey.GetModifiers() & HOTKEYF_ALT)
				bMod |= MOD_ALT;
			if(hotkey.GetModifiers() & HOTKEYF_EXT)
				bMod |= MOD_ALT; // :s
			RegisterHotKey(m_mainWindow.GetHwnd(), m_hotkeys.size(), bMod, hotkey.GetKeyCode());
			m_hotkeys.push_back(Action(action));
		}
	}
	return true;
}

bool GlobalHotkeysPlugin::UnregisterHotkeys()
{
	for(int i = 0; i < m_hotkeys.size(); i++)
	{
		UnregisterHotKey(m_mainWindow.GetHwnd(), i);
	}
	m_hotkeys.clear();
	return true;
}
