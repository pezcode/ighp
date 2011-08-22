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

#include "Action.h"

#include <windows.h>
#include <comutil.h>

#include "iTunesCOMInterface.h"

std::map<Action::Type, std::string> Action::names;

void Action::Perform() const
{
	switch(m_type)
	{
	case eActionPlayPause:
		PlayPause();
		break;
	case eActionPreviousTrack:
		PreviousTrack();
		break;
	case eActionNextTrack:
		NextTrack();
		break;
	case eActionRandom:
		Random();
		break;
	case eActionRepeat:
		Repeat();
		break;
	case eActionSongRatingClear:
		ClearSongRating();
		break;
	case eActionSongRating1:
		SongRating1();
		break;
	case eActionSongRating2:
		SongRating2();
		break;
	case eActionSongRating3:
		SongRating3();
		break;
	case eActionSongRating4:
		SongRating4();
		break;
	case eActionSongRating5:
		SongRating5();
		break;
	case eActionShowHide:
		ShowHide();
		break;
	case eActionVolumeUp:
		VolumeUp();
		break;
	case eActionVolumeDown:
		VolumeDown();
		break;
	case eActionToggleMute:
		ToggleMute();
		break;
	case eActionQuit:
		Quit();
		break;
	default:
		break;
	}
}

void Action::InitNames()
{
	if(names.size() != 0)
		return;

	names[eActionPlayPause] = "PlayPause";
	names[eActionNextTrack] = "NextTrack";
	names[eActionPreviousTrack] = "PreviousTrack";
	names[eActionRandom] = "ToggleRandom";
	names[eActionRepeat] = "ToggleRepeat";
	names[eActionSongRatingClear] = "SongRatingClear";
	names[eActionSongRating1] = "SongRating1";
	names[eActionSongRating2] = "SongRating2";
	names[eActionSongRating3] = "SongRating3";
	names[eActionSongRating4] = "SongRating4";
	names[eActionSongRating5] = "SongRating5";
	names[eActionShowHide] = "ShowHide";
	names[eActionVolumeUp] = "VolumeUp";
	names[eActionVolumeDown] = "VolumeDown";
	names[eActionToggleMute] = "ToggleMute";
	names[eActionQuit] = "Quit";
}

/*
void Action::ReloadHotkeys()
{
	const std::vector<Hotkey>& hotkeys = PluginSettings::Instance()->GetHotkeys();

	for (int i = 0: i < hotkeys.size(); i++) {
		UnregisterHotKey(hWd, i);
	}

	PluginSettings::Instance()->ReadConfig();

	hotkeys = PluginSettings::Instance()->GetHotkeys();

	for (int i = 0: i < hotkeys.size(); i++) {
		RegisterHotKey(hWd, i, hotkeys[i].GetModifiers(), hotkeys[i].GetKeyCode());
	}

	//MessageBox(NULL, "Hotkeys reloaded", "Ighp", MB_OK | MB_ICONINFORMATION);
}
*/

void Action::PlayPause()
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

void Action::NextTrack()
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

void Action::PreviousTrack()
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

void Action::Random()
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

void Action::Repeat()
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

void Action::RateSong(unsigned int rating)
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

void Action::ClearSongRating()
{
	RateSong(0);
}

void Action::SongRating1()
{
	RateSong(20);
}

void Action::SongRating2()
{
	RateSong(40);
}

void Action::SongRating3()
{
	RateSong(60);
}

void Action::SongRating4()
{
	RateSong(80);
}

void Action::SongRating5()
{
	RateSong(100);
}

void Action::ShowHide()
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

void Action::ToggleVolume(long step)
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

void Action::VolumeUp()
{
	ToggleVolume(5);
}

void Action::VolumeDown()
{
	ToggleVolume(-5);
}

void Action::ToggleMute()
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

void Action::Quit()
{
	IiTunes* iITunes = 0;
	HRESULT hRes;

	CoInitialize(0);

	// Create itunes interface
    hRes = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&iITunes);

	if(hRes == S_OK && iITunes) {
		iITunes->Quit();
		iITunes->Release();
	}

	CoUninitialize();
}
