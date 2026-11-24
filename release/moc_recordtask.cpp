/****************************************************************************
** Meta object code from reading C++ file 'recordtask.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../service/recordtask.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recordtask.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10RecordTaskE_t {};
} // unnamed namespace

template <> constexpr inline auto RecordTask::qt_create_metaobjectdata<qt_meta_tag_ZN10RecordTaskE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "RecordTask",
        "detectRecordFinish",
        "",
        "channelId",
        "sigNeedFlush",
        "sigCaptureSnapShot",
        "Mat",
        "frame",
        "channel_id",
        "type",
        "ch",
        "videoName",
        "offsetSec",
        "saveVideo",
        "mat"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'detectRecordFinish'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'sigNeedFlush'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sigCaptureSnapShot'
        QtMocHelpers::SignalData<void(Mat, int, int, QString, QString, int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 }, { QMetaType::Int, 8 }, { QMetaType::Int, 9 }, { QMetaType::QString, 10 },
            { QMetaType::QString, 11 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'saveVideo'
        QtMocHelpers::SlotData<void(Mat, int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 14 }, { QMetaType::Int, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RecordTask, qt_meta_tag_ZN10RecordTaskE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject RecordTask::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10RecordTaskE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10RecordTaskE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10RecordTaskE_t>.metaTypes,
    nullptr
} };

void RecordTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RecordTask *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->detectRecordFinish((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->sigNeedFlush(); break;
        case 2: _t->sigCaptureSnapShot((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[6]))); break;
        case 3: _t->saveVideo((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (RecordTask::*)(int )>(_a, &RecordTask::detectRecordFinish, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (RecordTask::*)()>(_a, &RecordTask::sigNeedFlush, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (RecordTask::*)(Mat , int , int , QString , QString , int )>(_a, &RecordTask::sigCaptureSnapShot, 2))
            return;
    }
}

const QMetaObject *RecordTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecordTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10RecordTaskE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RecordTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void RecordTask::detectRecordFinish(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void RecordTask::sigNeedFlush()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RecordTask::sigCaptureSnapShot(Mat _t1, int _t2, int _t3, QString _t4, QString _t5, int _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}
QT_WARNING_POP
