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

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Hotkeys.h"
#include "Actions.h"

std::map<const std::string, unsigned int> hotkeysMap = std::map<const std::string, unsigned int>();
std::map<const unsigned int, Hotkey*> hotkey = std::map<const unsigned int, Hotkey*>();

void InitHotkeysMap()
{
	char str[4];
	for (int i = 0; i < 4; i++)
		str[i] = '\0';

	// add 0-9
	for (unsigned int i = 0x30; i < 0x3A; i++) {
		str[0] = i; 
		hotkeysMap[std::string(str)] = i;
	}

	// add A-Z
	for (unsigned int i = 0x41; i <= 0x5A; i++) {
		str[0] = i; 
		hotkeysMap[std::string(str)] = i;
	}

	// add a-z
	unsigned int j  = 0x61;
	for (unsigned int i = 0x41; i <= 0x5A; i++) {
		str[0] = j++; 
		hotkeysMap[std::string(str)] = i;
	}

	str[0] = 'F';
	j  = 0x31;
	// add F1-F9
	for (unsigned int i = 0x70; i < 0x79; i++) {
		str[1] = j++; 
		hotkeysMap[std::string(str)] = i;
	}

	str[1] = '1';
	j  = 0x30;
	// add F10-F19
	for (unsigned int i = 0x79; i < 0x83; i++) {
		str[2] = j++; 
		hotkeysMap[std::string(str)] = i;
	}

	str[1] = '2';
	j  = 0x30;
	// add F20-F24
	for (unsigned int i = 0x83; i < 0x88; i++) {
		str[2] = j++; 
		hotkeysMap[std::string(str)] = i;
	}

	hotkeysMap["Spacebar"] = VK_SPACE;
	hotkeysMap["Backspace"] = VK_BACK;
	hotkeysMap["Tab"] = VK_TAB;
	hotkeysMap["Escape"] = VK_ESCAPE;
	
	hotkeysMap["PageUp"] = VK_PRIOR;
	hotkeysMap["PageDown"] = VK_NEXT;
	
	hotkeysMap["End"] = VK_END;
	hotkeysMap["Home"] = VK_HOME;

	hotkeysMap["Left"] = VK_LEFT;
	hotkeysMap["Up"] = VK_UP;
	hotkeysMap["Right"] = VK_RIGHT;
	hotkeysMap["Down"] = VK_DOWN;

	hotkeysMap["Insert"] = VK_INSERT;
	hotkeysMap["Delete"] = VK_DELETE;

	hotkeysMap["PrintScreen"] = VK_SNAPSHOT;
	hotkeysMap["Pause"] = VK_PAUSE;
	hotkeysMap["NumLock"] = VK_NUMLOCK;

	hotkeysMap["-"] = VK_SUBTRACT;
	hotkeysMap["="] = VK_ADD;
	hotkeysMap["/"] = VK_DIVIDE;
	hotkeysMap[","] = VK_OEM_COMMA;
	hotkeysMap["."] = VK_OEM_PERIOD;

	hotkeysMap["MMLaunch"] = VK_LAUNCH_MEDIA_SELECT;
	hotkeysMap["MMPlayPause"] = VK_MEDIA_PLAY_PAUSE;
	hotkeysMap["MMStop"] = VK_MEDIA_STOP;
	hotkeysMap["MMPrev"] = VK_MEDIA_PREV_TRACK;
	hotkeysMap["MMNext"] = VK_MEDIA_NEXT_TRACK;
	hotkeysMap["MMVolUp"] = VK_VOLUME_UP;
	hotkeysMap["MMVolDown"] = VK_VOLUME_DOWN;
	hotkeysMap["MMVolMute"] = VK_VOLUME_MUTE;
}

Hotkey::Hotkey(const std::string action_name, const std::string key_name, const std::string alt_str,
			   const std::string control_str, const std::string shift_str, const std::string win_str)
{
	action = action_name;
	key = key_name;

	key_code = GetKeyCode(key_name);
	
	alt = IsKeyUsed(alt_str);
	control = IsKeyUsed(control_str);
	shift = IsKeyUsed(shift_str);
	win = IsKeyUsed(win_str);

	/*id = key_code + (alt ? 1000 * MOD_ALT : 0) + (control ? 10000 * MOD_CONTROL : 0) + 
		(shift ? 100000 * MOD_SHIFT : 0) + (win ? 1000000 * MOD_WIN : 0);*/

	Action = GetAction(action_name);
}

Hotkey::~Hotkey()
{
	/* nothing to do */
}

void (*Hotkey::GetAction(const std::string action_name))(void)
{
	switch (actionsMap[action_name])
	{
	case eActionReloadHotkeys:
		return &ReloadHotkeys;
	case eActionOpenSettingsFile:
		return &OpenSettingsFile;
	case eActionOpenSettingsDialog:
		return &OpenSettingsDialog;
	case eActionPlayPause:
		return &PlayPause;
	case eActionPreviousTrack:
		return &PreviousTrack;
	case eActionNextTrack:
		return &NextTrack;
	case eActionRandom:
		return &Random;
	case eActionRepeat:
		return &Repeat;
	case eActionSongRatingClear:
		return &ClearSongRating;
	case eActionSongRating1:
		return &SongRating1;
	case eActionSongRating2:
		return &SongRating2;
	case eActionSongRating3:
		return &SongRating3;
	case eActionSongRating4:
		return &SongRating4;
	case eActionSongRating5:
		return &SongRating5;
	case eActionShowHide:
		return &ShowHide;
	case eActionVolumeUp:
		return &VolumeUp;
	case eActionVolumeDown:
		return &VolumeDown;
	case eActionToggleMute:
		return &ToggleMute;
	case eActionQuit:
		return &Quit;
	default:
		return 0;
	}
}

unsigned int Hotkey::GetKeyCode(const std::string key_name)
{
	return hotkeysMap[key_name];
}

bool Hotkey::IsKeyUsed(const std::string key_str)
{
	return (key_str == std::string("true"));
}

void Hotkey::PerformAction()
{
	if(Action > 0)
		Action();
}

const std::string Hotkey::GetHotkeyName()
{
	std::string hotkey_name = std::string();

	if (GetControl())
		hotkey_name.append("Ctrl + ");

	if (GetAlt())
		hotkey_name.append("Alt + ");

	if (GetShift())
		hotkey_name.append("Shift + ");

	if (GetWin())
		hotkey_name.append("Win + ");

	hotkey_name.append(key);

	return hotkey_name;
}

const std::string Hotkey::ToXmlString()
{
	std::string hotkey_tag = std::string();

	hotkey_tag.append("<hotkey ");
	hotkey_tag.append("action=\""); hotkey_tag.append(action); hotkey_tag.append("\" ");
	hotkey_tag.append("key=\""); hotkey_tag.append(key); hotkey_tag.append("\" ");
	if (GetControl())
		hotkey_tag.append("control=\"true\" ");
	if (GetAlt())
		hotkey_tag.append("alt=\"true\" ");
	if (GetShift())
		hotkey_tag.append("shift=\"true\" ");
	if (GetWin())
		hotkey_tag.append("win=\"true\" ");
	hotkey_tag.append("/>");

	return hotkey_tag;
}