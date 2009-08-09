
0. Prologue

This game has not been playable yet. That means, that you can go 
wherever you want on the map, shot at everything you see and hope 
that someone will shot you, but don't expect some thrilling contest ;).
Besides some maps can cause game to crash - this is due to bad waypoints 
calculating - to avoid this, just set number of bots to 0 (on Maps tab).

You have been warned ;)


1. Prerequisites

You'll need to have these libraries to build Cruel Hessian:

 cmake >= 2.6.0              (www.cmake.org)
 libsdl >= 1.2.13            (www.libsdl.org)
 libsdl_mixer >= 1.2.8       (www.libsdl.org/projects/SDL_mixer)
 cegui >= 0.6.0              (www.cegui.org.uk)

 and of course Soldat ( >= 1.4.2) from www.soldat.pl to run Cruel Hessian.


2. Installation

Check INSTALL.txt file


3. Controls

A - move left
D - move right
S - crouching
W - jumping
R - reload gun
SPACE - throw grenade
LMB - shooting
RMB - flying
F1 - players list with scores
F3 - statistics (FPS)
F4 - take screenshot (save to ~/.config/cruelhessian/Screens dir on UNIX)
F5 - stops/plays music
F6 - previous music track
F7 - next music track

You can change this controls (except LMB, RMB, F1-F7) in options.ini file.


4. Running

You can simply run 'cruelhessian' from the console after install, then 
you'll be able to see some info when something goes wrong.

The most important files (maps files, animation descriptions, 
in-game sounds, etc.) are loaded from existing installation of Soldat. 
So, on the first running, you'll have to select the Soldat directory 
on the 'Options' tab.

If you want to hear a music during the game, just put the m3u file(s) 
into Mp3 folder in Soldat directory. 


5. Uninstall

If you want, for some reason, to remove Cruel Hessian, just log as root
and type make uninstall in source dir.
