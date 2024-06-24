file(GLOB TEST_SRC "*.cpp")
find_package(GTest REQUIRED)
foreach(test_src ${TEST_SRC})
    # Get the name of the file without an extension
    get_filename_component(test_name ${test_src} NAME_WE)

    # Add executable for this source file
    add_executable(${test_name} ${test_src})

    # Link to the main library and gtest
    target_link_libraries(${test_name} InvertedIndex GTest::GTest GTest::Main)
endforeach()