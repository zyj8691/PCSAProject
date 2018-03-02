#pragma once

#include <QWidget>
#include "ui_visualprocessing.h"
#include"ui_Login.h"
#include <iostream>
#include<fstream>
//���halcon
#include "HalconCpp.h"
//���OpenCV
#include "cv.h"
#include "opencv.hpp"
//��Ӻ����㷨
#include "corealgorithm.h"
#include<QFile>
#include <QTimer>
#include <QDateTime>
#include <QTextStream>
#include<QMessageBox>
using  std::cout;
using  std::endl;
using namespace cv;
using namespace HalconCpp;


#define  Pi 3.14159265358979

class VisualProcessing : public QWidget
{
	Q_OBJECT

public:
	VisualProcessing(QWidget *parent = Q_NULLPTR);
	~VisualProcessing();

	//���������λ�˽ṹ
	struct RobotPose
	{
		double X;
		double Y;
		double Z;
		double A;//��Z����ת
		double B;//��Y����ת
		double C;//��X����ת
	};


	//��ʼ��lineEdit�ռ�
	void InitializationLineEdit();
	//��ȡ�����ļ�
	void ReadConfigParameterFile();
	//����ʱ��� ��ʽΪyyyy-MM-dd hh-mm-ssPointCloud
	QString ReturnTime();
	//�Ҳ�ͷ����ת�������ͷ����ϵ�£�Ӧ���ڱ�Ե����
	Vector<Point3f> RightToLeft(Vector<Point3f> points);
	//�Ҳ�ͷ����ת�������ͷ����ϵ�£�Ӧ���ڿռ�Բ��
	CoreAlgorithm::StereoCircle RightToLeft(CoreAlgorithm::StereoCircle centerResult);
	//�Ҳ�ͷ����ת�������ͷ����ϵ�£�Ӧ����ƽ�淨ʸ
	CoreAlgorithm::PlaneNormal RightToLeft(CoreAlgorithm::PlaneNormal planeparam);
	//��ͷ����ϵ�������˹�������ϵ��Ӧ���ڱ�Ե����
	Vector<Point3f> MeasureToRobotTool(Vector<Point3f> points);
	//��ͷ����ϵ�������˹�������ϵ��Ӧ���ڿռ�Բ��
	CoreAlgorithm::StereoCircle MeasureToRobotTool(CoreAlgorithm::StereoCircle centerResult);
	//��ͷ����ϵ�������˹�������ϵ��Ӧ����ƽ�淨ʸ
	CoreAlgorithm::PlaneNormal MeasureToRobotTool(CoreAlgorithm::PlaneNormal planeparam);

	//�����˹�������ϵ�������˻�������ϵ��Ӧ���ڱ�Ե����
	Vector<Point3f> RobotToolToBase(Vector<Point3f> points);
	//�����˹�������ϵ�������˻�������ϵ��Ӧ���ڿռ�Բ��
	CoreAlgorithm::StereoCircle RobotToolToBase(CoreAlgorithm::StereoCircle centerResult);
	//�����˹�������ϵ�������˻�������ϵ��Ӧ����ƽ�淨ʸ
	CoreAlgorithm::PlaneNormal RobotToolToBase(CoreAlgorithm::PlaneNormal planeparam);


	//����һ��һ������ദ��
	void stepOneFirstLeftProcessing(int num,HObject inputImageFirst, HObject inputImageSecond);
	//����һ��һ�����Ҳദ��
	void stepOneSecondRightProcessing(int num,HObject inputImageFirst, HObject inputImageSecond);
	//��������ദ��
	void stepTwoLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//�������Ҳദ��
	void stepTwoRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//��������ദ��
	void stepThreeLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//�������Ҳദ��
	void stepThreeRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//��������෨���״���
	void stepTwoLeftFlangeHoleProcessing(int num, HObject inputImage);
	//�������Ҳ෨���״���
	void stepTwoRightFlangeHoleProcessing(int num, HObject inputImage);
	//��������෨���״���
	void stepThreeLeftFlangeHoleProcessing(int num, HObject inputImage);
	//�������Ҳ෨���״���
	void stepThreeRightFlangeHoleProcessing(int num, HObject inputImage);
	//��������ദ��
	void stepFourLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//�������Ҳദ��
	void stepFourRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//��������ദ��
	void stepFiveLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//�������Ҳദ��
	void stepFiveRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond);
	//�Ƕ���ת������
	double deg2rad(double deg);
	//����ӻ����˹�������ϵ�������˻�������ϵ�ı任����
	void calculateRobotToolToBaseMatrix(RobotPose robotPose);

	//lineEdit�ؼ����
	const int lineEditNum = 124;
	QLineEdit* lineEdit[124];
	//�����ļ�������
	QString engineNumFileName;
	
	//����������λ��
	RobotPose robotPose;
	//�����ӻ����˹�������ϵ�������˻�������ϵ�任����
	Mat robotToolToBaseMatrix;
	//��դ�߷��������ƶ�����
	vector<double> leftRunDis;
	vector<double> rightRunDis;
signals:
	void sendProcessingResult(bool planeparamStatus/*ƽ�淨ʸ����״̬*/,
							CoreAlgorithm::PlaneNormal planeparam/*ƽ�淨ʸ*/,
							CoreAlgorithm::StereoCircle centerResult/*�ռ�Բ��*/,
							pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr/*��Ե���� PCL��ʽ*/,
							int num/*num = 1 ��ʾ����������������num = 2 ��ʾ�����Ҳ��������*/);
	void sendStepParam(double CameraNoScanSpeed, double CameraScanDistance, double CameraFirstNoScanDistance, double CameraSecondNoScanDistance);
public slots:
	//�����������
	void on_updataButton_clicked();
	//�ر�ʹ��
	void on_setButton_clicked();
	//���չ�����ţ�������ز�����
	void receiveStepNum(int num);
	//���մ���λ�ż���ͼƬ
	void receiveInformation(int num, HObject inputImageFirst, HObject inputImageSecond);
	//���չ�ɾ������
	void receiveLeftGratingData(vector<double>left);
	void receiveRightGratingData(vector<double>right);
	//���ջ�����λ��
	void receiveRobotPose(RobotPose robotPo);
private:
	Ui::VisualProcessing ui;
	CoreAlgorithm *alg;

	
};

class Login :public QWidget
{
	Q_OBJECT
public:
	Login();
	~Login();

public slots:
void on_loginButton_clicked();
private:
	Ui::Form ui;
	VisualProcessing *vp;
};

