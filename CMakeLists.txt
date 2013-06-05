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

#SET(CMAKE_C_COMPILER "llvm-gcc")
#SET(CMAKE_CXX_COMPILER "llvm-g++")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -pipe -fomit-frame-pointer")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -pipe -fomit-frame-pointer")

FIND_PACKAGE(SFML REQUIRED)
FIND_PACKAGE(CEGUI 0.6 REQUIRED)
FIND_PACKAGE(Boost 1.38 REQUIRED COMPONENTS filesystem system)

INCLUDE_DIRECTORIES( ${SFML_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${CEGUI_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )



SET(src_executable
	src/Arrow.cpp
	src/AudioManager.cpp
	src/Background.cpp
	src/Body.cpp
	src/Bonus.cpp
	src/BonusManager.cpp
    src/Bot.cpp
    src/BotManager.cpp
	src/Bullet.cpp
	src/Cluster.cpp
	src/ClusterGrenade.cpp
	src/ContactListener.cpp
	src/Controls.cpp
	src/FontManager.cpp
	src/decompress/UnLZMA.cpp
	src/decompress/UnTar.cpp
	src/decompress/lzmadec/7zFile.c
	src/decompress/lzmadec/7zStream.c
	src/decompress/lzmadec/Alloc.c
	src/decompress/lzmadec/LzFind.c
	src/decompress/lzmadec/LzmaDec.c
	src/Game.cpp
	src/Grenade.cpp
    src/GUI/DiskObject.cpp
    src/GUI/GUI.cpp
    src/GUI/GUIGraphics.cpp
	src/GUI/GUIMaps.cpp
	src/GUI/GUIMusic.cpp
	src/GUI/GUIOptions.cpp
	src/GUI/GUIPlayer.cpp
	src/GUI/GUIWindowFolderSelector.cpp
	src/GUI/GUIWindowNoSoldat.cpp
	src/GUI/GUIWindowUpdater.cpp
    src/InterfaceBaseManager.cpp
    src/InterfaceElement.cpp
    src/InterfaceManager.cpp
    src/Main.cpp
    src/Map.cpp
    src/MapElementPolygon.cpp
    src/MapElementProp.cpp
    src/Mouse.cpp
    src/parser/ConvertUTF.c
	src/ParserManager.cpp
	src/PhysicsManager.cpp
	src/physics/Contact.cpp
	src/physics/Polygon.cpp
	src/Regex.cpp
	src/RenderManager.cpp
	src/Shell.cpp
	src/SoundBufferManager.cpp
	src/SpatialManager.cpp
	src/soil/SOIL.c
	src/soil/image_DXT.c
	src/textinout/ChatLine.cpp
	src/textinout/CommandLine.cpp
	src/textinout/Line.cpp
	src/textinout/TextOutput.cpp
	src/TexturesLoader.cpp
	src/TMatrix2D.cpp
	src/TVector2D.cpp
	src/Weapon.cpp
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
LINK_LIBRARIES(sfml-graphics)
LINK_LIBRARIES(sfml-window)
LINK_LIBRARIES(sfml-system)
LINK_LIBRARIES(sfml-audio)
LINK_LIBRARIES(sfml-network)
LINK_LIBRARIES(boost_filesystem-mt)
LINK_LIBRARIES(boost_system-mt)
LINK_LIBRARIES(pcre)

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


