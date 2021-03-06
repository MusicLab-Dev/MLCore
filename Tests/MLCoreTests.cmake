project(MLCoreTests)

set(MLCoreTestsDir ${MLCoreDir}/Tests)

set(MLCoreTestsSources
    ${MLCoreTestsDir}/tests_Vector.cpp
    ${MLCoreTestsDir}/tests_FlatVector.cpp
    ${MLCoreTestsDir}/tests_FlatString.cpp
    ${MLCoreTestsDir}/tests_UniqueAlloc.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${MLCoreTestsSources})

add_test(NAME ${CMAKE_PROJECT_NAME} COMMAND ${CMAKE_PROJECT_NAME})

target_link_libraries(${CMAKE_PROJECT_NAME}
PUBLIC
    MLCoreLib
    GTest::GTest GTest::Main
)