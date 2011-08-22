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

#define NOMINMAX

/*
	includes
*/
#include <new>
#include <limits>
#include <cstring>

#include <windows.h>
#include <shlobj.h>

#include "iTunesVisualAPI.h"

#include "PluginSettings.h" 
#include "GlobalHotkeysPlugin.h"

/*
	typedef's, struct's, enum's, etc.
*/
const char kTVisualPluginName[] = "Global Hotkeys - http://ighp.berlios.de";
const OSType kTVisualPluginCreator = 'SCos';

const UInt8 kTVisualPluginMajorVersion = 1;
const UInt8 kTVisualPluginMinorVersion = 0;
const UInt8 kTVisualPluginReleaseStage = finalStage;
const UInt8 kTVisualPluginNonFinalRelease = 0;

static GlobalHotkeysPlugin* globalHotkeysPlugin = 0;
//static HINSTANCE dllHandle = 0;

struct VisualPluginData
{
	// plugin specific data, needed for iTunes APIs
	void* appCookie;
	ITAppProcPtr appProc;

	GRAPHICS_DEVICE destPort;
	Rect destRect;
};

/*
	RenderVisualPort
*/
static void RenderVisualPort(GRAPHICS_DEVICE destPort, const Rect* destRect)
{
	RECT srcRect;

	if(destPort == NULL)
		return;

	srcRect.left = destRect->left;
	srcRect.top = destRect->top;
	srcRect.right = destRect->right;
	srcRect.bottom = destRect->bottom;

	HDC hdc = GetDC(destPort);		
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &srcRect, hBrush);

	DeleteObject(hBrush);
	ReleaseDC(destPort, hdc);
}

/*
	ShowSettingsDialog
*/
void ShowSettingsDialog(HWND parent)
{
	// new dialog...
	// we should be doing synchronization here though
	GlobalHotkeysDialog& dialog = globalHotkeysPlugin->GetSettingsDialog();
	if(parent)
		dialog.SetDlgParent(FromHandle(parent));
	dialog.DoModal();
	dialog.SetDlgParent(NULL);
}

/*
	VisualPluginHandler
*/
static OSStatus VisualPluginHandler(OSType message, VisualPluginMessageInfo* messageInfo, void* refCon)
{
	VisualPluginData* visualPluginData = (VisualPluginData*)refCon;
	OSStatus status = noErr;

	switch(message)
	{
		/*
			Sent when the visual plugin is registered.  The plugin should do minimal
			memory allocations here.  The resource fork of the plugin is still available.
		*/		
		case kVisualPluginInitMessage:
		{
			try
			{
				visualPluginData = new VisualPluginData;
			}
			catch(std::bad_alloc&)
			{
				status = memFullErr;
				break;
			}

			visualPluginData->appCookie	= messageInfo->u.initMessage.appCookie;
			visualPluginData->appProc	= messageInfo->u.initMessage.appProc;
			visualPluginData->destPort = NULL;

			messageInfo->u.initMessage.refCon = (void*)visualPluginData;

			PluginSettings::Instance().SetiTunesData(visualPluginData->appCookie, visualPluginData->appProc);
			//PluginSettings::Instance().ReadConfig();
			//globalHotkeysPlugin->RegisterHotkeys(PluginSettings::Instance().GetHotkeys());
			break;
		}

		/*
			Sent when the visual plugin is unloaded
		*/		
		case kVisualPluginCleanupMessage:
			delete visualPluginData;
			//PluginSettings::Instance().WriteConfig();
			//globalHotkeysPlugin->UnregisterHotkeys();
			break;

		/*
			Sent when the visual plugin is enabled.  iTunes currently enables all
			loaded visual plugins.  The plugin should not do anything here.
		*/
		case kVisualPluginEnableMessage:
		case kVisualPluginDisableMessage:
			break;

		case kVisualPluginConfigureMessage:
			// get iTunes window as dialog parent
			ShowSettingsDialog(NULL);
			break;

		/*
			Sent when iTunes is going to show the visual plugin in a port.  At
			this point,the plugin should allocate any large buffers it needs.
		*/
		case kVisualPluginShowWindowMessage:
			visualPluginData->destPort = messageInfo->u.showWindowMessage.window;
			visualPluginData->destRect = messageInfo->u.showWindowMessage.drawRect;
			RenderVisualPort(visualPluginData->destPort, &visualPluginData->destRect);
			break;

		/*
			Sent when iTunes is no longer displayed.
		*/
		case kVisualPluginHideWindowMessage:
			visualPluginData->destPort = NULL;
			break;
		
		/*
			Sent when iTunes needs to change the port or rectangle of the currently
			displayed visual.
		*/
		case kVisualPluginSetWindowMessage:
			visualPluginData->destPort = messageInfo->u.setWindowMessage.window;
			visualPluginData->destRect = messageInfo->u.setWindowMessage.drawRect;
			RenderVisualPort(visualPluginData->destPort, &visualPluginData->destRect);
			break;
		
		/*
			Sent for the visual plugin to render a frame.
		*/
		case kVisualPluginRenderMessage:
			RenderVisualPort(visualPluginData->destPort, &visualPluginData->destRect);
			break;

		/*
			Sent in response to an update event.  The visual plugin should update
			into its remembered port.  This will only be sent if the plugin has been
			previously given a ShowWindow message.
		*/
		case kVisualPluginUpdateMessage:
			RenderVisualPort(visualPluginData->destPort, &visualPluginData->destRect);
			break;

		/*
			Sent when the player stops.
		*/
		case kVisualPluginStopMessage:
			//RenderVisualPort(visualPluginData->destPort, &visualPluginData->destRect);
			break;

		/*
			Sent when the player pauses.  iTunes does not currently use pause or unpause.
			A pause in iTunes is handled by stopping and remembering the position.
		*/
		case kVisualPluginPauseMessage:
			//RenderVisualPort(visualPluginData->destPort, &visualPluginData->destRect);
			break;

		default:
			status = unimpErr;
			break;
	}
	return status;	
}

