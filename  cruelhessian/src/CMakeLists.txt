
SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

ADD_DEFINITIONS( "-Wall -ansi -pedantic -O2" )

FIND_PACKAGE( SDL_image 1.2.6 REQUIRED)
FIND_PACKAGE( SDL_mixer 1.2.8 REQUIRED)
FIND_PACKAGE( SDL_ttf 2.0.9 REQUIRED)
FIND_PACKAGE( SDL 1.2.13 REQUIRED)
FIND_PACKAGE( CEGUI 0.6.0 REQUIRED)
FIND_PACKAGE( Boost 1.33 REQUIRED COMPONENTS filesystems regex)

INCLUDE_DIRECTORIES( ${SDLIMAGE_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${SDLMIXER_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${SDLTTF_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${SDL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${CEGUI_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

SET(src_executable_SRCS
    worldmap.cpp
    main.cpp
    draw.cpp
    physics.cpp
    moving_objects.cpp
    static_objects.cpp
    gui.cpp
    gun.cpp
    read_pms.cpp
    init.cpp
    grenade.cpp
    controls.cpp
    bot.cpp
    globals.cpp
    draw_extra.cpp
    bullet.cpp
)

LINK_LIBRARIES(CEGUIBase)
LINK_LIBRARIES(CEGUIOpenGLRenderer)
LINK_LIBRARIES(boost_filesystem)
LINK_LIBRARIES(boost_regex)
LINK_LIBRARIES(GLU)
LINK_LIBRARIES(GL)
LINK_LIBRARIES(SDL_ttf)
LINK_LIBRARIES(SDL_mixer)
LINK_LIBRARIES(SDL_image)

ADD_EXECUTABLE(CruelHessian ${src_executable_SRCS})

INSTALL(TARGETS CruelHessian DESTINATION bin)
