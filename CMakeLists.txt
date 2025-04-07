cmake_minimum_required(VERSION 3.16)

project(shms VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(PROJECT_SOURCES
    include/screens/dialog/AppointmentDialog.h
    include/screens/dialog/EmergencyAppointmentDialog.h
    include/screens/dialog/PatientDialog.h
    include/screens/dialog/RoomManagementDialog.h
    include/screens/dialog/MedicalHistoryDialog.h
    include/screens/AppointmentScreen.h
    include/screens/EmergencyAppointmentScreen.h
    include/screens/MedicalHistoryScreen.h
    include/screens/PatientScreen.h
    include/screens/RoomManagementScreen.h
    include/screens/SuggestDoctorScreen.h
    include/source/AppointmentSystem.h
    include/source/EmergencySystem.h
    include/source/PatientManagementSystem.h
    include/source/RoomManagementSystem.h
    include/source/Source.h
    include/Mainwindow.h
    include/SList.h
    include/AVLTree.h
    src/screens/dialog/AppointmentDialog.cpp
    src/screens/dialog/EmergencyAppointmentDialog.cpp
    src/screens/dialog/PatientDialog.cpp
    src/screens/dialog/RoomManagementDialog.cpp
    src/screens/dialog/MedicalHistoryDialog.cpp
    src/screens/AppointmentScreen.cpp
    src/screens/EmergencyAppointmentScreen.cpp
    src/screens/MedicalHistoryScreen.cpp
    src/screens/PatientScreen.cpp
    src/screens/RoomManagementScreen.cpp
    src/screens/SuggestDoctorScreen.cpp
    src/source/AppointmentSystem.cpp
    src/source/EmergencySystem.cpp
    src/source/PatientManagementSystem.cpp
    src/source/RoomManagementSystem.cpp
    src/source/Source.cpp
    src/Mainwindow.cpp
    main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(shms
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

# Define target properties for Android with Qt 6 as:
# set_property(TARGET shms APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(shms SHARED
            ${PROJECT_SOURCES}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(shms
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(shms PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(shms PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.shms)
endif()

set_target_properties(shms PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS shms
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(shms)
endif()
