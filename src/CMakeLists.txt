set(ndmanager_SRCS
    acquisitionsystempage.cpp
    anatomypage.cpp
    channelcolors.cpp
    channelcolorspage.cpp
    channeloffsetspage.cpp
    clusterspage.cpp
    descriptionwriter.cpp
    fileinformation.cpp
    filepage.cpp
    filespage.cpp
    generalinfopage.cpp
    generalinformation.cpp
    helper.cpp
    launcherpage.cpp
    lfppage.cpp
    main.cpp
    managerview.cpp
    miscellaneouspage.cpp
    ndmanager.cpp
    ndmanagerdoc.cpp
    neuroscopevideoinfo.cpp
    neuroscopevideopage.cpp
    parameterpage.cpp
    parameterview.cpp
    programinformation.cpp
    programspage.cpp
    programpage.cpp
    queryinputdialog.cpp
    queryoutputdialog.cpp
    spikepage.cpp
    tags.cpp
    unitlistlayout.cpp
    unitlistpage.cpp
    unittable.cpp
    videopage.cpp
    xmlreader.cpp
    xmlwriter.cpp
    acquisitionsystemlayout.cpp
    anatomylayout.cpp
    channelcolorslayout.cpp
    channeloffsetslayout.cpp
    clusterslayout.cpp
    filelayout.cpp
    generalinfolayout.cpp
    launcherlayout.cpp
    lfplayout.cpp
    neuroscopemisclayout.cpp
    neuroscopevideolayout.cpp
    parameterlayout.cpp
    queryinputdialogbase.cpp
    spikelayout.cpp
    videolayout.cpp
    ndmanagerutils.cpp
    colortable.cpp
    parametertree.cpp
)

qt_add_resources(ndmanager_SRCS ndmanager-icons.qrc ndmanager-html.qrc)

set(ndmanager_UI
    acquisitionsystemlayout.ui
    anatomylayout.ui
    channelcolorslayout.ui
    channeloffsetslayout.ui
    clusterslayout.ui
    filelayout.ui
    generalinfolayout.ui
    launcherlayout.ui
    lfplayout.ui
    neuroscopemisclayout.ui
    neuroscopevideolayout.ui
    parameterlayout.ui
    queryinputdialogbase.ui
    spikelayout.ui
    videolayout.ui
)
qt_wrap_ui(ndmanager_SRCS ${ndmanager_UI})

if(APPBUNDLE)
    add_executable(ndmanager MACOSX_BUNDLE ${ndmanager_SRCS})
else()
    add_executable(ndmanager WIN32 ${ndmanager_SRCS})
endif()

target_link_libraries(ndmanager PRIVATE neurosuite::neurosuite Qt6::Widgets Qt6::PrintSupport Qt6::Xml)

if(APPBUNDLE)
    install(TARGETS ndmanager BUNDLE DESTINATION ".")
else()
    install(TARGETS ndmanager RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# Bundle the Qt runtime on Windows and macOS at install time (after the target is installed).
if(WIN32 OR APPBUNDLE)
    qt_generate_deploy_app_script(
        TARGET ndmanager
        OUTPUT_SCRIPT NDMANAGER_DEPLOY_SCRIPT
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${NDMANAGER_DEPLOY_SCRIPT})
endif()

# Default parameter file and parameter templates
install(FILES ndManagerDefault.xml DESTINATION ${NDMANAGER_DATA_INSTALL_DIR})
file(GLOB templates "${CMAKE_CURRENT_SOURCE_DIR}/Template*.xml")
install(FILES ${templates} DESTINATION ${NDMANAGER_DATA_INSTALL_DIR}/templates)

if(UNIX AND NOT APPLE)
    set(APP_ID io.github.neurosuite.NDManager)
    set(ICON_DIR ${CMAKE_INSTALL_DATADIR}/icons/hicolor)
    foreach(SIZE 16 22 32 48 64)
        install(FILES hi${SIZE}-app-ndmanager.png
                DESTINATION ${ICON_DIR}/${SIZE}x${SIZE}/apps
                RENAME ndmanager.png)
        install(FILES hi${SIZE}-nphys-xml.png
                DESTINATION ${ICON_DIR}/${SIZE}x${SIZE}/mimetypes
                RENAME application-nphys-xml.png)
    endforeach()
    install(FILES ndmanager.desktop
            DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
            RENAME ${APP_ID}.desktop)
    install(FILES ${APP_ID}.metainfo.xml
            DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
    install(FILES ndmanager.xml
            DESTINATION ${CMAKE_INSTALL_DATADIR}/mime/packages)
endif()
