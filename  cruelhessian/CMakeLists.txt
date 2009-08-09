project (CruelHessian)

cmake_minimum_required(VERSION 2.6)
if(POLICY CMP0011)
   cmake_policy(SET CMP0011 NEW) # or even better, NEW
endif(POLICY CMP0011)


CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")


SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fomit-frame-pointer -Wall")

FIND_PACKAGE( SDL_mixer REQUIRED)
FIND_PACKAGE( SDL REQUIRED)
FIND_PACKAGE( CEGUI REQUIRED)
FIND_PACKAGE( Freetype REQUIRED)

INCLUDE_DIRECTORIES( ${SDLMIXER_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${SDL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${CEGUI_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIR_freetype2} )


SET(src_executable
    src/bot.cpp
	src/bullet.cpp
	src/controls.cpp
	src/draw.cpp
	src/draw_menus.cpp
	src/fonts.cpp
	src/globals.cpp
	src/grenade.cpp
    src/gui.cpp
	src/load.cpp
    src/main.cpp
    src/moving_object.cpp
	src/parser.cpp
	src/parser/ConvertUTF.c
	src/parser/ConvertUTF.h
	src/parser/SimpleIni.h
	src/physics.cpp
    src/read_pms.cpp
	src/soil/SOIL.c
	src/soil/image_DXT.c
	src/soil/image_helper.c
	src/soil/stb_image_aug.c
	src/specials.cpp
	src/worldmap.cpp
)

LINK_LIBRARIES(CEGUIBase)
LINK_LIBRARIES(CEGUIOpenGLRenderer)
LINK_LIBRARIES(GLU)
LINK_LIBRARIES(GL)
LINK_LIBRARIES(SDL_mixer)
LINK_LIBRARIES(freetype)

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


