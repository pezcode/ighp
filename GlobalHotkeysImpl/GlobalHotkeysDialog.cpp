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

#include "GlobalHotkeysPlugin.h"
#include "GlobalHotkeysDialog.h"
#include "PluginSettings.h" 

void CHotkey::PreCreate(CREATESTRUCT &cs)
{
	cs.hInstance      = GetApp()->GetInstanceHandle();
	cs.hMenu          = NULL;
	cs.hwndParent     = NULL;
	cs.lpCreateParams = NULL;
	cs.lpszName       = TEXT("");
	cs.lpszClass      = HOTKEY_CLASS;
	cs.style          = WS_CHILD | WS_VISIBLE;
	cs.dwExStyle      = NULL;
	cs.x              = CW_USEDEFAULT;
	cs.y              = CW_USEDEFAULT;
	cs.cx             = CW_USEDEFAULT;
	cs.cy             = CW_USEDEFAULT;
}

BOOL GlobalHotkeysDialog::OnInitDialog()
{
	Action::InitNames();

	m_hotkeysListView.AttachDlgItem(IDC_HOTKEYS_LIST, this);
	m_hotkeyInput.AttachDlgItem(IDC_HOTKEY_CONTROL, this);
	m_applyButton.AttachDlgItem(IDAPPLY, this);

	// Get a temporary copy
	m_hotkeys = PluginSettings::Instance().GetHotkeys();
	// Temporarily disable hotkeys
	GlobalHotkeysPlugin::Instance().UnregisterHotkeys();

	InitHotkeysListView();
	PopulateHotkeysList();
	return TRUE;
}

BOOL GlobalHotkeysDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
    {
		return TRUE;
	case IDC_SET:
		OnSet();
		return TRUE;
	case IDC_CLEAR:
		OnClear();
		return TRUE;
	case IDAPPLY:
		OnApply();
		return TRUE;
    }

	return FALSE;
}

LRESULT GlobalHotkeysDialog::OnNotify(WPARAM wParam, LPARAM lParam)
{
	switch(LPNMHDR(lParam)->idFrom)
	{
	case IDC_HOTKEYS_LIST:
		switch(LPNMHDR(lParam)->code)
		{
		case LVN_ITEMCHANGED:
			OnSelectedListItemChanged(LPNMLISTVIEW(lParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void GlobalHotkeysDialog::OnSet()
{
	int index = m_hotkeysListView.GetNextItem(-1, LVNI_SELECTED);
	Action::Type action = Action::Type(index + 1);

	WORD wKeys = m_hotkeyInput.GetHotKey();
	BYTE bKeyCode = LOBYTE(wKeys);
	BYTE bModifiers = HIBYTE(wKeys);
	Hotkey hotkey(bKeyCode, bModifiers);

	m_hotkeysListView.SetItemText(index, 1, hotkey.toString().c_str());
	m_hotkeys[action] = hotkey;

	m_applyButton.EnableWindow(true);
}

void GlobalHotkeysDialog::OnClear()
{
	int index = m_hotkeysListView.GetNextItem(-1, LVNI_SELECTED);
	Action::Type action = Action::Type(index + 1);
	m_hotkeys[action] = Hotkey(); // set empty

	m_hotkeyInput.SetHotKey(0, 0);
	m_hotkeysListView.SetItemText(index, 1, "");

	m_applyButton.EnableWindow(true);
}

void GlobalHotkeysDialog::OnOK()
{
	OnApply();
	CDialog::OnOK();
}

void GlobalHotkeysDialog::EndDialog(INT_PTR nResult)
{
	// Enable hotkeys again
	GlobalHotkeysPlugin::Instance().RegisterHotkeys(PluginSettings::Instance().GetHotkeys());
	CDialog::EndDialog(nResult);
}

void GlobalHotkeysDialog::OnApply()
{
	// save new hotkeys
	PluginSettings::Instance().SetHotkeys(m_hotkeys);
	PluginSettings::Instance().WriteConfig();

	m_applyButton.EnableWindow(false);
}

void GlobalHotkeysDialog::InitHotkeysListView()
{
	m_hotkeysListView.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_hotkeysListView.InsertColumn(0, "Action", LVCFMT_LEFT, -1, 0);
	m_hotkeysListView.InsertColumn(1, "Hotkey", LVCFMT_LEFT, -1, 1);
}

void GlobalHotkeysDialog::AddHotkeyListItem(const std::string& action, const std::string& hotkey)
{
	int index = m_hotkeysListView.InsertItem(m_hotkeysListView.GetItemCount(), action.c_str());
	m_hotkeysListView.SetItemText(index, 1, hotkey.c_str());
}

void GlobalHotkeysDialog::PopulateHotkeysList()
{
	const std::map<Action::Type, Hotkey>& hotkeys = PluginSettings::Instance().GetHotkeys();

	std::map<Action::Type, Hotkey>::const_iterator iter;
	for(iter = hotkeys.begin(); iter != hotkeys.end(); iter++)
	{
		Action::Type action = iter->first;
		Hotkey hotkey = iter->second;
		AddHotkeyListItem(Action::names[action], hotkey.toString());
	}

	// Update column widths after filling the listview (account for possible scrollbar)
	m_hotkeysListView.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_hotkeysListView.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
}

void GlobalHotkeysDialog::OnSelectedListItemChanged(const NMLISTVIEW* lpStateChange)
{
	if((lpStateChange->uOldState & LVIS_SELECTED) || !(lpStateChange->uNewState & LVIS_SELECTED))
		return;

	Action::Type action = Action::Type(lpStateChange->iItem + 1);
	Hotkey hotkey = m_hotkeys[action];

	m_hotkeyInput.SetHotKey(hotkey.GetKeyCode(), hotkey.GetModifiers());

	// Set focus to hotkey control
	// Only works while the mouse button is down, then goes back to the listview
	this->PostMessage(WM_NEXTDLGCTL, (WPARAM)m_hotkeyInput.GetHwnd(), TRUE);
}
