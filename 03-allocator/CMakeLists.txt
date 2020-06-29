cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(03-allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(03-allocator VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)

add_executable(03-allocator main.cpp allocator.h list.h)

set_target_properties(03-allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(03-allocator PRIVATE "${CMAKE_BINARY_DIR}")

if(MSVC)
    target_compile_options(03-allocator PRIVATE
        /W4
    )
else()
    target_compile_options(03-allocator PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 03-allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
