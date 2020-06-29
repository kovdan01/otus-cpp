cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(12-bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(12-bulk_server VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(Boost COMPONENTS COMPONENTS program_options system REQUIRED)

set(INTERFACES
    src/include/interfaces/command.h
    src/include/interfaces/processor.h
    src/include/interfaces/reader.h
    src/include/interfaces/writer.h)

set(HEADERS ${INTERFACES}
    src/include/implementation/dummy_command.h
    src/include/implementation/dummy_command_processor.h
    src/include/implementation/stream_reader.h
    src/include/implementation/console_writer.h
    src/include/implementation/file_writer.h
    src/include/implementation/common_command_buffer.h
    src/include/implementation/global_state.h

    src/include/network/server.h
    src/include/network/session.h
    src/include/network/raw_data_handler.h

    src/include/utils/thread_pool.h
    src/include/utils/utils.h)

set(SOURCES
    src/logic/implementation/command.cpp
    src/logic/implementation/dummy_command.cpp
    src/logic/implementation/dummy_command_processor.cpp
    src/logic/implementation/stream_reader.cpp
    src/logic/implementation/console_writer.cpp
    src/logic/implementation/file_writer.cpp
    src/logic/implementation/common_command_buffer.cpp
    src/logic/implementation/global_state.cpp

    src/logic/network/server.cpp
    src/logic/network/session.cpp
    src/logic/network/raw_data_handler.cpp

    src/utils/thread_pool.cpp
    src/utils/utils.cpp)

add_executable(bulk_server src/main.cpp ${SOURCES} ${HEADERS})

set_target_properties(bulk_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(bulk_server PRIVATE "${CMAKE_BINARY_DIR}" "src/include")
target_link_libraries(bulk_server PRIVATE Boost::program_options Boost::system pthread stdc++fs)

if(MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
else()
    target_compile_options(bulk_server PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()

install(TARGETS bulk_server DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
