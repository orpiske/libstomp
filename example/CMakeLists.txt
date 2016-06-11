add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE)

if (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 
 add_definitions(-DLINUX_BUILD) 
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 

add_executable(
	server
	server.c
)

add_executable(
	client
	client.c
)

target_link_libraries(
	server
	stomp
)

target_link_libraries(
	client
	stomp
)


