#ifndef DISPLAYGUI_H
#define DISPLAYGUI_H
#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <QtWidgets/QMainWindow>
#include<QString>
#include "ui_displaygui.h"
//����ͨѶͷ�ļ�
#include "networkcommunication.h"
//�˶�����ͷ�ļ�
#include "motioncontrol.h"
//����ɼ�ͷ�ļ�
#include "cameracapture.h"
//�����㷨ͷ�ļ�
#include "corealgorithm.h"
//����ͨѶ��ͷ�ļ�
#include "serialcommunication.h"
//�Ӿ�������ͷ�ļ�
#include "visualprocessing.h"
#include<iostream>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QByteArray>
#include<QMessageBox>
#include<QFile>
#include <QTextStream>
#include <QProcess>
#include <QDir>
// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>
#include<QVTKWidget.h>

#include "HalconCpp.h"

using namespace HalconCpp;

class DisplayGUI : public QMainWindow
{
	Q_OBJECT

public:
	DisplayGUI(QWidget *parent = 0);
	~DisplayGUI();
	//���崫�������Ϣ�ṹ��
	struct NormalMessage
	{
		QString A;
		QString B;
		QString C;
	};
	struct CenterMessage
	{
		QString X;
		QString Y;
		QString Z;
	};
	struct DVectorMessage
	{
		QString A;
		QString B;
		QString C;
	};
	struct ResultMessage 
	{
		NormalMessage leftNormal;
		CenterMessage leftCenter;
		QString leftPhaseAngle;
		NormalMessage rightNormal;
		CenterMessage rightCenter;
		QString rightPhaseAngle;
		DVectorMessage directionVector;
	};


