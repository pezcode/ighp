/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * Copyright (c) 2015 pezcode <mail@rvrs.in>
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

#include "Hotkey.h"

#include <windows.h>
#include <commctrl.h>

std::wstring Hotkey::toString() const
{
	std::wstring hotkey_name;

	if(!IsEmpty())
	{
		//ctrl, shift, alt, win

		if(m_modifiers & HOTKEYF_CONTROL)
			hotkey_name += GetKeyName(VK_CONTROL) + L" + ";

		if(m_modifiers & HOTKEYF_SHIFT)
			hotkey_name += GetKeyName(VK_SHIFT) + L" + ";

		if(m_modifiers & HOTKEYF_ALT)
			hotkey_name += GetKeyName(VK_LMENU) + L" + ";

		hotkey_name += GetKeyName(m_keyCode, (m_modifiers & HOTKEYF_EXT) != 0);
	}

	return hotkey_name;
}

std::wstring Hotkey::GetKeyName(BYTE virtualKey, bool extended)
{
	UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) << 16;

	if(extended) // set extended bit
		scanCode |= (1 << 24);

	scanCode |= (1 << 25); // don't care

	wchar_t keyName[50];
	if(GetKeyNameText(scanCode, keyName, _countof(keyName)))
	{
		return keyName;
	}

	return L"";
}
