cmake_minimum_required(VERSION 3.16)

project(DataLogStream VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick SerialPort Graphs Qml)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(DataLogStream
    main.cpp
)

qt_add_qml_module(DataLogStream
    URI DataLogStream
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES
            PortSelection.qml
            PlotStream.qml
        SOURCES
            portselectionservice.h portselectionservice.cpp
            plotstreamservice.h plotstreamservice.cpp
        RESOURCES
            refresh-icon.svg
)

qt_add_resources(DataLogStream "config"
    URI DataLogStream
    FILES
        qtquickcontrols2.conf
)

qt_add_resources(DataLogStream "images"
    URI DataLogStream
    FILES
        logo.png
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(DataLogStream PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DataLogStream
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(DataLogStream PRIVATE
    Qt6::Quick
    Qt6::SerialPort
    Qt6::Graphs
    Qt6::Qml
)

include(GNUInstallDirs)
install(TARGETS DataLogStream
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET DataLogStream
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

message("deploy script name: ${deploy_script}")
message("qt_deploy_support: ${QT_DEPLOY_SUPPORT}")
install(SCRIPT ${deploy_script})
