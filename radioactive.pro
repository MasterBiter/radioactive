TEMPLATE = app

QT += qml quick widgets printsupport sql

SOURCES += main.cpp \
    radioactivity.cpp \
    radioactive.cpp \
    qcustomplot.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    radioactivity.h \
    int_256.h \
    ttmath.h \
    ttmathbig.h \
    ttmathdec.h \
    ttmathint.h \
    ttmathmisc.h \
    ttmathobjects.h \
    ttmathparser.h \
    ttmaththreads.h \
    ttmathtypes.h \
    ttmathuint.h \
    ttmathuint_noasm.h \
    ttmathuint_x86.h \
    ttmathuint_x86_64.h \
    radioactive.h \
    qcustomplot.h \
    connectdb.h

FORMS += \
    radioactive.ui

OTHER_FILES +=
