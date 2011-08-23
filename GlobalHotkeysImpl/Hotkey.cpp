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

#include "Hotkey.h"

#include <windows.h>
#include <commctrl.h>

std::string Hotkey::toString() const
{
	std::string hotkey_name;

	if(!IsEmpty())
	{
		//ctrl, shift, alt, win

		if(m_modifiers & HOTKEYF_CONTROL)
			hotkey_name += GetKeyName(VK_CONTROL) + " + ";

		if(m_modifiers & HOTKEYF_SHIFT)
			hotkey_name += GetKeyName(VK_SHIFT) + " + ";

		if(m_modifiers & HOTKEYF_ALT)
			hotkey_name += GetKeyName(VK_LMENU) + " + ";

		hotkey_name += GetKeyName(m_keyCode, (m_modifiers & HOTKEYF_EXT) != 0);
	}

	return hotkey_name;
}

std::string Hotkey::GetKeyName(BYTE virtualKey, bool extended)
{
	UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) << 16;

	// because MapVirtualKey strips the extended bit for some keys
	/*
	switch(virtualKey)
	{
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
		case VK_PRIOR: case VK_NEXT: // page up and page down
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE: // numpad slash
		case VK_NUMLOCK:
		//case VK_VOLUME_MUTE: case VK_VOLUME_DOWN: case VK_VOLUME_UP: case VK_MEDIA_NEXT_TRACK: case VK_MEDIA_PREV_TRACK: case VK_MEDIA_STOP: case VK_MEDIA_PLAY_PAUSE:
		{
			scanCode |= (1 << 24); // set extended bit
			break;
		}
	}
	}*/
	if(extended) // set extended bit
		scanCode |= (1 << 24);

	scanCode |= (1 << 25); // don't care

	char keyName[50];
	if(GetKeyNameText(scanCode, keyName, _countof(keyName)))
	{
		return keyName;
	}
	return "";
}
