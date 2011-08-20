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

#include "GlobalHotkeysDialog.h"
#include "PluginSettings.h" 

extern std::map<const std::string, Actions> actionsMap;

BOOL GlobalHotkeysDialog::OnInitDialog()
{
	m_hotkeysListView.AttachDlgItem(IDC_HOTKEYS_LIST, this);
	m_actionsComboBox.AttachDlgItem(IDC_ACTIONS_COMBO, this);
	m_hotkeyTextEdit.AttachDlgItem(IDC_HOTKEY_TEXT, this);

	m_applyButton.AttachDlgItem(IDAPPLY, this);

	InitHotkeysListViewColumns();
	m_listviewIndex = 0;

	PopulateHotkeysList();
	PopulateActionsComboBox();

	return TRUE;
}

BOOL GlobalHotkeysDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
    {
	case IDC_ACTIONS_COMBO:
		switch(HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			OnSelectedActionChanged();
			return TRUE;
		}
		break;
	case IDC_ADD:
		OnAdd();
		return TRUE;
	case IDC_CLEAR:
		OnClear();
		return TRUE;
	case IDC_MODIFY:
		OnModify();
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

void GlobalHotkeysDialog::OnAdd()
{
	// get combo text
	// if(exists in listview column 0)
	//   set column 1
	// else
	//   add item
	// 

	m_applyButton.EnableWindow(TRUE);
}

void GlobalHotkeysDialog::OnClear()
{
	// get combo text
	// if(exists in listview column 0)
	//   delete
	// 

	m_applyButton.EnableWindow(TRUE);
}

void GlobalHotkeysDialog::OnModify()
{
	// get combo text
	// if(exists in listview column 0)
	//   set column 1
	//   
	m_applyButton.EnableWindow(TRUE);
}

void GlobalHotkeysDialog::OnOK()
{
	OnApply();
	CDialog::OnOK();
}

void GlobalHotkeysDialog::EndDialog(INT_PTR nResult)
{
	//ReloadHotkeys();
	CDialog::EndDialog(nResult);
}

void GlobalHotkeysDialog::OnApply()
{
	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();

	// get listview items
	// edit hotkeys accordingly

	PluginSettings::Instance()->WriteConfigFile(hotkeys);

	m_applyButton.EnableWindow(FALSE);
}

void GlobalHotkeysDialog::InitHotkeysListViewColumns()
{
	m_hotkeysListView.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_hotkeysListView.InsertColumn(0, "Action", LVCFMT_LEFT, 172, 0);
	m_hotkeysListView.InsertColumn(1, "Hotkey", LVCFMT_LEFT, 172, 1);
}

void GlobalHotkeysDialog::AddHotkeyListItem(const std::string action, const std::string hotkey)
{
	m_hotkeysListView.InsertItem(m_listviewIndex, action.c_str());
	m_hotkeysListView.SetItemText(m_listviewIndex, 1, hotkey.c_str());

	m_listviewIndex++;
}

void GlobalHotkeysDialog::PopulateHotkeysList()
{
	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::const_iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		const std::string action = iter->second->GetActionName();
		const std::string hotkey = iter->second->GetHotkeyName();
		
		AddHotkeyListItem(action, hotkey);
	}
}

void GlobalHotkeysDialog::PopulateActionsComboBox()
{
	std::map<const std::string, Actions>::const_iterator iter;
	for (iter = actionsMap.begin(); iter != actionsMap.end(); iter++) {
		m_actionsComboBox.AddString(iter->first.c_str());
	}	 
}

void GlobalHotkeysDialog::OnSelectedActionChanged()
{
	CString textCombo = m_actionsComboBox.GetWindowText();
	if(textCombo.GetLength() == 0)
		return;

	LVFINDINFO lfi;
	ZeroMemory(&lfi, sizeof(LVFINDINFO));

	lfi.flags = LVFI_STRING;
	lfi.psz = textCombo.c_str();

	int index = m_hotkeysListView.FindItem(lfi);
	if (-1 == index) {
		m_hotkeysListView.SetItemState(index, 0, LVIS_SELECTED);
		m_hotkeyTextEdit.SetWindowText(textCombo.c_str());
		return;
	}

	m_hotkeysListView.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
	std::string textHotkey = m_hotkeysListView.GetItemText(index, 1);
	m_hotkeyTextEdit.SetWindowText(textHotkey.c_str());
}

void GlobalHotkeysDialog::OnSelectedListItemChanged(LPNMLISTVIEW lpStateChange)
{
	if ((lpStateChange->uOldState & LVIS_SELECTED) || !(lpStateChange->uNewState & LVIS_SELECTED))
		return;

	int index = lpStateChange->iItem;
	std::string textAction = m_hotkeysListView.GetItemText(index, 0);

	int cbItemIndex = m_actionsComboBox.FindStringExact(-1, textAction.c_str());
	m_actionsComboBox.SetCurSel(cbItemIndex);

	std::string textHotkey = m_hotkeysListView.GetItemText(index, 1);
	m_hotkeyTextEdit.SetWindowText(textHotkey.c_str());
}
