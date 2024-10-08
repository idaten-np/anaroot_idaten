set(lib_name BigRIPSSortLib)
ROOT_GENERATE_DICTIONARY(G__TreeData ${CMAKE_SOURCE_DIR}/include/BigRIPSTreeData.hpp LINKDEF ${CMAKE_SOURCE_DIR}/include/LinkDefUser.hpp MODULE ${lib_name})
add_library(${lib_name} SHARED
    G__TreeData.cxx
)

target_link_libraries(${lib_name} ${ROOT_LIBRARIES})

install(TARGETS ${lib_name} DESTINATION lib)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/libBigRIPSSortLib_rdict.pcm
	${CMAKE_CURRENT_BINARY_DIR}/libBigRIPSSortLib.rootmap
	#${CMAKE_CURRENT_BINARY_DIR}/libTreeData_rdict.pcm
	#${CMAKE_CURRENT_BINARY_DIR}/libTreeData.rootmap
	DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/ DESTINATION include)

function(addExec exec_name)
add_executable(${exec_name} ${exec_name}.cpp)
target_include_directories(${exec_name} PRIVATE ${ANAROOT_DIR}/include ${CMAKE_SOURCE_DIR}/include)
target_link_directories(${exec_name} PRIVATE ${ANAROOT_DIR}/lib)
target_link_libraries(${exec_name}
    ${ROOT_LIBRARIES}
    XMLIO
    XMLParser
    anacore
    analoop
    anabrips
    anaroot
    ${lib_name}
)

install(TARGETS ${exec_name}
    RUNTIME DESTINATION bin
)
endfunction()

addExec(MakeFullBigRIPSTree)
addExec(MakePIDTree)
#addExec(Monitor)

