set(app_SOURCES ${app_SOURCES}
		${current_PROJECT_DIR}/db_service.cpp
		${current_PROJECT_DIR}/dbconnection.cpp
		${current_PROJECT_DIR}/ping.cpp)

set(app_HEADERS ${app_HEADERS}
		${current_PROJECT_DIR}/db_service.h
		${current_PROJECT_DIR}/dbconnection.h)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(salon_EXTRALIBS ${salon_EXTRALIBS} wsock32)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
