cmake_minimum_required(VERSION 3.16)
project(MinimalOpenCVTest)

set(CMAKE_CXX_STANDARD 14)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable MOC
set(CMAKE_AUTOMOC ON)

# Windows-specific OpenCV configuration
if(WIN32)
    # Include directories
    include_directories("F:/OpenCV/opencv/build/include")
    
    # Library directories
    link_directories("F:/OpenCV/opencv/build/x64/vc15/lib")
    
    # Set OpenCV libraries manually
    set(OpenCV_LIBS 
        debug opencv_world430d
        optimized opencv_world430
    )
endif()

# Create executable
add_executable(minimal_test ../minimal_opencv_test.cpp)

# Include directories
target_include_directories(minimal_test PRIVATE "F:/OpenCV/opencv/build/include")

# Link libraries
target_link_libraries(minimal_test Qt6::Core Qt6::Widgets ${OpenCV_LIBS})

# Copy DLLs on Windows
if(WIN32)
    set(QT6_DLL_DIR "C:/Qt/6.7.3/msvc2019_64/bin")
    set(QT6_DLLS "Qt6Core.dll" "Qt6Gui.dll" "Qt6Widgets.dll")
    
    foreach(QT_DLL ${QT6_DLLS})
        if(EXISTS "${QT6_DLL_DIR}/${QT_DLL}")
            add_custom_command(TARGET minimal_test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT6_DLL_DIR}/${QT_DLL}"
                $<TARGET_FILE_DIR:minimal_test>
            )
        endif()
    endforeach()
    
    # Copy OpenCV DLLs
    if(EXISTS "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430.dll")
        add_custom_command(TARGET minimal_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430.dll"
            $<TARGET_FILE_DIR:minimal_test>
        )
    endif()
    
    if(EXISTS "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430d.dll")
        add_custom_command(TARGET minimal_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430d.dll"
            $<TARGET_FILE_DIR:minimal_test>
        )
    endif()
    
    # Copy platform plugins
    set(QT6_PLATFORMS_DIR "C:/Qt/6.7.3/msvc2019_64/plugins/platforms")
    if(EXISTS "${QT6_PLATFORMS_DIR}")
        add_custom_command(TARGET minimal_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:minimal_test>/platforms
        )
        
        if(EXISTS "${QT6_PLATFORMS_DIR}/qwindows.dll")
            add_custom_command(TARGET minimal_test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT6_PLATFORMS_DIR}/qwindows.dll"
                $<TARGET_FILE_DIR:minimal_test>/platforms/
            )
        endif()
    endif()
endif()