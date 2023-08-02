cmake_minimum_required(VERSION 3.5)

project(CXXTP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# Google test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(
        SListTest
        test/SListTest.cpp
        test/SSetTest.cpp)
target_link_libraries(
        SListTest
        GTest::gtest_main
)
add_executable(
        SStackTest
        test/SStackTest.cpp

        SStack.h
)
target_link_libraries(
        SStackTest
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(SListTest)
gtest_discover_tests(SStackTest)

set(TS_FILES CXXTP_zh_CN.ts)

set(PROJECT_SOURCES
        mainwindow.ui
        main.cpp
        ${TS_FILES}
        MainUI.h MainUI.cpp loginwindow.cpp loginwindow.h loginwindow.ui studentwindow.cpp studentwindow.h studentwindow.ui classcard.cpp classcard.h classcard.ui SList.h SStack.h SSet.h SSetting.cpp SSetting.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CXXTP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CXXTP APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(CXXTP SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CXXTP
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(CXXTP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(CXXTP PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CXXTP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CXXTP)
endif()
