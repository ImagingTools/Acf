TEMPLATE = subdirs

SUBDIRS += ../AcfQt/QMake/AcfQt.pro
SUBDIRS += ../AcfStd/QMake/AcfStd.pro
win32{
	SUBDIRS += ../AcfWin/QMake/AcfWin.pro
}
