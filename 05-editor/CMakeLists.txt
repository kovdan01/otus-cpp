cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(05-paint VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(05-paint VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)

set(UTILS src/utils/point.h src/utils/color.h)
set(SHAPES src/shape/shape.h src/shape/circle.h src/shape/rectangle.h src/shape/segment.h)
set(DOCUMENT src/document/document.h src/document/file_io.h)
set(APPLICATION src/application/application.h)
set(RENDER src/render/renderable.h)
set(SOURCES src/main.cpp ${SHAPES} ${DOCUMENT} ${RENDER} ${APPLICATION} ${UTILS})

add_executable(05-paint ${SOURCES})

set_target_properties(05-paint PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(05-paint PRIVATE "${CMAKE_BINARY_DIR}")

if(MSVC)
    target_compile_options(05-paint PRIVATE
        /W4
    )
else()
    target_compile_options(05-paint PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 05-paint RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
