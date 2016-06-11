add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE)

if (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 
 add_definitions(-DLINUX_BUILD) 
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 

add_executable(
	engine-server
	engine/server.c
)

add_executable(
	engine-client
	engine/client.c
)

add_executable(
	messenger-client
	messenger/client.c
)

target_link_libraries(
	engine-server
	stomp
)

target_link_libraries(
	engine-client
	stomp
)

target_link_libraries(
	messenger-client
	stomp
)


