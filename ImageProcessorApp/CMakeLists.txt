cmake_minimum_required(VERSION 3.16)
project(ImageProcessorApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific OpenCV configuration
if(WIN32)
    # Set OpenCV directory explicitly for Windows
    set(OpenCV_DIR "F:/OpenCV/opencv/build/x64/vc15/lib" CACHE PATH "OpenCV config directory")
    
    # Include directories
    include_directories("F:/OpenCV/opencv/build/include")
    
    # Library directories
    link_directories("F:/OpenCV/opencv/build/x64/vc15/lib")
    
    # Set OpenCV libraries manually
    set(OpenCV_LIBS 
        debug opencv_world430d
        optimized opencv_world430
    )
    
    set(OpenCV_FOUND TRUE)
else()
    # For non-Windows systems, use find_package
    find_package(OpenCV REQUIRED)
endif()

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/ImageCanvas.cpp
    src/TransformDialog.cpp
    src/HistogramWidget.cpp
)

# Header files
set(HEADERS
    src/MainWindow.h
    src/ImageCanvas.h
    src/TransformDialog.h
    src/HistogramWidget.h
    include/ImageProcessor.h
)

# Resources
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "F:/OpenCV/opencv/build/include"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Windows specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # Copy OpenCV DLLs to output directory
    if(EXISTS "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
    
    if(EXISTS "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430d.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430d.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
    
    # Copy Qt6 DLLs to output directory
    set(QT6_DLL_DIR "C:/Qt/6.7.3/msvc2019_64/bin")
    set(QT6_DLLS 
        "Qt6Core.dll"
        "Qt6Gui.dll" 
        "Qt6Widgets.dll"
        "Qt6Network.dll"
        "Qt6OpenGL.dll"
    )
    
    foreach(QT_DLL ${QT6_DLLS})
        if(EXISTS "${QT6_DLL_DIR}/${QT_DLL}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT6_DLL_DIR}/${QT_DLL}"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
        endif()
    endforeach()
    
    # Copy Qt6 platform plugins
    set(QT6_PLATFORMS_DIR "C:/Qt/6.7.3/msvc2019_64/plugins/platforms")
    if(EXISTS "${QT6_PLATFORMS_DIR}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
        )
        
        if(EXISTS "${QT6_PLATFORMS_DIR}/qwindows.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT6_PLATFORMS_DIR}/qwindows.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
            )
        endif()
        
        if(EXISTS "${QT6_PLATFORMS_DIR}/qminimal.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT6_PLATFORMS_DIR}/qminimal.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
            )
        endif()
    endif()
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)