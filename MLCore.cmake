project(MLCore)

get_filename_component(MLCoreDir ${CMAKE_CURRENT_LIST_FILE} PATH)

include(${MLCoreDir}/MLCore/MLCoreLib.cmake)

if (${ML_TESTS})
    include(${MLCoreDir}/Tests/MLCoreTests.cmake)
endif ()

if (${ML_BENCHMARKS})
    include(${MLCoreDir}/Benchmarks/MLCoreBenchmarks.cmake)
endif ()