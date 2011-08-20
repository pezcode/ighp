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

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shlobj.h>

#include <tinyxml\tinyxml.h>

PluginSettings* PluginSettings::ms_instance = 0;

PluginSettings* PluginSettings::Instance()
{
	if (ms_instance == 0)
		ms_instance = new PluginSettings();
	return ms_instance;
}

void PluginSettings::Destroy()
{
	if (ms_instance != 0) {
		delete ms_instance;
		ms_instance = 0;
	}
}

PluginSettings::PluginSettings() : 
	m_configFile(std::string("GlobalHotkeysConfig.xml")), 
	m_keyId(-1)
{
	m_hotkeys = new std::map<const unsigned int, Hotkey*>();

	::InitActionsMap();
	::InitHotkeysMap();
}

PluginSettings::~PluginSettings()
{
	delete m_hotkeys;
}

std::map<const unsigned int, Hotkey*>* PluginSettings::GetHotkeys()
{
	return m_hotkeys;
}

bool PluginSettings::ReadConfigFile(std::map<const unsigned int, Hotkey*>* hotkeys)
{
	std::string configFilePath = std::string("");
	if (!GetConfigFile(&configFilePath))
		return false;

	TiXmlDocument configFile(configFilePath.c_str());
	if (configFile.LoadFile()) {
		m_keyId = -1;
		hotkeys->clear();

		const TiXmlElement* root = configFile.RootElement();

		const TiXmlElement* element = root->FirstChildElement();
		while (element) {
			std::string action_name = std::string(element->Attribute("action") != 0 ? 
				element->Attribute("action") : "NotDefined");
			std::string key_name = std::string(element->Attribute("key") != 0 ? 
				element->Attribute("key") : "NotDefined");
			std::string alt_str = std::string(element->Attribute("alt") != 0 ? 
				element->Attribute("alt") : "NotDefined");
			std::string control_str = std::string(element->Attribute("control") != 0 ? 
				element->Attribute("control") : "NotDefined");
			std::string shift_str = std::string(element->Attribute("shift") != 0 ? 
				element->Attribute("shift") : "NotDefined");
			std::string win_str = std::string(element->Attribute("win") != 0 ? 
				element->Attribute("win") : "NotDefined");

			(*hotkeys)[++m_keyId] = new Hotkey(action_name, key_name, alt_str, control_str, shift_str, win_str);

			element = element->NextSiblingElement();
		}

		AddDefaultHotkeys();
	} else {
		AddDefaultHotkeys();
		WriteConfigFile(hotkeys);
	}

	return true;
}

void PluginSettings::AddDefaultHotkeys()
{
	std::map<const unsigned int, Hotkey*>* hotkeys = this->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	bool addOpenSettingsFileHotkey = true;
	bool addReloadHotkeys = true;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		if (iter->second->GetActionName() == "OpenSettingsFile") {
			addOpenSettingsFileHotkey = false;
			continue;
		}
		if (iter->second->GetActionName() == "ReloadHotkeys") {
			addReloadHotkeys = false;
			continue;
		}
	}

	// Settings Dialog
	if (addOpenSettingsFileHotkey)
		(*hotkeys)[++m_keyId] = new Hotkey("OpenSettingsFile", "P", "false", "true", "true", "false");

	// Reload hotkeys
	if (addReloadHotkeys)
		(*hotkeys)[++m_keyId] = new Hotkey("ReloadHotkeys", "R", "false", "true", "true", "false");
}

bool PluginSettings::WriteConfigFile(std::map<const unsigned int, Hotkey*>* hotkeys)
{
	std::string configFilePathDir = std::string("");
	GetConfigFileDir(&configFilePathDir);
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(configFilePathDir.c_str())) {
		if (!CreateDirectory(configFilePathDir.c_str(), NULL))
			return false;
	}

	std::string configFilePath = std::string("");
	if (!GetConfigFile(&configFilePath))
		return false;

	std::string config_file = std::string();
	config_file.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n\r\n");
	
	config_file.append("\r\n<!--\r\n\r\n");
	config_file.append("  Here is an example of a hotkey:\r\n\r\n");
	config_file.append("  <hotkey action=\"PlayPause\" key=\"Spacebar\" alt=\"true\" control=\"true\" shift=\"true\" win=\"true\"/>\r\n\r\n");
	config_file.append("  Action can have one of the following values: PlayPause, NextTrack,\r\n");
	config_file.append("  PreviousTrack, ToggleRandom, ToggleRepeat, SongRatingClear, SongRating1,\r\n");
	config_file.append("  SongRating2, SongRating3, SongRating4, SongRating5, ShowHide, VolumeUp,\r\n");
	config_file.append("  VolumeDown, ToggleMute, OpenSettingsFile, ReloadHotkeys.\r\n\r\n");
	config_file.append("  Key can have one of the following values: a-z, A-Z, 0-9, F1-F24,\r\n");
	config_file.append("  Spacebar, Backspace, Tab, Escape, PageUp, PageDown, End, Home, Left, Up,\r\n");
	config_file.append("  Right, Down, Insert, Delete, PrintScreen, Pause, NumLock, -, =, /, . and ,\r\n\r\n");
	config_file.append("\r\n-->\r\n\r\n");

	config_file.append("<hotkeys>\r\n");

	std::map<const unsigned int, Hotkey*>::iterator iter;
	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		config_file.append("  ");
		config_file.append(iter->second->ToXmlString());
		config_file.append("\r\n");
	}

	config_file.append("\r\n  <!--\r\n");
	config_file.append("  <hotkey action=\"PlayPause\" key=\"Spacebar\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"NextTrack\" key=\".\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"PreviousTrack\" key=\",\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"ShowHide\" key=\"q\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"VolumeUp\" key=\"a\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"VolumeDown\" key=\"z\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"ToggleRandom\" key=\"s\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"ToggleRepeat\" key=\"d\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"SongRatingClear\" key=\"0\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"SongRating1\" key=\"1\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"SongRating2\" key=\"2\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"SongRating3\" key=\"3\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"SongRating4\" key=\"4\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"SongRating5\" key=\"5\" alt=\"true\"/>\r\n");
	config_file.append("  <hotkey action=\"ToggleMute\" key=\"m\" alt=\"true\"/>\r\n");
	config_file.append("  -->\r\n\r\n");

	config_file.append("</hotkeys>\r\n");

	HANDLE hFile;
	DWORD wmWritten;

	hFile = CreateFile(configFilePath.c_str() ,GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
	WriteFile(hFile, config_file.c_str(), (DWORD)(strlen(config_file.c_str())), &wmWritten, NULL);
	CloseHandle(hFile);

	return true;	
}

bool PluginSettings::GetConfigFileDir(std::string* str)
{
	OSVERSIONINFO osvi;
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	char path[MAX_PATH];
	ZeroMemory(path, sizeof(char) * MAX_PATH);

	if (osvi.dwMajorVersion > 5) {
		// Vista and above
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		str->append(path);
		str->append("\\Apple Computer");
	} else {
		// XP and 2000
		SHGetFolderPath(NULL, CSIDL_MYMUSIC, NULL, 0, path);
		str->append(path);
	}

	str->append("\\iTunes\\iTunes Plug-ins");

	return true;
}

bool PluginSettings::GetConfigFile(std::string* str)
{
	if (GetConfigFileDir(str)) {
		str->append("\\");
		str->append(m_configFile);
		return true;
	}

	return false;
}