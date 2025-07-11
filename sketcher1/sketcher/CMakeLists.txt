cmake_minimum_required(VERSION 3.16)
project(ProjectQT LANGUAGES CXX)

#  Set the correct Qt path for CMake to find Qt6 modules
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64/lib/cmake") # Update this path if incorrect

# Enforce C++17 for Qt6 compatibility
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt's automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

qt_standard_project_setup()

# Add your executable and all required source files
qt_add_executable(ProjectQT
    WIN32 MACOSX_BUNDLE

    # Entry point
    main.cpp

    # Main window
    mainwindow.cpp
    mainwindow.h

    # OpenGL rendering widget
    sketcherwidget.cpp
    sketcherwidget.h

    # Shape interface and implementations
    sketcher.cpp
    sketcher.h
    sketcher_primitives.h
)
include_directories(
    ${PROJECT_SOURCE_DIR}/geometry
    ${PROJECT_SOURCE_DIR}/geometry/src
    ${PROJECT_SOURCE_DIR}/geometry/include
)
# Link required libraries
target_link_libraries(ProjectQT
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        opengl32
)

#  Optional install step
include(GNUInstallDirs)

install(TARGETS ProjectQT
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

  #Optional deployment script
qt_generate_deploy_app_script(
    TARGET ProjectQT
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})