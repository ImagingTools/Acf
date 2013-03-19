#Root of ACF project

include (../../Config/QMake/GeneralConfig.pri)

TEMPLATE = subdirs
CONFIG += ordered


#Static

SUBDIRS += istd
SUBDIRS += iser
SUBDIRS += i2d
SUBDIRS += iimg
SUBDIRS += imath
SUBDIRS += icmm
SUBDIRS += imod
SUBDIRS += icomp
SUBDIRS += idoc
SUBDIRS += iprm
SUBDIRS += ibase
SUBDIRS += iqt
SUBDIRS += ifile
SUBDIRS += iqt2d
SUBDIRS += iview
SUBDIRS += iqtdoc
SUBDIRS += iqtgui
SUBDIRS += iqtprm
SUBDIRS += icmpstr
SUBDIRS += AcfLoc

istd.file = ../../Include/istd/QMake/istd.pro
iser.file = ../../Include/iser/QMake/iser.pro
i2d.file = ../../Include/i2d/QMake/i2d.pro
iimg.file = ../../Include/iimg/QMake/iimg.pro
imath.file = ../../Include/imath/QMake/imath.pro
icmm.file = ../../Include/icmm/QMake/icmm.pro
imod.file = ../../Include/imod/QMake/imod.pro
icomp.file = ../../Include/icomp/QMake/icomp.pro
idoc.file = ../../Include/idoc/QMake/idoc.pro
iprm.file = ../../Include/iprm/QMake/iprm.pro
ibase.file = ../../Include/ibase/QMake/ibase.pro
iqt.file = ../../Include/iqt/QMake/iqt.pro
ifile.file = ../../Include/ifile/QMake/ifile.pro
iqt2d.file = ../../Include/iqt2d/QMake/iqt2d.pro
iview.file = ../../Include/iview/QMake/iview.pro
iqtdoc.file = ../../Include/iqtdoc/QMake/iqtdoc.pro
iqtgui.file = ../../Include/iqtgui/QMake/iqtgui.pro
iqtprm.file = ../../Include/iqtprm/QMake/iqtprm.pro
icmpstr.file = ../../Include/icmpstr/QMake/icmpstr.pro
AcfLoc.file = ../../Impl/AcfLoc/QMake/AcfLoc.pro


#Component

SUBDIRS += BasePck
SUBDIRS += BitmapPck
SUBDIRS += QtPck
SUBDIRS += FilePck
SUBDIRS += QtGuiPck
SUBDIRS += QtViewPck
SUBDIRS += CompositorPck

BasePck.file = ../../Impl/BasePck/QMake/BasePck.pro
BitmapPck.file = ../../Impl/BitmapPck/QMake/BitmapPck.pro
QtPck.file = ../../Impl/QtPck/QMake/QtPck.pro
FilePck.file = ../../Impl/FilePck/QMake/FilePck.pro
QtGuiPck.file = ../../Impl/QtGuiPck/QMake/QtGuiPck.pro
QtViewPck.file = ../../Impl/QtViewPck/QMake/QtViewPck.pro
CompositorPck.file = ../../Impl/CompositorPck/QMake/CompositorPck.pro


#Tool

SUBDIRS += Acf
SUBDIRS += Arxc
SUBDIRS += CompositorMini
SUBDIRS += Compositor
SUBDIRS += XpcEditor

Acf.file = ../../Impl/AcfExe/QMake/Acf.pro
Arxc.file = ../../Impl/ArxcExe/QMake/Arxc.pro
Arxc.depends = Acf
CompositorMini.file = ../../Impl/CompositorMiniExe/QMake/CompositorMini.pro
CompositorMini.depends = Acf
Compositor.file = ../../Impl/CompositorExe/QMake/Compositor.pro
Compositor.depends = Arxc
XpcEditor.file = ../../Impl/XpcEditorExe/QMake/XpcEditor.pro
XpcEditor.depends = Arxc
