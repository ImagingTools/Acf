TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += Acf
SUBDIRS += Arxc
SUBDIRS += CompositorMini
SUBDIRS += Compositor

Acf.file = ../AcfExe/QMake/Acf.pro
Arxc.file = ../ArxcExe/QMake/Arxc.pro
Arxc.depends = Acf
CompositorMini.file = ../CompositorMiniExe/QMake/CompositorMini.pro
CompositorMini.depends = Acf
Compositor.file = ../CompositorExe/QMake/Compositor.pro
Compositor.depends = Arxc
