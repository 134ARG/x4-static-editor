TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        bnode.cpp \
        btree.cpp \
        file_reader.cpp \
        main.cpp \
        vfile.cpp

HEADERS += \
    bnode.h \
    btree.h \
    file_reader.h \
    vfile.h
