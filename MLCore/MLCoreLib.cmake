project(MLCoreLib)

get_filename_component(MLCoreLibDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(MLCoreLibSources
    ${MLCoreLibDir}/Utils.hpp
    ${MLCoreLibDir}/FlatVector.hpp
    ${MLCoreLibDir}/FlatVector.ipp
    ${MLCoreLibDir}/FlatString.hpp
    ${MLCoreLibDir}/FlatString.ipp
    ${MLCoreLibDir}/Core.cpp
)

add_library(${PROJECT_NAME} ${MLCoreLibSources})

target_include_directories(${PROJECT_NAME} PUBLIC ${MLCoreDir})

# target_link_libraries(${PROJECT_NAME}
# PUBLIC
# )
