/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * Copyright (c) 2021 pezcode <mail@rvrs.in>
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

#ifndef GLOBAL_HOTKEYS_DIALOG_H
#define GLOBAL_HOTKEYS_DIALOG_H

#include <windows.h>

#include <Win32++\wxx_propertysheet.h>
#include <Win32++\wxx_stdcontrols.h>
#include <Win32++\wxx_controls.h>
#include <Win32++\wxx_listview.h>
#include "resource.h"

#include "Action.h"
#include "Hotkey.h"

class CSettingsPage : public CPropertyPage
{
public:
	CSettingsPage() : CPropertyPage(IDD_GH_SETTINGS, L"Settings") { }

	//virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	virtual BOOL OnInitDialog() override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	
	virtual BOOL OnApply() override;

	virtual void OnClose() override;
	//virtual void OnDestroy() override;

private:
	CListView m_hotkeysListView;
	CHotKey m_hotkeyInput;

	std::map<Action::Type, Hotkey> m_hotkeys; // temporary copy

	void OnSet();
	void OnClear();

	void InitHotkeysListView();
	void PopulateHotkeysList();

	void AddHotkeyListItem(const std::wstring& action, const std::wstring& hotkey);

	void OnSelectedListItemChanged(const NMLISTVIEW* lpStateChange);
};

class CAboutPage : public CPropertyPage
{
public:
	CAboutPage() : CPropertyPage(IDD_GH_ABOUT, L"About") { }

	virtual BOOL OnInitDialog() override;
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;

private:
	CStatic m_Title;
	CStatic m_Version;
	CWnd m_URL;

	static const wchar_t URL[];

	void OnURL(const NMLINK* lpClick);

	void InitURLControl();

	void MakeBold(CWnd* Window, bool bold);
	void Resize(CWnd* Window, int delta);
	void MakeItalic(CWnd* Window, bool italic);
};

class GlobalHotkeysDialog : public CPropertySheet
{
public:
	GlobalHotkeysDialog(HWND parent = NULL) : CPropertySheet(L"Global Hotkeys Plugin", parent) { }

protected:
	virtual void OnInitialUpdate() override;
};

#endif /* GLOBAL_HOTKEYS_DIALOG_H */
