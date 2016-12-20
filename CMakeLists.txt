cmake_minimum_required(VERSION 2.8)

PROJECT (NEWHELLO)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory(${PROJECT_SOURCE_DIR}/lib LIB_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/protocol PROTOCOL_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/hsm HSM_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/db DB_SRC_LIST)
include_directories(
					${PROJECT_SOURCE_DIR}/lib
			 		${PROJECT_SOURCE_DIR}/protocol
			 		${PROJECT_SOURCE_DIR}/hsm
			 		${PROJECT_SOURCE_DIR}/db
					)
ADD_SUBDIRECTORY(src)



