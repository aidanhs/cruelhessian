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

SET(CMAKE_C_COMPILER "/opt/intel/Compiler/11.1/072/bin/ia32/icc")
SET(CMAKE_CXX_COMPILER "/opt/intel/Compiler/11.1/072/bin/ia32/icpc")

SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pipe -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pipe -fomit-frame-pointer")

FIND_PACKAGE(SDL_mixer REQUIRED)
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(CEGUI 0.6 REQUIRED)
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(Boost 1.38 REQUIRED COMPONENTS filesystem)
FIND_LIBRARY(CURL_LIBRARY NAMES curl)

INCLUDE_DIRECTORIES( ${SDLMIXER_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${SDL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${CEGUI_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIR_freetype2} )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


SET(src_executable
	src/Arrow.cpp
	src/AudioManager.cpp
	src/Background.cpp
	src/Bonus.cpp
	src/BonusManager.cpp
    src/Bot.cpp
    src/BotManager.cpp
	src/Bullet.cpp
	src/Chat.cpp
	src/Collisions.cpp
	src/Console.cpp
	src/Controls.cpp
	src/FontManager.cpp
	src/decompress/UnLZMA.cpp
	src/decompress/UnTar.cpp
	src/decompress/lzmadec/7zFile.cpp
	src/decompress/lzmadec/7zStream.cpp
	src/decompress/lzmadec/Alloc.cpp
	src/decompress/lzmadec/LzFind.cpp
	src/decompress/lzmadec/LzmaDec.cpp
	src/Fonts.cpp
	src/Game.cpp
	src/Grenade.cpp
    src/GUI.cpp
    src/GUI/DiskObject.cpp
    src/GUI/GUIGraphics.cpp
	src/GUI/GUIMaps.cpp
	src/GUI/GUIMusic.cpp
	src/GUI/GUIOptions.cpp
	src/GUI/GUIPlayer.cpp
	src/GUI/GUIWindowFolderSelector.cpp
	src/GUI/GUIWindowNoSoldat.cpp
	src/GUI/GUIWindowUpdater.cpp
    src/InterfaceBaseManager.cpp
    src/InterfaceManager.cpp
	src/Load.cpp
    src/Main.cpp
    src/Map.cpp
    src/Mouse.cpp
    src/MovingObject.cpp
	src/ParserManager.cpp
	src/parser/ConvertUTF.c
	src/parser/ConvertUTF.h
	src/parser/SimpleIni.h
	src/regexp.cpp
	src/soil/SOIL.c
	src/soil/image_DXT.c
	src/soil/image_helper.c
	src/soil/stb_image_aug.c
	src/TexturesLoader.cpp
	src/TVector2D.cpp
	src/WeaponManager.cpp
	src/WindowExit.cpp
	src/WindowGuns.cpp
	src/WindowScores.cpp
	src/WorldMap.cpp
)

LINK_LIBRARIES(CEGUIBase)
LINK_LIBRARIES(CEGUIOpenGLRenderer)
LINK_LIBRARIES(GLU)
LINK_LIBRARIES(GL)
LINK_LIBRARIES(SDL_mixer)
LINK_LIBRARIES(freetype)
LINK_LIBRARIES(boost_filesystem-mt)
LINK_LIBRARIES(curl)

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


