set(app_SOURCES ${app_SOURCES}
		${current_PROJECT_DIR}/client_model.cpp
		${current_PROJECT_DIR}/coach_model.cpp
		${current_PROJECT_DIR}/group_model.cpp
		${current_PROJECT_DIR}/service_model.cpp
		${current_PROJECT_DIR}/item_model.cpp
		${current_PROJECT_DIR}/hall_model.cpp
		${current_PROJECT_DIR}/card_model.cpp
		${current_PROJECT_DIR}/shedule_model.cpp
		${current_PROJECT_DIR}/cs_model.cpp
		${current_PROJECT_DIR}/visit_model.cpp
		${current_PROJECT_DIR}/vid_model.cpp
		${current_PROJECT_DIR}/model_factory.cpp

		${current_PROJECT_DIR}/item_proxy_model.cpp

		${current_PROJECT_DIR}/cs_fetcher.cpp
		${current_PROJECT_DIR}/vid_fetcher.cpp
		${current_PROJECT_DIR}/visit_fetcher.cpp
		${current_PROJECT_DIR}/shedule_fetcher.cpp
		${current_PROJECT_DIR}/client_fetcher.cpp
		${current_PROJECT_DIR}/coach_fetcher.cpp
		${current_PROJECT_DIR}/service_fetcher.cpp
		${current_PROJECT_DIR}/hall_fetcher.cpp
		${current_PROJECT_DIR}/card_fetcher.cpp
		${current_PROJECT_DIR}/group_fetcher.cpp
		${current_PROJECT_DIR}/fetcher.cpp
		${current_PROJECT_DIR}/queue_fetcher.cpp

		${current_PROJECT_DIR}/item.cpp
		${current_PROJECT_DIR}/vid_item.cpp
		${current_PROJECT_DIR}/group_item.cpp
		${current_PROJECT_DIR}/client_service_item.cpp
		${current_PROJECT_DIR}/client_item.cpp
		${current_PROJECT_DIR}/visit_item.cpp
		${current_PROJECT_DIR}/shedule_item.cpp
		${current_PROJECT_DIR}/card_item.cpp
		${current_PROJECT_DIR}/service_item.cpp
		${current_PROJECT_DIR}/hall_item.cpp
		${current_PROJECT_DIR}/coach_item.cpp)

set(app_HEADERS ${app_HEADERS}
		${current_PROJECT_DIR}/client_model.h
		${current_PROJECT_DIR}/item_model.h
		${current_PROJECT_DIR}/coach_model.h
		${current_PROJECT_DIR}/hall_model.h
		${current_PROJECT_DIR}/card_model.h
		${current_PROJECT_DIR}/shedule_model.h

		${current_PROJECT_DIR}/cs_fetcher.h
		${current_PROJECT_DIR}/vid_fetcher.h
		${current_PROJECT_DIR}/visit_fetcher.h
		${current_PROJECT_DIR}/shedule_fetcher.h
		${current_PROJECT_DIR}/client_fetcher.h
		${current_PROJECT_DIR}/coach_fetcher.h
		${current_PROJECT_DIR}/service_fetcher.h
		${current_PROJECT_DIR}/hall_fetcher.h
		${current_PROJECT_DIR}/card_fetcher.h
		${current_PROJECT_DIR}/group_fetcher.h
		${current_PROJECT_DIR}/fetcher.h
		${current_PROJECT_DIR}/queue_fetcher.h)
