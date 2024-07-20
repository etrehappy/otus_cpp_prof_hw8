cmake_minimum_required(VERSION 3.12)

project(bayan_prj)

set(Boost_DEBUG OFF)

if(${CMAKE_VERSION} VERSION_LESS "3.30")
    message(STATUS "CMake version is ${CMAKE_VERSION}. Used CMP0167 OLD")    
else()
    message(STATUS "CMake version >= 3.30. Used CMP0167 NEW. Need to set BOOST_ROOT and Boost_DIR")
    cmake_policy(SET CMP0167 NEW)
    #set (BOOST_ROOT "C:/Program Files/boost/boost_1_85_0")
    #set (Boost_DIR "${BOOST_ROOT}/bin/x64/lib/cmake/Boost-1.85.0")
endif()

find_package (Boost COMPONENTS 
    filesystem 
    program_options 
    iostreams    
    REQUIRED
) 

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

include_directories (SYSTEM ${Boost_INCLUDE_DIR})

set(src_files 
    main.cpp
    arguments.h arguments.cpp
    dfinder.h dfinder.cpp
)


add_executable(bayan ${src_files})

if(MSVC)
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bayan)       
endif ()

target_link_libraries(bayan
    ${Boost_LIBRARIES}
)

target_compile_options(bayan  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

set_target_properties(bayan PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

if(MSVC)
    set(RUN_COMMAND powershell -Command "Get-Content ${CMAKE_SOURCE_DIR}\\arguments.txt | ForEach-Object { ${CMAKE_BINARY_DIR}\\Debug\\bayan.exe $_ }")
else()
    set(RUN_COMMAND xargs -a ${CMAKE_SOURCE_DIR}/arguments.txt ${CMAKE_BINARY_DIR}/Debug/bayan)
endif()

add_custom_target(run
    COMMAND ${RUN_COMMAND}
    DEPENDS bayan    
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}   
    COMMENT "##### Running with arguments.txt #####"
)
