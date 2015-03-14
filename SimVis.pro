TEMPLATE = subdirs
SUBDIRS += src examples
CONFIG += ordered

examples.depends = src
