#target_link_directories(${PROJECT_NAME} PUBLIC ${ACFDIR}/Lib/${TARGETNAME})
#target_link_directories(${PROJECT_NAME} PUBLIC ${ACFSLNDIR}/Lib/${TARGETNAME})

target_link_libraries(${PROJECT_NAME} iqt ipackage idoc ifile iimg icmm i2d i3d iprm ilog ibase icomp iattr iser imod imath istd)

