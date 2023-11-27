QT       += core gui network core-private core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SettingsUI.cpp \
    main.cpp \
    BackuperUI.cpp

HEADERS += \
    BackuperUI.hpp \
    SettingsUI.hpp

FORMS += \
    BackuperUI.ui \
    SettingsUI.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
android: include(D:/AndroidSDK/android_openssl/openssl.pri)

DISTFILES +=
