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

#ifndef GLOBAL_HOTKEYS_DIALOG_H
#define GLOBAL_HOTKEYS_DIALOG_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Win32++\dialog.h>
#include "resource.h"

class ActionsComboBox : public CWnd
{
public:
	explicit ActionsComboBox() { };
	virtual ~ActionsComboBox() { };

private:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnSelectedActionChanged();
};

class HotkeysListView : public CWnd
{
public:
	explicit HotkeysListView() { };
	virtual ~HotkeysListView() { };

private:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnSelectedListItemChanged(LPNMLISTVIEW lpStateChange);
};

class HotkeyTextEdit : public CWnd
{
public:
	explicit HotkeyTextEdit() { };
	virtual ~HotkeyTextEdit() { };

private:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class GlobalHotkeysDialog : public CDialog
{
public:
	explicit GlobalHotkeysDialog(UINT nResID, CWnd* pParent = NULL) : CDialog(nResID, pParent) { };
	virtual ~GlobalHotkeysDialog() { };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

private:
	ActionsComboBox m_actionsComboBox;
	HotkeysListView m_hotkeysListView;
	HotkeyTextEdit m_hotkeyTextEdit;

	void OnApply();

	void InitHotkeysListViewColumns();
	void PopulateHotkeysList();
	void PopulateActionsComboBox();

	void AddHotkeyListItem(const std::string action, const std::string hotkey);

};

#endif /* GLOBAL_HOTKEYS_DIALOG_H */
