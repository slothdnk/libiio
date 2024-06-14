cmake_minimum_required(VERSION 3.10)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
project(iiopp-enum LANGUAGES CXX)

#####################################
# Define Target Information
#####################################
# Set Project version (used for library versioning and for CPack)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 1)
set(CMAKE_PROJECT_VERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH})

set(TARGET_NAME iiopp)
set(TARGET_INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
set(TARGET_SOURCES  
	    ${CMAKE_CURRENT_SOURCE_DIR}/iiopp.cpp
		)

set(CMAKE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/iiopp")
#####################################
# Define Target according to options
#####################################
add_library(${TARGET_NAME} ${TARGET_SOURCES})

#####################################
# Define Target Properties
#####################################

# add target properties such as debug postfix, MSVC Runtime and library version
set_target_properties(${TARGET_NAME} PROPERTIES 
                                      DEBUG_POSTFIX "-d" 
                                      MSVC_RUNTIME_LIBRARY "${MSVC_RUNTIME_TYPE}" 
                                      VERSION ${CMAKE_PROJECT_VERSION}
                                      SOVERSION ${CMAKE_PROJECT_VERSION})

# set target compile options as defined in the cmake/compiler_options.cmake Module
target_compile_options(${TARGET_NAME} PRIVATE ${compiler_options})

target_link_libraries(${TARGET_NAME} PUBLIC iio)

# generate an export header to expose symbols when using the library
generate_export_header(${TARGET_NAME} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/lib${TARGET_NAME}_export.h)

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# include all the necessary directories
target_include_directories(${TARGET_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
   ${TARGET_INCLUDE_FOLDER}
   ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER 
		"")
install(TARGETS ${TARGET_NAME} 
	DESTINATION lib
	EXPORT ${TARGET_NAME}
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
	COMPONENT ${TARGET_NAME}
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/${TARGET_NAME}"
    COMPONENT dev
    )

add_executable(iiopp-enum examples/iiopp-enum.cpp iiopp.h ${LIBIIO_RC})
target_include_directories(iiopp-enum PRIVATE ./)
set_target_properties(iiopp-enum PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(iiopp-enum iio iiopp)
