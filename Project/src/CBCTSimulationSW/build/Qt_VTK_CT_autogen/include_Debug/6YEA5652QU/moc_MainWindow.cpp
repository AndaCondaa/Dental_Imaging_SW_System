/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/mainwindow.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_MainWindow_t {
    uint offsetsAndSizes[38];
    char stringdata0[11];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[12];
    char stringdata5[12];
    char stringdata6[11];
    char stringdata7[34];
    char stringdata8[33];
    char stringdata9[27];
    char stringdata10[27];
    char stringdata11[27];
    char stringdata12[26];
    char stringdata13[15];
    char stringdata14[8];
    char stringdata15[5];
    char stringdata16[15];
    char stringdata17[5];
    char stringdata18[28];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 11),  // "READYSignal"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 11),  // "ControlType"
        QT_MOC_LITERAL(36, 11),  // "RESETSignal"
        QT_MOC_LITERAL(48, 11),  // "STARTSignal"
        QT_MOC_LITERAL(60, 10),  // "STOPSignal"
        QT_MOC_LITERAL(71, 33),  // "on_CaptureStartPushButton_cli..."
        QT_MOC_LITERAL(105, 32),  // "on_CaptureStopPushButton_clicked"
        QT_MOC_LITERAL(138, 26),  // "on_READYPushButton_clicked"
        QT_MOC_LITERAL(165, 26),  // "on_RESETPushButton_clicked"
        QT_MOC_LITERAL(192, 26),  // "on_STARTPushButton_clicked"
        QT_MOC_LITERAL(219, 25),  // "on_STOPPushButton_clicked"
        QT_MOC_LITERAL(245, 14),  // "slot_panoImage"
        QT_MOC_LITERAL(260, 7),  // "QImage*"
        QT_MOC_LITERAL(268, 4),  // "pImg"
        QT_MOC_LITERAL(273, 14),  // "slot_cephImage"
        QT_MOC_LITERAL(288, 4),  // "cImg"
        QT_MOC_LITERAL(293, 27)   // "on_openGLWidget_All_resized"
    },
    "MainWindow",
    "READYSignal",
    "",
    "ControlType",
    "RESETSignal",
    "STARTSignal",
    "STOPSignal",
    "on_CaptureStartPushButton_clicked",
    "on_CaptureStopPushButton_clicked",
    "on_READYPushButton_clicked",
    "on_RESETPushButton_clicked",
    "on_STARTPushButton_clicked",
    "on_STOPPushButton_clicked",
    "slot_panoImage",
    "QImage*",
    "pImg",
    "slot_cephImage",
    "cImg",
    "on_openGLWidget_All_resized"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   92,    2, 0x06,    1 /* Public */,
       4,    1,   95,    2, 0x06,    3 /* Public */,
       5,    1,   98,    2, 0x06,    5 /* Public */,
       6,    1,  101,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,  104,    2, 0x08,    9 /* Private */,
       8,    0,  105,    2, 0x08,   10 /* Private */,
       9,    0,  106,    2, 0x08,   11 /* Private */,
      10,    0,  107,    2, 0x08,   12 /* Private */,
      11,    0,  108,    2, 0x08,   13 /* Private */,
      12,    0,  109,    2, 0x08,   14 /* Private */,
      13,    1,  110,    2, 0x08,   15 /* Private */,
      16,    1,  113,    2, 0x08,   17 /* Private */,
      18,    0,  116,    2, 0x08,   19 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   17,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSizes,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'READYSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ControlType, std::false_type>,
        // method 'RESETSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ControlType, std::false_type>,
        // method 'STARTSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ControlType, std::false_type>,
        // method 'STOPSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ControlType, std::false_type>,
        // method 'on_CaptureStartPushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_CaptureStopPushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_READYPushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_RESETPushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_STARTPushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_STOPPushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_panoImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QImage *, std::false_type>,
        // method 'slot_cephImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QImage *, std::false_type>,
        // method 'on_openGLWidget_All_resized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->READYSignal((*reinterpret_cast< std::add_pointer_t<ControlType>>(_a[1]))); break;
        case 1: _t->RESETSignal((*reinterpret_cast< std::add_pointer_t<ControlType>>(_a[1]))); break;
        case 2: _t->STARTSignal((*reinterpret_cast< std::add_pointer_t<ControlType>>(_a[1]))); break;
        case 3: _t->STOPSignal((*reinterpret_cast< std::add_pointer_t<ControlType>>(_a[1]))); break;
        case 4: _t->on_CaptureStartPushButton_clicked(); break;
        case 5: _t->on_CaptureStopPushButton_clicked(); break;
        case 6: _t->on_READYPushButton_clicked(); break;
        case 7: _t->on_RESETPushButton_clicked(); break;
        case 8: _t->on_STARTPushButton_clicked(); break;
        case 9: _t->on_STOPPushButton_clicked(); break;
        case 10: _t->slot_panoImage((*reinterpret_cast< std::add_pointer_t<QImage*>>(_a[1]))); break;
        case 11: _t->slot_cephImage((*reinterpret_cast< std::add_pointer_t<QImage*>>(_a[1]))); break;
        case 12: _t->on_openGLWidget_All_resized(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(ControlType );
            if (_t _q_method = &MainWindow::READYSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(ControlType );
            if (_t _q_method = &MainWindow::RESETSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(ControlType );
            if (_t _q_method = &MainWindow::STARTSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(ControlType );
            if (_t _q_method = &MainWindow::STOPSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::READYSignal(ControlType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::RESETSignal(ControlType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::STARTSignal(ControlType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::STOPSignal(ControlType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
