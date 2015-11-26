SET(TEST_SOURCES
	test.cc
    AsyncWrapper.cc
    Optional.cc
    Any.cc
    Variant.cc
    function_traits.cc
)

INCLUDE_DIRECTORIES(../inc)
ADD_EXECUTABLE(zbase_test ${TEST_SOURCES})
TARGET_LINK_LIBRARIES(zbase_test)
ADD_CUSTOM_TARGET(run_test COMMAND ${CMAKE_BINARY_DIR}/test/zbase_test DEPENDS zbase_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
