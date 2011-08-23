/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * Copyright (c) 2011 pezcode <mail@rvrs.in>
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

#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <map>

class Action
{
public:
	enum Type
	{
		eActionNotDefined,
		eActionPlayPause,
		eActionStop,
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

	Action(Type type = eActionNotDefined) : m_type(type) { }

	void Perform() const;

	static void InitNames();

public:
	static std::map<Type, std::string> names;

private:
	Type m_type;

	// Helpers
	static void RateSong(unsigned int rating);
	static void ToggleVolume(long step);

	// Actions
	static void PlayPause();
	static void Stop();
	static void NextTrack();
	static void PreviousTrack();
	static void Random();
	static void Repeat();
	static void ClearSongRating();
	static void SongRating1();
	static void SongRating2();
	static void SongRating3();
	static void SongRating4();
	static void SongRating5();
	static void ShowHide();
	static void VolumeUp();
	static void VolumeDown();
	static void ToggleMute();
	static void Quit();
};

#endif /* ACTION_H */
