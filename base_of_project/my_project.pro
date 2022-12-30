QT       += core gui opengl charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
mac: LIBS += -framework GLUT
else:unix|win32: LIBS += -lGLUT

#LIBS += -lglut -lGLU #
#LIBS += -lopengl32 -lglu32
CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algoritmi_sa_vezbi/ga03_konveksniomotac.cpp \
    max_circle_inside_nonconvex_polygon/max_circle_inside_nonconvex_polygon.cpp \
    algoritambaza.cpp \
    animacijanit.cpp \
    main.cpp \
    mainwindow.cpp \
    oblastcrtanja.cpp \
    oblastcrtanjaopengl.cpp \
    pomocnefunkcije.cpp \
    timemeasurementthread.cpp

HEADERS += \
    algoritmi_sa_vezbi/ga03_konveksniomotac.h \
    max_circle_inside_nonconvex_polygon/max_circle_inside_nonconvex_polygon.h \
    algoritambaza.h \
    animacijanit.h \
    config.h \
    mainwindow.h \
    oblastcrtanja.h \
    oblastcrtanjaopengl.h \
    pomocnefunkcije.h \
    timemeasurementthread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    input_files/max_circle_inside_nonconvex_polygon/input1.txt \
    input_files/max_circle_inside_nonconvex_polygon/input2.txt \
    docs/ga06_presekPravougaonika.docx \
    docs/ga06_presekPravougaonika.pdf \
    input_files/ga00_DCELDemo/mushroom.off \
    input_files/ga00_DCELDemo/test0.off \
    input_files/ga00_KonveksniOmotac3D/input1.txt \
    input_files/ga06_presekPravougaonika/input1.txt \
    input_files/ga06_presekPravougaonika/input2.txt \
    input_files/ga06_presekPravougaonika/input3.txt
