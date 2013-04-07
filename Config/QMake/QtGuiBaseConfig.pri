# Standard settings for a Qt based UI project

include (QtBaseConfig.pri)

FORMS += ../*.ui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
