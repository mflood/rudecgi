add_executable(test_get test_get.cpp)
target_link_libraries(test_get PRIVATE rudecgi::rudecgi)
add_test(NAME get COMMAND test_get)

add_executable(test_post test_post.cpp)
target_link_libraries(test_post PRIVATE rudecgi::rudecgi)
add_test(NAME post COMMAND test_post)

add_executable(test_cookie test_cookie.cpp)
target_link_libraries(test_cookie PRIVATE rudecgi::rudecgi)
add_test(NAME cookie COMMAND test_cookie)

# Parsing happens once, in the rude::CGI constructor, so every malformed
# CONTENT_LENGTH scenario needs its own process.
add_executable(test_content_length test_content_length.cpp)
target_link_libraries(test_content_length PRIVATE rudecgi::rudecgi)

set(CONTENT_LENGTH_SCENARIOS
    missing missing_with_cap empty over under
    negative garbage trailing_garbage overflow capped)

foreach(s IN LISTS CONTENT_LENGTH_SCENARIOS)
    add_test(NAME content_length_${s} COMMAND test_content_length ${s})
    list(APPEND CONTENT_LENGTH_TESTS content_length_${s})
endforeach()

# Each test runs in its own scratch dir so file fixtures don't collide.
set(ALL_TESTS get post cookie ${CONTENT_LENGTH_TESTS})
set_tests_properties(${ALL_TESTS} PROPERTIES TIMEOUT 60)
foreach(t IN LISTS ALL_TESTS)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/run-${t}")
    set_tests_properties(${t} PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/run-${t}")
endforeach()

# Guards the 5.1.1 pkg-config fix: rudecgi.pc must not hardcode the
# configure-time prefix, or "cmake --install --prefix <dir>" ships a .pc
# pointing somewhere else entirely (worst case, an older system copy).
add_test(NAME pkgconfig_relocatable
    COMMAND "${CMAKE_COMMAND}"
        "-DPC_FILE=${CMAKE_BINARY_DIR}/rudecgi.pc"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/check_pc_relocatable.cmake")
