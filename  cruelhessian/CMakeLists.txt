project (CruelHessian)

cmake_minimum_required(VERSION 2.6)
if(POLICY CMP0011)
   cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)


CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -pipe -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ansi -pedantic -O2 -pipe -fomit-frame-pointer")

FIND_PACKAGE( SDL_mixer REQUIRED)
FIND_PACKAGE( SDL REQUIRED)
FIND_PACKAGE( CEGUI 0.6 REQUIRED)
FIND_PACKAGE( Freetype REQUIRED)
FIND_PACKAGE( Boost 1.38 REQUIRED COMPONENTS filesystem)

INCLUDE_DIRECTORIES( ${SDLMIXER_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${SDL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${CEGUI_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIR_freetype2} )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


SET(src_executable
	src/arrow.cpp
	src/background.cpp
	src/bonus.cpp
    src/bot.cpp
	src/bullet.cpp
	src/chat.cpp
	src/collisions.cpp
	src/console.cpp
	src/controls.cpp
	src/disk_object.cpp
	src/fonts.cpp
	src/game.cpp
	src/globals.cpp
	src/grenade.cpp
    src/gui.cpp
    src/gui_fselector.cpp
    src/gui_graphics.cpp
    src/gui_interface.cpp
    src/gui_maps.cpp
    src/gui_music.cpp
    src/gui_player.cpp
    src/interface.cpp
	src/load.cpp
    src/main.cpp
    src/map.cpp
    src/mouse.cpp
    src/moving_object.cpp
	src/parser.cpp
	src/parser/ConvertUTF.c
	src/parser/ConvertUTF.h
	src/parser/SimpleIni.h
	src/regexp.cpp
	src/soil/SOIL.c
	src/soil/image_DXT.c
	src/soil/image_helper.c
	src/soil/stb_image_aug.c
	src/specials.cpp
	src/textures_loader.cpp
	src/tvector2d.cpp
	src/window_exit.cpp
	src/window_guns.cpp
	src/window_scores.cpp
	src/worldmap.cpp
)

LINK_LIBRARIES(CEGUIBase)
LINK_LIBRARIES(CEGUIOpenGLRenderer)
LINK_LIBRARIES(GLU)
LINK_LIBRARIES(GL)
LINK_LIBRARIES(SDL_mixer)
LINK_LIBRARIES(freetype)
LINK_LIBRARIES(boost_filesystem-mt)

ADD_EXECUTABLE(cruelhessian ${src_executable})


IF(WIN32 AND NOT UNIX)

	INSTALL(DIRECTORY data DESTINATION "data/" PATTERN ".svn" EXCLUDE)
	INSTALL(TARGETS cruelhessian DESTINATION "bin")

ELSE(WIN32 AND NOT UNIX)

	INSTALL(DIRECTORY data DESTINATION "/usr/share/cruelhessian" PATTERN ".svn" EXCLUDE)
	INSTALL(TARGETS cruelhessian DESTINATION "/usr/bin")
	INSTALL(FILES cruelhessian.desktop DESTINATION "/usr/share/applications")
	INSTALL(FILES cruelhessian.png DESTINATION "/usr/share/pixmaps")

ENDIF(WIN32 AND NOT UNIX)


