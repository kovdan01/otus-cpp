cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(16-realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(16-realty VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)

find_package(dlib REQUIRED)

set(HEADERS src/apartment.h)
set(SOURCES src/apartment.cpp)

add_executable(rclst src/rclst.cpp ${HEADERS} ${SOURCES})
add_executable(rclss src/rclss.cpp ${HEADERS} ${SOURCES})

set_target_properties(rclst rclss PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(rclst PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(rclss PRIVATE "${CMAKE_BINARY_DIR}")
target_link_libraries(rclst PRIVATE dlib::dlib)
target_link_libraries(rclss PRIVATE dlib::dlib)

if(MSVC)
    target_compile_options(rclst PRIVATE
        /W4
    )
    target_compile_options(rclss PRIVATE
        /W4
    )
else()
    target_compile_options(rclst PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(rclss PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS rclst rclss RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
