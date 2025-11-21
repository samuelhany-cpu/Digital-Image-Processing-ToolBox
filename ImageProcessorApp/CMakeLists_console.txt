cmake_minimum_required(VERSION 3.10)
project(ImageProcessorConsole)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
else()
    # For non-Windows systems, use find_package
    find_package(OpenCV REQUIRED)
endif()

# Console version executable
add_executable(console_app ../console_version.cpp)

# Include directories
target_include_directories(console_app PRIVATE "F:/OpenCV/opencv/build/include")

# Link OpenCV
target_link_libraries(console_app ${OpenCV_LIBS})

# Windows specific settings
if(WIN32)
    # Copy OpenCV DLLs to output directory
    if(EXISTS "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430.dll")
        add_custom_command(TARGET console_app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430.dll"
            $<TARGET_FILE_DIR:console_app>
        )
    endif()
    
    if(EXISTS "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430d.dll")
        add_custom_command(TARGET console_app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "F:/OpenCV/opencv/build/x64/vc15/bin/opencv_world430d.dll"
            $<TARGET_FILE_DIR:console_app>
        )
    endif()
endif()