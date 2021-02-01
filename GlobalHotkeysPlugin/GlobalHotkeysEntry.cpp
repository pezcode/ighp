/*
 * Copyright (c) 2008 Stefan Cosma <stefan.cosma@gmail.com>
 * Copyright (c) 2021 pezcode <mail@rvrs.in>
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

/*
	includes
*/
#include <new>
#include <cstring>
#include <type_traits>

#include <windows.h>
#include <shlobj.h>

#include "iTunesVisualAPI.h"

#include "PluginSettings.h" 
#include "GlobalHotkeysPlugin.h"

/*
	typedefs, structs, enums, etc.
*/
const wchar_t kTVisualPluginName[] = L"Global Hotkeys";
const OSType kTVisualPluginCreator = 'pzcd';

const UInt8 kTVisualPluginMajorVersion = 0;
const UInt8 kTVisualPluginMinorVersion = 1;
const UInt8 kTVisualPluginReleaseStage = finalStage;
const UInt8 kTVisualPluginNonFinalRelease = 0;

static GlobalHotkeysPlugin* globalHotkeysPlugin = nullptr;

struct VisualPluginData
{
	// needed for iTunes APIs
	void* appCookie = nullptr;
	ITAppProcPtr appProc = nullptr;

	VISUAL_PLATFORM_VIEW view = NULL;
	bool initialized = false;
} visualPluginData;

static void DrawVisual(VISUAL_PLATFORM_VIEW view)
{
	if(!IsWindow(view))
		return;

	RECT rect;
	GetClientRect(view, &rect);

	HDC hdc = GetDC(view);
	FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	ReleaseDC(view, hdc);
}

static OSStatus VisualPluginHandler(OSType message, VisualPluginMessageInfo* messageInfo, void* refCon)
{
	OSStatus status = noErr;

	switch(message)
	{
		/*
			Sent when the visual plugin is registered.  The plugin should do minimal
			memory allocations here.
		*/		
		case kVisualPluginInitMessage:
			if(!visualPluginData.initialized)
			{
				visualPluginData.appCookie = messageInfo->u.initMessage.appCookie;
				visualPluginData.appProc = messageInfo->u.initMessage.appProc;
				visualPluginData.view = NULL;

				PluginSettings::Instance().SetiTunesData(visualPluginData.appCookie, visualPluginData.appProc);
				PluginSettings::Instance().ReadConfig();
				globalHotkeysPlugin->RegisterHotkeys(PluginSettings::Instance().GetHotkeys());

				visualPluginData.initialized = true;
			}
			break;

		/*
			Sent when the visual plugin is unloaded.
		*/		
		case kVisualPluginCleanupMessage:
			if(visualPluginData.initialized)
			{
				globalHotkeysPlugin->UnregisterHotkeys();
				globalHotkeysPlugin->DestroySettingsDialog();

				visualPluginData.initialized = false;
			}
			break;

		/*
			Sent when the visual plugin is enabled/disabled.  iTunes currently enables all
			loaded visual plugins at launch.  The plugin should not do anything here.
		*/
		case kVisualPluginEnableMessage:
		case kVisualPluginDisableMessage:
			break;

		/*
			Sent if the plugin requests idle messages.  Do this by setting the kVisualWantsIdleMessages
			option in the RegisterVisualMessage.options field.

			DO NOT DRAW in this routine.  It is for updating internal state only.
		*/
		case kVisualPluginIdleMessage:
			break;
		/*
			Sent if the plugin requests the ability for the user to configure it. Do this by setting
			the kVisualWantsConfigure option in the RegisterVisualMessage.options field.
		*/
		case kVisualPluginConfigureMessage:
			// this message isn't sent with 12.9+ (possibly even earlier) anymore
			// get iTunes window as dialog parent
			// FindWindow doesn't work anymore with 10.4+
			//Parent = FindWindow("iTunes", NULL);
			//HWND Parent;
			//Parent = NULL;
			//globalHotkeysPlugin->ShowSettingsDialog(Parent);
			break;

		/*
			Sent when iTunes is going to show the visual plugin. At this
			point, the plugin should allocate any large buffers it needs.
		*/
		case kVisualPluginActivateMessage:
			// note: do not draw here if you can avoid it, a draw message will be sent as soon as possible
			visualPluginData.view = messageInfo->u.activateMessage.view;
			globalHotkeysPlugin->ShowSettingsDialog(visualPluginData.view);
			break;

		/*
			Sent when iTunes is no longer displayed.
		*/
		case kVisualPluginDeactivateMessage:
			visualPluginData.view = NULL;
			globalHotkeysPlugin->DestroySettingsDialog();
			break;

		/*
			Sent when iTunes is moving the destination view to a new parent window (e.g. to/from fullscreen).
		*/
		case kVisualPluginWindowChangedMessage:
			break;

		/*
			Sent when iTunes has changed the rectangle of the currently displayed visual.
			Note: for custom NSView subviews, the subview's frame is automatically resized.
		*/
		case kVisualPluginFrameChangedMessage:
			break;

		/*
			Sent for the visual plugin to update its internal animation state.
			Plugins are allowed to draw at this time but it is more efficient if they
			wait until the kVisualPluginDrawMessage is sent OR they simply invalidate
			their own subview. The pulse message can be sent faster than the system
			will allow drawing to support spectral analysis-type plugins but drawing
			will be limited to the system refresh rate.
		*/
		case kVisualPluginPulseMessage:
			break;
		
		/*
			It's time for the plugin to draw a new frame.

			For plugins using custom subviews, you should ignore this message and just
			draw in your view's draw method. It will never be called if your subview
			is set up properly.
		*/
		case kVisualPluginDrawMessage:
			DrawVisual(visualPluginData.view);
			break;

		/*
			Sent when the player starts.
		*/
		case kVisualPluginPlayMessage:
			break;

		/*
			Sent when the player changes the current track information. This
			is used when the information about a track changes.
		*/
		case kVisualPluginChangeTrackMessage:
			break;

		/*
			Artwork for the currently playing song is being delivered per a previous request.
			Note that NULL for messageInfo->u.coverArtMessage.coverArt means the currently playing song has no artwork.
		*/
		case kVisualPluginCoverArtMessage:
			break;

		/*
			Sent when the player stops or pauses.
		*/
		case kVisualPluginStopMessage:
			break;

		/*
			Sent when the player changes the playback position.
		*/
		case kVisualPluginSetPositionMessage:
			break;

		default:
			status = unimpErr;
			break;
	}

	return status;	
}

