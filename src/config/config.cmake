set(app_SOURCES ${app_SOURCES}
	   	${current_PROJECT_DIR}/config.cpp)

set(app_HEADERS ${app_HEADERS})

configure_file(
	${current_PROJECT_DIR}/salon.ini ${CMAKE_BINARY_DIR}/salon.ini
	COPYONLY)
