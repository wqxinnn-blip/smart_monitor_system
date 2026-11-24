/****************************************************************************
** Meta object code from reading C++ file 'reviewwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../view/reviewwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reviewwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ReviewWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ReviewWidget::qt_create_metaobjectdata<qt_meta_tag_ZN12ReviewWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ReviewWidget",
        "sigStartPlay",
        "",
        "path",
        "sigSaveImg",
        "Mat",
        "showFrame",
        "frame",
        "count",
        "refreshRecordList",
        "updatePage"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sigStartPlay'
        QtMocHelpers::SignalData<void(QString)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'sigSaveImg'
        QtMocHelpers::SignalData<void(Mat, int, int, QString, QString, int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 2 }, { QMetaType::Int, 2 }, { QMetaType::Int, 2 }, { QMetaType::QString, 2 },
            { QMetaType::QString, 2 }, { QMetaType::Int, 2 },
        }}),
        // Slot 'showFrame'
        QtMocHelpers::SlotData<void(Mat, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 7 }, { QMetaType::Int, 8 },
        }}),
        // Slot 'refreshRecordList'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updatePage'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ReviewWidget, qt_meta_tag_ZN12ReviewWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ReviewWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReviewWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReviewWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ReviewWidgetE_t>.metaTypes,
    nullptr
} };

void ReviewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ReviewWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sigStartPlay((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->sigSaveImg((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[6]))); break;
        case 2: _t->showFrame((*reinterpret_cast<std::add_pointer_t<Mat>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->refreshRecordList(); break;
        case 4: _t->updatePage(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ReviewWidget::*)(QString )>(_a, &ReviewWidget::sigStartPlay, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ReviewWidget::*)(Mat , int , int , QString , QString , int )>(_a, &ReviewWidget::sigSaveImg, 1))
            return;
    }
}

const QMetaObject *ReviewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReviewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ReviewWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ReviewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ReviewWidget::sigStartPlay(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ReviewWidget::sigSaveImg(Mat _t1, int _t2, int _t3, QString _t4, QString _t5, int _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}
QT_WARNING_POP
