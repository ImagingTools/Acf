#General ACF settings

win32{
        ARX_COMPILER = Arxc.exe
}
else{
        ARX_COMPILER = Arxc
}

# custom build for Subversion Transformations
win32{
        svnTransformation.name = SvnTransf
        svnTransformation.CONFIG += target_predeps
        svnTransformation.output = ${QMAKE_FILE_IN_PATH}/Generated/${QMAKE_FILE_BASE}
        svnTransformation.commands = SubWCRev.exe $${SVNTR_REPOSITORY} ${QMAKE_FILE_NAME} $${SVNTR_DESTINATION}/${QMAKE_FILE_BASE}
        svnTransformation.depends = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER
        svnTransformation.input = SVNTR_FILES
        QMAKE_EXTRA_COMPILERS += svnTransformation
}

# custom build for ACF Registry Compiler (Arxc)
arxCompiler.name = ACF-Components
arxCompiler.CONFIG += target_predeps
arxCompiler.output = ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.cpp
arxCompiler.commands = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER ${QMAKE_FILE_IN} -o ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.cpp -config $${ARXC_CONFIG}
arxCompiler.depends = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER
arxCompiler.input = ARXC_FILES
arxCompiler.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += arxCompiler

