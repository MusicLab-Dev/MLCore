project(MLCoreBenchmarks)

set(MLCoreBenchmarksDir ${MLCoreDir}/Benchmarks)

set(MLCoreBenchmarksSources
    ${MLCoreBenchmarksDir}/Main.cpp
    ${MLCoreBenchmarksDir}/bench_SafeQueue.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${MLCoreBenchmarksSources})

target_link_libraries(${CMAKE_PROJECT_NAME}
PUBLIC
    MLCoreLib
    benchmark::benchmark
)