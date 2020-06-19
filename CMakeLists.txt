cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(15-kkmeans VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(15-kkmeans VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)

find_package(dlib REQUIRED)

set(HEADERS)
set(SOURCES src/main.cpp)

add_executable(15-kkmeans ${SOURCES} ${HEADERS})

set_target_properties(15-kkmeans PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(15-kkmeans PRIVATE "${CMAKE_BINARY_DIR}")
target_link_libraries(15-kkmeans PRIVATE dlib::dlib)

if(MSVC)
    target_compile_options(15-kkmeans PRIVATE
        /W4
    )
else()
    target_compile_options(15-kkmeans PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 15-kkmeans RUNTIME DESTINATION bin)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data/" DESTINATION data)
install(FILES "${PROJECT_SOURCE_DIR}/graphics.py" DESTINATION scripts)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
