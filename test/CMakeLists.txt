project(tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(test1_source ranges.cpp)
set(test2_source operations.cpp)
set(test3_source derivate.cpp)
set(test4_source minimize_1d.cpp)

add_executable(test1 ${test1_source})
add_executable(test2 ${test2_source})
add_executable(test3 ${test3_source})
add_executable(test4 ${test4_source})

set(libs_list ${Boost_LIBRARIES})

target_link_libraries(test1 ${libs_list})
target_link_libraries(test2 ${libs_list})
target_link_libraries(test3 ${libs_list})
target_link_libraries(test4 ${libs_list})

add_test(NAME Ranges COMMAND test1)
add_test(NAME Operations COMMAND test2)
add_test(NAME Derivate COMMAND test3)
add_test(NAME Minimize1D COMMAND test4)