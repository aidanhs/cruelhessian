
------------------------------------------------------------------------
----------------------        0. Prologue        -----------------------
------------------------------------------------------------------------

This game has not been playable yet. That means, that you can go 
wherever you want on the map, shot at everything you see and hope 
that someone will shot you, but don't expect some thrilling contest.

You have been warned ;)

Project's site : http://cruelhessian.konp.eu

From 0.1.2 version, Cruel Hessian has started not to use the current
installation of Soldat. So now you can choose one of two modes: 
Soldat compatibility mode (still using Soldat files) or 
Cruel Hessian mode (using files based on mods created by Soldat users - 
these ones can be automatically downloaded from temporary server by 
clicking 'Check for update' button in Options tab in CH). 
At this moment almost all originally Soldat files have been replaced,
except Anims directory.

I'm not sure about license of these mods, so I don't provide them 
with this sources.



------------------------------------------------------------------------
--------------------        1. Prerequisites        --------------------
------------------------------------------------------------------------



You'll need to have these libraries to build Cruel Hessian:

 cmake >= 2.6.0              (www.cmake.org)
 cegui = 0.6.0 - 0.6.2       (www.cegui.org.uk)
 libsfml = 1.6               (www.sfml-dev.org)
 boost_filesystem >= 1.38.0
 boost_system >= 1.38.0

    and Soldat ( >= 1.4.2) from http://www.soldat.pl if you want to run 
    Soldat compatibility mode in Cruel Hessian. 

If you are using Ubuntu, you can type:

sudo apt-get install libcegui-mk2-dev libsfml-dev cmake 
  libboost-filesystem-dev libboost-system-dev g++
  
On ArchLinux type:

yaourt -S cmake sfml boost cegui-0.6



------------------------------------------------------------------------
--------------------        2. Installation        ---------------------
------------------------------------------------------------------------

2.1. Compilation under Linux

Config file will be placed in ~/.config/cruelhessian/ directory.


Building Cruel Hessian with CMake
------------------------

Type :

   * cmake .
   * make
   
   	and (as root)
   	
   * make install

in the main directory.


------------------------------------------------------------------------
------------------        3. Default controls        -------------------
------------------------------------------------------------------------



A - move left
D - move right
S - crouching
W - jumping
R - reload gun
/ - enter command
T - chat
SPACE - throw grenade
LMB - shooting
RMB - flying
F1 - players list with scores
F3 - statistics (FPS)
F4 - take screenshot (save to ~/.config/cruelhessian/Screens/ dir on UNIX)
F5 - stops/plays music
F6 - previous music track
F7 - next music track


------------------------------------------------------------------------
-----------------------        4. Running        -----------------------
------------------------------------------------------------------------


You can simply run 'cruelhessian' from the console after install, then 
you'll be able to see some info when something goes wrong.

The most important files (maps files, animation descriptions, in-game 
sounds, etc.) are loading from existing installation of Soldat or from 
downloaded files. So, on the first running, you'll have to select the 
Soldat directory (and/or CH directory) on the 'Options' tab.

If you want to hear a music during the game, just put the m3u file(s) 
into Mp3 folder in Soldat/CH directory. 


------------------------------------------------------------------------
----------------------        5. Uninstall        ----------------------
------------------------------------------------------------------------


If you want, for some reason, to remove Cruel Hessian, just log as root
and type make uninstall in source dir.
