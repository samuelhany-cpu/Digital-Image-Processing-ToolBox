cmake_minimum_required(VERSION 3.16)
project(SimplifiedMain)

set(CMAKE_CXX_STANDARD 14)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable MOC
set(CMAKE_AUTOMOC ON)

# Windows-specific OpenCV configuration
if(WIN32)
    include_directories("F:/OpenCV/opencv/build/include")
    link_directories("F:/OpenCV/opencv/build/x64/vc15/lib")
    set(OpenCV_LIBS 
        debug opencv_world430d
        optimized opencv_world430
    )
endif()

# Create executable
add_executable(simplified_main ../simplified_main.cpp)

# Include directories
target_include_directories(simplified_main PRIVATE "F:/OpenCV/opencv/build/include")

# Link libraries
target_link_libraries(simplified_main Qt6::Core Qt6::Widgets ${OpenCV_LIBS})

# Copy DLLs
if(WIN32)
    set(QT6_DLL_DIR "C:/Qt/6.7.3/msvc2019_64/bin")
    set(QT6_DLLS "Qt6Core.dll" "Qt6Gui.dll" "Qt6Widgets.dll")
    
    foreach(QT_DLL ${QT6_DLLS})
        add_custom_command(TARGET simplified_main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT6_DLL_DIR}/${QT_DLL}"
            $<TARGET_FILE_DIR:simplified_main>
        )
    endforeach()
    
    # Copy OpenCV DLLs
    add_custom_command(TARGET simplified_main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430.dll"
        $<TARGET_FILE_DIR:simplified_main>
    )
    
    # Copy platform plugins
    add_custom_command(TARGET simplified_main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:simplified_main>/platforms
    )
    add_custom_command(TARGET simplified_main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Qt/6.7.3/msvc2019_64/plugins/platforms/qwindows.dll"
        $<TARGET_FILE_DIR:simplified_main>/platforms/
    )
endif()