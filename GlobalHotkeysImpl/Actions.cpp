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

#include "Actions.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <comutil.h>

#include "iTunesCOMInterface.h"
#include "GlobalHotkeysPlugin.h"

#include "PluginSettings.h" 
#include "Hotkeys.h"

extern HWND hWd;

std::map<const std::string, Actions> actionsMap = std::map<const std::string, Actions>();

void InitActionsMap()
{
	actionsMap["ReloadHotkeys"] = eActionReloadHotkeys;
	actionsMap["OpenSettingsFile"] = eActionOpenSettingsFile;
	actionsMap["OpenSettingsDialog"] = eActionOpenSettingsDialog;
	actionsMap["PlayPause"] = eActionPlayPause;
	actionsMap["NextTrack"] = eActionNextTrack;
	actionsMap["PreviousTrack"] = eActionPreviousTrack;
	actionsMap["ToggleRandom"] = eActionRandom;
	actionsMap["ToggleRepeat"] = eActionRepeat;
	actionsMap["SongRatingClear"] = eActionSongRatingClear;
	actionsMap["SongRating1"] = eActionSongRating1;
	actionsMap["SongRating2"] = eActionSongRating2;
	actionsMap["SongRating3"] = eActionSongRating3;
	actionsMap["SongRating4"] = eActionSongRating4;
	actionsMap["SongRating5"] = eActionSongRating5;
	actionsMap["ShowHide"] = eActionShowHide;
	actionsMap["VolumeUp"] = eActionVolumeUp;
	actionsMap["VolumeDown"] = eActionVolumeDown;
	actionsMap["ToggleMute"] = eActionToggleMute;
}

void ReloadHotkeys()
{
	std::map<const unsigned int, Hotkey*>* hotkeys = PluginSettings::Instance()->GetHotkeys();
	std::map<const unsigned int, Hotkey*>::iterator iter;

	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		UnregisterHotKey(hWd, iter->first);
		delete iter->second;
		iter->second = 0;
	}

	PluginSettings::Instance()->ReadConfigFile(PluginSettings::Instance()->GetHotkeys());

	hotkeys = PluginSettings::Instance()->GetHotkeys();
	for (iter = hotkeys->begin(); iter != hotkeys->end(); iter++) {
		unsigned int modifiers = (iter->second->GetAlt() ? MOD_ALT : 0) | 
			                     (iter->second->GetControl() ? MOD_CONTROL : 0) | 
			                     (iter->second->GetShift() ? MOD_SHIFT : 0) | 
			                     (iter->second->GetWin() ? MOD_WIN : 0);
		RegisterHotKey(hWd, iter->first, modifiers, iter->second->GetKeyCode());
	}

	MessageBox(NULL, "Hotkeys reloaded", "Ighp", MB_OK | MB_ICONINFORMATION);
}

void OpenSettingsFile()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	std::string configFilePath = std::string("");
	if (!(PluginSettings::Instance()->GetConfigFile(&configFilePath)))
		return;

	char params[MAX_PATH];
	ZeroMemory(params, sizeof(char) * MAX_PATH);

	strncpy_s(params, MAX_PATH, "notepad.exe ", _TRUNCATE);
	strcat_s(params, MAX_PATH, configFilePath.c_str());

	CreateProcess(NULL, params, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi); 
}

void OpenSettingsDialog()
{
	GetGlobalHotkeysPlugin().GetGlobalHotkeysDialog().DoModal();
}

void PlayPause()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->PlayPause();
		iITunes->Release();
	}

	CoUninitialize();
}

void NextTrack()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->NextTrack();
		iITunes->Release();
	}

	CoUninitialize();

}

void PreviousTrack()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->PreviousTrack();
		iITunes->Release();
	}

	CoUninitialize();
}

void Random()
{
	IiTunes* iITunes = 0;
	IITPlaylist* iIPlaylist = 0;
	HRESULT hRes;
	VARIANT_BOOL suffle;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_CurrentPlaylist(&iIPlaylist);
		if (iIPlaylist) {
			iIPlaylist->get_Shuffle(&suffle);
			
			if (suffle)
				iIPlaylist->put_Shuffle(0); // 0 == false
			else
				iIPlaylist->put_Shuffle(-1);// -1 == true

			iIPlaylist->Release();
		}
		
		iITunes->Release();
	}

	CoUninitialize();

}

void Repeat()
{
	IiTunes* iITunes = 0;
	IITPlaylist* iIPlaylist = 0;
	HRESULT hRes;
	ITPlaylistRepeatMode repeatMode;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_CurrentPlaylist(&iIPlaylist);
		if (iIPlaylist) {
			iIPlaylist->get_SongRepeat(&repeatMode);
			
			switch(repeatMode) {
				case ITPlaylistRepeatModeOff:
					repeatMode = ITPlaylistRepeatModeAll;
					break;
				case ITPlaylistRepeatModeAll:
					repeatMode = ITPlaylistRepeatModeOne;
					break;
				case ITPlaylistRepeatModeOne:
					repeatMode = ITPlaylistRepeatModeOff;
					break;
			}
			
			iIPlaylist->put_SongRepeat(repeatMode);
			

			iIPlaylist->Release();
		}

		iITunes->Release();
	}

	CoUninitialize();

}

void RateSong(unsigned int rating)
{
	IiTunes* iITunes = 0;
	IITTrack* iITTrack = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_CurrentTrack(&iITTrack);

		if(iITTrack) {
			iITTrack->put_Rating(rating);
			iITTrack->Release();
		}

		iITunes->Release();
	}

	CoUninitialize();
}

void ClearSongRating()
{
	RateSong(0);
}

void SongRating1()
{
	RateSong(20);
}

void SongRating2()
{
	RateSong(40);
}

void SongRating3()
{
	RateSong(60);
}

void SongRating4()
{
	RateSong(80);
}

void SongRating5()
{
	RateSong(100);
}

void ShowHide()
{
	IiTunes* iITunes = 0;
	IITBrowserWindow* iITBrowserWindow = 0; 
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->get_BrowserWindow(&iITBrowserWindow);

		if(iITBrowserWindow) {
			VARIANT_BOOL isVisible = 0;
			iITBrowserWindow->get_Minimized(&isVisible);

			isVisible = (isVisible == 0) ? -1 : 0;
			iITBrowserWindow->put_Minimized(isVisible);

			iITBrowserWindow->Release();
		}

		iITunes->Release();
	}

	CoUninitialize();
}

void ToggleVolume(long step)
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		long volume = 50;
		iITunes->get_SoundVolume(&volume);

		volume += step;
		iITunes->put_SoundVolume(volume);

		iITunes->Release();
	}

	CoUninitialize();
}

void VolumeUp()
{
	ToggleVolume(5);
}

void VolumeDown()
{
	ToggleVolume(-5);
}

void ToggleMute()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		VARIANT_BOOL isMuted = 0;
		iITunes->get_Mute(&isMuted);
		
		isMuted = (isMuted == 0) ? -1 : 0;
		iITunes->put_Mute(isMuted);

		iITunes->Release();
	}

	CoUninitialize();
}