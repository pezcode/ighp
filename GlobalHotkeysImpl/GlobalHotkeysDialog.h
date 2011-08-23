/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * Copyright (c) 2011 pezcode <mail@rvrs.in>
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

//#include <Win32++\dialog.h>
#include <Win32++\propertysheet.h>
#include <Win32++\stdcontrols.h>
#include <Win32++\controls.h>
#include <Win32++\listview.h>
#include "resource.h"

#include "Action.h"
#include "Hotkey.h"

class CHotkey : public CWnd
{
public:
	CHotkey() { } 
	virtual ~CHotkey() { }

	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	WORD GetHotKey()
	{
		return (WORD)SendMessage(HKM_GETHOTKEY, 0, 0);
	}

	void SetHotKey(BYTE bKeyCode, BYTE bModifier)
	{
		SendMessage(HKM_SETHOTKEY, MAKEWORD(bKeyCode, bModifier), 0);
	}

	void SetRules(WORD wInvalid, WORD wReplacement)
	{
		SendMessage(HKM_SETRULES, wInvalid, wReplacement);
	}

protected:
	virtual void PreCreate(CREATESTRUCT &cs);
};

class CSettingsPage : public CPropertyPage
{
public:
	CSettingsPage() : CPropertyPage(IDD_GH_SETTINGS, "Settings") { }
	virtual ~CSettingsPage() { }

	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	
	virtual int  OnOK();
	virtual void OnCancel();
	virtual int  OnApply();

private:
	CListView m_hotkeysListView;
	CHotkey m_hotkeyInput;

	std::map<Action::Type, Hotkey> m_hotkeys; // temporary copy

	void OnSet();
	void OnClear();

	void InitHotkeysListView();
	void PopulateHotkeysList();

	void AddHotkeyListItem(const std::string& action, const std::string& hotkey);

	void OnSelectedListItemChanged(const NMLISTVIEW* lpStateChange);
};

class CAboutPage : public CPropertyPage
{
public:
	CAboutPage() : CPropertyPage(IDD_GH_ABOUT, "About") { }
	virtual ~CAboutPage() { }

	virtual BOOL OnInitDialog();

private:

};

class GlobalHotkeysDialog : public CPropertySheet
{
public:
	GlobalHotkeysDialog(CWnd* pParent = NULL) : CPropertySheet("Global Hotkeys Plugin", pParent) { }
	virtual ~GlobalHotkeysDialog() { };

protected:
	virtual void OnInitialUpdate();
};

#endif /* GLOBAL_HOTKEYS_DIALOG_H */
