
Global Hotkeys for iTunes v0.0.3
http://ighp.berlios.de



iTunes Global Hotkeys Plugin (ighp) for Windows enables the user to trigger 
different actions using predefined shortcuts even if the player is minimized 
or hidden.


 * Supported actions


The following actions are supported:

Action	           Description
PlayPause          toggle play/pause
NextTrack          advance to the next track in playlist
PreviousTrack      go back to previous track in playlist
ToggleRandom       toggle random on/off
ToggleRepeat       toggle repeat on/off
SongRatingClear    clear rating for the selected song
SongRating1        set rating to one star
SongRating2        set rating to two stars
SongRating3        set rating to three stars
SongRating4        set rating to four stars
SongRating5        set rating to five stars
ShowHide           minimize/restore player window
VolumeUp           increase volume level
VolumeDown         decrease volume level
ToggleMute         toggle mute/unmute
OpenSettingsFile   open settings file in notepad
ReloadHotkeys      read the configuration file and reload the hotkey bindings


 * Download and Install


The latest release of the plugin can be found on 
http://developer.berlios.de/project/filelist.php?group_id=9978. You can choose 
between the installer, the binary package or the source package. The source 
code can be compiled using Visual C++ 2005 Express and above. If you downloaded
the dll files or the source code you must copy them to the iTunes plugins
directory (C:\Program Files\iTunes\Plug-ins) and then configure the hotkeys.


 * Configuration


To customize the hotkeys you must edit the configuration file
GlobalHotkeysConfig.xml. Use the default shortcut for the OpenSettingsFile
action to open the settings file Ctrl + Shift + P. This file has the
following structure:

<?xml version="1.0" encoding="UTF-8" ?>
 
<hotkeys>
  <hotkey action="PlayPause" key="Spacebar" alt="true" />
  <hotkey action="VolumeUp" key="." control="true" />
  <hotkey action="VolumeDown" key="," control="true" />
</hotkeys>

Each action is defined on a line like this:

<hotkey action="action_id" key="hotkey" alt="true/false" control="true/false"
   shift="true/false" win="true/false"/>
	
where

 - action is an action from the above table
 - key is the hothey bound to the specified action. Keys can have one of the
   following values: a-z, A-Z, 0-9, F1-F24 Spacebar, Backspace, Tab, Escape,
   PageUp, PageDown, End, Home, Left, Up, Right, Down, Insert, Delete,
   PrintScreen, Pause, NumLock, -, =, /, . and ,
 - alt, control, shift and win can take one of the values true or false. If the
   value is true, then you must use this key modifier with the hotkey to enable
   the action. If the value is set to false, it can be omitted.

After you modify the configuration file, use the default shortcut for the
ReloadHotkeys action Ctrl + Shift + R to load the new shortcuts.


 * License


iTunes Global Hotkeys Plugin is released under the MIT/X Consortium License.