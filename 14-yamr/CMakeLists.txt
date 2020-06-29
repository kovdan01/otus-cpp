cmake_minimum_required(VERSION 3.2)


if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(14-yamr VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(14-yamr VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS COMPONENTS program_options REQUIRED)

set(HEADERS
    src/thread_pool.h
    src/controller.h
    src/mapper.h
    src/reducer.h
    src/splitter.h)

set(SOURCES
    src/thread_pool.cpp
    src/controller.cpp
    src/mapper.cpp
    src/reducer.cpp
    src/splitter.cpp)

add_executable(14-yamr src/main.cpp ${SOURCES} ${HEADERS})
add_executable(tests src/tests.cpp ${SOURCES} ${HEADERS})

file(COPY data DESTINATION "${CMAKE_BINARY_DIR}")

set_target_properties(14-yamr tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# add version.h
target_include_directories(14-yamr PRIVATE "${CMAKE_BINARY_DIR}")
target_link_libraries(14-yamr PRIVATE pthread Boost::program_options)

target_include_directories(tests PRIVATE ${GTest_INCLUDE_DIR})
target_link_libraries(tests PRIVATE gtest pthread)

if(MSVC)
    target_compile_options(14-yamr PRIVATE
        /W4
    )
    target_compile_options(tests PRIVATE
        /W4
    )
else()
    target_compile_options(14-yamr PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
    target_compile_options(tests PRIVATE
        -Werror -Wall -Wimplicit-fallthrough -pedantic-errors
    )
endif()


install(TARGETS 14-yamr RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


enable_testing()
add_test(tests tests)
