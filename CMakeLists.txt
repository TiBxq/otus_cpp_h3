cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
#project(allocator VERSION 0.0.1)

#find_package(GTest REQUIRED)
#find_package(Threads REQUIRED)

add_executable(allocator allocator.cpp)
#add_executable(test_version_gtest test_version_gtest.cpp)

set_target_properties(allocator PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON 
)

#target_include_directories(test_version_gtest
#    PRIVATE
#    ${GTEST_INCLUDE_DIRS}
#)

#target_link_libraries(test_version_gtest
#    ${GTEST_LIBRARIES}
#    ${CMAKE_THREAD_LIBS_INIT}
#    version_lib
#)

if (MSVC)
    target_compile_options(allocator PRIVATE
        /W4
    )
#    target_compile_options(test_version_gtest PRIVATE
#        /W4
#    )
else ()
    target_compile_options(allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
#    target_compile_options(test_version_gtest PRIVATE
#        -Wall -Wextra -pedantic -Werror
#    )
endif()

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT savenko.bogdan@gmail.com)

include(CPack)

#enable_testing()

#add_test(test_version_gtest test_version_gtest)