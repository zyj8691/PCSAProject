/****************************************************************************
** Meta object code from reading C++ file 'visualprocessing.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../visualprocessing.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'visualprocessing.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VisualProcessing_t {
    QByteArrayData data[33];
    char stringdata0[564];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VisualProcessing_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VisualProcessing_t qt_meta_stringdata_VisualProcessing = {
    {
QT_MOC_LITERAL(0, 0, 16), // "VisualProcessing"
QT_MOC_LITERAL(1, 17, 20), // "sendProcessingResult"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 16), // "planeparamStatus"
QT_MOC_LITERAL(4, 56, 26), // "CoreAlgorithm::PlaneNormal"
QT_MOC_LITERAL(5, 83, 10), // "planeparam"
QT_MOC_LITERAL(6, 94, 27), // "CoreAlgorithm::StereoCircle"
QT_MOC_LITERAL(7, 122, 12), // "centerResult"
QT_MOC_LITERAL(8, 135, 35), // "pcl::PointCloud<pcl::PointXYZ..."
QT_MOC_LITERAL(9, 171, 15), // "borderCloud_ptr"
QT_MOC_LITERAL(10, 187, 3), // "num"
QT_MOC_LITERAL(11, 191, 13), // "sendStepParam"
QT_MOC_LITERAL(12, 205, 17), // "CameraNoScanSpeed"
QT_MOC_LITERAL(13, 223, 18), // "CameraScanDistance"
QT_MOC_LITERAL(14, 242, 25), // "CameraFirstNoScanDistance"
QT_MOC_LITERAL(15, 268, 26), // "CameraSecondNoScanDistance"
QT_MOC_LITERAL(16, 295, 21), // "on_openButton_clicked"
QT_MOC_LITERAL(17, 317, 23), // "on_updataButton_clicked"
QT_MOC_LITERAL(18, 341, 20), // "on_setButton_clicked"
QT_MOC_LITERAL(19, 362, 14), // "receiveStepNum"
QT_MOC_LITERAL(20, 377, 18), // "receiveInformation"
QT_MOC_LITERAL(21, 396, 7), // "HObject"
QT_MOC_LITERAL(22, 404, 15), // "inputImageFirst"
QT_MOC_LITERAL(23, 420, 16), // "inputImageSecond"
QT_MOC_LITERAL(24, 437, 22), // "receiveLeftGratingData"
QT_MOC_LITERAL(25, 460, 14), // "vector<double>"
QT_MOC_LITERAL(26, 475, 4), // "left"
QT_MOC_LITERAL(27, 480, 23), // "receiveRightGratingData"
QT_MOC_LITERAL(28, 504, 5), // "right"
QT_MOC_LITERAL(29, 510, 16), // "receiveRobotPose"
QT_MOC_LITERAL(30, 527, 9), // "RobotPose"
QT_MOC_LITERAL(31, 537, 7), // "robotPo"
QT_MOC_LITERAL(32, 545, 18) // "readStepConfigSlot"

    },
    "VisualProcessing\0sendProcessingResult\0"
    "\0planeparamStatus\0CoreAlgorithm::PlaneNormal\0"
    "planeparam\0CoreAlgorithm::StereoCircle\0"
    "centerResult\0pcl::PointCloud<pcl::PointXYZ>::Ptr\0"
    "borderCloud_ptr\0num\0sendStepParam\0"
    "CameraNoScanSpeed\0CameraScanDistance\0"
    "CameraFirstNoScanDistance\0"
    "CameraSecondNoScanDistance\0"
    "on_openButton_clicked\0on_updataButton_clicked\0"
    "on_setButton_clicked\0receiveStepNum\0"
    "receiveInformation\0HObject\0inputImageFirst\0"
    "inputImageSecond\0receiveLeftGratingData\0"
    "vector<double>\0left\0receiveRightGratingData\0"
    "right\0receiveRobotPose\0RobotPose\0"
    "robotPo\0readStepConfigSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VisualProcessing[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   69,    2, 0x06 /* Public */,
      11,    4,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    0,   89,    2, 0x0a /* Public */,
      17,    0,   90,    2, 0x0a /* Public */,
      18,    0,   91,    2, 0x0a /* Public */,
      19,    1,   92,    2, 0x0a /* Public */,
      20,    3,   95,    2, 0x0a /* Public */,
      24,    1,  102,    2, 0x0a /* Public */,
      27,    1,  105,    2, 0x0a /* Public */,
      29,    1,  108,    2, 0x0a /* Public */,
      32,    0,  111,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 4, 0x80000000 | 6, 0x80000000 | 8, QMetaType::Int,    3,    5,    7,    9,   10,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,   12,   13,   14,   15,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 21, 0x80000000 | 21,   10,   22,   23,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 25,   28,
    QMetaType::Void, 0x80000000 | 30,   31,
    QMetaType::Void,

       0        // eod
};

void VisualProcessing::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VisualProcessing *_t = static_cast<VisualProcessing *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendProcessingResult((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< CoreAlgorithm::PlaneNormal(*)>(_a[2])),(*reinterpret_cast< CoreAlgorithm::StereoCircle(*)>(_a[3])),(*reinterpret_cast< pcl::PointCloud<pcl::PointXYZ>::Ptr(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 1: _t->sendStepParam((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 2: _t->on_openButton_clicked(); break;
        case 3: _t->on_updataButton_clicked(); break;
        case 4: _t->on_setButton_clicked(); break;
        case 5: _t->receiveStepNum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->receiveInformation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< HObject(*)>(_a[2])),(*reinterpret_cast< HObject(*)>(_a[3]))); break;
        case 7: _t->receiveLeftGratingData((*reinterpret_cast< vector<double>(*)>(_a[1]))); break;
        case 8: _t->receiveRightGratingData((*reinterpret_cast< vector<double>(*)>(_a[1]))); break;
        case 9: _t->receiveRobotPose((*reinterpret_cast< RobotPose(*)>(_a[1]))); break;
        case 10: _t->readStepConfigSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VisualProcessing::*_t)(bool , CoreAlgorithm::PlaneNormal , CoreAlgorithm::StereoCircle , pcl::PointCloud<pcl::PointXYZ>::Ptr , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VisualProcessing::sendProcessingResult)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VisualProcessing::*_t)(double , double , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VisualProcessing::sendStepParam)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject VisualProcessing::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VisualProcessing.data,
      qt_meta_data_VisualProcessing,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VisualProcessing::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VisualProcessing::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VisualProcessing.stringdata0))
        return static_cast<void*>(const_cast< VisualProcessing*>(this));
    return QWidget::qt_metacast(_clname);
}

int VisualProcessing::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void VisualProcessing::sendProcessingResult(bool _t1, CoreAlgorithm::PlaneNormal _t2, CoreAlgorithm::StereoCircle _t3, pcl::PointCloud<pcl::PointXYZ>::Ptr _t4, int _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VisualProcessing::sendStepParam(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_Login_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Login_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Login_t qt_meta_stringdata_Login = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Login"
QT_MOC_LITERAL(1, 6, 22), // "on_loginButton_clicked"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "Login\0on_loginButton_clicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Login[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Login::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Login *_t = static_cast<Login *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_loginButton_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Login::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Login.data,
      qt_meta_data_Login,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Login::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Login::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Login.stringdata0))
        return static_cast<void*>(const_cast< Login*>(this));
    return QWidget::qt_metacast(_clname);
}

int Login::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
