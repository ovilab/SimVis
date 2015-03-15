TEMPLATE = subdirs
SUBDIRS = src examples \
    ../../sandbox/untitled7
CONFIG += ordered
examples.depends = src
