 #-------------------------------------------------
#
# Project created by QtCreator 2013-10-23T15:28:53
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Edge
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

# LIBS += -L/opt/opencv-2.4.6.1/release/lib
unix:!macx: LIBS += -lopencv_core -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres #-lopencv_ts
