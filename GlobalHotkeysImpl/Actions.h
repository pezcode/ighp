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

#ifndef ACTIONS_H
#define ACTIONS_H

#include <string>
#include <map>

static enum Actions
{
	eActionNotDefined,
	eActionReloadHotkeys,
	eActionOpenSettingsFile,
	eActionOpenSettingsDialog,
	eActionPlayPause,
	eActionNextTrack,
	eActionPreviousTrack,
	eActionRandom,
	eActionRepeat,
	eActionSongRatingClear,
	eActionSongRating1,
	eActionSongRating2,
	eActionSongRating3,
	eActionSongRating4,
	eActionSongRating5,
	eActionShowHide,
	eActionVolumeUp,
	eActionVolumeDown,
	eActionToggleMute,
	eActionQuit
};

extern std::map<const std::string, Actions> actionsMap;

void InitActionsMap();

void ReloadHotkeys();
void OpenSettingsFile();
void OpenSettingsDialog();

void PlayPause();
void NextTrack();
void PreviousTrack();
void Random();
void Repeat();
void ClearSongRating();
void SongRating1();
void SongRating2();
void SongRating3();
void SongRating4();
void SongRating5();
void ShowHide();
void VolumeUp();
void VolumeDown();
void ToggleMute();
void Quit();

#endif /* ACTIONS_H */