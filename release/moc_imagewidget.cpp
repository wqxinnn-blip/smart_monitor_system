/****************************************************************************
** Meta object code from reading C++ file 'imagewidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/imagewidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagewidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11ImageWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ImageWidget::qt_create_metaobjectdata<qt_meta_tag_ZN11ImageWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ImageWidget",
        "sigSeekAndPlay",
        "",
        "ms",
        "sigStopPlay",
        "refreshImg",
        "onCheckTime",
        "labOperation",
        "updateFrame",
        "Mat"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sigSeekAndPlay'
        QtMocHelpers::SignalData<void(QString, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 }, { QMetaType::Int, 3 },
        }}),
        // Signal 'sigStopPlay'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshImg'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onCheckTime'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Slot 'labOperation'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateFrame'
        QtMocHelpers::SlotData<void(Mat, int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 2 }, { QMetaType::Int, 2 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ImageWidget, qt_meta_tag_ZN11ImageWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ImageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ImageWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ImageWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11ImageWidgetE_t>.metaTypes,
    nullptr
} };

void ImageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ImageWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sigSeekAndPlay((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->sigStopPlay(); break;
        case 2: _t->refreshImg(); break;
        case 3: _t->onCheckTime((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->labOperation(); break;
        case 5: _t->updateFrame((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ImageWidget::*)(QString , int )>(_a, &ImageWidget::sigSeekAndPlay, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ImageWidget::*)()>(_a, &ImageWidget::sigStopPlay, 1))
            return;
    }
}

const QMetaObject *ImageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ImageWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ImageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ImageWidget::sigSeekAndPlay(QString _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void ImageWidget::sigStopPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
