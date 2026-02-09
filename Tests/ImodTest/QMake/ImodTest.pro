# SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
include($$(ACFDIR)/Config/QMake/AcfQt.pri)
include($$(ACFDIR)/Config/QMake/AcfStd.pri)
include($$(ACFDIR)/Config/QMake/AcfStdGui.pri)

TEMPLATE = app
TARGET = ImodTest
CONFIG += testcase
QT += testlib

LIBS += -limod -liser -litest

SOURCES += \
	../main.cpp \
	../CModelBaseTest.cpp \
	../CSingleModelObserverBaseTest.cpp \
	../CMultiModelObserverBaseTest.cpp \
	../TModelWrapTest.cpp \
	../CModelProxyTest.cpp \
	../CModelUpdateBridgeTest.cpp \
	../CMultiModelDispatcherBaseTest.cpp

HEADERS += \
	../CModelBaseTest.h \
	../CSingleModelObserverBaseTest.h \
	../CMultiModelObserverBaseTest.h \
	../TModelWrapTest.h \
	../CModelProxyTest.h \
	../CModelUpdateBridgeTest.h \
	../CMultiModelDispatcherBaseTest.h
