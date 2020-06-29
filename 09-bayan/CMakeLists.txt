cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(09-bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(09-bayan VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS COMPONENTS program_options REQUIRED)

set(HEADERS
    src/filesystem_traverser.h
    src/mask_filesystem_traverser.h
    src/filesystem_path_hash.h
    src/duplicate_finder.h)

set(FILESYSTEM
    src/mask_filesystem_traverser.cpp
    src/filesystem_path_hash.cpp)

set(HASH
    src/hash/crc32.h
    src/hash/crc16.h
    src/hash/crc32.cpp
    src/hash/crc16.cpp)

set(SOURCES ${HEADERS} ${HASH} ${FILESYSTEM} src/main.cpp)
set(TEST_SOURCES ${HASH} ${FILESYSTEM} test/tests.cpp)

add_executable(09-bayan ${SOURCES})
add_executable(tests ${TEST_SOURCES})
file(COPY test/a DESTINATION ${CMAKE_BINARY_DIR})

set_target_properties(09-bayan tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(09-bayan PRIVATE "${CMAKE_BINARY_DIR}")
target_link_libraries(09-bayan PRIVATE Boost::program_options stdc++fs)

target_include_directories(tests PRIVATE ${GTest_INCLUDE_DIR} src)
target_link_libraries(tests PRIVATE gtest pthread stdc++fs)

if(MSVC)
    target_compile_options(09-bayan PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
else()
    target_compile_options(09-bayan PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(tests PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 09-bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