static OSStatus RegisterVisualPlugin(PluginMessageInfo* messageInfo)
{
	PlayerMessageInfo playerMessageInfo = {};

	// copy in name length byte first
	playerMessageInfo.u.registerVisualPluginMessage.name[0] = UniChar(wcslen(kTVisualPluginName));
	// now copy in actual name
	wcscpy_s((wchar_t*)&playerMessageInfo.u.registerVisualPluginMessage.name[1], std::extent<decltype(playerMessageInfo.u.registerVisualPluginMessage.name)>::value - 1, kTVisualPluginName);

	SetNumVersion(&playerMessageInfo.u.registerVisualPluginMessage.pluginVersion, kTVisualPluginMajorVersion, kTVisualPluginMinorVersion, kTVisualPluginReleaseStage, kTVisualPluginNonFinalRelease);

	playerMessageInfo.u.registerVisualPluginMessage.options					= kVisualWantsConfigure;
	playerMessageInfo.u.registerVisualPluginMessage.handler					= VisualPluginHandler;
	playerMessageInfo.u.registerVisualPluginMessage.registerRefCon			= nullptr;
	playerMessageInfo.u.registerVisualPluginMessage.creator					= kTVisualPluginCreator;
	
	playerMessageInfo.u.registerVisualPluginMessage.pulseRateInHz			= 0;
	playerMessageInfo.u.registerVisualPluginMessage.numWaveformChannels		= 0;
	playerMessageInfo.u.registerVisualPluginMessage.numSpectrumChannels		= 0;
	
	playerMessageInfo.u.registerVisualPluginMessage.minWidth				= 0; // no restriction
	playerMessageInfo.u.registerVisualPluginMessage.minHeight				= 0; // no restriction
	playerMessageInfo.u.registerVisualPluginMessage.maxWidth				= 0; // no restriction
	playerMessageInfo.u.registerVisualPluginMessage.maxHeight				= 0; // no restriction

	return PlayerRegisterVisualPlugin(messageInfo->u.initMessage.appCookie, messageInfo->u.initMessage.appProc, &playerMessageInfo);
}

/*
	iTunes entrypoint
*/
extern "C" __declspec(dllexport) OSStatus iTunesPluginMain(OSType message, PluginMessageInfo* messageInfo, void* refCon)
{
	OSStatus status = noErr;

	switch(message)
	{
		case kPluginInitMessage:
			// Register one or more plugins here
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
	Windows entrypoint
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			globalHotkeysPlugin = new GlobalHotkeysPlugin(hinstDLL);
			break;
		case DLL_PROCESS_DETACH:
			delete globalHotkeysPlugin;
			globalHotkeysPlugin = nullptr;
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}

	return true;
}
