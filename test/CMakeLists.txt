find_package(Boost COMPONENTS serialization unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable( basic_usage basic_usage.cxx )
target_link_libraries(basic_usage ${Boost_LIBRARIES} )

add_executable( nested_structs nested_structs.cxx )
target_link_libraries( nested_structs ${Boost_LIBRARIES} )

add_executable( storage_conversion storage_conversion.cxx )
target_link_libraries( storage_conversion ${Boost_LIBRARIES} )

add_executable( basic_structs basic_structs.cxx )
target_link_libraries( basic_structs ${Boost_LIBRARIES} )

add_executable( basic_union basic_union.cxx )
target_link_libraries( basic_union ${Boost_LIBRARIES} )

add_executable( union_visitor union_visitor.cxx )
target_link_libraries( union_visitor ${Boost_LIBRARIES} )

