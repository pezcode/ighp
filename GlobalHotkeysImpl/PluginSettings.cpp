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

#include "PluginSettings.h" 
#include "GlobalHotkeysPlugin.h" 

#include <windows.h>

PluginSettings PluginSettings::ms_instance;

PluginSettings::PluginSettings()
{
	Action::InitNames();
}

void PluginSettings::SetiTunesData(void* appCookie, ITAppProcPtr appProc)
{
	this->appCookie = appCookie;
	this->appProc = appProc;
}

bool PluginSettings::ReadConfig()
{
	m_hotkeys.clear();

	int version = 0;
	UInt32 size = sizeof(version);
	if(noErr == PlayerGetPluginNamedData(appCookie, appProc, (ConstStringPtr)"Version", &version, size, &size))
	{
		if(version > GlobalHotkeysPlugin::version)
		{
			return false; // a newer version was running before
		}
		// Check for breaking versions here...
	}

	// Loop all existing actions
	std::map<Action::Type, std::string>::const_iterator iter;
	for(iter = Action::names.begin(); iter != Action::names.end(); iter++)
	{
		Action::Type action_code = iter->first;
		ConstStringPtr action_name = (ConstStringPtr)iter->second.c_str();
		WORD keys = 0;
		UInt32 size = sizeof(keys);
		OSStatus res = PlayerGetPluginNamedData(appCookie, appProc, action_name, &keys, size, &size);
		if(res == noErr && size == sizeof(keys))
		{
			// Set hotkey for that action
			m_hotkeys[action_code] = Hotkey(LOBYTE(keys), HIBYTE(keys));
		}
		else
		{
			m_hotkeys[action_code] = Hotkey();
		}
	}
	return true;
}

bool PluginSettings::WriteConfig()
{
	PlayerSetPluginNamedData(appCookie, appProc, (ConstStringPtr)"Version", (void*)&GlobalHotkeysPlugin::version, sizeof(GlobalHotkeysPlugin::version));

	// Loop all existing actions
	std::map<Action::Type, std::string>::const_iterator iter;
	for(iter = Action::names.begin(); iter != Action::names.end(); iter++)
	{
		Action::Type action_code = iter->first;
		ConstStringPtr action_name = (ConstStringPtr)iter->second.c_str();
		// Get hotkey for that action
		Hotkey hotkey = m_hotkeys[action_code]; // creates an empty hotkey if none exists
		UInt16 keys = MAKEWORD(hotkey.GetKeyCode(), hotkey.GetModifiers());
		PlayerSetPluginNamedData(appCookie, appProc, action_name, &keys, sizeof(keys));
	}
	return true;	
}
