include(NewExecutable)

function(OPENMEEG_TEST TEST_NAME PROG)
    unset(DEFAULT)
    unset(DEPENDS)
    PARSE_ARGUMENTS("DEPENDS" "" "DEFAULT" ${ARGN})
    set(TEST_PROG ${PROG})
    if (NOT IS_ABSOLUTE ${PROG})
        set(TEST_PROG ${CMAKE_CURRENT_BINARY_DIR}/${PROG})
    endif()
    add_test(${TEST_NAME} ${TEST_PROG} ${DEFAULT})
    foreach (DEPENDENCY ${DEPENDS})
        set_property(TEST ${TEST_NAME} APPEND PROPERTY DEPENDS ${DEPENDENCY})
    endforeach()
endfunction()

function(OPENMEEG_UNIT_TEST TEST_NAME)
    PARSE_ARGUMENTS("SOURCES;LIBRARIES;PARAMETERS;DEPENDS" "" "DEFAULT" ${ARGN})
    NEW_EXECUTABLE(${TEST_NAME} SOURCES ${SOURCES} LIBRARIES ${LIBRARIES})

    set(TEST_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}")
    if (WIN32)
        set(TEST_COMMAND "${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME}")
    endif()
    if (DEPENDS)
        set(DEPENDS DEPENDS ${DEPENDS})
    endif()
    OPENMEEG_TEST(${TEST_NAME} ${TEST_COMMAND} ${PARAMETERS} ${DEPENDS})
endfunction()
