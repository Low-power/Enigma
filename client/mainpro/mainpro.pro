TEMPLATE = app
#CONFIG += console
CONFIG -= qt
SOURCES += \
    socket.c \
    service.c \
    main.c \
    model.c \
    #queue_dataitem.c \
    xqueue.c \
    batch.c \
    systeminfo.c \
    postman.c \
    http.c \
    base.c \
    transfile.c

HEADERS += \
    service.h \
    socket.h \
    common.h \
    model.h \
    #queue_dataitem.h \
    xqueue.h \
    batch.h \
    systeminfo.h \
    postman.h \
    http.h \
    transfile.h

LIBS += -luserenv
LIBS += -lws2_32
LIBS +=-lnetapi32
LIBS += -lwtsapi32
RC_FILE += init.rc
