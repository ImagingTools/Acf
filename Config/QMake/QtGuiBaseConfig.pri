# Standard settings for a Qt based UI project

include (QtBaseConfig.pri)

FORMS += ../*.ui

# Check Qt version
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)

# If the version is greater 4, then the UI compiler must be separately activated:
greaterThan(QT_VER_MAJ, 4)){
    QT += widgets
}
