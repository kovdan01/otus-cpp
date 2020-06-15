cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(07-bulk VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(07-bulk VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS COMPONENTS program_options REQUIRED)

set(INTERFACES
    src/interfaces/command.h
    src/interfaces/processor.h
    src/interfaces/reader.h
    src/interfaces/storage.h
    src/interfaces/writer.h)

set(HEADERS ${INTERFACES}
    src/dummy_command.h
    src/dummy_command_processor.h
    src/command_storage.h
    src/console_reader.h
    src/console_writer.h
    src/file_writer.h)

set(SOURCES
    src/interfaces/command.cpp
    src/dummy_command.cpp
    src/dummy_command_processor.cpp
    src/command_storage.cpp
    src/console_reader.cpp
    src/console_writer.cpp
    src/file_writer.cpp)

add_executable(07-bulk src/main.cpp ${SOURCES} ${HEADERS})
add_executable(tests src/tests.cpp ${SOURCES} ${HEADERS})

set_target_properties(07-bulk tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(07-bulk PRIVATE "${CMAKE_BINARY_DIR}")
target_link_libraries(07-bulk PRIVATE Boost::program_options)

target_include_directories(tests PRIVATE ${GTest_INCLUDE_DIR})
target_link_libraries(tests PRIVATE gtest pthread)

if(MSVC)
    target_compile_options(07-bulk PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
else()
    target_compile_options(07-bulk PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(tests PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 07-bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
