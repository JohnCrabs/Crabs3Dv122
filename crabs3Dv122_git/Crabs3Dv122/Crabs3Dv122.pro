#-------------------------------------------------
#
# Project created by QtCreator 2018-03-17T11:36:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crabs3Dv122
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    glwidget.cpp \
    systemgl.cpp \
    geomshapes.cpp \
    templates.cpp \
    header_c3d.cpp \
    c3d.cpp \
    parameter_c3d.cpp \
    data_c3d.cpp \
    pointcloud.cpp \
    accumulation_noise_remover.cpp \
    nearest_neighbor_remover.cpp \
    units_dialog.cpp \
    export_cloud_csv.cpp \
    cluster_window.cpp \
    set_parameters_kmeans.cpp \
    crabs_editor.cpp \
    point_list.cpp

HEADERS += \
        mainwindow.h \
    glwidget.h \
    systemgl.h \
    geomshapes.h \
    templates.h \
    header_c3d.h \
    c3d.h \
    parameter_c3d.h \
    data_c3d.h \
    pointcloud.h \
    types.h \
    accumulation_noise_remover.h \
    nearest_neighbor_remover.h \
    units_dialog.h \
    export_cloud_csv.h \
    cluster_window.h \
    set_parameters_kmeans.h \
    crabs_editor.h \
    point_list.h

FORMS += \
        mainwindow.ui \
    accumulation_noise_remover.ui \
    nearest_neighbor_remover.ui \
    units_dialog.ui \
    export_cloud_csv.ui \
    cluster_window.ui \
    set_parameters_kmeans.ui \
    crabs_editor.ui \
    point_list.ui

LIBS += -lGL -lGLU -lglut -lm

RESOURCES += \
    resource.qrc
