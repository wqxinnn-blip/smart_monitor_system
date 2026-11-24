/****************************************************************************
** Meta object code from reading C++ file 'videotask.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../service/videotask.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videotask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN9VideoTaskE_t {};
} // unnamed namespace

template <> constexpr inline auto VideoTask::qt_create_metaobjectdata<qt_meta_tag_ZN9VideoTaskE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "VideoTask",
        "sigVideoFrame",
        "",
        "Mat",
        "finishVideo",
        "sigTotalFrames",
        "sigTotalToImg",
        "sigSendShotFrame",
        "sigSendShotFrameAndTime",
        "getSnapFrame",
        "seekToMsAndPlay",
        "ms"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sigVideoFrame'
        QtMocHelpers::SignalData<void(Mat, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 2 }, { QMetaType::Int, 2 },
        }}),
        // Signal 'finishVideo'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sigTotalFrames'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Signal 'sigTotalToImg'
        QtMocHelpers::SignalData<void(int, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 }, { QMetaType::Int, 2 },
        }}),
        // Signal 'sigSendShotFrame'
        QtMocHelpers::SignalData<void(Mat)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 2 },
        }}),
        // Signal 'sigSendShotFrameAndTime'
        QtMocHelpers::SignalData<void(Mat, int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 2 }, { QMetaType::Int, 2 },
        }}),
        // Slot 'getSnapFrame'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'seekToMsAndPlay'
        QtMocHelpers::SlotData<void(QString, int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 }, { QMetaType::Int, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<VideoTask, qt_meta_tag_ZN9VideoTaskE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject VideoTask::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9VideoTaskE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9VideoTaskE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9VideoTaskE_t>.metaTypes,
    nullptr
} };

void VideoTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<VideoTask *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sigVideoFrame((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->finishVideo(); break;
        case 2: _t->sigTotalFrames((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->sigTotalToImg((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->sigSendShotFrame((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1]))); break;
        case 5: _t->sigSendShotFrameAndTime((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->getSnapFrame(); break;
        case 7: _t->seekToMsAndPlay((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (VideoTask::*)(Mat , int )>(_a, &VideoTask::sigVideoFrame, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTask::*)()>(_a, &VideoTask::finishVideo, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTask::*)(int )>(_a, &VideoTask::sigTotalFrames, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTask::*)(int , int )>(_a, &VideoTask::sigTotalToImg, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTask::*)(Mat )>(_a, &VideoTask::sigSendShotFrame, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTask::*)(Mat , int )>(_a, &VideoTask::sigSendShotFrameAndTime, 5))
            return;
    }
}

const QMetaObject *VideoTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9VideoTaskE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void VideoTask::sigVideoFrame(Mat _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void VideoTask::finishVideo()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VideoTask::sigTotalFrames(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void VideoTask::sigTotalToImg(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void VideoTask::sigSendShotFrame(Mat _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void VideoTask::sigSendShotFrameAndTime(Mat _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}
QT_WARNING_POP
