add_executable(test_get test_get.cpp)
target_link_libraries(test_get PRIVATE rudecgi::rudecgi)
add_test(NAME get COMMAND test_get)

add_executable(test_post test_post.cpp)
target_link_libraries(test_post PRIVATE rudecgi::rudecgi)
add_test(NAME post COMMAND test_post)

add_executable(test_cookie test_cookie.cpp)
target_link_libraries(test_cookie PRIVATE rudecgi::rudecgi)
add_test(NAME cookie COMMAND test_cookie)

# Each test runs in its own scratch dir so file fixtures don't collide.
set_tests_properties(get post cookie PROPERTIES TIMEOUT 60)
foreach(t get post cookie)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/run-${t}")
    set_tests_properties(${t} PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/run-${t}")
endforeach()
