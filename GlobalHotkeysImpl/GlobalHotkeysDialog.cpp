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

#include <commctrl.h>

extern std::map<const std::string, Actions> actionsMap;

BOOL GlobalHotkeysDialog::OnInitDialog()
{
	m_hotkeysListView.AttachDlgItem(IDC_HOTKEYS_LIST, this);
	m_actionsComboBox.AttachDlgItem(IDC_ACTIONS_COMBO, this);
	m_hotkeyTextEdit.AttachDlgItem(IDC_HOTKEY_TEXT, this);

	InitHotkeysListViewColumns();
	PopulateHotkeysList();

	PopulateActionsComboBox();

	return TRUE;
}

BOOL GlobalHotkeysDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
    {
	case IDAPPLY:
		OnApply();
		return TRUE;
    }

	return FALSE;
}

void GlobalHotkeysDialog::OnOK()
{
	OnApply();
}

void GlobalHotkeysDialog::OnApply()
{

}

void GlobalHotkeysDialog::InitHotkeysListViewColumns()
{
	HWND hwndListView = GetDlgItem(IDC_HOTKEYS_LIST)->GetHwnd();

	ListView_SetExtendedListViewStyle(hwndListView, LVS_EX_FULLROWSELECT);

	// init columns
	LVCOLUMN lvc;
	ZeroMemory(&lvc, sizeof(LVCOLUMN));

	lvc.mask = LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 172;

	// Action
	lvc.iOrder = 0;
	lvc.iSubItem = 0;
	lvc.pszText = TEXT("Action");

	ListView_InsertColumn(hwndListView, 0, &lvc);

	// Hotkey
	lvc.iOrder = 1;
	lvc.iSubItem = 1;
	lvc.pszText = TEXT("Hotkey");

	ListView_InsertColumn(hwndListView, 1, &lvc);
}

void GlobalHotkeysDialog::AddHotkeyListItem(const std::string action, const std::string hotkey)
{
	static int index = 0;
	HWND hwndListView = GetDlgItem(IDC_HOTKEYS_LIST)->GetHwnd();

	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(LVITEM));

	lvi.mask = LVIF_TEXT | LVIF_STATE;
	lvi.state = 0;
	lvi.stateMask = 0;
	lvi.iItem = index;
	lvi.iSubItem = 0;
	lvi.pszText = (LPSTR) action.c_str();

	ListView_InsertItem(hwndListView, &lvi);
	ListView_SetItemText(hwndListView, index, 1, (LPSTR) hotkey.c_str());

	index++;
}

void GlobalHotkeysDialog::PopulateHotkeysList()
{
	//HWND hwndListView = GetDlgItem(IDC_HOTKEYS_LIST)->GetHwnd();

	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		const std::string action = iter->second->GetActionName();
		const std::string hotkey = iter->second->GetHotkeyName();
		
		AddHotkeyListItem(action, hotkey);
	}
}

void GlobalHotkeysDialog::PopulateActionsComboBox()
{
	CWnd* wndCombo = GetDlgItem(IDC_ACTIONS_COMBO);

	std::map<const std::string, Actions>::iterator iter;
	for (iter = actionsMap.begin(); iter != actionsMap.end(); iter++) {
		wndCombo->SendMessage(CB_ADDSTRING, 0, (LPARAM) iter->first.c_str());
	}	 
}

LRESULT ActionsComboBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_COMMAND && HIWORD(wParam) == CBN_SELCHANGE)
		OnSelectedActionChanged();

	return WndProcDefault(uMsg, wParam, lParam);
}

void ActionsComboBox::OnSelectedActionChanged()
{
	CWnd* wndListView = GetParent()->GetDlgItem(IDC_HOTKEYS_LIST);
	CWnd* wndCombo    = GetParent()->GetDlgItem(IDC_ACTIONS_COMBO);
	CWnd* wndEditText = GetParent()->GetDlgItem(IDC_HOTKEY_TEXT);

	char selectedItem[255];
	ZeroMemory(&selectedItem, sizeof(char) * 255);

	if (0 == wndCombo->SendMessage(WM_GETTEXT, sizeof(selectedItem), (LPARAM) &selectedItem))
		return;

	LVFINDINFO lfi;
	ZeroMemory(&lfi, sizeof(LVFINDINFO));

	lfi.flags = LVFI_STRING;
	lfi.psz = selectedItem;

	int index = ListView_FindItem(wndListView->GetHwnd(), -1, &lfi);
	ZeroMemory(&selectedItem, sizeof(char) * 255);

	if (-1 == index) {
		ListView_SetItemState(wndListView->GetHwnd(), index, 0 , LVIS_SELECTED);
		wndEditText->SendMessage(WM_SETTEXT, 0, (LPARAM) &selectedItem);
		return;
	}

	ListView_SetItemState(wndListView->GetHwnd(), index, LVIS_SELECTED , LVIS_SELECTED);
	ListView_GetItemText (wndListView->GetHwnd(), index, 1, selectedItem, 255);
	wndEditText->SendMessage(WM_SETTEXT, 0, (LPARAM) &selectedItem);
}

LRESULT HotkeysListView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NOTIFY && (LPNMHDR(lParam))->code == LVN_ITEMCHANGED) {
		OnSelectedListItemChanged(LPNMLISTVIEW(lParam));
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

void HotkeysListView::OnSelectedListItemChanged(LPNMLISTVIEW lpStateChange)
{
	CWnd* wndListView = GetParent()->GetDlgItem(IDC_HOTKEYS_LIST);
	CWnd* wndCombo    = GetParent()->GetDlgItem(IDC_ACTIONS_COMBO);
	CWnd* wndEditText = GetParent()->GetDlgItem(IDC_HOTKEY_TEXT);

	int i = 0; i = 1/ i;

	if (lpStateChange->uOldState == LVIS_SELECTED || lpStateChange->uNewState != LVIS_SELECTED)
		return;

	int index = lpStateChange->iItem;

	char selectedItem[255];
	ZeroMemory(&selectedItem, sizeof(char) * 255);

	ListView_GetItemText (wndListView->GetHwnd(), index, 0, selectedItem, 255);
	
	int cbItemIndex = wndCombo->SendMessage(CB_FINDSTRINGEXACT, -1, (LPARAM)(LPCSTR)&selectedItem);
	int cbSelectedIndex = wndCombo->SendMessage(CB_GETCURSEL, 0, 0);

	if (cbSelectedIndex != cbItemIndex)
		wndCombo->SendMessage(CB_SETCURSEL, cbItemIndex, 0);

	ZeroMemory(&selectedItem, sizeof(char) * 255);
	ListView_GetItemText (wndListView->GetHwnd(), index, 1, selectedItem, 255);
	wndEditText->SendMessage(WM_SETTEXT, 0, (LPARAM) &selectedItem);
}

LRESULT HotkeyTextEdit::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	default:
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}