	//���庯��
	//��ʾ����
	//����ԲΪ��ɫ
	//���Բ��Ϊ��ɫ���Ҳ�Ϊ��ɫ
	void displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr LeftCloud_ptr/*������*/, CoreAlgorithm::StereoCircle leftCenterResult/*���Բ��*/,
		pcl::PointCloud<pcl::PointXYZ>::Ptr RightCloud_ptr/*�Ҳ����*/, CoreAlgorithm::StereoCircle RightCenterResult/*�Ҳ�Բ��*/);
	//�������� ��ʾ����2 ����3����
	//���������Բ��Ϊ��ɫ���Ҳ�Ϊ��ɫ
	void displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr LeftCloud_ptr/*������*/, 
		CoreAlgorithm::StereoCircle leftCenterResult/*���Բ��*/,
		pcl::PointCloud<pcl::PointXYZ>::Ptr RightCloud_ptr/*�Ҳ����*/,
		CoreAlgorithm::StereoCircle RightCenterResult/*�Ҳ�Բ��*/,
		pcl::PointCloud<pcl::PointXYZ>::Ptr LeftFlangeHoleCloud_ptr/*��෨���׵���*/,
		CoreAlgorithm::StereoCircle leftFlangeHoleCenterResult/*��෨����Բ��*/,
		pcl::PointCloud<pcl::PointXYZ>::Ptr RightFlangeHoleCloud_ptr/*�Ҳ෨���׵���*/,
		CoreAlgorithm::StereoCircle RightFlangeHoleCenterResult/*�Ҳ෨����Բ��*/);
	//ֻ��ʾһ��������ʱ��Բ��Ϊ��ɫ
	void displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr LeftCloud_ptr/*������*/,
		CoreAlgorithm::StereoCircle leftCenterResult/*���Բ��*/,
		pcl::PointCloud<pcl::PointXYZ>::Ptr RightCloud_ptr/*�Ҳ����*/,
		CoreAlgorithm::StereoCircle RightCenterResult/*�Ҳ�Բ��*/,
		pcl::PointCloud<pcl::PointXYZ>::Ptr FlangeHoleCloud_ptr/*ĳ�෨���׵���*/,
		CoreAlgorithm::StereoCircle FlangeHoleCenterResult/*ĳ�෨����Բ��*/);
	void displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr Cloud_ptr, CoreAlgorithm::StereoCircle centerResult);
	//void displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr CloudFirst_ptr, CoreAlgorithm::StereoCircle centerResultFirst);
	//��ʾͼƬ
	void displayImage(HObject Image,int num);
	//����1��һ����
	void stepOneFirst();
	//����1�ڶ�����
	void stepOneSecond();
	//����2��һ����
	void stepTwoFirst();
	//����2�ڶ�����
	void stepTwoSecond();
	//����3��һ����
	void stepThreeFirst();
	//����3�ڶ�����
	void stepThreeSecond();
	//����4��һ����
	void stepFourFirst();
	//����4�ڶ�����
	void stepFourSecond();
	//����5��һ����
	void stepFiveFirst();
	//����5�ڶ�����
	void stepFiveSecond();

	/**
	*���÷�����
	*/
	//����1��һ����
	void VStepOneFirst();
	//����1�ڶ�����
	void VStepOneSecond();
	//����2��һ����
	void VStepTwoFirst();
	//����2�ڶ�����
	void VStepTwoSecond();
	//����3��һ����
	void VStepThreeFirst();
	//����3�ڶ�����
	void VStepThreeSecond();
	//HObjectת��ΪMat����
	Mat HImageToIplImage(HObject &Hobj);
	//Matת��ΪQImage����
	QImage cvMat2QImage(const cv::Mat& mat);
	//��ʼ������
	void initialization();
	//���ͷ����
	void LeftScannerRun(int num,pcl::PointCloud<pcl::PointXYZ>::Ptr &borderCloud_ptr, CoreAlgorithm::StereoCircle &centerResult);
	//�Ҳ�ͷ����
	void RightScannerRun(int num,pcl::PointCloud<pcl::PointXYZ>::Ptr &borderCloud_ptr, CoreAlgorithm::StereoCircle &centerResult);
	//��ʼ��lineEdit�ռ�
	//void InitializationLineEdit();
	//��ȡ�����ļ�
	//void ReadConfigParameterFile();
	//����ʱ��� ��ʽΪyyyy-MM-dd hh-mm-ss
	QString ReturnTime();
	//����ͼƬ
	void SaveImage(HObject Image,QString name);
	//�ֽ⹤�ػ����͵��ź�
	void resolveSignal(QString info);
	//������־�ļ���Ϣ
	void saveInformationToJournalFile(QString info);
	//������������Ҫ�ĸ����ļ���
	void createProjectDirectory(QString info);
	//�����������ݱ����ļ���ͬʱ���ɲ�����
	void createParamFirle(QString info);
	//���ƿռ�����Բ
	void createTheoryCircle(CoreAlgorithm::StereoCircle theroyCircle, pcl::PointCloud<pcl::PointXYZ>::Ptr theroyCirclePoints);

	//�������
	//Socket��������
	QString engineVersion;
	int socketNum;
	//lineEdit�ؼ����
	//QLineEdit* lineEdit[72];

	/**
	*��ʵ�ʲ����У����ֳ����ȷ����������ǰ�������Լ��ۺϴ������������ң�
	*/
	//�����Ե�����
	pcl::PointCloud<pcl::PointXYZ>::Ptr leftCircle_cloudPoints;
	pcl::PointCloud<pcl::PointXYZ>::Ptr rightCircle_cloudPoints;
	//����ռ�Բ��
	CoreAlgorithm::StereoCircle leftCenterResult;
	CoreAlgorithm::StereoCircle rightCenterResult;
	//����ƽ�淨ʸ
	CoreAlgorithm::PlaneNormal leftPlaneNormal;
	CoreAlgorithm::PlaneNormal rightPlaneNormal;
	//���������� ֻҪ��ʸ��ϳɹ���Ϊtrue
	bool leftProcessingStatus;
	bool rightProcessingStatus;
	//���巨���ױ�Ե����
	pcl::PointCloud<pcl::PointXYZ>::Ptr leftFlangeHoleCircle_cloudPoints;
	pcl::PointCloud<pcl::PointXYZ>::Ptr rightFlangeHoleCircle_cloudPoints;
	//���巨���׿ռ�Բ��
	CoreAlgorithm::StereoCircle leftFlangeHoleCenterResult;
	CoreAlgorithm::StereoCircle rightFlangeHoleCenterResult;
	/**
	*Ϊÿ����λ������������
	*/
	//Ӧ���ڹ���һ�ڶ����� ǰ��������ۺϴ���������
	//��һ��
	//�����Ե�����
	pcl::PointCloud<pcl::PointXYZ>::Ptr transmissionCaseCircleFirst_cloudPoints;
	//����ռ�Բ��
	CoreAlgorithm::StereoCircle transmissionCaseCenterResultFirst;
	//�ڶ���
	//�����Ե�����
	pcl::PointCloud<pcl::PointXYZ>::Ptr transmissionCaseCircleSecond_cloudPoints;
	//����ռ�Բ��
	CoreAlgorithm::StereoCircle transmissionCaseCenterResultSecond;
	//�������β���Բ�ĵķ���ʸ��
	Point3f directionVector;

	/**
	*���÷���������
	*/



	//�����ļ�������
	QString engineNumFileName;
	//���幤������
	double CameraNoScanSpeed;//��ͷ��ɨ��ʱ�˶��ٶ�
	double CameraScanDistance;//��ͷɨ��ʱ�˶�����
	double CameraFirstNoScanDistance;//��ͷ�˶�����һ����λ�õ��˶�����
	double CameraSecondNoScanDistance;//��ͷ�˶����ڶ�����λ�õ��˶�����

	//��դ�߷��������ƶ�����
	vector<double> leftRunDis;
	vector<double> rightRunDis;
	//���ͽ����Ϣ
	ResultMessage resultMessage;
