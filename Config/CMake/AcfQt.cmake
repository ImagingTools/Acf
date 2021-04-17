#target_link_directories(${PROJECT_NAME} PUBLIC ${ACFDIR}/Lib/${TARGETNAME})
#target_link_directories(${PROJECT_NAME} PUBLIC ${ACFSLNDIR}/Lib/${TARGETNAME})

target_link_libraries(${PROJECT_NAME} iqtdoc iview iqt2d iqtprm iloggui ifilegui iqtgui iwidgets)

