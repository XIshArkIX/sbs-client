cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(sbs-client VERSION 0.0.1 LANGUAGES CXX)

# load GNU rules
include(GNUInstallDirs)

# C options
set(CMAKE_C_COMPILER "/bin/gcc-10")
set(CMAKE_C_STANDARD_REQUIRED 17)
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O4 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

# C++ options
set(CMAKE_CXX_COMPILER "/bin/g++-10")
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g") # -H for showing headers
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# boost
find_package(Boost REQUIRED)
# jsoncpp
find_package(jsoncpp REQUIRED)
# enable threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# generate objects file
add_library(${PROJECT_NAME}-objs OBJECT
src/lib/sbs-client.hpp
src/lib/sbs-client.raw.cpp
src/lib/sbs-client.squad.cpp
src/lib/sbs-client.user.cpp
src/lib/sbs-client.queryBuilder.cpp)
target_compile_options(${PROJECT_NAME}-objs PRIVATE -Wall -fPIC)

# generate shared library
add_library(${PROJECT_NAME}-so SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-objs>)
# link boost and jsoncpp
target_link_libraries(${PROJECT_NAME}-so PUBLIC Boost::boost jsoncpp)

# build static library
add_library(${PROJECT_NAME}-a STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-objs>)
target_link_libraries(${PROJECT_NAME}-a PUBLIC Boost::boost jsoncpp)

# set library version
set_target_properties(${PROJECT_NAME}-so PROPERTIES VERSION ${PROJECT_VERSION})
# expose public header file
set_target_properties(${PROJECT_NAME}-so PROPERTIES PUBLIC_HEADER src/lib/sbs-client.hpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# declare files to install
install(TARGETS ${PROJECT_NAME}-so
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# configure .pc.in file
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

# install library
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# testing
set(TESTS_LIBS Threads::Threads Boost::boost jsoncpp ${PROJECT_NAME}-a)
set(TESTS_TIMEOUT 1)
set(USER_TESTS_PREFIX ${PROJECT_NAME}.user)
set(USER_TESTS_PATH src/tests/user/sbs-client)
# generate executable (tests)
add_executable(${USER_TESTS_PREFIX}.getAll.test ${USER_TESTS_PATH}.getAll.cpp)
add_executable(${USER_TESTS_PREFIX}.getById.test ${USER_TESTS_PATH}.getById.cpp)
add_executable(${USER_TESTS_PREFIX}.stress.test ${USER_TESTS_PATH}.stress.cpp)
# link library staticly
target_link_libraries(${USER_TESTS_PREFIX}.getAll.test ${TESTS_LIBS})
target_link_libraries(${USER_TESTS_PREFIX}.getById.test ${TESTS_LIBS})
target_link_libraries(${USER_TESTS_PREFIX}.stress.test ${TESTS_LIBS})
# include ctest
include(CTest)
enable_testing()
# getAll test
add_test(GetAllTest.User ${USER_TESTS_PREFIX}.getAll.test)
set_tests_properties(GetAllTest.User PROPERTIES TIMEOUT ${TESTS_TIMEOUT})
set_tests_properties(GetAllTest.User PROPERTIES
PASS_REGULAR_EXPRESSION "\"code\":0}"
FAIL_REGULAR_EXPRESSION "GetAllTest fail;socket error")
# getById test
add_test(GetByIdTest.User ${USER_TESTS_PREFIX}.getById.test)
set_tests_properties(GetByIdTest.User PROPERTIES TIMEOUT ${TESTS_TIMEOUT})
set_tests_properties(GetByIdTest.User PROPERTIES
PASS_REGULAR_EXPRESSION "\"code\":0}"
FAIL_REGULAR_EXPRESSION "GetByIdTest fail;socket error")
# stress test
add_test(StressTest.User ${USER_TESTS_PREFIX}.stress.test)
set_tests_properties(StressTest.User PROPERTIES TIMEOUT 10)
set_tests_properties(StressTest.User PROPERTIES
PASS_REGULAR_EXPRESSION "StressTest ok"
FAIL_REGULAR_EXPRESSION "StressTest fail;socket error")