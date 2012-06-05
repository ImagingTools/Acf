TEMPLATE = subdirs
CONFIG += ordered
CONFIG += debug_and_release

SUBDIRS += Static
SUBDIRS += Component
SUBDIRS += Tool

Static.file = Static.pro
Component.file = Component.pro
Component.depends = Static
Tool.file = Tool.pro
Tool.depends = Component
