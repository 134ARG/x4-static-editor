TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/file_process/bnode.cpp \
        src/file_process/btree.cpp \
        src/file_process/file_reader.cpp \
        src/file_process/main.cpp \
        src/file_process/vfile.cpp

HEADERS += \
    src/file_process/bnode.h \
    src/file_process/btree.h \
    src/file_process/file_reader.h \
    src/file_process/vfile.h

DISTFILES += \
    LICENSE \
    README.md \
    test_samples/01.cat \
    test_samples/test.cat
