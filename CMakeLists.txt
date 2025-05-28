cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(DataRenderingApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES DataRenderingApp_en_150.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DataRenderingApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        widget.h widget.cpp
        datareader.h datareader.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DataRenderingApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(DataRenderingApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(DataRenderingApp
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(DataRenderingApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DataRenderingApp)
endif()
set_target_properties(DataRenderingApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS DataRenderingApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DataRenderingApp)
endif()



# Google Test Integration

# Use FetchContent to automatically download GoogleTest from GitHub
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git  # Official repo
  GIT_TAG release-1.12.1                                   # Specific stable release
)

# On Windows, forces use of shared C runtime to avoid CRT conflicts with Qt
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Actually fetch and make GoogleTest available to the build
FetchContent_MakeAvailable(googletest)

# Enable CMake's testing feature (allows use of `ctest`, etc.)
enable_testing()

# Define the test executable and include all test sources + dependent modules
add_executable(MyAppTests
    tests/test_datareader.cpp   # Tests for the datareader module
    datareader.cpp              # Code under test
    tests/test_widger.cpp       # Tests for the Widget class
    widget.cpp                  # Code under test
)

# Include the main project’s headers in the test project
target_include_directories(MyAppTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}     # So we can #include "widget.h", etc.
)

# Find and link against QtCore (for QString) and QtWidgets (for QWidget-based code)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(MyAppTests
    PRIVATE
        gtest_main              # Google Test main() implementation
        Qt6::Core               # Needed for QString, QFile, etc.
        Qt6::Widgets            # Required for testing QWidget-based code
)

# On Windows, copy required Qt DLLs to test output folder so it can launch successfully
if (WIN32)
    add_custom_command(TARGET MyAppTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${Qt6_DIR}/../../../bin/Qt6Core.dll"
            "${Qt6_DIR}/../../../bin/Qt6Gui.dll"
            "${Qt6_DIR}/../../../bin/Qt6Widgets.dll"
            $<TARGET_FILE_DIR:MyAppTests>
    )
endif()

# Automatically register all test cases defined with TEST()
include(GoogleTest)
gtest_discover_tests(MyAppTests)

# Optional: allow running tests with `ctest` via `add_test()`
add_test(NAME AllTests COMMAND MyAppTests)
