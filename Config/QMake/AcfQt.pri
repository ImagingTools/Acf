# For non-shadow build use lib directory inside source tree:
isEmpty(ACFDIRBUILD){
	LIBS += -L$$OUT_PWD/../../../Lib/$$COMPILER_DIR
	LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR
}

# Set lib directory for shadow build:
!isEmpty(ACFDIRBUILD){
	LIBS += -L$$ACFDIRBUILD/Lib/$$COMPILER_DIR
}

LIBS += -liqtdoc -liview -liqt2d -liqtprm -liloggui -lifilegui -liqtgui -liwidgets


