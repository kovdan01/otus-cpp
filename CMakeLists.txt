cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(11-async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(11-async VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS COMPONENTS program_options REQUIRED)

set(INTERFACES
    src/include/interfaces/command.h
    src/include/interfaces/processor.h
    src/include/interfaces/reader.h
    src/include/interfaces/storage.h
    src/include/interfaces/writer.h)

set(HEADERS ${INTERFACES}
    src/include/dummy_command.h
    src/include/dummy_command_processor.h
    src/include/command_storage.h
    src/include/console_reader.h
    src/include/console_writer.h
    src/include/file_writer.h
    src/include/thread_pool.h
    src/include/stats.h)

set(SOURCES
    src/logic/command.cpp
    src/logic/dummy_command.cpp
    src/logic/dummy_command_processor.cpp
    src/logic/command_storage.cpp
    src/logic/console_reader.cpp
    src/logic/console_writer.cpp
    src/logic/file_writer.cpp
    src/thread/thread_pool.cpp)

add_executable(bulkmt src/main.cpp)
add_executable(tests src/tests.cpp)

add_library(async SHARED lib/async.cpp lib/async.h ${SOURCES} ${HEADERS})
add_executable(async_cli src/client.cpp)

target_link_libraries(async_cli async)

set_target_properties(bulkmt tests async async_cli PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(bulkmt PRIVATE "${CMAKE_BINARY_DIR}" src/include)
target_link_libraries(bulkmt PRIVATE Boost::program_options pthread stdc++fs async)

target_include_directories(async PRIVATE "${CMAKE_BINARY_DIR}" src/include)
target_link_libraries(async PRIVATE pthread stdc++fs)

target_include_directories(tests PRIVATE ${GTest_INCLUDE_DIR} src/include lib)
target_link_libraries(tests PRIVATE gtest pthread stdc++fs async)

target_include_directories(async_cli PRIVATE "${CMAKE_BINARY_DIR}" lib)

if(MSVC)
    target_compile_options(bulkmt PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
    target_compile_options(async PRIVATE
        /W4
    )
    target_compile_options(async_cli PRIVATE
        /W4
    )
else()
    target_compile_options(bulkmt PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(tests PRIVATE
        -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(async PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(async_cli PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()

install(TARGETS async LIBRARY DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
