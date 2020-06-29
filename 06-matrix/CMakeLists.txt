cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(06-matrix VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(06-matrix VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(GTest REQUIRED)

set(HEADERS src/matrix.h)
set(SOURCES src/main.cpp)
set(TEST_SOURCES src/tests.cpp)

add_executable(06-matrix ${SOURCES} ${HEADERS})
add_executable(tests ${TEST_SOURCES} ${HEADERS})

set_target_properties(06-matrix tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(06-matrix PRIVATE "${CMAKE_BINARY_DIR}")

target_include_directories(tests PRIVATE ${GTest_INCLUDE_DIR})
target_link_libraries(tests PRIVATE gtest pthread)

if(MSVC)
    target_compile_options(06-matrix PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
else()
    target_compile_options(06-matrix PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(tests PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 06-matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
