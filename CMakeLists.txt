project('irose')
cmake_minimum_required(VERSION 3.2)

set(COMMON_SRC
	common/config.cpp
	common/globals.cpp
	common/log.cpp
	common/rosecrypt.cpp
	common/socket-base.cpp
	common/socket-client.cpp
	common/socket-server.cpp
	common/database/database.cpp
)

set(LOGIN_SRC
	${COMMON_SRC}
	login/loginclient.cpp
	login/extrafunction.cpp
	login/loginpackets.cpp
	login/loginserver.cpp
	login/main.cpp
	login/signal.cpp
)

if(MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX /D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Os")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Wextra -Werror -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g2")
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4")
	endif()
endif()

add_executable(irose-login ${LOGIN_SRC})
target_link_libraries(irose-login mysqlclient)
