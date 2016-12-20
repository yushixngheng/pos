SET(POS_SRC_LIST pos.c)
ADD_EXECUTABLE(pos  	${LIB_SRC_LIST}
						${DB_SRC_LIST}
						${HSM_SRC_LIST}
                        ${PROTOCOL_SRC_LIST}
                        ${POS_SRC_LIST})
target_link_libraries(pos m)
target_link_libraries(pos libqpid-proton.so)
target_link_libraries(pos libocilib.so)
target_link_libraries(pos libqpid-proton.so)
target_link_libraries(pos libmemcached.so)
