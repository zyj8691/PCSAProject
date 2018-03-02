#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include "motioncontrol.h"
#include <QWidget>
#include "ui_cameracapture.h"
#include"halconcpp.h"
#include <iostream>
#include"define.h"
using namespace HalconCpp;

class CameraCapture : public QWidget
{
	Q_OBJECT

public:
	CameraCapture();
	~CameraCapture();
	HObject captureImage();
	double RateToSpeed(HTuple hv_RateValue);
	//�����ʼ��
	void initializationCamera();
	//ͨѶPLC�������
	void sendPLC();

	// Local iconic variables
	HObject  ho_Image;
	// Local control variables
	HTuple  hv_Information, hv_ValueList, hv_Device;
	HTuple  hv_AcqHandle, hv_RateValue, hv_Exception;

	HTuple  hv_CalibFile, hv_FileHandle, hv_variableName;
	HTuple  hv_variableValue, hv_Index, hv_OutLine, hv_IsEOF;
	HTuple  hv_Substrings, hv_HigherOrderCoefficients, hv_Length;
	HTuple  hv_num, hv_i;
	//���״̬
	bool cameraStatus;
public slots:
	void on_openButton_clicked();
	void on_closeButton_clicked();

private:
	Ui::CameraCapture ui;
	MotionControl *motion;

signals:
	void sendInfo(QString,int);
	void toPLCInfo(QByteArray Byte);
};


#endif // CAMERACAPTURE_H
