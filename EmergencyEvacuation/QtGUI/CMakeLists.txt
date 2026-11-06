cmake_minimum_required(VERSION 3.19)
project(FireScape LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(FireScape
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    mapwidget.h 
    mapwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Building.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Location.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Room.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Hallway.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Exit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Pathfinder.h
)

# adds the root project directory to the include paths
# so the Qt application can access the shared header files.
target_include_directories(FireScape PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(FireScape
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS FireScape
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET FireScape
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
