FireScape – Emergency Simulator

FireScape is a C++ emergency evacuation simulator designed to help users identify available evacuation routes, simulate emergency situations, and compare exit options within a building.

GUI Framework
- Qt 6 Widgets
- Qt 6.12.0
- C++20
- Visual Studio Community 2022
- CMake

Project File Structure: The project is organized into source files, header files, and interface files that work together to implement the FireScape GUI.
- main.cpp – Contains the application's entry point and initializes the Qt application.
- mainwindow.h / mainwindow.cpp – Define and implement the main application window, including button actions, user interactions, and communication between the GUI and the application logic.
- mainwindow.ui – Defines the main window's visual layout and interface elements using Qt Designer.
- mapwidget.h / mapwidget.cpp – Define and implement the building map, including rooms, hallways, exits, and visual updates such as evacuation routes and blocked locations.
- mainwidget.h – Contains additional widget declarations used by the GUI.
- CMakeLists.txt – Defines the project configuration, source files, dependencies, and build settings required by CMake.

GUI Features
- Find Evacuation Route
- Block Hallway
- Block Exit
- Compare Exits

Build Instructions
1. Open the FireScape GUI project in Visual Studio Community 2022.
2. Make sure the Qt 6.12.0 MSVC 2022 64-bit kit and the required Qt Widgets components are configured.
3. Allow CMake to configure the project.
4. Select the desired build configuration (Debug or Release).
5. Build the project using Build Solution.

Run Instructions
1. After a successful build, locate the FireScape executable in the project's build directory.
2. Select the executable as the startup target in Visual Studio.
3. Run the application using Start Without Debugging or Start Debugging.
4. The FireScape Emergency Simulator main window should appear.
