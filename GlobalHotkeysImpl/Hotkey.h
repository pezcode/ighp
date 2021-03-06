/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * Copyright (c) 2015 pezcode <mail@rvrs.in>
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

#ifndef HOTKEY_H
#define HOTKEY_H

#include <string>

class Hotkey
{
public:
	Hotkey() : m_keyCode(0), m_modifiers(0) { }
	explicit Hotkey(unsigned char key_code, unsigned char modifiers) : m_keyCode(key_code), m_modifiers(modifiers) { }

	unsigned char GetKeyCode() const { return m_keyCode; }
	unsigned char GetModifiers() const { return m_modifiers; }

	std::wstring toString() const;

	bool IsEmpty() const { return (m_keyCode == 0); }

private:
	unsigned char m_keyCode;
	unsigned char m_modifiers;

	static std::wstring GetKeyName(unsigned char keyCode, bool extended = false);
};

#endif /* HOTKEY_H */
