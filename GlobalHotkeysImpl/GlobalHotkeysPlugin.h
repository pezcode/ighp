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

#ifndef GLOBAL_HOTKEYS_PLUGIN_H
#define GLOBAL_HOTKEYS_PLUGIN_H

#include <windows.h>

#include <Win32++\WinCore.h>
#include "resource.h"

#include "GlobalHotkeysDialog.h"

#include "Action.h"
#include "Hotkey.h"

#include <vector>

class GlobalHotkeysWnd : public CWnd
{
public:
	explicit GlobalHotkeysWnd() { } 
	virtual ~GlobalHotkeysWnd() { }

protected:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void PreRegisterClass(WNDCLASS &wc);
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	void OnHotkey(WPARAM wParam, LPARAM lParam);
	void OnDestroy();
};

class GlobalHotkeysPlugin : public CWinApp
{
public:
	explicit GlobalHotkeysPlugin(HINSTANCE dllHandle); 
	virtual ~GlobalHotkeysPlugin();

	static GlobalHotkeysPlugin& Instance() { return *((GlobalHotkeysPlugin*)GetApp()); }

	GlobalHotkeysDialog GetSettingsDialog() { return GlobalHotkeysDialog(IDD_GH_DIALOG); }

	bool UnregisterHotkeys();
	bool RegisterHotkeys(const std::map<Action::Type, Hotkey>& hotkeys);

	const std::vector<Action>& GetHotKeys() { return m_hotkeys; }

	static const int version = 10;

private:
	GlobalHotkeysWnd m_mainWindow;

	std::vector<Action> m_hotkeys;
};

#endif /* GLOBAL_HOTKEYS_PLUGIN_H */