signals:
	//����״̬����Ϣ
	void statusUpdate(QString,int);
	//���ʹ���λ��ż���ͼƬ
	void sendProcessingInformation(int num, HObject inputImageFirst, HObject inputImageSecond);
	//������������λ�����ƶ������ٶ���Ϣ
	void sendStepNum(int num);
	//�������ɼ�ʱ������ʵ���˶���
	void sendLeftGratingData(vector<double> leftRunDis);
	//�����Ҳ�ɼ�ʱ������ʵ���˶���
	void sendRightGratingData(vector<double> rightRunDis);
	//���ͻ�����λ��
	void sendRobotPose(VisualProcessing::RobotPose robotPose);
public slots:
	//������ͨѶ����
	void networkActionSlot();
	//���˶�����
	void motionActionSlot();
	//���������
	void cameraActionSlot();
	//�����۱궨����
	void eyeHandActionSlot();
	//���Ӿ���������趨
	void visualProcessingActionSlot();
	//�����������ݣ����й�������
	void receiveSocket(QString info);
	//��ʾ��ʷ����
	void displayDataActionSlot();
	//����״̬��Ϣ
	void updateStatusInformation(QString Info,int num);
	//�����������
	//void on_updataButton_clicked();
	//�ر�ʹ��
	//void on_setButton_clicked();
	//�����Ӿ���������������ʾ
	void receiveProcessingResult(bool planeparamStatus/*ƽ�淨ʸ����״̬*/,
		CoreAlgorithm::PlaneNormal planeparam/*ƽ�淨ʸ*/,
		CoreAlgorithm::StereoCircle centerResult/*�ռ�Բ��*/,
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr/*��Ե���� PCL��ʽ*/,
		int num/*������*/);
	/*num = 1 ��ʾ����������������num = 2 ��ʾ�����Ҳ����������num = 3 ��ʾ������෨���ײ���������num = 4 ��ʾ�����Ҳ෨���ײ�������*/
	//����VisualProcessing UI�Ĳ���
	void receiveStepParam(double CamNoScanSpeed, double CamScanDistance, double CamFirstNoScanDistance, double CamSecondNoScanDistance);
private:
	Ui::DisplayGUIClass ui;
	NetworkCommunication *socket;
	MotionControl *motion;
	CoreAlgorithm *alg;
	CameraCapture *cap;
	SerialCommunication *serial;
	VisualProcessing *vp;
	Login *login;
	//PCL��ʾ
	pcl::visualization::PCLVisualizer::Ptr viewer_1;
	pcl::visualization::PCLVisualizer::Ptr viewer_2;
};

#endif // DISPLAYGUI_H
