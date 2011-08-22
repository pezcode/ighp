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

#ifndef PLUGIN_SETTINGS_H
#define PLUGIN_SETTINGS_H

#include "iTunesAPI.h"

#include "Hotkey.h"
#include "Action.h"

#include <map>

class PluginSettings
{
public:
	static PluginSettings& Instance() { return ms_instance; }

	const std::map<Action::Type, Hotkey>& GetHotkeys() const { return m_hotkeys; }
	void SetHotkeys(const std::map<Action::Type, Hotkey>& hotkeys) { m_hotkeys = hotkeys; }

	void SetiTunesData(void* appCookie, ITAppProcPtr appProc);

	bool ReadConfig();
	bool WriteConfig();

private:
	PluginSettings();

	static PluginSettings ms_instance;

	//iTunes data, needed for API calls
	void* appCookie;
	ITAppProcPtr appProc;

	// List of actions and their associated hotkeys
	std::map<Action::Type, Hotkey> m_hotkeys;
};

#endif /* PLUGIN_SETTINGS_H */
