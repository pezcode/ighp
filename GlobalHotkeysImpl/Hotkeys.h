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

#ifndef HOTKEYS_H
#define HOTKEYS_H

#include <string>
#include <map>

extern std::map<const std::string, unsigned int> hotkeysMap;

void InitHotkeysMap();

class Hotkey
{
public:
	explicit Hotkey() : Action(0), key(0), alt(false), control(false), shift(false), win(false) { };
	explicit Hotkey(const std::string action_name, const std::string key_name, const std::string alt_str, 
		const std::string control_str, const std::string shift_str, const std::string win_str);
	~Hotkey();

	inline unsigned int GetKeyCode() { return key_code; };

	inline bool GetAlt() { return alt; };
	inline bool GetControl() { return control; };
	inline bool GetShift() { return shift; };
	inline bool GetWin() { return win; };

	inline const std::string GetActionName() { return action; };
	const std::string GetHotkeyName();

	void PerformAction();	

	const std::string ToXmlString();

private:
	// int id;
	unsigned int key_code;
	bool alt;
	bool control;
	bool shift;
	bool win;

	std::string action;
	std::string key;

	void (*Action)(void);

	void (*GetAction(const std::string action_name))(void);
	unsigned int GetKeyCode(const std::string key_name);
	bool IsKeyUsed(const std::string key_str);
};

#endif /* HOTKEYS_H */