/*
	RegisterVisualPlugin
*/
static OSStatus RegisterVisualPlugin(PluginMessageInfo* messageInfo)
{
	PlayerMessageInfo playerMessageInfo = { 0 };

	// copy in name length byte first
	playerMessageInfo.u.registerVisualPluginMessage.name[0] = strlen(kTVisualPluginName);
	// now copy in actual name
	strcpy_s((char*)&playerMessageInfo.u.registerVisualPluginMessage.name[1], 63, kTVisualPluginName);		

	SetNumVersion(&playerMessageInfo.u.registerVisualPluginMessage.pluginVersion, kTVisualPluginMajorVersion, kTVisualPluginMinorVersion, kTVisualPluginReleaseStage, kTVisualPluginNonFinalRelease);

	playerMessageInfo.u.registerVisualPluginMessage.options					= kVisualWantsConfigure;
	playerMessageInfo.u.registerVisualPluginMessage.handler					= VisualPluginHandler;
	playerMessageInfo.u.registerVisualPluginMessage.registerRefCon			= 0;
	playerMessageInfo.u.registerVisualPluginMessage.creator					= kTVisualPluginCreator;
	
	playerMessageInfo.u.registerVisualPluginMessage.timeBetweenDataInMS		= 0xFFFFFFFE; // 16 milliseconds = 1 Tick,0xFFFFFFFF = Often as possible.
	playerMessageInfo.u.registerVisualPluginMessage.numWaveformChannels		= 2;
	playerMessageInfo.u.registerVisualPluginMessage.numSpectrumChannels		= 2;
	
	playerMessageInfo.u.registerVisualPluginMessage.minWidth				= 64;
	playerMessageInfo.u.registerVisualPluginMessage.minHeight				= 64;
	playerMessageInfo.u.registerVisualPluginMessage.maxWidth				= std::numeric_limits<SInt16>::max();
	playerMessageInfo.u.registerVisualPluginMessage.maxHeight				= std::numeric_limits<SInt16>::max();
	playerMessageInfo.u.registerVisualPluginMessage.minFullScreenBitDepth	= 0;
	playerMessageInfo.u.registerVisualPluginMessage.maxFullScreenBitDepth	= 0;
	playerMessageInfo.u.registerVisualPluginMessage.windowAlignmentInBytes	= 0;

	return PlayerRegisterVisualPlugin(messageInfo->u.initMessage.appCookie, messageInfo->u.initMessage.appProc, &playerMessageInfo);
}

/*
	iTunes entrypoint
*/
extern "C" __declspec(dllexport) OSStatus iTunesPluginMain(OSType message, PluginMessageInfo* messageInfo, void* refCon)
{
	(void)refCon; //unused

	OSStatus status = noErr;

	switch(message)
	{
		case kPluginInitMessage:
			status = RegisterVisualPlugin(messageInfo);
			break;

		case kPluginCleanupMessage:
			break;

		default:
			status = unimpErr;
			break;
	}
	
	return status;
}

/*
	Window entrypoint
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			//dllHandle = hinstDLL;
			globalHotkeysPlugin = new GlobalHotkeysPlugin(hinstDLL);
			break;
		case DLL_PROCESS_DETACH:
			delete globalHotkeysPlugin;
			globalHotkeysPlugin = 0;
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}

	return true;
}
