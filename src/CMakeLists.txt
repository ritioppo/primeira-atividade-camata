
file(GLOB source_files *.cc *.hh)
add_library(siga ${source_files})
target_include_directories(siga PUBLIC ${PROJECT_SOURCE_DIR}/src)

set_tests_properties(test2 PROPERTIES DEPENDS "test1")
set_tests_properties(test3 PROPERTIES DEPENDS "test2")
set_tests_properties(test4 PROPERTIES DEPENDS "test3")
set_tests_properties(test5 PROPERTIES DEPENDS "test4")
set_tests_properties(test6 PROPERTIES DEPENDS "test5")