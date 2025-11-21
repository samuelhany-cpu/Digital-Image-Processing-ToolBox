cmake_minimum_required(VERSION 3.16)
project(SimpleTest)

set(CMAKE_CXX_STANDARD 14)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable MOC
set(CMAKE_AUTOMOC ON)

# Create executable
add_executable(simple_test ../simple_test.cpp)

# Link Qt6
target_link_libraries(simple_test Qt6::Core Qt6::Widgets)

# Copy Qt6 DLLs on Windows
if(WIN32)
    set(QT6_DLL_DIR "C:/Qt/6.7.3/msvc2019_64/bin")
    set(QT6_DLLS "Qt6Core.dll" "Qt6Gui.dll" "Qt6Widgets.dll")
    
    foreach(QT_DLL ${QT6_DLLS})
        if(EXISTS "${QT6_DLL_DIR}/${QT_DLL}")
            add_custom_command(TARGET simple_test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT6_DLL_DIR}/${QT_DLL}"
                $<TARGET_FILE_DIR:simple_test>
            )
        endif()
    endforeach()
    
    # Copy platform plugins
    set(QT6_PLATFORMS_DIR "C:/Qt/6.7.3/msvc2019_64/plugins/platforms")
    if(EXISTS "${QT6_PLATFORMS_DIR}")
        add_custom_command(TARGET simple_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:simple_test>/platforms
        )
        
        if(EXISTS "${QT6_PLATFORMS_DIR}/qwindows.dll")
            add_custom_command(TARGET simple_test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT6_PLATFORMS_DIR}/qwindows.dll"
                $<TARGET_FILE_DIR:simple_test>/platforms/
            )
        endif()
    endif()
endif()