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

GlobalHotkeysPlugin* globalHotkeysPlugin = 0;
HANDLE dllHandle = 0;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	dllHandle = hModule;

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return true;
}

extern "C" void WINAPI Initialize()
{
	// Write here all the code you need to initialize the DLL
}

extern "C" void WINAPI Release()
{
	// Write here all the code you need to free everything ...
}

extern "C" void WINAPI InitGlobalHotkeysPlugin()
{
	PluginSettings::Instance()->ReadConfigFile(PluginSettings::Instance()->GetHotkeys());

	globalHotkeysPlugin = new GlobalHotkeysPlugin();
}

extern "C" void WINAPI ReleaseGlobalHotkeysPlugin()
{
	delete globalHotkeysPlugin;
	globalHotkeysPlugin = 0;

	PluginSettings::Destroy();
}

extern "C" void WINAPI ShowSettingsDialog(HWND parent)
{
	GlobalHotkeysDialog& dialog = GetGlobalHotkeysPlugin().GetGlobalHotkeysDialog();
	if(parent)
		dialog.SetDlgParent(FromHandle(parent));
	dialog.DoModal();
	dialog.SetDlgParent(NULL);
}
