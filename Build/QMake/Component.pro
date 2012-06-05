TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += BasePck
SUBDIRS += QtPck
SUBDIRS += QtGuiPck
SUBDIRS += QtViewPck
SUBDIRS += CompositorPck
SUBDIRS += WizardPck
SUBDIRS += QtWizardPck

BasePck.file = ../../Impl/BasePck/QMake/BasePck.pro
QtPck.file = ../../Impl/QtPck/QMake/QtPck.pro
QtGuiPck.file = ../../Impl/QtGuiPck/QMake/QtGuiPck.pro
QtViewPck.file = ../../Impl/QtViewPck/QMake/QtViewPck.pro
CompositorPck.file = ../../Impl/CompositorPck/QMake/CompositorPck.pro
WizardPck.file = ../../Impl/WizardPck/QMake/WizardPck.pro
QtWizardPck.file = ../../Impl/QtWizardPck/QMake/QtWizardPck.pro
