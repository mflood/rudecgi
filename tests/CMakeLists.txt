add_executable(test_get test_get.cpp)
target_link_libraries(test_get PRIVATE rudecgi::rudecgi)
add_test(NAME get COMMAND test_get)

add_executable(test_post test_post.cpp)
target_link_libraries(test_post PRIVATE rudecgi::rudecgi)
add_test(NAME post COMMAND test_post)

add_executable(test_version test_version.cpp)
target_link_libraries(test_version PRIVATE rudecgi::rudecgi)
target_compile_definitions(test_version PRIVATE EXPECTED_VERSION="${PROJECT_VERSION}")
add_test(NAME version COMMAND test_version)

add_executable(test_contract test_contract.cpp)
target_link_libraries(test_contract PRIVATE rudecgi::rudecgi)
add_test(NAME contract COMMAND test_contract)

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

add_executable(test_precedence test_precedence.cpp)
target_link_libraries(test_precedence PRIVATE rudecgi::rudecgi)
add_test(NAME precedence_get COMMAND test_precedence get)
add_test(NAME precedence_post COMMAND test_precedence post)

add_executable(test_post_read_timeout test_post_read_timeout.cpp)
target_link_libraries(test_post_read_timeout PRIVATE rudecgi::rudecgi)
add_test(NAME post_read_timeout COMMAND test_post_read_timeout)

# Waits out the built-in idle limit, so it takes about a minute by
# construction: a 60-second bound cannot be observed in less than 60 seconds.
# The alternative is not testing that the default terminates at all, which is
# the hang this suite exists to prevent.
add_executable(test_post_read_default test_post_read_default.cpp)
target_link_libraries(test_post_read_default PRIVATE rudecgi::rudecgi)
add_test(NAME post_read_default COMMAND test_post_read_default)
set_tests_properties(post_read_default PROPERTIES TIMEOUT 180)

# Each test runs in its own scratch dir so file fixtures don't collide.
set(ALL_TESTS get post cookie version ${CONTENT_LENGTH_TESTS})
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
