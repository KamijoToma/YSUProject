cmake_minimum_required(VERSION 3.21.1)

set(BUILD_QDS_COMPONENTS ON CACHE BOOL "Build design studio components")

project(YunaPlayerApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QT NAMES Qt6 COMPONENTS Gui Qml Quick Multimedia Sql)
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Multimedia Sql)
qt_policy(SET QTP0001 NEW)


qt_add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/MediaPlayer.cpp src/MediaPlayer.h
        src/bpbtable/bpptablecolumn.cpp
        src/bpbtable/bpptablecolumn.h
        src/bpbtable/bpptablecolumnlist.cpp
        src/bpbtable/bpptablecolumnlist.h
        src/bpbtable/bpptabledatabase.cpp
        src/bpbtable/bpptabledatabase.h
        src/bpbtable/bpptableheading.cpp
        src/bpbtable/bpptableheading.h
        src/bpbtable/bpptablemodel.cpp
        src/bpbtable/bpptablemodel.h
        src/SList.h src/Manage.cpp src/Manage.h src/Manage.h src/Manage.cpp)


# qt_standard_project_setup() requires Qt 6.3 or higher. See https://doc.qt.io/qt-6/qt-standard-project-setup.html for details.
qt6_standard_project_setup()


qt_add_resources(${CMAKE_PROJECT_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Multimedia
        Qt${QT_VERSION_MAJOR}::Sql
)

if (${BUILD_QDS_COMPONENTS})
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif ()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)
