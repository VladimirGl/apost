cmake_minimum_required(VERSION 2.8)

project(apost)

SET(CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Build examples
add_executable(ex_apost examples/ex_apost_arith.cpp)
target_link_libraries(ex_apost flint)

add_executable(ex_det examples/ex_det.cpp)
target_link_libraries(ex_det flint)

add_executable(ex_leq examples/ex_leq.cpp)
target_link_libraries(ex_leq flint)


# Build benchmarks
add_executable(b_det benchmark/b_det.cpp)
target_link_libraries(b_det flint)

add_executable(b_det_time benchmark/b_det_time.cpp)
target_link_libraries(b_det_time flint)

add_executable(b_leq_time benchmark/b_leq_time.cpp)
target_link_libraries(b_leq_time flint)

# Install apost library
SET(HEADERS
    apost.h
    interval.h
    value.h
    precision.h)
    
add_library(apost STATIC ${HEADERS})
set_target_properties(apost PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS apost DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)

