/****************************************************************************
** Meta object code from reading C++ file 'cbctrawimageviewer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/cbctrawimageviewer.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cbctrawimageviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_CBCTRawImageViewer_t {
    uint offsetsAndSizes[14];
    char stringdata0[19];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[18];
    char stringdata5[17];
    char stringdata6[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CBCTRawImageViewer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CBCTRawImageViewer_t qt_meta_stringdata_CBCTRawImageViewer = {
    {
        QT_MOC_LITERAL(0, 18),  // "CBCTRawImageViewer"
        QT_MOC_LITERAL(19, 17),  // "signals_panoImage"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 7),  // "QImage*"
        QT_MOC_LITERAL(46, 17),  // "signals_cephImage"
        QT_MOC_LITERAL(64, 16),  // "timeoutPanoTimer"
        QT_MOC_LITERAL(81, 16)   // "timeoutCephTimer"
    },
    "CBCTRawImageViewer",
    "signals_panoImage",
    "",
    "QImage*",
    "signals_cephImage",
    "timeoutPanoTimer",
    "timeoutCephTimer"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CBCTRawImageViewer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,
       4,    1,   41,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   44,    2, 0x08,    5 /* Private */,
       6,    0,   45,    2, 0x08,    6 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject CBCTRawImageViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CBCTRawImageViewer.offsetsAndSizes,
    qt_meta_data_CBCTRawImageViewer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CBCTRawImageViewer_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CBCTRawImageViewer, std::true_type>,
        // method 'signals_panoImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QImage *, std::false_type>,
        // method 'signals_cephImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QImage *, std::false_type>,
        // method 'timeoutPanoTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'timeoutCephTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CBCTRawImageViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CBCTRawImageViewer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signals_panoImage((*reinterpret_cast< std::add_pointer_t<QImage*>>(_a[1]))); break;
        case 1: _t->signals_cephImage((*reinterpret_cast< std::add_pointer_t<QImage*>>(_a[1]))); break;
        case 2: _t->timeoutPanoTimer(); break;
        case 3: _t->timeoutCephTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CBCTRawImageViewer::*)(QImage * );
            if (_t _q_method = &CBCTRawImageViewer::signals_panoImage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CBCTRawImageViewer::*)(QImage * );
            if (_t _q_method = &CBCTRawImageViewer::signals_cephImage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *CBCTRawImageViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBCTRawImageViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CBCTRawImageViewer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CBCTRawImageViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CBCTRawImageViewer::signals_panoImage(QImage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CBCTRawImageViewer::signals_cephImage(QImage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
