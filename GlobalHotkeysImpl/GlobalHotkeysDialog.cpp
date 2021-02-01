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

#include "GlobalHotkeysPlugin.h"
#include "GlobalHotkeysDialog.h"
#include "PluginSettings.h"

#include <shellapi.h>
#include <type_traits>

const wchar_t CAboutPage::URL[] = L"http://pezcode.github.com/ighp/";

BOOL CSettingsPage::OnInitDialog()
{
	Action::InitNames();

	m_hotkeysListView.AttachDlgItem(IDC_HOTKEYS_LIST, *this);
	m_hotkeyInput.AttachDlgItem(IDC_HOTKEY_CONTROL, *this);

	// Get a temporary copy
	m_hotkeys = PluginSettings::Instance().GetHotkeys();
	// Temporarily disable hotkeys
	GlobalHotkeysPlugin::Instance().UnregisterHotkeys();

	InitHotkeysListView();
	PopulateHotkeysList();
	return TRUE;
}

BOOL CSettingsPage::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
    {
	case IDC_SET:
		OnSet();
		return TRUE;
	case IDC_CLEAR:
		OnClear();
		return TRUE;
    }
	
	return CPropertyPage::OnCommand(wParam, lParam);
}

LRESULT CSettingsPage::OnNotify(WPARAM wParam, LPARAM lParam)
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

	return CPropertyPage::OnNotify(wParam, lParam);
}

BOOL CSettingsPage::OnApply()
{
	// save new hotkeys
	PluginSettings::Instance().SetHotkeys(m_hotkeys);
	PluginSettings::Instance().WriteConfig();
	return TRUE;
}

void CSettingsPage::OnClose()
{
	// Enable hotkeys again
	GlobalHotkeysPlugin::Instance().RegisterHotkeys(PluginSettings::Instance().GetHotkeys());
	CPropertyPage::OnClose();
}

/*
void CSettingsPage::OnDestroy()
{
	
}
*/

void CSettingsPage::OnSet()
{
	int index = m_hotkeysListView.GetNextItem(-1, LVNI_SELECTED);
	if(index < 0)
		return;

	Action::Type action = Action::Type(index + 1);

	WORD wKeys = LOWORD(m_hotkeyInput.GetHotKey());
	BYTE bKeyCode = LOBYTE(wKeys);
	BYTE bModifiers = HIBYTE(wKeys);
	Hotkey hotkey(bKeyCode, bModifiers);

	m_hotkeysListView.SetItemText(index, 1, hotkey.toString().c_str());
	m_hotkeys[action] = hotkey;

	SetModified(TRUE);
}

void CSettingsPage::OnClear()
{
	int index = m_hotkeysListView.GetNextItem(-1, LVNI_SELECTED);
	if(index < 0)
		return;

	Action::Type action = Action::Type(index + 1);
	m_hotkeys[action] = Hotkey();

	m_hotkeyInput.SetHotKey(0);
	m_hotkeysListView.SetItemText(index, 1, L"");

	SetModified(TRUE);
}

void CSettingsPage::InitHotkeysListView()
{
	m_hotkeysListView.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_hotkeysListView.InsertColumn(0, L"Action", LVCFMT_LEFT, -1, 0);
	m_hotkeysListView.InsertColumn(1, L"Hotkey", LVCFMT_LEFT, -1, 1);
}

void CSettingsPage::AddHotkeyListItem(const std::wstring& action, const std::wstring& hotkey)
{
	int index = m_hotkeysListView.InsertItem(m_hotkeysListView.GetItemCount(), action.c_str());
	m_hotkeysListView.SetItemText(index, 1, hotkey.c_str());
}

void CSettingsPage::PopulateHotkeysList()
{
	for(auto iter = Action::names.begin(); iter != Action::names.end(); iter++)
	{
		Hotkey hotkey = m_hotkeys[iter->first];
		AddHotkeyListItem(iter->second, hotkey.toString());
	}

	// Update column widths after filling the listview (account for possible scrollbar)
	m_hotkeysListView.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_hotkeysListView.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER); // use remaining space
}

