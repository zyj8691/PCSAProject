/****************************************************************************
** Meta object code from reading C++ file 'displaygui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../displaygui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'displaygui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DisplayGUI_t {
    QByteArrayData data[41];
    char stringdata0[688];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DisplayGUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DisplayGUI_t qt_meta_stringdata_DisplayGUI = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DisplayGUI"
QT_MOC_LITERAL(1, 11, 12), // "statusUpdate"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 25), // "sendProcessingInformation"
QT_MOC_LITERAL(4, 51, 3), // "num"
QT_MOC_LITERAL(5, 55, 7), // "HObject"
QT_MOC_LITERAL(6, 63, 15), // "inputImageFirst"
QT_MOC_LITERAL(7, 79, 16), // "inputImageSecond"
QT_MOC_LITERAL(8, 96, 11), // "sendStepNum"
QT_MOC_LITERAL(9, 108, 19), // "sendLeftGratingData"
QT_MOC_LITERAL(10, 128, 14), // "vector<double>"
QT_MOC_LITERAL(11, 143, 10), // "leftRunDis"
QT_MOC_LITERAL(12, 154, 20), // "sendRightGratingData"
QT_MOC_LITERAL(13, 175, 11), // "rightRunDis"
QT_MOC_LITERAL(14, 187, 13), // "sendRobotPose"
QT_MOC_LITERAL(15, 201, 27), // "VisualProcessing::RobotPose"
QT_MOC_LITERAL(16, 229, 9), // "robotPose"
QT_MOC_LITERAL(17, 239, 14), // "sendStepConfig"
QT_MOC_LITERAL(18, 254, 17), // "networkActionSlot"
QT_MOC_LITERAL(19, 272, 16), // "motionActionSlot"
QT_MOC_LITERAL(20, 289, 16), // "cameraActionSlot"
QT_MOC_LITERAL(21, 306, 17), // "eyeHandActionSlot"
QT_MOC_LITERAL(22, 324, 26), // "visualProcessingActionSlot"
QT_MOC_LITERAL(23, 351, 13), // "receiveSocket"
QT_MOC_LITERAL(24, 365, 4), // "info"
QT_MOC_LITERAL(25, 370, 21), // "displayDataActionSlot"
QT_MOC_LITERAL(26, 392, 23), // "updateStatusInformation"
QT_MOC_LITERAL(27, 416, 4), // "Info"
QT_MOC_LITERAL(28, 421, 23), // "receiveProcessingResult"
QT_MOC_LITERAL(29, 445, 16), // "planeparamStatus"
QT_MOC_LITERAL(30, 462, 26), // "CoreAlgorithm::PlaneNormal"
QT_MOC_LITERAL(31, 489, 10), // "planeparam"
QT_MOC_LITERAL(32, 500, 27), // "CoreAlgorithm::StereoCircle"
QT_MOC_LITERAL(33, 528, 12), // "centerResult"
QT_MOC_LITERAL(34, 541, 35), // "pcl::PointCloud<pcl::PointXYZ..."
QT_MOC_LITERAL(35, 577, 15), // "borderCloud_ptr"
QT_MOC_LITERAL(36, 593, 16), // "receiveStepParam"
QT_MOC_LITERAL(37, 610, 14), // "CamNoScanSpeed"
QT_MOC_LITERAL(38, 625, 15), // "CamScanDistance"
QT_MOC_LITERAL(39, 641, 22), // "CamFirstNoScanDistance"
QT_MOC_LITERAL(40, 664, 23) // "CamSecondNoScanDistance"

    },
    "DisplayGUI\0statusUpdate\0\0"
    "sendProcessingInformation\0num\0HObject\0"
    "inputImageFirst\0inputImageSecond\0"
    "sendStepNum\0sendLeftGratingData\0"
    "vector<double>\0leftRunDis\0"
    "sendRightGratingData\0rightRunDis\0"
    "sendRobotPose\0VisualProcessing::RobotPose\0"
    "robotPose\0sendStepConfig\0networkActionSlot\0"
    "motionActionSlot\0cameraActionSlot\0"
    "eyeHandActionSlot\0visualProcessingActionSlot\0"
    "receiveSocket\0info\0displayDataActionSlot\0"
    "updateStatusInformation\0Info\0"
    "receiveProcessingResult\0planeparamStatus\0"
    "CoreAlgorithm::PlaneNormal\0planeparam\0"
    "CoreAlgorithm::StereoCircle\0centerResult\0"
    "pcl::PointCloud<pcl::PointXYZ>::Ptr\0"
    "borderCloud_ptr\0receiveStepParam\0"
    "CamNoScanSpeed\0CamScanDistance\0"
    "CamFirstNoScanDistance\0CamSecondNoScanDistance"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DisplayGUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   99,    2, 0x06 /* Public */,
       3,    3,  104,    2, 0x06 /* Public */,
       8,    1,  111,    2, 0x06 /* Public */,
       9,    1,  114,    2, 0x06 /* Public */,
      12,    1,  117,    2, 0x06 /* Public */,
      14,    1,  120,    2, 0x06 /* Public */,
      17,    0,  123,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  124,    2, 0x0a /* Public */,
      19,    0,  125,    2, 0x0a /* Public */,
      20,    0,  126,    2, 0x0a /* Public */,
      21,    0,  127,    2, 0x0a /* Public */,
      22,    0,  128,    2, 0x0a /* Public */,
      23,    1,  129,    2, 0x0a /* Public */,
      25,    0,  132,    2, 0x0a /* Public */,
      26,    2,  133,    2, 0x0a /* Public */,
      28,    5,  138,    2, 0x0a /* Public */,
      36,    4,  149,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 5, 0x80000000 | 5,    4,    6,    7,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   27,    4,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 30, 0x80000000 | 32, 0x80000000 | 34, QMetaType::Int,   29,   31,   33,   35,    4,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,   37,   38,   39,   40,

       0        // eod
};

void DisplayGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DisplayGUI *_t = static_cast<DisplayGUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->statusUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->sendProcessingInformation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< HObject(*)>(_a[2])),(*reinterpret_cast< HObject(*)>(_a[3]))); break;
        case 2: _t->sendStepNum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sendLeftGratingData((*reinterpret_cast< vector<double>(*)>(_a[1]))); break;
        case 4: _t->sendRightGratingData((*reinterpret_cast< vector<double>(*)>(_a[1]))); break;
        case 5: _t->sendRobotPose((*reinterpret_cast< VisualProcessing::RobotPose(*)>(_a[1]))); break;
        case 6: _t->sendStepConfig(); break;
        case 7: _t->networkActionSlot(); break;
        case 8: _t->motionActionSlot(); break;
        case 9: _t->cameraActionSlot(); break;
        case 10: _t->eyeHandActionSlot(); break;
        case 11: _t->visualProcessingActionSlot(); break;
        case 12: _t->receiveSocket((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->displayDataActionSlot(); break;
        case 14: _t->updateStatusInformation((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->receiveProcessingResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< CoreAlgorithm::PlaneNormal(*)>(_a[2])),(*reinterpret_cast< CoreAlgorithm::StereoCircle(*)>(_a[3])),(*reinterpret_cast< pcl::PointCloud<pcl::PointXYZ>::Ptr(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 16: _t->receiveStepParam((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DisplayGUI::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayGUI::statusUpdate)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DisplayGUI::*_t)(int , HObject , HObject );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayGUI::sendProcessingInformation)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DisplayGUI::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayGUI::sendStepNum)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (DisplayGUI::*_t)(vector<double> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayGUI::sendLeftGratingData)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (DisplayGUI::*_t)(vector<double> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayGUI::sendRightGratingData)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (DisplayGUI::*_t)(VisualProcessing::RobotPose );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayGUI::sendRobotPose)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (DisplayGUI::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DisplayGUI::sendStepConfig)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject DisplayGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DisplayGUI.data,
      qt_meta_data_DisplayGUI,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DisplayGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DisplayGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayGUI.stringdata0))
        return static_cast<void*>(const_cast< DisplayGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DisplayGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void DisplayGUI::statusUpdate(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DisplayGUI::sendProcessingInformation(int _t1, HObject _t2, HObject _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DisplayGUI::sendStepNum(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DisplayGUI::sendLeftGratingData(vector<double> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DisplayGUI::sendRightGratingData(vector<double> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DisplayGUI::sendRobotPose(VisualProcessing::RobotPose _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DisplayGUI::sendStepConfig()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
