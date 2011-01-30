#-------------------------------------------------
#
# Project created by QtCreator 2010-11-25T09:34:52
#
#-------------------------------------------------

QT       += core gui sql

TARGET = database_search_engine
TEMPLATE = app

CONFIG += qt debug warn_on

SOURCES += main.cpp\
        frmMainWindow.cpp \
    frmEscolheBancoDeDados.cpp \
    buscaTipoDado/buscaTipoDadoString.cpp \
    buscaTipoDado/buscaTipoDadoDataHora.cpp \
    buscaTipoDado/buscaTipoDadoDataHoraSeg.cpp \
    buscaTipoDado/buscaTipoDadoData.cpp \
    buscaTipoDado/buscaTipoDadoExpReg.cpp \
    buscaTipoDado/buscaTipoDadoHora.cpp \
    buscaTipoDado/buscaTipoDadoHoraSeg.cpp \
    buscaTipoDado/buscaTipoDadoNumero.cpp \
    buscaTipoDado/buscaTipoDadoSeleciona.cpp \
    buscaTipoDado/buscaTipoDadoTxt.cpp \
    frmUmaTabela.cpp

HEADERS  += frmMainWindow.h \
    frmEscolheBancoDeDados.h \
    buscaTipoDado/buscaTipoDado.h \
    buscaTipoDado/buscaTipoDadoString.h \
    buscaTipoDado/buscaTipoDadoDataHora.h \
    buscaTipoDado/buscaTipoDadoDataHoraSeg.h \
    buscaTipoDado/buscaTipoDadoData.h \
    buscaTipoDado/buscaTipoDadoExpReg.h \
    buscaTipoDado/buscaTipoDadoHora.h \
    buscaTipoDado/buscaTipoDadoHoraSeg.h \
    buscaTipoDado/buscaTipoDadoNumero.h \
    buscaTipoDado/buscaTipoDadoSeleciona.h \
    buscaTipoDado/buscaTipoDadoTxt.h \
    frmUmaTabela.h

FORMS    += frmMainWindow.ui \
    frmEscolheBancoDeDados.ui \
    frmUmaTabela.ui

OTHER_FILES += \
    BUILDING.txt