void CSettingsPage::OnSelectedListItemChanged(const NMLISTVIEW* lpStateChange)
{
	if((lpStateChange->uOldState & LVIS_SELECTED) || !(lpStateChange->uNewState & LVIS_SELECTED))
		return;

	Action::Type action = Action::Type(lpStateChange->iItem + 1);
	Hotkey hotkey = m_hotkeys[action];

	m_hotkeyInput.SetHotKey(MAKEWORD(hotkey.GetKeyCode(), hotkey.GetModifiers()));

	// Set focus to hotkey control
	// Only works while the mouse button is down, then goes back to the listview
	//this->PostMessage(WM_NEXTDLGCTL, (WPARAM)m_hotkeyInput.GetHwnd(), TRUE);
}

void CAboutPage::MakeItalic(CWnd* Window, bool italic)
{
	LOGFONT lf = Window->GetFont().GetLogFont();
	lf.lfItalic = italic ? TRUE : FALSE;
	lf.lfOutPrecision = italic ? OUT_TT_ONLY_PRECIS : lf.lfOutPrecision;
	lf.lfQuality = italic ? PROOF_QUALITY : lf.lfQuality;
	CFont fontNew(lf);
	Window->SetFont(fontNew, false);
}

BOOL CAboutPage::OnInitDialog()
{
	m_Title.AttachDlgItem(IDC_TITLE, *this);
	m_Version.AttachDlgItem(IDC_VERSION, *this);
	m_URL.AttachDlgItem(IDC_URL, *this);

	m_Version.SetWindowText(GlobalHotkeysPlugin::version_str);

	//MakeBold(&m_Title, true);
	//Resize(&m_Title, 3);

	//InitURLControl();

	return TRUE;
}

LRESULT CAboutPage::OnNotify(WPARAM wParam, LPARAM lParam)
{
	switch(LPNMHDR(lParam)->idFrom)
	{
	case IDC_URL:
		switch(LPNMHDR(lParam)->code)
		{
		case NM_CLICK:  // mouse
		case NM_RETURN: // keyboard
			OnURL(PNMLINK(lParam));
			return TRUE;
		}
		break;
	}

	return CPropertyPage::OnNotify(wParam, lParam);
}

void CAboutPage::OnURL(const NMLINK* lpClick)
{
	const LITEM& item = lpClick->item;
	if(item.iLink == 0)
	{
		ShellExecuteW(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOW);
	}
}

void CAboutPage::InitURLControl()
{
	// Set the text first so the SysLink controls which URL is which iLink
	std::wstring title = std::wstring(L"<a>") + URL + L"</a>";
	m_URL.SetWindowText(title.c_str());

	LITEM item = {};
	item.mask = LIF_ITEMINDEX | LIF_URL;
	item.iLink = 0;
	wcscpy_s(item.szUrl, std::extent<decltype(item.szUrl)>::value, URL);
	m_URL.SendMessage(LM_SETITEM, NULL, (LPARAM)&item);
}

void CAboutPage::MakeBold(CWnd* Window, bool bold)
{
	LOGFONT lf = Window->GetFont().GetLogFont();
	lf.lfWeight = bold ? FW_BOLD : FW_NORMAL;
	CFont fontNew(lf);
	Window->SetFont(fontNew, false);
}

void CAboutPage::Resize(CWnd* Window, int delta)
{
	LOGFONT lf = Window->GetFont().GetLogFont();
	if(lf.lfHeight)
	{
		if(lf.lfHeight < 0)
			lf.lfHeight -= delta;
		else
			lf.lfHeight += delta;
	}
	CFont fontNew(lf);
	Window->SetFont(fontNew, false);
}

void GlobalHotkeysDialog::OnInitialUpdate()
{
	CenterWindow();
}
