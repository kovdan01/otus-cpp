cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(12-bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(12-bulk_server VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(Boost COMPONENTS COMPONENTS program_options REQUIRED)

set(INTERFACES
    src/include/interfaces/command.h
    src/include/interfaces/processor.h
    src/include/interfaces/reader.h
    src/include/interfaces/writer.h)

set(HEADERS ${INTERFACES}
    src/include/dummy_command.h
    src/include/dummy_command_processor.h
    src/include/stream_reader.h
    src/include/console_writer.h
    src/include/network_server.h
    src/include/network_session.h
    src/include/file_writer.h
    src/include/common_command_buffer.h
    src/include/thread_pool.h
    src/include/stats.h
    src/include/utils.h
    src/include/global_state.h
    src/include/controller.h)

set(SOURCES
    src/logic/command.cpp
    src/logic/dummy_command.cpp
    src/logic/dummy_command_processor.cpp
    src/logic/stream_reader.cpp
    src/logic/console_writer.cpp
    src/logic/network_server.cpp
    src/logic/network_session.cpp
    src/logic/file_writer.cpp
    src/logic/controller.cpp
    src/logic/common_command_buffer.cpp
    src/logic/utils.cpp
    src/logic/global_state.cpp
    src/thread/thread_pool.cpp)

add_executable(bulk_server src/main.cpp ${SOURCES} ${HEADERS})

set_target_properties(bulk_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(bulk_server PRIVATE "${CMAKE_BINARY_DIR}" "src/include")
target_link_libraries(bulk_server PRIVATE Boost::program_options pthread stdc++fs)

if(MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
else()
    target_compile_options(bulk_server PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()

install(TARGETS bulk_server LIBRARY DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
