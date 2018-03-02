#ifndef MOTIONCONTROL_H
#define MOTIONCONTROL_H
#include "stdafx.h"
#include <QWidget>
#include "ui_motioncontrol.h"
#include <iostream>
#include "mpint.h"
#include <stdio.h>
#include"define.h"
#include <QMessageBox>

class MotionControl : public QWidget
{
	Q_OBJECT

public:
	MotionControl(QWidget *parent = 0);
	~MotionControl();
	//���켰ת̨��ʼ��
	void initializationMotion();
	//���õ����ٶ�
	void setLeadRailVelocity(double speed);
	//���õ����˶�����
	void setLeadRailMotionDistance(double distance);
	//���첻�ɼ�������˶�
	void leadRailMotion(double distance);
	//ת̨������ת��180��
	void platformLtoRRotate();
	//ת̨���ҵ���ת��180��
	void platformRtoLRotate();
	//������ת̨��λ
	void reset(double startPosition,double endPosition);
	//��⵼���ת̨�Ƿ��˶�
	void CheckRun();
	//��ȡ��ǰλ��
	double GetCurrentPositon();
public slots:
	//�����˶�
	void on_forwardButton_pressed();
	void on_forwardButton_released();
	void on_backwardButton_pressed();
	void on_backwardButton_released();
	//ת̨�˶�
	void on_leftButton_pressed();
	void on_leftButton_released();
	void on_rightButton_pressed();
	void on_rightButton_released();
	//��λ
	void on_resetButton_clicked();
	//ת̨����λ����ת
	void on_rotateButton_clicked();
private:
	Ui::MotionControl ui;
	//��������
	MDCE* pComD2 = NULL;
	//����ת̨
	MDCE* pComDD = NULL;
	//��������
	int bConnectedD2;
	//����ת̨
	int bConnectedDD;
signals:
	void sendInfo(QString,int);
	void toPLCInfo(QByteArray Byte);
};

#endif // MOTIONCONTROL_H
