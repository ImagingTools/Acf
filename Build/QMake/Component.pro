TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += BasePck
SUBDIRS += QtPck
SUBDIRS += FilePck
SUBDIRS += QtGuiPck
SUBDIRS += QtViewPck
SUBDIRS += CompositorPck

BasePck.file = ../../Impl/BasePck/QMake/BasePck.pro
QtPck.file = ../../Impl/QtPck/QMake/QtPck.pro
FilePck.file = ../../Impl/FilePck/QMake/FilePck.pro
QtGuiPck.file = ../../Impl/QtGuiPck/QMake/QtGuiPck.pro
QtViewPck.file = ../../Impl/QtViewPck/QMake/QtViewPck.pro
CompositorPck.file = ../../Impl/CompositorPck/QMake/CompositorPck.pro
