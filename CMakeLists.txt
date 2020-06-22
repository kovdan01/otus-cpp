cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(13-join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(13-join_server VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS COMPONENTS program_options system REQUIRED)

set(HEADERS
    src/include/logic/database.h
    src/include/logic/message.h
    src/include/logic/command.h

    src/include/network/server.h
    src/include/network/session.h

    src/include/utils/thread_pool.h)

set(DATABASE
    src/logic/database.cpp
    src/logic/message.cpp
    src/logic/command.cpp)

set(NETWORK
    src/network/server.cpp
    src/network/session.cpp)

set(UTILS
    src/utils/thread_pool.cpp)

SET(SOURCES ${DATABASE} ${NETWORK} ${UTILS})

add_executable(join_server src/main.cpp ${SOURCES} ${HEADERS})
add_executable(tests src/tests.cpp ${DATABASE} ${HEADERS})

set_target_properties(join_server tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(join_server PRIVATE "${CMAKE_BINARY_DIR}" "src/include")
target_link_libraries(join_server PRIVATE Boost::program_options Boost::system pthread stdc++fs)

target_include_directories(tests PRIVATE "${CMAKE_BINARY_DIR}" "src/include")
target_link_libraries(tests PRIVATE gtest pthread)

if(MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
else()
    target_compile_options(join_server PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(tests PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()

install(TARGETS join_server DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
