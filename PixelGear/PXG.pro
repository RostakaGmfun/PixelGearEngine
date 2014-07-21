TEMPLATE = lib static
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../include
INCLUDEPATH += ../glm
INCLUDEPATH += /usr/local/include/bullet
INCLUDEPATH += /usr/include/freetype2

CONFIG += c++11
CONFIG += staticlib

SOURCES += \
    ../src/pxgMaterial.cpp \
    ../src/pxgTexture.cpp \
    ../src/pxgTerrain.cpp \
    ../src/pxgShaderLib.cpp \
    ../src/pxgShader.cpp \
    ../src/pxgScene.cpp \
    ../src/pxgRigidBody.cpp \
    ../src/pxgPhysics.cpp \
    ../src/pxgObject.cpp \
    ../src/pxgNode.cpp \
    ../src/pxgMeshLib.cpp \
    ../src/pxgLight.cpp \
    ../src/pxgHUDObject.cpp \
    ../src/pxgHUDManager.cpp \
    ../src/pxgCamera.cpp \
    ../src/pxgBuffer.cpp \
    ../src/pxgAnimation.cpp \
    ../src/PXG.cpp \
    ../src/pxgTexturePool.cpp \
    ../src/pxgInstanceGroup.cpp \
    ../src/pxgCollisionShape.cpp \
    ../src/pxgTools.cpp \
    ../src/pxgWorld.cpp


HEADERS += \
    ../include/pxgMaterial.h \
    ../include/pxgWorld.h \
    ../include/pxgVertexLayouts.h \
    ../include/pxgUniforms.h \
    ../include/pxgTexture.h \
    ../include/pxgTerrain.h \
    ../include/pxgShaderLib.h \
    ../include/pxgShader.h \
    ../include/pxgScene.h \
    ../include/pxgRigidBody.h \
    ../include/pxgPhysics.h \
    ../include/pxgObject.h \
    ../include/pxgNode.h \
    ../include/pxgMeshLib.h \
    ../include/pxgLight.h \
    ../include/pxgKeyframe.h \
    ../include/pxgHUDObject.h \
    ../include/pxgHUDManager.h \
    ../include/pxgCollisionShape.h \
    ../include/pxgCamera.h \
    ../include/pxgBuffer.h \
    ../include/pxgApp.h \
    ../include/pxgAnimation.h \
    ../include/PXG.h \
    ../include/pxgCallback.h \
    ../include/pxgTexturePool.h \
    ../include/pxgInstanceGroup.h \
    ../include/pxgTools.h \
    ../include/pxgLogicalObject.h


unix:!macx: LIBS += -L$$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletFileLoader/ -lBulletFileLoader

INCLUDEPATH += $$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletFileLoader
DEPENDPATH += $$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletFileLoader

unix:!macx: PRE_TARGETDEPS += $$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletFileLoader/libBulletFileLoader.a

unix:!macx: LIBS += -L$$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletWorldImporter/ -lBulletWorldImporter

INCLUDEPATH += $$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletWorldImporter
DEPENDPATH += $$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletWorldImporter

unix:!macx: PRE_TARGETDEPS += $$PWD/../../bullet-2.82-r2704/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a

OTHER_FILES += \
    ../README \
    ../LICENCE
