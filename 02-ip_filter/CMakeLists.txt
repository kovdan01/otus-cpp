cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(02-ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(02-ip_filter VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(GTest REQUIRED)

add_library(utils utils.cpp utils.h)
add_executable(02-ip_filter main.cpp filter.h ip.h)
add_executable(tests tests.cpp input.h filter.h ip.h)

set_target_properties(02-ip_filter tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(02-ip_filter PRIVATE "${CMAKE_BINARY_DIR}")
target_link_libraries(02-ip_filter PRIVATE utils)

target_include_directories(tests PRIVATE ${GTest_INCLUDE_DIR})
target_link_libraries(tests PRIVATE pthread gtest utils)

if(MSVC)
    target_compile_options(02-ip_filter PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
    target_compile_options(utils PRIVATE
        /W4
    )
else()
    target_compile_options(02-ip_filter PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(tests PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(utils PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 02-ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
