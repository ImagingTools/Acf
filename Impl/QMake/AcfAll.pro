TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += Static
SUBDIRS += Component
SUBDIRS += Tool

Component.depends = Static
Tool.depends = Component
