set(LIBS pthread kc)

file(GLOB_RECURSE SRC *.cpp)
list(REMOVE_ITEM SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(${PROJECT_NAME}_lib STATIC ${SRC})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${LIBS})
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY CXX_STANDARD 20)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib ${LIBS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)