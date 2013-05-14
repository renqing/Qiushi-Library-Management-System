#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T16:26:16
#
#-------------------------------------------------

QT       += core gui sql

TARGET = Library
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login_dialog.cpp \
    user_dialog.cpp \
    book_dialog.cpp \
    bookitem_dialog.cpp \
    returnbook_dialog.cpp \
    usermanagement_dialog.cpp \
    managebook_dialog.cpp

HEADERS  += mainwindow.h \
    login_dialog.h \
    qlms_public.h \
    user_dialog.h \
    book_dialog.h \
    bookitem_dialog.h \
    returnbook_dialog.h \
    usermanagement_dialog.h \
    managebook_dialog.h

FORMS    += mainwindow.ui \
    login_dialog.ui \
    user_dialog.ui \
    book_dialog.ui \
    bookitem_dialog.ui \
    returnbook_dialog.ui \
    usermanagement_dialog.ui \
    managebook_dialog.ui
