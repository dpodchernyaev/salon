######## ensure debug if no build type specified #########
if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
	set(VERBOSE_COMPILER "-Wall -Wextra -g")
	set(CMAKE_CXX_FLAGS_DEBUG ${VERBOSE_COMPILER})
	set(CMAKE_C_FLAGS_DEBUG ${VERBOSE_COMPILER})
	message(STATUS "Default build type set to ${CMAKE_BUILD_TYPE}")
else(NOT DEFINED CMAKE_BUILD_TYPE)
	message(STATUS "Inherited build type is ${CMAKE_BUILD_TYPE}")
endif(NOT DEFINED CMAKE_BUILD_TYPE)


#option(LIBFSQT_STATIC "Static libfsqt linking" OFF)
#message(STATUS "Static libfsqt linking: ${LIBFSQT_STATIC}")

project(salon)
cmake_minimum_required(VERSION 2.4.0)
if(NOT QT4_FOUND)
		find_package(Qt4 REQUIRED)
endif(NOT QT4_FOUND)

#if (NOT LIBFSQT_STATIC)
#	find_package(FSQt REQUIRED zmq)
#endif()

#if (NOT ZEROMQ_FOUND)
	#find_package(ZeroMQ REQUIRED)
#endif()

if(QT_QTSVG_FOUND)
	message ( STATUS "SVG FOUND")
else(QT_QTSVG_FOUND)
	message ( STATUS "SVG NOT FOUND")
endif(QT_QTSVG_FOUND)

# show compiler output
set(CMAKE_VERBOSE_MAKEFILE OFF)

#include(CMakeFSConfig.cmake)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--export-all-symbols")
	set(CMAKE_CXX_FLAGS "-export-all-symbols")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
######### END #########

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dunix")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Dunix")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


if(NOT salon_VERSION_MAJOR)
	set(salon_VERSION_MAJOR 0)
endif(NOT salon_VERSION_MAJOR)

if(NOT salon_VERSION_MINOR)
	set(salon_VERSION_MINOR 0)
endif(NOT salon_VERSION_MINOR)

if(NOT salon_VERSION_PATCH)
	set(salon_VERSION_PATCH 0)
endif(NOT salon_VERSION_PATCH)

if(NOT salon_VERSION_BUILD)
	set(salon_VERSION_BUILD 0)
endif(NOT salon_VERSION_BUILD)

if(NOT salon_SVN_REVISION)
	set(salon_SVN_REVISION 0)
endif(NOT salon_SVN_REVISION)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake_config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake_config.h)


if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

######### include subprojects #########
macro(INCLUDE_SUBDIR SUBDIR CMAKENAME)
	set(current_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
	set(current_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${SUBDIR})
	include(${SUBDIR}/${CMAKENAME}.cmake)
endmacro(INCLUDE_SUBDIR SUBDIR)

#INCLUDE_DIRECTORIES( ${ZEROMQ_INCLUDE_DIR} )
#INCLUDE_LIBFSQT()
# include build directory to locate and bind generated moc, qrc and ui files
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${FSQt_INCLUDES})

set(salon_EXTRALIBS "")
set(SYSTEM_EXTRALIBS "")

include (app.cmake)
INCLUDE_SUBDIR ("db" "db")
INCLUDE_SUBDIR ("config" "config")
INCLUDE_SUBDIR ("gui" "gui")
INCLUDE_SUBDIR ("report" "report")
INCLUDE_SUBDIR ("model" "model")
INCLUDE_SUBDIR ("smtp/src" "smtp")
INCLUDE_SUBDIR ("smtp_client" "smtp_client")

#INCLUDE_SUBDIR ("config/widgets" "widgets")

include_directories(${FSQt_INCLUDES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#include_directories("/usr/include/fsqt/libfs")
	#include_directories("/usr/include/fsqt")
	set(salon_EXTRALIBS odbc)
    set(SYSTEM_EXTRALIBS "X11")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif()

set(SHARED_INCLUDES ${SHARED_INCLUDES}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR})

######### generate qt files #########
# generate moc_*.cpp files
QT4_WRAP_CPP(app_HEADERS_MOC ${app_HEADERS})

######### END #########

######### inlcude qt directories and definitions #########
set(QT_USE_QTXML TRUE)
set(QT_USE_QTNETWORK TRUE)
set(QT_USE_QTXMLPATTERNS TRUE)
set(QT_USE_QTSQL TRUE)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTSVG TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS} ${FSQt_DEFINITIONS})
add_definitions("-DQT_NO_KEYWORDS")
add_definitions("-DSMTP_BUILD")

######### END #########

### making executable program

# no crash dialog
if(${NO_CRASH_HANDLER} MATCHES "YES")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_CRASH_HANDLER")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CRASH_HANDLER")
endif(${NO_CRASH_HANDLER} MATCHES "YES")

# making executable module
add_executable(salon WIN32
				${app_SOURCES}
				${app_HEADERS_MOC}
				${app_RESOURCES})

target_link_libraries(salon ${SHARED_EXTRALIBS} ${QT_LIBRARIES} ${salon_EXTRALIBS} ${SYSTEM_EXTRALIBS})


file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../pics/"
	DESTINATION	"${CMAKE_BINARY_DIR}/pics/"
	FILES_MATCHING PATTERN "*")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../pics/"
	DESTINATION	"${CMAKE_BINARY_DIR}/pics/"
	FILES_MATCHING PATTERN "*")

