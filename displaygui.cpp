#define SETUP
//#define READIMAGE
//#define PROGRESS
#include "displaygui.h"
using  std::cout;
using  std::endl;

DisplayGUI::DisplayGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//lineEdit[72] = new QLineEdit();
	//��ʼ��lineEdit�ؼ�
	//InitializationLineEdit();
	//��ȡ�����������궨����
	//ReadConfigParameterFile();
	/*for (int i = 0; i < 72; i++){
		lineEdit[i]->setEnabled(false);
	}*/
	//��ʼ��PCL��ʾ�ؼ�
	viewer_1.reset(new pcl::visualization::PCLVisualizer("viewer1", false));
	ui.PCLwidget_1->SetRenderWindow(viewer_1->getRenderWindow());
	viewer_1->setupInteractor(ui.PCLwidget_1->GetInteractor(), ui.PCLwidget_1->GetRenderWindow());
	ui.PCLwidget_1->update();
	//��ʼ�������㷨
	alg = new CoreAlgorithm(0);
	//��ʼ���˶�����
	motion = new MotionControl();
	//��ʼ������ɼ�
	cap = new CameraCapture();
	//��ʼ������ͨѶ
	serial = new SerialCommunication();
	//��ʼ������ͨѶ
	socket = new NetworkCommunication;
	//��ʼ���Ӿ�����
	vp = new VisualProcessing;
	//��¼����
	login = new Login;

	//��������
	connect(socket, &NetworkCommunication::stringReceived, this, &DisplayGUI::receiveSocket);
	//״̬��Ϣ����
	connect(this, &DisplayGUI::statusUpdate, this, &DisplayGUI::updateStatusInformation);
	connect(cap, &CameraCapture::sendInfo, this, &DisplayGUI::updateStatusInformation);
	connect(socket, &NetworkCommunication::sendInfo, this, &DisplayGUI::updateStatusInformation);
	connect(motion, &MotionControl::sendInfo, this, &DisplayGUI::updateStatusInformation);

	connect(cap, &CameraCapture::toPLCInfo, serial, &SerialCommunication::sendPlC);
	connect(motion, &MotionControl::toPLCInfo, serial, &SerialCommunication::sendPlC);
	
	connect(this, &DisplayGUI::sendProcessingInformation, vp, &VisualProcessing::receiveInformation);
	connect(vp, &VisualProcessing::sendProcessingResult, this, &DisplayGUI::receiveProcessingResult);

	connect(this, &DisplayGUI::sendStepNum, vp, &VisualProcessing::receiveStepNum);
	connect(vp, &VisualProcessing::sendStepParam, this, &DisplayGUI::receiveStepParam);
	
	connect(ui.networkAction, SIGNAL(triggered()), this, SLOT(networkActionSlot()));
	connect(ui.motionAction, SIGNAL(triggered()), this, SLOT(motionActionSlot()));
	connect(ui.cameraAction, SIGNAL(triggered()), this, SLOT(cameraActionSlot()));
	connect(ui.eyeHandAction, SIGNAL(triggered()), this, SLOT(eyeHandActionSlot()));
	connect(ui.visualProcessingAction, SIGNAL(triggered()), this, SLOT(visualProcessingActionSlot()));
	connect(ui.displayDataAction, SIGNAL(triggered()), this, SLOT(displayDataActionSlot()));

	connect(this, &DisplayGUI::sendLeftGratingData, vp, &VisualProcessing::receiveLeftGratingData);
	connect(this, &DisplayGUI::sendRightGratingData, vp, &VisualProcessing::receiveRightGratingData);
	connect(this, &DisplayGUI::sendRobotPose, vp, &VisualProcessing::receiveRobotPose);

	connect(this, &DisplayGUI::sendStepConfig, login, &Login::readStepConfigSlot);
	//�˵�����ť
	//connect(ui.networkAction, SIGNAL(triggered()), this, SLOT(networkButton()));
	
	//��ʽ��
	ui.LeftImageFirstlabel->setFrameShape(QFrame::Box);
	ui.LeftImageFirstlabel->setStyleSheet("border: 2px solid  rgb(0, 0, 0)");
	ui.LeftImageFirstlabel->setStyleSheet("background-color: rgb(0, 0, 0);");

	ui.LeftImageSecondlabel->setFrameShape(QFrame::Box);
	ui.LeftImageSecondlabel->setStyleSheet("border: 2px solid  rgb(0, 0, 0)");
	ui.LeftImageSecondlabel->setStyleSheet("background-color: rgb(0, 0, 0);");

	ui.RightImageFirstlabel->setFrameShape(QFrame::Box);
	ui.RightImageFirstlabel->setStyleSheet("border: 2px solid  rgb(0, 0, 0)");
	ui.RightImageFirstlabel->setStyleSheet("background-color: rgb(0, 0, 0);");

	ui.RightImageSecondlabel->setFrameShape(QFrame::Box);
	ui.RightImageSecondlabel->setStyleSheet("border: 2px solid  rgb(0, 0, 0)");
	ui.RightImageSecondlabel->setStyleSheet("background-color: rgb(0, 0, 0);");
	//tabҳ
	ui.toolBox->setCurrentIndex(0);
	ui.tabWidget->setCurrentIndex(0);
	ui.tabWidget_2->setCurrentIndex(0);
	ui.tabWidget_3->setCurrentIndex(0);
	//��ȡ�����ļ����ļ�����
	QString fileName = "ConfigFile/engineNumFileName.txt";
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Warnning", "engineNumFileName File can't open", QMessageBox::Yes);
	}
	QTextStream in(&file);
	engineNumFileName = in.readLine();
	file.close();
	/*vp->engineNumFileName = engineNumFileName;*///���������ɱ��룬��������ʱ���������ǽ�������������receiveSocket����λ��
}

DisplayGUI::~DisplayGUI()
{
	delete cap;
	delete motion;
	delete alg;
	delete serial;
	delete socket;
	delete vp;
	delete login;
	cap = nullptr;
	motion = nullptr;
	alg = nullptr;
	serial = nullptr;
	socket = nullptr;
	vp = nullptr;
	login = nullptr;
}

void DisplayGUI::networkActionSlot()
{
	socket->show();
}

void DisplayGUI::receiveSocket(QString info)
{
	resolveSignal(info);
	//socketNum = info.toInt();
	cout << "�������� = " << socketNum << endl;
	vp->engineNumFileName = engineNumFileName;
	if (engineVersion == "H")
	{
		switch (socketNum)
		{
		case 101:{
					 cout << "101" << endl;
					 ui.toolBox->setCurrentIndex(0);
					 ui.tabWidget_2->setCurrentIndex(0);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����1��һ����"), 0);
					 stepOneFirst();
					 statusUpdate(QStringLiteral("����1��һ���ֽ���"), 0);
					 break;
		}
		case 102:{
					 cout << "102" << endl;
					 ui.toolBox->setCurrentIndex(0);
					 ui.tabWidget_2->setCurrentIndex(0);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����1�ڶ�����"), 0);
					 stepOneSecond();
					 statusUpdate(QStringLiteral("����1�ڶ����ֽ���"), 0);
					 break;
		}
		case 201:{
				   cout << "201" << endl;
				   ui.toolBox->setCurrentIndex(0);
				   ui.tabWidget_2->setCurrentIndex(1);
				   socket->send("OK");
				   statusUpdate(QStringLiteral("��ʼ����2��һ����"), 0);
				   stepTwoFirst();
				   statusUpdate(QStringLiteral("����2��һ���ֽ���"), 0);
				   break;
		}
		case 202:{
				   cout << "202" << endl;
				   ui.toolBox->setCurrentIndex(0);
				   ui.tabWidget_2->setCurrentIndex(1);
				   socket->send("OK");
				   statusUpdate(QStringLiteral("��ʼ����2�ڶ�����"), 0);
				   stepTwoSecond();
				   statusUpdate(QStringLiteral("����2�ڶ����ֽ���"), 0);
				   break;
		}
		case 301:{
					 cout << "301" << endl;
					 ui.toolBox->setCurrentIndex(0);
					 ui.tabWidget_2->setCurrentIndex(2);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����3��һ����"), 0);
					 stepThreeFirst();
					 statusUpdate(QStringLiteral("����3��һ���ֽ���"), 0);
					 break;
		}
		case 302:{
					 cout << "302" << endl;
					 ui.toolBox->setCurrentIndex(0);
					 ui.tabWidget_2->setCurrentIndex(2);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����3�ڶ�����"), 0);
					 stepThreeSecond();
					 statusUpdate(QStringLiteral("����3�ڶ����ֽ���"), 0);
					 break;
		}
		case 401:{
				   ui.toolBox->setCurrentIndex(0);
				   ui.tabWidget_2->setCurrentIndex(3);
				   cout << "4" << endl;
				   socket->send("OK");
				   statusUpdate(QStringLiteral("��ʼ����4��һ����"), 0);
				   stepFourFirst();
				   statusUpdate(QStringLiteral("����4��һ���ֽ���"), 0);
				   
				   break;
		}
		case 402:{
					 ui.toolBox->setCurrentIndex(0);
					 ui.tabWidget_2->setCurrentIndex(3);
					 cout << "4" << endl;
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����4�ڶ�����"), 0);
					 stepFourSecond();
					 statusUpdate(QStringLiteral("����4�ڶ����ֽ���"), 0);
					 
					 break;
		}
		case 501:{
					 ui.toolBox->setCurrentIndex(0);
					 ui.tabWidget_2->setCurrentIndex(4);
					 cout << "5" << endl;
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����5��һ����"), 0);
					 stepFiveFirst();
					 statusUpdate(QStringLiteral("����5��һ���ֽ���"), 0);
					
					 break;
		}
		case 502:{
					 ui.toolBox->setCurrentIndex(0);
					 ui.tabWidget_2->setCurrentIndex(4);
					 cout << "5" << endl;
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����5�ڶ�����"), 0);
					 stepFiveSecond();
					 statusUpdate(QStringLiteral("����5�ڶ����ֽ���"), 0);
					 
					 break;
		}
		default:{
					cout << "�����������" << endl;
					statusUpdate(QStringLiteral("�����������"), 1);
					socket->send("NG");
					break;
		}

		}
	}
	else if (engineVersion == "V")
	{
		switch (socketNum)
		{
		case 101:{
					 cout << "101" << endl;
					 ui.toolBox->setCurrentIndex(1);
					 ui.tabWidget_3->setCurrentIndex(0);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����1��һ����"), 0);
					 VStepOneFirst();
					 statusUpdate(QStringLiteral("����1��һ���ֽ���"), 0);
					 break;
		}
		case 102:{
					 cout << "102" << endl;
					 ui.toolBox->setCurrentIndex(1);
					 ui.tabWidget_3->setCurrentIndex(0);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����1�ڶ�����"), 0);
					 VStepOneSecond();
					 statusUpdate(QStringLiteral("����1�ڶ����ֽ���"), 0);
					 break;
		}
		case 201:{
					 cout << "201" << endl;
					 ui.toolBox->setCurrentIndex(1);
					 ui.tabWidget_3->setCurrentIndex(1);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����2��һ����"), 0);
					 VStepTwoFirst();
					 statusUpdate(QStringLiteral("����2��һ���ֽ���"), 0);
					 break;
		}
		case 202:{
					 cout << "202" << endl;
					 ui.toolBox->setCurrentIndex(1);
					 ui.tabWidget_3->setCurrentIndex(1);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����2�ڶ�����"), 0);
					 VStepTwoSecond();
					 statusUpdate(QStringLiteral("����2�ڶ����ֽ���"), 0);
					 break;
		}
		case 301:{
					 cout << "301" << endl;
					 ui.toolBox->setCurrentIndex(1);
					 ui.tabWidget_3->setCurrentIndex(2);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����3��һ����"), 0);
					 VStepThreeFirst();
					 statusUpdate(QStringLiteral("����3��һ���ֽ���"), 0);
					 break;
		}
		case 302:{
					 cout << "302" << endl;
					 ui.toolBox->setCurrentIndex(1);
					 ui.tabWidget_3->setCurrentIndex(2);
					 socket->send("OK");
					 statusUpdate(QStringLiteral("��ʼ����3�ڶ�����"), 0);
					 VStepThreeSecond();
					 statusUpdate(QStringLiteral("����3�ڶ����ֽ���"), 0);
					 break;
		}
		default:{
					cout << "�����������" << endl;
					statusUpdate(QStringLiteral("�����������"), 1);
					socket->send("NG");
					break;
		}
	 }
	}
	else
	{
		if (socketNum==0)
		{
		
			 socket->send("OK");
			 statusUpdate(QStringLiteral("�����ļ��гɹ�"), 0);
		
		}
		else
		{
			cout << "�����������" << endl;
			statusUpdate(QStringLiteral("�����������"), 1);
			socket->send("NG");
		}
		
	}
	

}

void DisplayGUI::updateStatusInformation(QString Info,int num)
{
	QDateTime CurrentTime = QDateTime::currentDateTime();
	QString Timestr = CurrentTime.toString("yyyy-MM-dd hh:mm:ss"); // ������ʾ�ĸ�ʽ
	QString statusStr = Timestr + ": " + Info;;
	ui.statusEdit->setCurrentFont(QFont("Helvetica [Cronyx]", 12));
	if (num == 0){
		ui.statusEdit->setTextColor("black");
		ui.statusEdit->append(statusStr);
	}
	if (num == 1){
		ui.statusEdit->setTextColor("red");
		ui.statusEdit->append(statusStr);
	}
	QString fileName = engineNumFileName+"/JournalFile.txt";//д���ļ���Ŀ¼
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);
	out << statusStr << "\n";
	file.close();
}

void DisplayGUI::displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr Cloud_ptr, CoreAlgorithm::StereoCircle centerResult)
{
		// ����Բ�ĵ��ƣ�ָ�룩 
		pcl::PointCloud<pcl::PointXYZ>::Ptr center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
		//������ƴ�С�������ɵĿռ�ԲԲ�����긳������
		center_cloud->resize(1);
		//Բ�����긳ֵ
		{
			(*center_cloud)[0].x = centerResult.center.x;
			(*center_cloud)[0].y = centerResult.center.y;
			(*center_cloud)[0].z = centerResult.center.z;
		}
		//�Ƴ����ڵ���
		viewer_1->removeAllPointClouds();
		//��������
		//���õ�����ɫ
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> cloud_color(Cloud_ptr, 255, 255, 255);
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> center_cloud_color(Cloud_ptr, 255, 0, 0);
		//������ɫ��Ⱦ	
		viewer_1->addPointCloud(Cloud_ptr, cloud_color, "cloud");
		viewer_1->addPointCloud(center_cloud, center_cloud_color, "center_cloud");
		//���õ��ƴ�С
		viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud");
		viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "center_cloud");
		/*viewer_1->resetCamera();
		ui.PCLwidget_1->update();*/
		//��������Բ
		pcl::PointCloud<pcl::PointXYZ>::Ptr theroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);
		createTheoryCircle(centerResult,theroyCirclePoints);
		//Բ�ĵ�������
		//���õ�����ɫ
		//viewer_1->removePointCloud("theroyCircleCloud");
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> theroyCircleCloud_color(theroyCirclePoints, 255, 0, 0);
		//������ɫ��Ⱦ	
		viewer_1->addPointCloud(theroyCirclePoints, theroyCircleCloud_color, "theroyCircleCloud");
		//���õ��ƴ�С
		viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "theroyCircleCloud");
		viewer_1->resetCamera();
		ui.PCLwidget_1->update();
		
}

void DisplayGUI::displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr LeftCloud_ptr/*������*/, CoreAlgorithm::StereoCircle leftCenterResult/*���Բ��*/,
	pcl::PointCloud<pcl::PointXYZ>::Ptr RightCloud_ptr/*�Ҳ����*/, CoreAlgorithm::StereoCircle RightCenterResult/*�Ҳ�Բ��*/)
{
	// ����Բ�ĵ��ƣ�ָ�룩 
	pcl::PointCloud<pcl::PointXYZ>::Ptr left_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr right_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	//������ƴ�С�������ɵĿռ�ԲԲ�����긳������
	left_center_cloud->resize(1);
	right_center_cloud->resize(1);
	//Բ�����긳ֵ
	{
		(*left_center_cloud)[0].x = leftCenterResult.center.x;
		(*left_center_cloud)[0].y = leftCenterResult.center.y;
		(*left_center_cloud)[0].z = leftCenterResult.center.z;

		(*right_center_cloud)[0].x = RightCenterResult.center.x;
		(*right_center_cloud)[0].y = RightCenterResult.center.y;
		(*right_center_cloud)[0].z = RightCenterResult.center.z;

	}
	//�Ƴ����ڵ���
	viewer_1->removeAllPointClouds();
	//��������
	//���õ�����ɫ
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> cloud_color(LeftCloud_ptr, 255, 255, 255);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> leftCenter_cloud_color(left_center_cloud, 255, 0, 0);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> righrCenter_cloud_color(right_center_cloud, 0, 255, 0);
	//������ɫ��Ⱦ	
	viewer_1->addPointCloud(LeftCloud_ptr, cloud_color, "leftCloud");
	viewer_1->addPointCloud(RightCloud_ptr, cloud_color, "rightCloud");
	viewer_1->addPointCloud(left_center_cloud, leftCenter_cloud_color, "leftCenter_cloud");
	viewer_1->addPointCloud(right_center_cloud, righrCenter_cloud_color, "rightCenter_cloud");
	//���õ��ƴ�С
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "leftCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "rightCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "leftCenter_cloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "rightCenter_cloud");
	/*viewer_1->resetCamera();
	ui.PCLwidget_1->update();*/
	//��������Բ
	pcl::PointCloud<pcl::PointXYZ>::Ptr leftTheroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr rightTheroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);
	createTheoryCircle(leftCenterResult, leftTheroyCirclePoints);
	createTheoryCircle(RightCenterResult, rightTheroyCirclePoints);

	//Բ�ĵ�������
	//���õ�����ɫ
	//viewer_1->removePointCloud("theroyCircleCloud");
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> leftTheroyCircleCloud_color(leftTheroyCirclePoints, 255, 0, 0);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> rightTheroyCircleCloud_color(rightTheroyCirclePoints, 255, 0, 0);

	//������ɫ��Ⱦ	
	viewer_1->addPointCloud(leftTheroyCirclePoints, leftTheroyCircleCloud_color, "leftTheroyCirclePoints");
	viewer_1->addPointCloud(rightTheroyCirclePoints, rightTheroyCircleCloud_color, "rightTheroyCirclePoints");
	//���õ��ƴ�С
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "leftTheroyCirclePoints");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "rightTheroyCirclePoints");
	viewer_1->resetCamera();
	ui.PCLwidget_1->update();
}

void DisplayGUI::displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr LeftCloud_ptr/*������*/, 
	CoreAlgorithm::StereoCircle leftCenterResult/*���Բ��*/, 
	pcl::PointCloud<pcl::PointXYZ>::Ptr RightCloud_ptr/*�Ҳ����*/, 
	CoreAlgorithm::StereoCircle RightCenterResult/*�Ҳ�Բ��*/, 
	pcl::PointCloud<pcl::PointXYZ>::Ptr LeftFlangeHoleCloud_ptr/*��෨���׵���*/, 
	CoreAlgorithm::StereoCircle leftFlangeHoleCenterResult/*��෨����Բ��*/, 
	pcl::PointCloud<pcl::PointXYZ>::Ptr RightFlangeHoleCloud_ptr/*�Ҳ෨���׵���*/, 
	CoreAlgorithm::StereoCircle RightFlangeHoleCenterResult/*�Ҳ෨����Բ��*/)
{
	// ����Բ�ĵ��ƣ�ָ�룩 
	//ֹ��Բ��
	pcl::PointCloud<pcl::PointXYZ>::Ptr left_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr right_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	//������Բ��
	pcl::PointCloud<pcl::PointXYZ>::Ptr leftFlangeHole_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr rightFlangeHole_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	//������ƴ�С�������ɵĿռ�ԲԲ�����긳������
	left_center_cloud->resize(1);
	right_center_cloud->resize(1);
	leftFlangeHole_center_cloud->resize(1);
	rightFlangeHole_center_cloud->resize(1);
	//Բ�����긳ֵ
	{
		(*left_center_cloud)[0].x = leftCenterResult.center.x;
		(*left_center_cloud)[0].y = leftCenterResult.center.y;
		(*left_center_cloud)[0].z = leftCenterResult.center.z;

		(*right_center_cloud)[0].x = RightCenterResult.center.x;
		(*right_center_cloud)[0].y = RightCenterResult.center.y;
		(*right_center_cloud)[0].z = RightCenterResult.center.z;

		(*leftFlangeHole_center_cloud)[0].x = leftFlangeHoleCenterResult.center.x;
		(*leftFlangeHole_center_cloud)[0].y = leftFlangeHoleCenterResult.center.y;
		(*leftFlangeHole_center_cloud)[0].z = leftFlangeHoleCenterResult.center.z;

		(*rightFlangeHole_center_cloud)[0].x = RightFlangeHoleCenterResult.center.x;
		(*rightFlangeHole_center_cloud)[0].y = RightFlangeHoleCenterResult.center.y;
		(*rightFlangeHole_center_cloud)[0].z = RightFlangeHoleCenterResult.center.z;

	}
	//�Ƴ����ڵ���
	viewer_1->removeAllPointClouds();
	//��������
	//���õ�����ɫ
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> cloud_color(LeftCloud_ptr, 255, 255, 255);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> leftCenter_cloud_color(left_center_cloud, 255, 0, 0);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> righrCenter_cloud_color(right_center_cloud, 0, 255, 0);

	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> leftFlangeHolecenter_cloud_color(leftFlangeHole_center_cloud, 0, 0, 255);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> rightFlangeHolecenter_cloud_color(leftFlangeHole_center_cloud, 255, 255, 0);
	//������ɫ��Ⱦ	
	viewer_1->addPointCloud(LeftCloud_ptr, cloud_color, "leftCloud");
	viewer_1->addPointCloud(RightCloud_ptr, cloud_color, "rightCloud");
	viewer_1->addPointCloud(left_center_cloud, leftCenter_cloud_color, "leftCenter_cloud");
	viewer_1->addPointCloud(right_center_cloud, righrCenter_cloud_color, "rightCenter_cloud");

	viewer_1->addPointCloud(LeftFlangeHoleCloud_ptr, cloud_color, "leftFlangeHoleCloud");
	viewer_1->addPointCloud(RightFlangeHoleCloud_ptr, cloud_color, "rightFlangeHoleCloud");
	viewer_1->addPointCloud(leftFlangeHole_center_cloud, leftFlangeHolecenter_cloud_color, "leftFlangeHoleCenter_cloud");
	viewer_1->addPointCloud(rightFlangeHole_center_cloud, rightFlangeHolecenter_cloud_color, "rightFlangeHoleCenter_cloud");
	//���õ��ƴ�С
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "leftCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "rightCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "leftCenter_cloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "rightCenter_cloud");

	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "leftFlangeHoleCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "rightFlangeHoleCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "leftFlangeHoleCenter_cloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "rightFlangeHoleCenter_cloud");
	/*viewer_1->resetCamera();
	ui.PCLwidget_1->update();*/
	//��������Բ
	pcl::PointCloud<pcl::PointXYZ>::Ptr leftTheroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr rightTheroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);
	createTheoryCircle(leftCenterResult, leftTheroyCirclePoints);
	createTheoryCircle(RightCenterResult, rightTheroyCirclePoints);

	//Բ�ĵ�������
	//���õ�����ɫ
	//viewer_1->removePointCloud("theroyCircleCloud");
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> leftTheroyCircleCloud_color(leftTheroyCirclePoints, 255, 0, 0);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> rightTheroyCircleCloud_color(rightTheroyCirclePoints, 255, 0, 0);

	//������ɫ��Ⱦ	
	viewer_1->addPointCloud(leftTheroyCirclePoints, leftTheroyCircleCloud_color, "leftTheroyCirclePoints");
	viewer_1->addPointCloud(rightTheroyCirclePoints, rightTheroyCircleCloud_color, "rightTheroyCirclePoints");
	//���õ��ƴ�С
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "leftTheroyCirclePoints");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "rightTheroyCirclePoints");
	viewer_1->resetCamera();
	ui.PCLwidget_1->update();
}

void DisplayGUI::displayCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr LeftCloud_ptr/*������*/,
	CoreAlgorithm::StereoCircle leftCenterResult/*���Բ��*/, 
	pcl::PointCloud<pcl::PointXYZ>::Ptr RightCloud_ptr/*�Ҳ����*/, 
	CoreAlgorithm::StereoCircle RightCenterResult/*�Ҳ�Բ��*/, 
	pcl::PointCloud<pcl::PointXYZ>::Ptr FlangeHoleCloud_ptr/*ĳ�෨���׵���*/, 
	CoreAlgorithm::StereoCircle FlangeHoleCenterResult/*ĳ�෨����Բ��*/)
{
	// ����Բ�ĵ��ƣ�ָ�룩 
	//ֹ��Բ��
	pcl::PointCloud<pcl::PointXYZ>::Ptr left_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr right_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	//������Բ��
	pcl::PointCloud<pcl::PointXYZ>::Ptr FlangeHole_center_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	//������ƴ�С�������ɵĿռ�ԲԲ�����긳������
	left_center_cloud->resize(1);
	right_center_cloud->resize(1);
	FlangeHole_center_cloud->resize(1);
	FlangeHole_center_cloud->resize(1);
	//Բ�����긳ֵ
	{
		(*left_center_cloud)[0].x = leftCenterResult.center.x;
		(*left_center_cloud)[0].y = leftCenterResult.center.y;
		(*left_center_cloud)[0].z = leftCenterResult.center.z;

		(*right_center_cloud)[0].x = RightCenterResult.center.x;
		(*right_center_cloud)[0].y = RightCenterResult.center.y;
		(*right_center_cloud)[0].z = RightCenterResult.center.z;

		(*FlangeHole_center_cloud)[0].x = FlangeHoleCenterResult.center.x;
		(*FlangeHole_center_cloud)[0].y = FlangeHoleCenterResult.center.y;
		(*FlangeHole_center_cloud)[0].z = FlangeHoleCenterResult.center.z;

	}
	//�Ƴ����ڵ���
	viewer_1->removeAllPointClouds();
	//��������
	//���õ�����ɫ
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> cloud_color(LeftCloud_ptr, 255, 255, 255);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> leftCenter_cloud_color(left_center_cloud, 255, 0, 0);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> righrCenter_cloud_color(right_center_cloud, 0, 255, 0);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> FlangeHolecenter_cloud_color(FlangeHole_center_cloud, 0, 0, 255);
	//������ɫ��Ⱦ	
	viewer_1->addPointCloud(LeftCloud_ptr, cloud_color, "leftCloud");
	viewer_1->addPointCloud(RightCloud_ptr, cloud_color, "rightCloud");
	viewer_1->addPointCloud(left_center_cloud, leftCenter_cloud_color, "leftCenter_cloud");
	viewer_1->addPointCloud(right_center_cloud, righrCenter_cloud_color, "rightCenter_cloud");

	viewer_1->addPointCloud(FlangeHoleCloud_ptr, cloud_color, "FlangeHoleCloud");
	viewer_1->addPointCloud(FlangeHole_center_cloud, FlangeHolecenter_cloud_color, "FlangeHoleCenter_cloud");

	//���õ��ƴ�С
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "leftCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "rightCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "leftCenter_cloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "rightCenter_cloud");

	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "FlangeHoleCloud");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "FlangeHoleCenter_cloud");
	//viewer_1->resetCamera();
	//ui.PCLwidget_1->update();
	//��������Բ
	pcl::PointCloud<pcl::PointXYZ>::Ptr leftTheroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr rightTheroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);
	createTheoryCircle(leftCenterResult, leftTheroyCirclePoints);
	createTheoryCircle(RightCenterResult, rightTheroyCirclePoints);

	//Բ�ĵ�������
	//���õ�����ɫ
	//viewer_1->removePointCloud("theroyCircleCloud");
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> leftTheroyCircleCloud_color(leftTheroyCirclePoints, 255, 0, 0);
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> rightTheroyCircleCloud_color(rightTheroyCirclePoints, 255, 0, 0);

	//������ɫ��Ⱦ	
	viewer_1->addPointCloud(leftTheroyCirclePoints, leftTheroyCircleCloud_color, "leftTheroyCirclePoints");
	viewer_1->addPointCloud(rightTheroyCirclePoints, rightTheroyCircleCloud_color, "rightTheroyCirclePoints");
	//���õ��ƴ�С
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "leftTheroyCirclePoints");
	viewer_1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 0.5, "rightTheroyCirclePoints");
	viewer_1->resetCamera();
	ui.PCLwidget_1->update();
}

void DisplayGUI::displayImage(HObject Image, int num)
{
	Mat MImage;
	MImage = HImageToIplImage(Image);
	QImage img;
	img = cvMat2QImage(MImage);
	if (num == 101){
		ui.LeftImageFirstlabel->clear();
		ui.LeftImageFirstlabel->setPixmap(QPixmap::fromImage(img).scaled(QSize(MImage.cols / 8, MImage.rows / 8)));
		ui.LeftImageFirstlabel->resize(ui.LeftImageFirstlabel->pixmap()->size());
	}
	else if (num == 102){
		ui.LeftImageSecondlabel->clear();
		ui.LeftImageSecondlabel->setPixmap(QPixmap::fromImage(img).scaled(QSize(MImage.cols / 8, MImage.rows / 8)));
		ui.LeftImageSecondlabel->resize(ui.LeftImageSecondlabel->pixmap()->size());
	}
	else if (num == 201){
		ui.RightImageFirstlabel->clear();
		ui.RightImageFirstlabel->setPixmap(QPixmap::fromImage(img).scaled(QSize(MImage.cols / 8, MImage.rows / 8)));
		ui.RightImageFirstlabel->resize(ui.RightImageFirstlabel->pixmap()->size());
	}
	else if (num == 202){
		ui.RightImageSecondlabel->clear();
		ui.RightImageSecondlabel->setPixmap(QPixmap::fromImage(img).scaled(QSize(MImage.cols / 8, MImage.rows / 8)));
		ui.RightImageSecondlabel->resize(ui.RightImageSecondlabel->pixmap()->size());
	}
}

cv::Mat DisplayGUI::HImageToIplImage(HObject &Hobj)
{
	HTuple htCh = HTuple();
	HTuple cType;
	Mat Image;
	ConvertImageType(Hobj, &Hobj, "byte");
	CountChannels(Hobj, &htCh);
	HTuple wid;
	HTuple hgt;
	int W, H;
	if (htCh[0].I() == 1)
	{
		HTuple ptr;
		GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		W = (Hlong)wid;
		H = (Hlong)hgt;
		Image.create(H, W, CV_8UC1);
		uchar* pdata = (uchar*)ptr[0].I();
		memcpy(Image.data, pdata, W*H);
	}
	else if (htCh[0].I() == 3)
	{
		HTuple ptrR, ptrG, ptrB;
		GetImagePointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
		W = (Hlong)wid;
		H = (Hlong)hgt;
		Image.create(H, W, CV_8UC3);
		vector<Mat> vecM(3);
		vecM[2].create(H, W, CV_8UC1);
		vecM[1].create(H, W, CV_8UC1);
		vecM[0].create(H, W, CV_8UC1);
		uchar* pr = (uchar*)ptrR[0].I();
		uchar* pg = (uchar*)ptrG[0].I();
		uchar* pb = (uchar*)ptrB[0].I();
		memcpy(vecM[2].data, pr, W*H);
		memcpy(vecM[1].data, pg, W*H);
		memcpy(vecM[0].data, pb, W*H);
		merge(vecM, Image);
	}
	return Image;
}

QImage DisplayGUI::cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat  
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{

		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

void DisplayGUI::motionActionSlot()
{
	motion->show();
}

void DisplayGUI::initialization()
{
	
	
	socket->on_ConnectButton_clicked();
#ifndef SETUP
	double startPosition;
	double endPosition;
	cap->initializationCamera();
	motion->initializationMotion();
	startPosition = motion->GetCurrentPositon();
	//��ʼʱ�ɼ�һ��
	/**
	*������Ϊ���ֳ���������������һ��
	*�ɼ����������̫׼ȷ
	*/
	//motion->leadRailMotion(50);
	HObject IMG;
	//��ȡ�����˶��ٶ�
	double speed;
	speed = cap->RateToSpeed(cap->hv_RateValue);
	//�������쿪ʼ�˶�
	//sendPLC();
	motion->setLeadRailVelocity(speed);
	motion->setLeadRailMotionDistance(50);
	IMG = cap->captureImage();
	motion->CheckRun();
	Sleep(100);
	endPosition = motion->GetCurrentPositon();
	motion->reset(startPosition, endPosition);
	motion->CheckRun();
#endif
	
	socket->send("readyOK");

}

void DisplayGUI::cameraActionSlot()
{
	cap->show();
}

void DisplayGUI::displayDataActionSlot()
{
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	QProcess::startDetached("DisplayMessage.exe", strList);
}


void DisplayGUI::LeftScannerRun(int num, pcl::PointCloud<pcl::PointXYZ>::Ptr &borderCloud_ptr, CoreAlgorithm::StereoCircle &centerResult)
{
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	double startPosition;
	double endPosition;
	double position;
	//***��������**************************************************************//

	//��ȡ������ز���
	emit sendStepNum(num);

	//��ȡ���쵱ǰλ��
#ifndef SETUP
	startPosition = motion->GetCurrentPositon();
#endif //SETUP
	
	//�������ͼ
	HObject inputImageFirst, inputImageSecond;
	//��������
	double speed;
	Vector<Point3f> points;
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	start = clock();
#ifndef SETUP
	//��ɾ�߼�������
	serial->clearGratingData();
	Sleep(100);
	//���õ����˶��ٶ�
	motion->setLeadRailVelocity(CameraNoScanSpeed);
	//�˶�����һ�βɼ�λ��
	motion->setLeadRailMotionDistance(CameraFirstNoScanDistance);
	motion->CheckRun();
	cout << "��ʼ�ɼ�λ�ã�" << endl;
	position=motion->GetCurrentPositon();
	Sleep(500);
	//��ȡ��ɾ�߷��������˶�����
	serial->readGratingData(leftRunDis);
	Sleep(500);
	//��ȡ�����˶��ٶ�
	speed =cap-> RateToSpeed(cap->hv_RateValue);
	//���õ����˶��ٶ�
	motion->setLeadRailVelocity(speed);	
	//��ȡͼ��
	GrabImageStart(cap->hv_AcqHandle, -1);
	//�������
	cap->sendPLC();
	//�������쿪ʼ�˶�, �ƶ�����Ϊ50mm
	motion->setLeadRailMotionDistance(CameraScanDistance);
	GrabImageAsync(&inputImageFirst, cap->hv_AcqHandle, -1);
#endif //SETUP

#ifndef READIMAGE
	QString ImageFileName;
	switch (num)
	{
	case 101:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 201:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";;
		break;
	case 203:
		ImageFileName = "edge/hole1.tif";
		break;
	case 301:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";;
		break;
	case 303:
		ImageFileName = "edge/hole1.tif";
		break;
	case 401:
		ImageFileName = "gear/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 501:
		ImageFileName = "gear/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 1010:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 2010:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";;
		break;
	case 2030:
		ImageFileName = "edge/hole1.tif";
		break;
	case 3010:
		ImageFileName = "gear/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
		break;
	default:
		break;
	}
	
	cout << ImageFileName.toStdString() << endl << endl;
	QByteArray ba = ImageFileName.toLatin1();
	const char *str = ba.data();
	cout << ImageFileName.toStdString() << endl << endl;
	HTuple  ImageName(str);
	ReadImage(&inputImageFirst, ImageName);
#endif // !READIMAGE
	//��ʾͼƬ
	displayImage(inputImageFirst, 101);
	//����ͼƬ
	SaveImage(inputImageFirst,"LeftFirst");
	
#ifndef SETUP
	if (num == 203 || num == 303 || num == 2030){
		HObject  ho_EmptyRegion;
		GenEmptyRegion(&ho_EmptyRegion);
		RegionToBin(ho_EmptyRegion, &inputImageSecond, 255, 0, 4096, 2000);
	}
	else{
		motion->CheckRun();
		//���õ����˶��ٶ�
		motion->setLeadRailVelocity(CameraNoScanSpeed);
		//�˶����ڶ��βɼ�λ��
		motion->setLeadRailMotionDistance(CameraSecondNoScanDistance);
		motion->CheckRun();
		cout << "�ڶ��βɼ�λ�ã�" << endl;
		position = motion->GetCurrentPositon();
		Sleep(500);
		//��ȡ��ɾ�߷��������˶�����
		serial->readGratingData(leftRunDis);
		Sleep(500);
		//�������쿪ʼ�˶�
		motion->setLeadRailVelocity(speed);
		//�������쿪ʼ�˶�, �ƶ�����Ϊ50mm

		//��ȡͼ��
		GrabImageStart(cap->hv_AcqHandle, -1);
		cap->sendPLC();
		motion->setLeadRailMotionDistance(CameraScanDistance);
		GrabImageAsync(&inputImageSecond, cap->hv_AcqHandle, -1);
	}
	

#endif //SETUP

#ifndef READIMAGE
	switch (num)
	{
	case 101:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 201:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 301:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 401:
		ImageFileName = "gear/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 501:
		ImageFileName = "gear/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 1010:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 2010:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 3010:
		ImageFileName = "gear/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	default:
		break;
	}
	if (num != 203 && num != 303 && num != 2030){
		ba = ImageFileName.toLatin1();
		const char *str1 = ba.data();
		HTuple  ImageName1(str1);
		ReadImage(&inputImageSecond, ImageName1);
	}
	else{
		HObject  ho_EmptyRegion;
		GenEmptyRegion(&ho_EmptyRegion);
		RegionToBin(ho_EmptyRegion, &inputImageSecond, 255, 0, 4096, 2000);
	}
	
#endif //READIMAGE
	//��ʾͼƬ
	displayImage(inputImageSecond, 102);
	//����ͼƬ
	SaveImage(inputImageSecond, "LeftSecond");	
	
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	cout << "����" << endl;
	cout << "�˶�����λ�ã�" << endl;
	position=motion->GetCurrentPositon();
	Sleep(500);
	//��ȡ�ɼ�����λ��
	endPosition = motion->GetCurrentPositon();
	//���츴λ
	motion->reset(startPosition,endPosition);
	cout << "����" << endl;
	//��������
	emit sendLeftGratingData(leftRunDis);
	//������գ�Ϊ�´δ�����׼��
	leftRunDis.clear();
#endif //SETUP

#ifndef PROGRESS
	switch (num)
	{
	case 101:
		emit sendProcessingInformation(101, inputImageFirst, inputImageSecond);
		break;
	case 201:
		emit sendProcessingInformation(201, inputImageFirst, inputImageSecond);
		break;
	case 203:
		emit sendProcessingInformation(203, inputImageFirst, inputImageSecond);
		break;
	case 301:
		emit sendProcessingInformation(301, inputImageFirst, inputImageSecond);
		break;
	case 303:
		emit sendProcessingInformation(303, inputImageFirst, inputImageSecond);
		break;
	case 401:
		emit sendProcessingInformation(401, inputImageFirst, inputImageSecond);
		break;
	case 501:
		emit sendProcessingInformation(501, inputImageFirst, inputImageSecond);
		break;
	case 1010:
		emit sendProcessingInformation(1010, inputImageFirst, inputImageSecond);
		break;
	case 2010:
		emit sendProcessingInformation(2010, inputImageFirst, inputImageSecond);
		break;
	case 2030:
		emit sendProcessingInformation(2030, inputImageFirst, inputImageSecond);
		break;
	case 3010:
		emit sendProcessingInformation(3010, inputImageFirst, inputImageSecond);
		break;
	case 3030:
		emit sendProcessingInformation(3030, inputImageFirst, inputImageSecond);
		break;
	default:
		break;
	}
#endif //PROGRESS
	cout << "over!" << endl;
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	//��ձ���
	points.clear();

	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;
}

void DisplayGUI::RightScannerRun(int num, pcl::PointCloud<pcl::PointXYZ>::Ptr &borderCloud_ptr, CoreAlgorithm::StereoCircle &centerResult)
{
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	double startPosition;
	double endPosition;
	double position;
	//***��������**************************************************************//
	//��ȡ���쵱ǰλ��
#ifndef SETUP
	startPosition = motion->GetCurrentPositon();
#endif //SETUP
	//��ȡ������ز���
	emit sendStepNum(num);
	//�������ͼ
	HObject inputImageFirst, inputImageSecond;
	//��������
	double speed;
	Vector<Point3f> points;
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	start = clock();
#ifndef SETUP
	serial->clearGratingData();
	Sleep(100);
	motion->setLeadRailVelocity(CameraNoScanSpeed);
	//�˶�����һ�βɼ�λ��
	motion->setLeadRailMotionDistance(CameraFirstNoScanDistance);
	motion->CheckRun();	
	cout << "��ʼ�ɼ�λ�ã�" << endl;
	position = motion->GetCurrentPositon();
	Sleep(500);
	//��ȡ��ɾ�߷��������˶�����
	serial->readGratingData(rightRunDis);
	Sleep(500);
	//��ȡ�����˶��ٶ�
	speed = cap->RateToSpeed(cap->hv_RateValue);
	//���õ����˶��ٶ�
	motion->setLeadRailVelocity(speed);
	//��ȡͼ��
	GrabImageStart(cap->hv_AcqHandle, -1);
	cap->sendPLC();
	//�������쿪ʼ�˶�, �ƶ�����Ϊ50mmCameraScanDistance
	motion->setLeadRailMotionDistance(CameraScanDistance);
	GrabImageAsync(&inputImageFirst, cap->hv_AcqHandle, -1);
#endif //SETUP

#ifndef READIMAGE
	QString ImageFileName;
	switch (num)
	{
	case 102:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 104:
		ImageFileName = "edge/LeftFirst2.tif";
		break;
	case 202:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 204:
		ImageFileName = "edge/hole1.tif";
		break;
	case 302:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 304:
		ImageFileName = "edge/hole1.tif";
		break;
	case 402:
		ImageFileName = "gear/RightFirst" + ui.numEdit->text() + ".tif";
		break;
	case 502:
		ImageFileName = "gear/RightFirst" + ui.numEdit->text() + ".tif";
		break;
	case 1020:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 1040:
		ImageFileName = "edge/LeftFirst2.tif";
		break;
	case 2020:
		ImageFileName = "edge/LeftFirst" + ui.numEdit->text() + ".tif";
		break;
	case 2040:
		ImageFileName = "edge/hole1.tif";
		break;
	case 3020:
		ImageFileName = "gear/RightFirst" + ui.numEdit->text() + ".tif";
		break;
	default:
		break;
	}
	cout << ImageFileName.toStdString() << endl << endl;
	QByteArray ba = ImageFileName.toLatin1();
	const char *str = ba.data();
	cout << ImageFileName.toStdString() << endl << endl;
	HTuple  ImageName(str);
	ReadImage(&inputImageFirst, ImageName);
#endif // !READIMAGE
	//��ʾͼƬ
	displayImage(inputImageFirst, 201);
	//����ͼƬ
	SaveImage(inputImageFirst, "RightFirst");

#ifndef SETUP
	if (num == 204 || num == 304||num == 2040 ){
		HObject  ho_EmptyRegion;
		GenEmptyRegion(&ho_EmptyRegion);
		RegionToBin(ho_EmptyRegion, &inputImageSecond, 255, 0, 4096, 2000);
	}
	else{
		motion->CheckRun();
		Sleep(100);
		//���õ����˶��ٶ�
		motion->setLeadRailVelocity(CameraNoScanSpeed);
		//�˶����ڶ��βɼ�λ��
		motion->setLeadRailMotionDistance(CameraSecondNoScanDistance);
		motion->CheckRun();
		cout << "�ڶ��βɼ�λ�ã�" << endl;
		position = motion->GetCurrentPositon();
		Sleep(500);
		//��ȡ��ɾ�߷��������˶�����
		serial->readGratingData(rightRunDis);
		Sleep(500);
		//�������쿪ʼ�˶�
		motion->setLeadRailVelocity(speed);
		//�������쿪ʼ�˶�, �ƶ�����Ϊ50mm

		//��ȡͼ��
		GrabImageStart(cap->hv_AcqHandle, -1);
		cap->sendPLC();
		motion->setLeadRailMotionDistance(CameraScanDistance);
		GrabImageAsync(&inputImageSecond, cap->hv_AcqHandle, -1);
	}


#endif //SETUP

#ifndef READIMAGE
	switch (num)
	{
	case 102:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 104:
		ImageFileName = "edge/LeftSecond2.tif";
		break;
	case 202:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	/*case 204:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;*/
	case 302:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 402:
		ImageFileName = "gear/RightSecond" + ui.numEdit->text() + ".tif";
		break;
	case 502:
		ImageFileName = "gear/RightSecond" + ui.numEdit->text() + ".tif";
		break;
	case 1020:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 1040:
		ImageFileName = "edge/LeftSecond2.tif";
		break;
	case 2020:
		ImageFileName = "edge/LeftSecond" + ui.numEdit->text() + ".tif";
		break;
	case 3020:
		ImageFileName = "gear/RightSecond" + ui.numEdit->text() + ".tif";
		break;
	default:
		break;
	}
	if (num != 204 && num != 304 && num != 2040){
		ba = ImageFileName.toLatin1();
		const char *str1 = ba.data();
		HTuple  ImageName1(str1);
		ReadImage(&inputImageSecond, ImageName1);
	}
	else{
		HObject  ho_EmptyRegion;
		GenEmptyRegion(&ho_EmptyRegion);
		RegionToBin(ho_EmptyRegion, &inputImageSecond, 255, 0, 4096, 2000);
	}

	
#endif //READIMAGE


	//��ʾͼƬ
	displayImage(inputImageSecond, 202);
	//����ͼƬ
	SaveImage(inputImageSecond, "RightSecond");
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	cout << "����" << endl;
	cout << "�˶�����λ�ã�" << endl;
	position = motion->GetCurrentPositon();
	Sleep(500);
	//��ȡ�ɼ�����λ��
	endPosition = motion->GetCurrentPositon();
	//���츴λ
	motion->reset(startPosition, endPosition);
	cout << "����" << endl;
	//��������
	emit sendRightGratingData(rightRunDis);
	//������գ�Ϊ�´δ�����׼��
	rightRunDis.clear();
#endif //SETUP

#ifndef PROGRESS 
	switch (num)
	{
	case 102:
		emit sendProcessingInformation(102, inputImageFirst, inputImageSecond);
		break;
	case 104:
		emit sendProcessingInformation(104, inputImageFirst, inputImageSecond);
		break;
	case 202:
		emit sendProcessingInformation(202, inputImageFirst, inputImageSecond);
		break;
	case 204:
		emit sendProcessingInformation(204, inputImageFirst, inputImageSecond);
		break;
	case 302:
		emit sendProcessingInformation(302, inputImageFirst, inputImageSecond);
		break;
	case 304:
		emit sendProcessingInformation(304, inputImageFirst, inputImageSecond);
		break;
	case 402:
		emit sendProcessingInformation(402, inputImageFirst, inputImageSecond);
		break;
	case 502:
		emit sendProcessingInformation(502, inputImageFirst, inputImageSecond);
		break;
	case 1020:
		emit sendProcessingInformation(1020, inputImageFirst, inputImageSecond);
		break;
	case 1040:
		emit sendProcessingInformation(1040, inputImageFirst, inputImageSecond);
		break;
	case 2020:
		emit sendProcessingInformation(2020, inputImageFirst, inputImageSecond);
		break;
	case 2040:
		emit sendProcessingInformation(2040, inputImageFirst, inputImageSecond);
		break;
	case 3020:
		emit sendProcessingInformation(3020, inputImageFirst, inputImageSecond);
		break;
	case 3040:
		emit sendProcessingInformation(3040, inputImageFirst, inputImageSecond);
		break;
	default:
		break;
	}
	
#endif //PROGRESS
	cout << "over!" << endl;
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	//��ձ���
	points.clear();

	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;
}

/*void DisplayGUI::on_updataButton_clicked()
{
	for (int i = 0; i < 72; i++){
		lineEdit[i]->setEnabled(true);
	}
}*/

/*void DisplayGUI::on_setButton_clicked()
{
	QMessageBox box;
	box.setWindowTitle(tr("Information"));
	box.setIcon(QMessageBox::Information);
	box.setText(tr("Do you save the datas?"));
	box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	if (box.exec() == QMessageBox::Yes)
	{

		//д���ļ�
		QString fileName = "ConfigFile/ConfigInSteps.txt";//д���ļ���Ŀ¼
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		{
			QMessageBox::warning(this, "ConfigInSteps", "can't open", QMessageBox::Yes);
		}
		QTextStream out(&file);
		for (int i = 0; i < 72; i++){
			out << lineEdit[i]->text().toDouble() << "\n";
		}
		file.close();
		//�ر�ʹ��
	}
	else
	{

	}
	for (int i = 0; i < 72; i++){
		lineEdit[i]->setEnabled(false);
	}
}*/

void DisplayGUI::stepOneFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	LeftScannerRun(101, leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(102, transmissionCaseCircleFirst_cloudPoints, transmissionCaseCenterResultFirst);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP
	cout << "Բ��: " << endl;
	/*cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;*/
	cout << transmissionCaseCenterResultFirst.center.x
		<< " " << transmissionCaseCenterResultFirst.center.y
		<< " " << transmissionCaseCenterResultFirst.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	
	if (leftProcessingStatus == false && rightProcessingStatus == false)
	{
		socket->send("ERROR101");
	}
	else{
		
		QString sendInfo = QString("H101,%1,%2,%3,%4,%5,%6").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B).arg(resultMessage.leftNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (transmissionCaseCenterResultFirst.status == true){
			displayCloud(transmissionCaseCircleFirst_cloudPoints, transmissionCaseCenterResultFirst);
		}
	}
#endif//��PROGRESS
	statusUpdate(QStringLiteral("��λ1��һ�����������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepOneSecond()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif

	RightScannerRun(104, transmissionCaseCircleSecond_cloudPoints, transmissionCaseCenterResultSecond);
	Sleep(200);

#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);

	cout << "Բ��: " << endl;
	cout << transmissionCaseCenterResultSecond.center.x << " " << transmissionCaseCenterResultSecond.center.y << " " << transmissionCaseCenterResultSecond.center.z << endl;

	//���㶨�����β���Բ�ĵķ���ʸ��
	//	directionVector=

#ifndef PROGRESS

	
	if (rightProcessingStatus == false)
	{
		socket->send("ERROR102");
		
	}
	else{
		QString sendInfo = QString("H102,%1,%2,%3,%4,%5,%6").arg(resultMessage.rightCenter.X)
			.arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z)
			.arg(resultMessage.directionVector.A).arg(resultMessage.directionVector.B).arg(resultMessage.directionVector.C);
		socket->send(sendInfo);
		//��ʾ��Ե���Ƽ�Բ��
		displayCloud(transmissionCaseCircleFirst_cloudPoints, transmissionCaseCenterResultFirst,
			transmissionCaseCircleSecond_cloudPoints, transmissionCaseCenterResultSecond);
	}
#endif//��PROGRESS
	statusUpdate(QStringLiteral("��λ1�������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepTwoFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	//�Ȳ�ֹ�ڷ�ʸ�ͱ�ԵԲ��
	LeftScannerRun(201, leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(202, rightCircle_cloudPoints, rightCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;
	cout << rightCenterResult.center.x << " " << rightCenterResult.center.y << " " << rightCenterResult.center.z << endl;
	Sleep(2000);
	//����
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("H201,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B)
			.arg(resultMessage.leftNormal.C)
			.arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y).arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightNormal.A).arg(resultMessage.rightNormal.B).arg(resultMessage.rightNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftCenterResult.status == true&&rightCenterResult.status==false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult);
		}
		if (rightCenterResult.status == true && leftCenterResult.status == false)
		{
			displayCloud(rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftCenterResult.status == true && rightCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
		
	}
	else{
		socket->send("ERROR201");
	}
#endif // !PROGRESS
	statusUpdate(QStringLiteral("��λ2��һ�����������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepTwoSecond()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	//�ٲⷨ����Բ��
	LeftScannerRun(203, leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(204, rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftFlangeHoleCenterResult.center.x << " " << leftFlangeHoleCenterResult.center.y << " " << leftFlangeHoleCenterResult.center.z << endl;
	cout << rightFlangeHoleCenterResult.center.x << " " << rightFlangeHoleCenterResult.center.y << " " << rightFlangeHoleCenterResult.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("H202,%1,%2,%3,%4,%5,%6").arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y)
			.arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftFlangeHoleCenterResult.status == true && rightFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult);
		}
		if (rightFlangeHoleCenterResult.status == true && leftFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
		}
		if (leftFlangeHoleCenterResult.status == false && rightFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftFlangeHoleCenterResult.status == true && rightFlangeHoleCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult, rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
		}

	}
	else{
		socket->send("ERROR202");
	}
#endif // !PROGRESS
	statusUpdate(QStringLiteral("��λ2�ڶ������������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepThreeFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	//�Ȳ�ֹ�ڷ�ʸ�ͱ�ԵԲ��
	LeftScannerRun(301, leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(302, rightCircle_cloudPoints, rightCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;
	cout << rightCenterResult.center.x << " " << rightCenterResult.center.y << " " << rightCenterResult.center.z << endl;
	Sleep(2000);
	//����
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("H301,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B)
			.arg(resultMessage.leftNormal.C)
			.arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y).arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightNormal.A).arg(resultMessage.rightNormal.B).arg(resultMessage.rightNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftCenterResult.status == true && rightCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult);
		}
		if (rightCenterResult.status == true && leftCenterResult.status == false)
		{
			displayCloud(rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftCenterResult.status == true && rightCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
	}
	else{
		socket->send("ERROR301");
	}
#endif // !PROGRESS
	statusUpdate(QStringLiteral("��λ3��һ�����������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepThreeSecond()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	//�ٲⷨ����Բ��
	LeftScannerRun(303, leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(304, rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftFlangeHoleCenterResult.center.x << " " << leftFlangeHoleCenterResult.center.y << " " << leftFlangeHoleCenterResult.center.z << endl;
	cout << rightFlangeHoleCenterResult.center.x << " " << rightFlangeHoleCenterResult.center.y << " " << rightFlangeHoleCenterResult.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("H302,%1,%2,%3,%4,%5,%6").arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y)
			.arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftFlangeHoleCenterResult.status == true && rightFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult);
		}
		if (rightFlangeHoleCenterResult.status == true && leftFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
		}
		if (leftFlangeHoleCenterResult.status == false && rightFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftFlangeHoleCenterResult.status == true && rightFlangeHoleCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult, rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
		}
	}
	else{
		socket->send("ERROR302");
	}
#endif // !PROGRESS
	statusUpdate(QStringLiteral("��λ3�ڶ������������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepFourFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	////�����Ե�����
	//pcl::PointCloud<pcl::PointXYZ>::Ptr leftCircle_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	//pcl::PointCloud<pcl::PointXYZ>::Ptr rightCircle_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	////����ռ�Բ��
	//CoreAlgorithm::StereoCircle leftCenterResult;
	//CoreAlgorithm::StereoCircle rightCenterResult;
	start = clock();
	Sleep(100);
	LeftScannerRun(401,leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(402,rightCircle_cloudPoints, rightCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP
	
	cout << "Բ��: " << endl;
	cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;
	cout << rightCenterResult.center.x << " " << rightCenterResult.center.y << " " << rightCenterResult.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("H401,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B)
			.arg(resultMessage.leftNormal.C)
			.arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y).arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightNormal.A).arg(resultMessage.rightNormal.B).arg(resultMessage.rightNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftCenterResult.status == true && rightCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult);
		}
		if (rightCenterResult.status == true && leftCenterResult.status == false)
		{
			displayCloud(rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftCenterResult.status == true && rightCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
	}
	else{
		socket->send("ERROR401");
	}
#endif// !PROGRESS
	statusUpdate(QStringLiteral("��λ4�������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepFourSecond()
{

}

void DisplayGUI::stepFiveFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	////�����Ե�����
	//pcl::PointCloud<pcl::PointXYZ>::Ptr leftCircle_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	//pcl::PointCloud<pcl::PointXYZ>::Ptr rightCircle_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	////����ռ�Բ��
	//CoreAlgorithm::StereoCircle leftCenterResult;
	//CoreAlgorithm::StereoCircle rightCenterResult;
	start = clock();
	Sleep(100);
	LeftScannerRun(501, leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(502, rightCircle_cloudPoints, rightCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;
	cout << rightCenterResult.center.x << " " << rightCenterResult.center.y << " " << rightCenterResult.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("H501,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B)
			.arg(resultMessage.leftNormal.C)
			.arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y).arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightNormal.A).arg(resultMessage.rightNormal.B).arg(resultMessage.rightNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftCenterResult.status == true && rightCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult);
		}
		if (rightCenterResult.status == true && leftCenterResult.status == false)
		{
			displayCloud(rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftCenterResult.status == true && rightCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
	}
	else{
		socket->send("ERROR501");
	}
#endif// !PROGRESS
	statusUpdate(QStringLiteral("��λ4�������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::stepFiveSecond()
{

}

/*void DisplayGUI::InitializationLineEdit()
{
	//��λ1
	lineEdit[0] = ui.LeftRunFirst_StepOne;
	lineEdit[1] = ui.LeftRunSecond_StepOne;

	lineEdit[2] = ui.RightRunFirst_StepOne;
	lineEdit[3] = ui.RightRunSecond_StepOne;

	lineEdit[4] = ui.LeftRestrictRadius_StepOne;
	lineEdit[5] = ui.RightRestrictRadius_StepOne;

	lineEdit[6] = ui.LeftCameraScan_StepOne;
	lineEdit[7] = ui.RightCameraScan_StepOne;

	//��λ2
	lineEdit[8] = ui.LeftRunFirst_StepTwo;
	lineEdit[9] = ui.LeftRunSecond_StepTwo;

	lineEdit[10] = ui.RightRunFirst_StepTwo;
	lineEdit[11] = ui.RightRunSecond_StepTwo;

	lineEdit[12] = ui.LeftRestrictRadius_StepTwo;
	lineEdit[13] = ui.RightRestrictRadius_StepTwo;

	lineEdit[14] = ui.LeftCameraScan_StepTwo;
	lineEdit[15] = ui.RightCameraScan_StepTwo;

	//��λ3
	lineEdit[16] = ui.LeftRunFirst_StepThree;
	lineEdit[17] = ui.LeftRunSecond_StepThree;

	lineEdit[18] = ui.RightRunFirst_StepThree;
	lineEdit[19] = ui.RightRunSecond_StepThree;

	lineEdit[20] = ui.LeftRestrictRadiu_StepThree;
	lineEdit[21] = ui.RightRestrictRadius_StepThree;

	lineEdit[22] = ui.LeftCameraScan_StepThree;
	lineEdit[23] = ui.RightCameraScan_StepThree;

	//��λ4
	lineEdit[24] = ui.LeftRunFirst_StepFour;
	lineEdit[25] = ui.LeftRunSecond_StepFour;

	lineEdit[26] = ui.RightRunFirst_StepFour;
	lineEdit[27] = ui.RightRunSecond_StepFour;

	lineEdit[28] = ui.LeftRestrictRadius_StepFour;
	lineEdit[29] = ui.RightRestrictRadius_StepFour;

	lineEdit[30] = ui.LeftCameraScan_StepFour;
	lineEdit[31] = ui.RightCameraScan_StepFour;

	//��λ5
	lineEdit[32] = ui.LeftRunFirst_StepFive;
	lineEdit[33] = ui.LeftRunSecond_StepFive;

	lineEdit[34] = ui.RightRunFirst_StepFive;
	lineEdit[35] = ui.RightRunSecond_StepFive;

	lineEdit[36] = ui.LeftRestrictRadius_StepFive;
	lineEdit[37] = ui.RightRestrictRadius_StepFive;

	lineEdit[38] = ui.LeftCameraScan_StepFive;
	lineEdit[39] = ui.RightCameraScan_StepFive;

	//�Ҳ�ͷ����ϵ�����ͷ����ϵ�任����
	lineEdit[40] = ui.RtoL_M00;
	lineEdit[41] = ui.RtoL_M01;
	lineEdit[42] = ui.RtoL_M02;
	lineEdit[43] = ui.RtoL_M03;

	lineEdit[44] = ui.RtoL_M10;
	lineEdit[45] = ui.RtoL_M11;
	lineEdit[46] = ui.RtoL_M12;
	lineEdit[47] = ui.RtoL_M13;

	lineEdit[48] = ui.RtoL_M20;
	lineEdit[49] = ui.RtoL_M21;
	lineEdit[50] = ui.RtoL_M22;
	lineEdit[51] = ui.RtoL_M23;

	lineEdit[52] = ui.RtoL_M30;
	lineEdit[53] = ui.RtoL_M31;
	lineEdit[54] = ui.RtoL_M32;
	lineEdit[55] = ui.RtoL_M33;

	//���۱궨����
	lineEdit[56] = ui.EyeInHand_M00;
	lineEdit[57] = ui.EyeInHand_M01;
	lineEdit[58] = ui.EyeInHand_M02;
	lineEdit[59] = ui.EyeInHand_M03;

	lineEdit[60] = ui.EyeInHand_M10;
	lineEdit[61] = ui.EyeInHand_M11;
	lineEdit[62] = ui.EyeInHand_M12;
	lineEdit[63] = ui.EyeInHand_M13;

	lineEdit[64] = ui.EyeInHand_M20;
	lineEdit[65] = ui.EyeInHand_M21;
	lineEdit[66] = ui.EyeInHand_M22;
	lineEdit[67] = ui.EyeInHand_M23;

	lineEdit[68] = ui.EyeInHand_M30;
	lineEdit[69] = ui.EyeInHand_M31;
	lineEdit[70] = ui.EyeInHand_M32;
	lineEdit[71] = ui.EyeInHand_M33;

}*/

/*void DisplayGUI::ReadConfigParameterFile()
{
	QString fileName = "ConfigFile/ConfigInSteps.txt";
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Warnning", "File can't open", QMessageBox::Yes);
	}
	QTextStream in(&file);
	double variable;
	for (int i = 0; i < 72; i++){
		variable = in.readLine().toDouble();
		lineEdit[i]->setText(QString::number(variable));
	}
}*/

QString DisplayGUI::ReturnTime()
{
	QDateTime CurrentTime = QDateTime::currentDateTime();
	QString Timestr = CurrentTime.toString("yyyy-MM-dd hh-mm-ss"); // ������ʾ�ĸ�ʽ
	return Timestr;
}

void DisplayGUI::SaveImage(HObject Image, QString name)
{
	QString TimeStr;
	QString ImageFileName;
	Mat Img;
	TimeStr = ReturnTime();
	QDir dir;
	bool isExists = dir.exists(engineNumFileName + "/JournalImage");
	if (!isExists){
		dir.mkdir(engineNumFileName + "/JournalImage");
	}
	ImageFileName = engineNumFileName + "/JournalImage/" + TimeStr + " " + name + ".tif";
	QByteArray ba = ImageFileName.toLatin1();
	const char *str = ba.data();
	HTuple  ImageName(str);
	WriteImage(Image, "tiff", 0, ImageName);

}

void DisplayGUI::eyeHandActionSlot()
{
	QStringList strList;//����list
	strList << "" ;//���贫��������в���������������ÿռ���
	QProcess::startDetached("EyeinHand.exe", strList);
}

void DisplayGUI::visualProcessingActionSlot()
{
	login->show();
}


void DisplayGUI::receiveProcessingResult(bool planeparamStatus/*ƽ�淨ʸ����״̬*/,
	CoreAlgorithm::PlaneNormal planeparam/*ƽ�淨ʸ*/, 
	CoreAlgorithm::StereoCircle centerResult/*�ռ�Բ��*/, 
	pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr/*��Ե���� PCL��ʽ*/, 
	int num/*������*/)
{
	/*if (num % 2 == 1){
		leftProcessingStatus = status;
		}
		else{
		rightProcessingStatus = status;
		}*/

	
	if (num == 101){
		if (planeparamStatus == true){
			//������
			ui.StepOne_horizontalEngine_normal_x_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalEngine_normal_y_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalEngine_normal_z_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
			ui.StepOne_horizontalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
			ui.StepOne_horizontalEngine_normal_z_Edit->setText(QString::number(planeparam.C));
			leftPlaneNormal = planeparam;
			leftProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = QString::number(planeparam.A);
			resultMessage.leftNormal.B = QString::number(planeparam.B);
			resultMessage.leftNormal.C = QString::number(planeparam.C);
		}
		else{
			statusUpdate(QStringLiteral("���ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepOne_horizontalEngine_normal_x_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalEngine_normal_y_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalEngine_normal_z_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalEngine_normal_x_Edit->setText("NULL");
			ui.StepOne_horizontalEngine_normal_y_Edit->setText("NULL");
			ui.StepOne_horizontalEngine_normal_z_Edit->setText("NULL");
			leftProcessingStatus = false;		
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
		}
		
	}	
	if (num == 102){
		transmissionCaseCenterResultFirst = centerResult;
		transmissionCaseCircleFirst_cloudPoints = borderCloud_ptr;
		if (transmissionCaseCenterResultFirst.status == true){
			//������
			ui.StepOne_horizontalFrontDriveBox_First_center_x_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_First_center_y_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_First_center_z_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_First_center_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepOne_horizontalFrontDriveBox_First_center_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepOne_horizontalFrontDriveBox_First_center_z_Edit->setText(QString::number(centerResult.center.z));
			rightProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = QString::number(centerResult.center.x);
			resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
			resultMessage.rightCenter.Z = QString::number(centerResult.center.z);
		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepOne_horizontalFrontDriveBox_First_center_x_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_First_center_y_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_First_center_z_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_First_center_x_Edit->setText("NULL");
			ui.StepOne_horizontalFrontDriveBox_First_center_y_Edit->setText("NULL");
			ui.StepOne_horizontalFrontDriveBox_First_center_z_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}
		
	}
	if (num == 104){
		transmissionCaseCenterResultSecond = centerResult;
		transmissionCaseCircleSecond_cloudPoints = borderCloud_ptr;
		if (transmissionCaseCenterResultSecond.status == true)
		{
			//������
			//���㷽��ʸ��
			directionVector.x = transmissionCaseCenterResultFirst.center.x - transmissionCaseCenterResultSecond.center.x;
			directionVector.y = transmissionCaseCenterResultFirst.center.y - transmissionCaseCenterResultSecond.center.y;
			directionVector.z = transmissionCaseCenterResultFirst.center.z - transmissionCaseCenterResultSecond.center.z;
			QString fileName = engineNumFileName + "/outputFile/StepOne/frontDriveBoxDirectionVector.csv";//д���ļ���Ŀ¼
			QString time = ReturnTime();
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time <<","<< directionVector.x << "," << directionVector.y << "," << directionVector.y << "\n";
			file.close();
			ui.StepOne_horizontalFrontDriveBox_Second_center_x_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_Second_center_y_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_Second_center_z_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_directionVector_x_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_directionVector_y_Edit->setStyleSheet("color:black");
			ui.StepOne_horizontalFrontDriveBox_directionVector_z_Edit->setStyleSheet("color:black");

			ui.StepOne_horizontalFrontDriveBox_Second_center_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepOne_horizontalFrontDriveBox_Second_center_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepOne_horizontalFrontDriveBox_Second_center_z_Edit->setText(QString::number(centerResult.center.z));

			ui.StepOne_horizontalFrontDriveBox_directionVector_x_Edit->setText(QString::number(directionVector.x));
			ui.StepOne_horizontalFrontDriveBox_directionVector_y_Edit->setText(QString::number(directionVector.y));
			ui.StepOne_horizontalFrontDriveBox_directionVector_z_Edit->setText(QString::number(directionVector.z));
			rightProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = QString::number(centerResult.center.x);
			resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
			resultMessage.rightCenter.Z = QString::number(centerResult.center.z);
			resultMessage.directionVector.A = QString::number(directionVector.x);
			resultMessage.directionVector.B = QString::number(directionVector.y);
			resultMessage.directionVector.C = QString::number(directionVector.z);
		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepOne_horizontalFrontDriveBox_Second_center_x_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_Second_center_y_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_Second_center_z_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_directionVector_x_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_directionVector_y_Edit->setStyleSheet("color:red");
			ui.StepOne_horizontalFrontDriveBox_directionVector_z_Edit->setStyleSheet("color:red");

			ui.StepOne_horizontalFrontDriveBox_Second_center_x_Edit->setText("NULL");
			ui.StepOne_horizontalFrontDriveBox_Second_center_y_Edit->setText("NULL");
			ui.StepOne_horizontalFrontDriveBox_Second_center_z_Edit->setText("NULL");

			ui.StepOne_horizontalFrontDriveBox_directionVector_x_Edit->setText("NULL");
			ui.StepOne_horizontalFrontDriveBox_directionVector_y_Edit->setText("NULL");
			ui.StepOne_horizontalFrontDriveBox_directionVector_y_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
			resultMessage.directionVector.A = "NULL";
			resultMessage.directionVector.B = "NULL";
			resultMessage.directionVector.C = "NULL";
		}

	}

	if (num == 201){
		leftCenterResult = centerResult;
		leftCircle_cloudPoints = borderCloud_ptr;
		leftPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (leftCenterResult.status == true){
				//������
				ui.StepTwo_horizontalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalEngine_normal_z_Edit->setStyleSheet("color:black");

				ui.StepTwo_horizontalEngine_center_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalEngine_center_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalEngine_center_z_Edit->setStyleSheet("color:black");
				
				ui.StepTwo_horizontalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_horizontalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_horizontalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_horizontalEngine_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepTwo_horizontalEngine_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepTwo_horizontalEngine_center_z_Edit->setText(QString::number(centerResult.center.z));
				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = QString::number(centerResult.center.x);
				resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
				resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("���Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepTwo_horizontalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalEngine_normal_z_Edit->setStyleSheet("color:black");

				ui.StepTwo_horizontalEngine_center_x_Edit->setStyleSheet("color:red");
				ui.StepTwo_horizontalEngine_center_y_Edit->setStyleSheet("color:red");
				ui.StepTwo_horizontalEngine_center_z_Edit->setStyleSheet("color:red");

				ui.StepTwo_horizontalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_horizontalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_horizontalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_horizontalEngine_center_x_Edit->setText("NULL");
				ui.StepTwo_horizontalEngine_center_y_Edit->setText("NULL");
				ui.StepTwo_horizontalEngine_center_z_Edit->setText("NULL");

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = "NULL";
				resultMessage.leftCenter.Y = "NULL";
				resultMessage.leftCenter.Z = "NULL";
			}
		}
		else
		{
			statusUpdate(QStringLiteral("���ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepTwo_horizontalEngine_normal_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalEngine_normal_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalEngine_normal_z_Edit->setStyleSheet("color:red");

			ui.StepTwo_horizontalEngine_center_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalEngine_center_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalEngine_center_z_Edit->setStyleSheet("color:red");

			ui.StepTwo_horizontalEngine_normal_x_Edit->setText("NULL");
			ui.StepTwo_horizontalEngine_normal_y_Edit->setText("NULL");
			ui.StepTwo_horizontalEngine_normal_z_Edit->setText("NULL");

			ui.StepTwo_horizontalEngine_center_x_Edit->setText("NULL");
			ui.StepTwo_horizontalEngine_center_y_Edit->setText("NULL");
			ui.StepTwo_horizontalEngine_center_z_Edit->setText("NULL");

			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}
		

	}
	if (num ==202){
		rightCenterResult = centerResult;
		rightCircle_cloudPoints = borderCloud_ptr;
		rightPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (rightCenterResult.status == true){
				//������
				ui.StepTwo_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:black");


				ui.StepTwo_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_horizontalFrontDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepTwo_horizontalFrontDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepTwo_horizontalFrontDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));
				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = QString::number(centerResult.center.x);
				resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
				resultMessage.rightCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepTwo_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepTwo_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepTwo_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepTwo_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");

				ui.StepTwo_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
				ui.StepTwo_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
				ui.StepTwo_horizontalFrontDriveBox_center_z_Edit->setText("NULL");
				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = "NULL";
				resultMessage.rightCenter.Y = "NULL";
				resultMessage.rightCenter.Z = "NULL";

			}
		}
		else
		{
			statusUpdate(QStringLiteral("�Ҳ�ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepTwo_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");

			ui.StepTwo_horizontalFrontDriveBox_normal_x_Edit->setText("NULL");
			ui.StepTwo_horizontalFrontDriveBox_normal_y_Edit->setText("NULL");
			ui.StepTwo_horizontalFrontDriveBox_normal_z_Edit->setText("NULL");

			ui.StepTwo_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
			ui.StepTwo_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
			ui.StepTwo_horizontalFrontDriveBox_center_z_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightNormal.A = "NULL";
			resultMessage.rightNormal.B = "NULL";
			resultMessage.rightNormal.C = "NULL";
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}
		

	}
	if (num == 203){
		leftFlangeHoleCenterResult = centerResult;
		leftFlangeHoleCircle_cloudPoints = borderCloud_ptr;
		if (rightCenterResult.status == true){
			//������
			ui.StepTwo_horizontalEngine_holeCenter_x_Edit->setStyleSheet("color:black");
			ui.StepTwo_horizontalEngine_holeCenter_y_Edit->setStyleSheet("color:black");
			ui.StepTwo_horizontalEngine_holeCenter_z_Edit->setStyleSheet("color:black");

			ui.StepTwo_horizontalEngine_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepTwo_horizontalEngine_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepTwo_horizontalEngine_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));

			leftProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.leftCenter.X = QString::number(centerResult.center.x);
			resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
			resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepTwo_horizontalEngine_holeCenter_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalEngine_holeCenter_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalEngine_holeCenter_z_Edit->setStyleSheet("color:red");

			ui.StepTwo_horizontalEngine_holeCenter_x_Edit->setText("NULL");
			ui.StepTwo_horizontalEngine_holeCenter_y_Edit->setText("NULL");
			ui.StepTwo_horizontalEngine_holeCenter_z_Edit->setText("NULL");

			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}

	}
	if (num == 204){
		rightFlangeHoleCenterResult = centerResult;
		rightFlangeHoleCircle_cloudPoints = borderCloud_ptr;
		if (rightCenterResult.status == true){
			//������
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_x_Edit->setStyleSheet("color:black");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_y_Edit->setStyleSheet("color:black");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_z_Edit->setStyleSheet("color:black");

			ui.StepTwo_horizontalFrontDriveBox_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));
			rightProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = QString::number(centerResult.center.x);
			resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
			resultMessage.rightCenter.Z = QString::number(centerResult.center.z);

		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_z_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_x_Edit->setText("NULL");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_y_Edit->setText("NULL");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_z_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}

	}
	if (num == 301){
		leftCenterResult = centerResult;
		leftCircle_cloudPoints = borderCloud_ptr;
		leftPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (leftCenterResult.status == true){
				//������
				ui.StepThree_horizontalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:black");

				ui.StepThree_horizontalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_horizontalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_horizontalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_horizontalSyntheticalDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepThree_horizontalSyntheticalDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepThree_horizontalSyntheticalDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = QString::number(centerResult.center.x);
				resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
				resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("���Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepThree_horizontalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepThree_horizontalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepThree_horizontalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:red");

				ui.StepThree_horizontalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_horizontalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_horizontalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_horizontalSyntheticalDriveBox_center_x_Edit->setText("NULL");
				ui.StepThree_horizontalSyntheticalDriveBox_center_y_Edit->setText("NULL");
				ui.StepThree_horizontalSyntheticalDriveBox_center_z_Edit->setText("NULL");

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = "NULL";
				resultMessage.leftCenter.Y = "NULL";
				resultMessage.leftCenter.Z = "NULL";

			}
		}
		else
		{
			statusUpdate(QStringLiteral("���ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepThree_horizontalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:red");

			ui.StepThree_horizontalSyntheticalDriveBox_normal_x_Edit->setText("NULL");
			ui.StepThree_horizontalSyntheticalDriveBox_normal_y_Edit->setText("NULL");
			ui.StepThree_horizontalSyntheticalDriveBox_normal_z_Edit->setText("NULL");

			ui.StepThree_horizontalSyntheticalDriveBox_center_x_Edit->setText("NULL");
			ui.StepThree_horizontalSyntheticalDriveBox_center_y_Edit->setText("NULL");
			ui.StepThree_horizontalSyntheticalDriveBox_center_z_Edit->setText("NULL");
			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";

		}

	}
	if (num == 302){
		rightCenterResult = centerResult;
		rightCircle_cloudPoints = borderCloud_ptr;
		rightPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (rightCenterResult.status == true){
				//������
				ui.StepThree_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:black");

				ui.StepThree_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_horizontalFrontDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepThree_horizontalFrontDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepThree_horizontalFrontDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = QString::number(centerResult.center.x);
				resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
				resultMessage.rightCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepThree_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepThree_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepThree_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");

				ui.StepThree_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
				ui.StepThree_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
				ui.StepThree_horizontalFrontDriveBox_center_z_Edit->setText("NULL");

				rightProcessingStatus = true;


			}
		}
		else
		{
			statusUpdate(QStringLiteral("�Ҳ�ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepThree_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");

			ui.StepThree_horizontalFrontDriveBox_normal_x_Edit->setText("NULL");
			ui.StepThree_horizontalFrontDriveBox_normal_y_Edit->setText("NULL");
			ui.StepThree_horizontalFrontDriveBox_normal_z_Edit->setText("NULL");

			ui.StepThree_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
			ui.StepThree_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
			ui.StepThree_horizontalFrontDriveBox_center_z_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightNormal.A = "NULL";
			resultMessage.rightNormal.B = "NULL";
			resultMessage.rightNormal.C = "NULL";
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}

	}
	if (num == 303){
		leftFlangeHoleCenterResult = centerResult;
		leftFlangeHoleCircle_cloudPoints = borderCloud_ptr;
		if (rightCenterResult.status == true){
			//������
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_x_Edit->setStyleSheet("color:black");
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_y_Edit->setStyleSheet("color:black");
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_z_Edit->setStyleSheet("color:black");

			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));

			leftProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.leftCenter.X = QString::number(centerResult.center.x);
			resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
			resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

		}
		else{
			statusUpdate(QStringLiteral("���Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_x_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_y_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_z_Edit->setStyleSheet("color:red");

			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_x_Edit->setText("NULL");
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_y_Edit->setText("NULL");
			ui.StepThree_horizontalSyntheticalDriveBox_holeCenter_z_Edit->setText("NULL");

			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}

	}
	if (num == 304){
		rightFlangeHoleCenterResult = centerResult;
		rightFlangeHoleCircle_cloudPoints = borderCloud_ptr;
		if (rightCenterResult.status == true){
			//������
			ui.StepThree_horizontalFrontDriveBox_holeCenter_x_Edit->setStyleSheet("color:black");
			ui.StepThree_horizontalFrontDriveBox_holeCenter_y_Edit->setStyleSheet("color:black");
			ui.StepThree_horizontalFrontDriveBox_holeCenter_z_Edit->setStyleSheet("color:black");

			ui.StepThree_horizontalFrontDriveBox_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepThree_horizontalFrontDriveBox_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepThree_horizontalFrontDriveBox_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));

			rightProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = QString::number(centerResult.center.x);
			resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
			resultMessage.rightCenter.Z = QString::number(centerResult.center.z);

		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepThree_horizontalFrontDriveBox_holeCenter_x_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalFrontDriveBox_holeCenter_y_Edit->setStyleSheet("color:red");
			ui.StepThree_horizontalFrontDriveBox_holeCenter_z_Edit->setStyleSheet("color:red");

			ui.StepThree_horizontalFrontDriveBox_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepThree_horizontalFrontDriveBox_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepThree_horizontalFrontDriveBox_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));

			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}

	}
	if (num == 401){
		leftCenterResult = centerResult;
		leftCircle_cloudPoints = borderCloud_ptr;
		leftPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (leftCenterResult.status == true){
				//������
				ui.StepFour_horizontalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:black");

				ui.StepFour_horizontalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFour_horizontalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFour_horizontalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFour_horizontalSyntheticalDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepFour_horizontalSyntheticalDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepFour_horizontalSyntheticalDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = QString::number(centerResult.center.x);
				resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
				resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("���Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepFour_horizontalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepFour_horizontalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepFour_horizontalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:red");
				ui.StepFour_horizontalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFour_horizontalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFour_horizontalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFour_horizontalSyntheticalDriveBox_center_x_Edit->setText("NULL");
				ui.StepFour_horizontalSyntheticalDriveBox_center_y_Edit->setText("NULL");
				ui.StepFour_horizontalSyntheticalDriveBox_center_z_Edit->setText("NULL");

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = "NULL";
				resultMessage.leftCenter.Y = "NULL";
				resultMessage.leftCenter.Z = "NULL";

			}
		}
		else
		{
			statusUpdate(QStringLiteral("�ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepFour_horizontalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalSyntheticalDriveBox_normal_x_Edit->setText("NULL");
			ui.StepFour_horizontalSyntheticalDriveBox_normal_y_Edit->setText("NULL");
			ui.StepFour_horizontalSyntheticalDriveBox_normal_z_Edit->setText("NULL");

			ui.StepFour_horizontalSyntheticalDriveBox_center_x_Edit->setText("NULL");
			ui.StepFour_horizontalSyntheticalDriveBox_center_y_Edit->setText("NULL");
			ui.StepFour_horizontalSyntheticalDriveBox_center_z_Edit->setText("NULL");

			leftProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}

	}
	if (num == 402){
		rightCenterResult = centerResult;
		rightCircle_cloudPoints = borderCloud_ptr;
		rightPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (rightCenterResult.status == true){
				//������
				ui.StepFour_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:black");

				ui.StepFour_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFour_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFour_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFour_horizontalFrontDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepFour_horizontalFrontDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepFour_horizontalFrontDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = QString::number(centerResult.center.x);
				resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
				resultMessage.rightCenter.Z = QString::number(centerResult.center.z);
			}
			else{
				statusUpdate(QStringLiteral("�Ҳ�Բ��������������²ɼ���"), 1);
				ui.StepFour_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFour_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepFour_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepFour_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");
				ui.StepFour_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFour_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFour_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFour_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
				ui.StepFour_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
				ui.StepFour_horizontalFrontDriveBox_center_z_Edit->setText("NULL");

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = "NULL";
				resultMessage.rightCenter.Y = "NULL";
				resultMessage.rightCenter.Z = "NULL";
			}
		}
		else
		{
			statusUpdate(QStringLiteral("�Ҳ�ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepFour_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");
			ui.StepFour_horizontalFrontDriveBox_normal_x_Edit->setText("NULL");
			ui.StepFour_horizontalFrontDriveBox_normal_y_Edit->setText("NULL");
			ui.StepFour_horizontalFrontDriveBox_normal_z_Edit->setText("NULL");

			ui.StepFour_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
			ui.StepFour_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
			ui.StepFour_horizontalFrontDriveBox_center_z_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightNormal.A = "NULL";
			resultMessage.rightNormal.B = "NULL";
			resultMessage.rightNormal.C = "NULL";
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";

		}
	}
	if (num == 501){
		leftCenterResult = centerResult;
		leftCircle_cloudPoints = borderCloud_ptr;
		leftPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (leftCenterResult.status == true){
				//������
				ui.StepFive_horizontalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_center_x_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_center_y_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_center_z_Edit->setStyleSheet("color:black");

				ui.StepFive_horizontalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFive_horizontalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFive_horizontalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFive_horizontalEngine_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepFive_horizontalEngine_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepFive_horizontalEngine_center_z_Edit->setText(QString::number(centerResult.center.z));

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = QString::number(centerResult.center.x);
				resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
				resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("���Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepFive_horizontalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalEngine_center_x_Edit->setStyleSheet("color:red");
				ui.StepFive_horizontalEngine_center_y_Edit->setStyleSheet("color:red");
				ui.StepFive_horizontalEngine_center_z_Edit->setStyleSheet("color:red");

				ui.StepFive_horizontalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFive_horizontalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFive_horizontalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFive_horizontalEngine_center_x_Edit->setText("NULL");
				ui.StepFive_horizontalEngine_center_y_Edit->setText("NULL");
				ui.StepFive_horizontalEngine_center_z_Edit->setText("NULL");

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = "NULL";
				resultMessage.leftCenter.Y = "NULL";
				resultMessage.leftCenter.Z = "NULL";
			}
		}
		else
		{
			
			statusUpdate(QStringLiteral("���ɼ��������⣬�����λ�����²ɼ���"), 1); 
			ui.StepFive_horizontalEngine_normal_x_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalEngine_normal_y_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalEngine_normal_z_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalEngine_center_x_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalEngine_center_y_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalEngine_center_z_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalEngine_normal_x_Edit->setText("NULL");
			ui.StepFive_horizontalEngine_normal_y_Edit->setText("NULL");
			ui.StepFive_horizontalEngine_normal_z_Edit->setText("NULL");

			ui.StepFive_horizontalEngine_center_x_Edit->setText("NULL");
			ui.StepFive_horizontalEngine_center_y_Edit->setText("NULL");
			ui.StepFive_horizontalEngine_center_z_Edit->setText("NULL");

			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}
		

	}
	if (num == 502)
	{
		rightCenterResult = centerResult;
		rightCircle_cloudPoints = borderCloud_ptr;
		rightPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (rightCenterResult.status == true){
				ui.StepFive_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:black");

				ui.StepFive_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFive_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFive_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFive_horizontalFrontDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepFive_horizontalFrontDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepFive_horizontalFrontDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = QString::number(centerResult.center.x);
				resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
				resultMessage.rightCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepFive_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepFive_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepFive_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepFive_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");
				ui.StepFive_horizontalFrontDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepFive_horizontalFrontDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepFive_horizontalFrontDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepFive_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
				ui.StepFive_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
				ui.StepFive_horizontalFrontDriveBox_center_z_Edit->setText("NULL");

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = "NULL";
				resultMessage.rightCenter.Y = "NULL";
				resultMessage.rightCenter.Z = "NULL";
			}
		}
		else
		{
			statusUpdate(QStringLiteral("�Ҳ�ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepFive_horizontalFrontDriveBox_normal_x_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalFrontDriveBox_normal_y_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalFrontDriveBox_normal_z_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalFrontDriveBox_center_x_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalFrontDriveBox_center_y_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalFrontDriveBox_center_z_Edit->setStyleSheet("color:red");
			ui.StepFive_horizontalFrontDriveBox_normal_x_Edit->setText("NULL");
			ui.StepFive_horizontalFrontDriveBox_normal_y_Edit->setText("NULL");
			ui.StepFive_horizontalFrontDriveBox_normal_z_Edit->setText("NULL");

			ui.StepFive_horizontalFrontDriveBox_center_x_Edit->setText("NULL");
			ui.StepFive_horizontalFrontDriveBox_center_y_Edit->setText("NULL");
			ui.StepFive_horizontalFrontDriveBox_center_z_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightNormal.A = "NULL";
			resultMessage.rightNormal.B = "NULL";
			resultMessage.rightNormal.C = "NULL";
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}
	}
		

	
	/**
	*���÷�����
	*/
	if (num == 1010){
		if (planeparamStatus == true){
			//������
			ui.StepOne_verticalEngine_normal_x_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalEngine_normal_y_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalEngine_normal_z_Edit->setStyleSheet("color:black");

			ui.StepOne_verticalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
			ui.StepOne_verticalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
			ui.StepOne_verticalEngine_normal_z_Edit->setText(QString::number(planeparam.C));
			
			leftPlaneNormal = planeparam;
			leftProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = QString::number(planeparam.A);
			resultMessage.leftNormal.B = QString::number(planeparam.B);
			resultMessage.leftNormal.C = QString::number(planeparam.C);
		}
		else{
			statusUpdate(QStringLiteral("���ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepOne_verticalEngine_normal_x_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalEngine_normal_y_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalEngine_normal_z_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalEngine_normal_x_Edit->setText("NULL");
			ui.StepOne_verticalEngine_normal_y_Edit->setText("NULL");
			ui.StepOne_verticalEngine_normal_z_Edit->setText("NULL");

			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
		}

	}
	if (num == 1020){
		transmissionCaseCenterResultFirst = centerResult;
		transmissionCaseCircleFirst_cloudPoints = borderCloud_ptr;
		if (transmissionCaseCenterResultFirst.status == true){
			//������
			ui.StepOne_verticalSyntheticalDriveBox_First_center_x_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_First_center_y_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_First_center_z_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_First_center_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepOne_verticalSyntheticalDriveBox_First_center_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepOne_verticalSyntheticalDriveBox_First_center_z_Edit->setText(QString::number(centerResult.center.z));

			rightProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = QString::number(centerResult.center.x);
			resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
			resultMessage.rightCenter.Z = QString::number(centerResult.center.z);
		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepOne_verticalSyntheticalDriveBox_First_center_x_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_First_center_y_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_First_center_z_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_First_center_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepOne_verticalSyntheticalDriveBox_First_center_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepOne_verticalSyntheticalDriveBox_First_center_z_Edit->setText(QString::number(centerResult.center.z));
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}

	}
	if (num == 1040){
		transmissionCaseCenterResultSecond = centerResult;
		transmissionCaseCircleSecond_cloudPoints = borderCloud_ptr;
		if (transmissionCaseCenterResultSecond.status == true)
		{
			//���㷽��ʸ��
			directionVector.x = transmissionCaseCenterResultFirst.center.x - transmissionCaseCenterResultSecond.center.x;
			directionVector.y = transmissionCaseCenterResultFirst.center.y - transmissionCaseCenterResultSecond.center.y;
			directionVector.z = transmissionCaseCenterResultFirst.center.z - transmissionCaseCenterResultSecond.center.z;
			QString fileName = engineNumFileName + "/outputFile/StepOne/syntheticalDriveBoxDirectionVector.csv";//д���ļ���Ŀ¼
			QString time = ReturnTime();
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << directionVector.x << "," << directionVector.y << "," << directionVector.y << "\n";
			file.close();
			//������
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_x_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_y_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_z_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_x_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_y_Edit->setStyleSheet("color:black");
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_z_Edit->setStyleSheet("color:black");

			ui.StepOne_verticalSyntheticalDriveBox_Second_center_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_z_Edit->setText(QString::number(centerResult.center.z));

			ui.StepOne_verticalSyntheticalDriveBox_directionVector_x_Edit->setText(QString::number(directionVector.x));
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_y_Edit->setText(QString::number(directionVector.y));
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_z_Edit->setText(QString::number(directionVector.z));

			rightProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = QString::number(centerResult.center.x);
			resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
			resultMessage.rightCenter.Z = QString::number(centerResult.center.z);
			resultMessage.directionVector.A = QString::number(directionVector.x);
			resultMessage.directionVector.B = QString::number(directionVector.y);
			resultMessage.directionVector.C = QString::number(directionVector.z);
		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_x_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_y_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_z_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_x_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_y_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_z_Edit->setStyleSheet("color:red");
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepOne_verticalSyntheticalDriveBox_Second_center_z_Edit->setText(QString::number(centerResult.center.z));

			ui.StepOne_verticalSyntheticalDriveBox_directionVector_x_Edit->setText(QString::number(directionVector.x));
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_y_Edit->setText(QString::number(directionVector.y));
			ui.StepOne_verticalSyntheticalDriveBox_directionVector_z_Edit->setText(QString::number(directionVector.z));

			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
			resultMessage.directionVector.A = "NULL";
			resultMessage.directionVector.B = "NULL";
			resultMessage.directionVector.C = "NULL";
		}

	}

	if (num == 2010){
		leftCenterResult = centerResult;
		leftCircle_cloudPoints = borderCloud_ptr;
		leftPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (leftCenterResult.status == true){
				//������
				ui.StepTwo_verticalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalEngine_normal_z_Edit->setStyleSheet("color:black");

				ui.StepTwo_verticalEngine_center_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalEngine_center_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalEngine_center_z_Edit->setStyleSheet("color:black");

				ui.StepTwo_verticalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_verticalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_verticalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_verticalEngine_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepTwo_verticalEngine_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepTwo_verticalEngine_center_z_Edit->setText(QString::number(centerResult.center.z));

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = QString::number(centerResult.center.x);
				resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
				resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepTwo_verticalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalEngine_normal_z_Edit->setStyleSheet("color:black");

				ui.StepTwo_verticalEngine_center_x_Edit->setStyleSheet("color:red");
				ui.StepTwo_verticalEngine_center_y_Edit->setStyleSheet("color:red");
				ui.StepTwo_verticalEngine_center_z_Edit->setStyleSheet("color:red");

				ui.StepTwo_verticalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_verticalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_verticalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_verticalEngine_center_x_Edit->setText("NULL");
				ui.StepTwo_verticalEngine_center_y_Edit->setText("NULL");
				ui.StepTwo_verticalEngine_center_z_Edit->setText("NULL");

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = "NULL";
				resultMessage.leftCenter.Y = "NULL";
				resultMessage.leftCenter.Z = "NULL";

			}
		}
		else
		{
			statusUpdate(QStringLiteral("���ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepTwo_verticalEngine_normal_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalEngine_normal_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalEngine_normal_z_Edit->setStyleSheet("color:red");

			ui.StepTwo_verticalEngine_center_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalEngine_center_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalEngine_center_z_Edit->setStyleSheet("color:red");

			ui.StepTwo_verticalEngine_normal_x_Edit->setText("NULL");
			ui.StepTwo_verticalEngine_normal_y_Edit->setText("NULL");
			ui.StepTwo_verticalEngine_normal_z_Edit->setText("NULL");

			ui.StepTwo_verticalEngine_center_x_Edit->setText("NULL");
			ui.StepTwo_verticalEngine_center_y_Edit->setText("NULL");
			ui.StepTwo_verticalEngine_center_z_Edit->setText("NULL");
			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}
		

	}
	if (num == 2020){
		rightCenterResult = centerResult;
		rightCircle_cloudPoints = borderCloud_ptr;
		rightPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (rightCenterResult.status == true){
				//������
				ui.StepTwo_verticalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:black");

				ui.StepTwo_verticalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_verticalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_verticalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_verticalSyntheticalDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepTwo_verticalSyntheticalDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepTwo_verticalSyntheticalDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = QString::number(centerResult.center.x);
				resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
				resultMessage.rightCenter.Z = QString::number(centerResult.center.z);

			}
			else{
				statusUpdate(QStringLiteral("�Ҳ�Բ�������ʧ�ܣ����²ɼ���"), 1);
				ui.StepTwo_verticalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepTwo_verticalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepTwo_verticalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepTwo_verticalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:red");
				ui.StepTwo_verticalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepTwo_verticalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepTwo_verticalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepTwo_verticalSyntheticalDriveBox_center_x_Edit->setText("NULL");
				ui.StepTwo_verticalSyntheticalDriveBox_center_y_Edit->setText("NULL");
				ui.StepTwo_verticalSyntheticalDriveBox_center_z_Edit->setText("NULL");

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.rightNormal.A = QString::number(planeparam.A);
				resultMessage.rightNormal.B = QString::number(planeparam.B);
				resultMessage.rightNormal.C = QString::number(planeparam.C);
				resultMessage.rightCenter.X = "NULL";
				resultMessage.rightCenter.Y = "NULL";
				resultMessage.rightCenter.Z = "NULL";
			}
		}
		else
		{
			statusUpdate(QStringLiteral("�Ҳ�ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepTwo_verticalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalSyntheticalDriveBox_normal_x_Edit->setText("NULL");
			ui.StepTwo_verticalSyntheticalDriveBox_normal_y_Edit->setText("NULL");
			ui.StepTwo_verticalSyntheticalDriveBox_normal_z_Edit->setText("NULL");

			ui.StepTwo_verticalSyntheticalDriveBox_center_x_Edit->setText("NULL");
			ui.StepTwo_verticalSyntheticalDriveBox_center_y_Edit->setText("NULL");
			ui.StepTwo_verticalSyntheticalDriveBox_center_z_Edit->setText("NULL");
			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.rightNormal.A = "NULL";
			resultMessage.rightNormal.B = "NULL";
			resultMessage.rightNormal.C = "NULL";
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}

	}
	if (num == 2030){
		leftFlangeHoleCenterResult = centerResult;
		leftFlangeHoleCircle_cloudPoints = borderCloud_ptr;
		if (rightCenterResult.status == true){
			//������
			ui.StepTwo_verticalEngine_holeCenter_x_Edit->setStyleSheet("color:black");
			ui.StepTwo_verticalEngine_holeCenter_y_Edit->setStyleSheet("color:black");
			ui.StepTwo_verticalEngine_holeCenter_z_Edit->setStyleSheet("color:black");

			ui.StepTwo_verticalEngine_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepTwo_verticalEngine_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepTwo_verticalEngine_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));

			leftProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.leftCenter.X = QString::number(centerResult.center.x);
			resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
			resultMessage.leftCenter.Z = QString::number(centerResult.center.z);

		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepTwo_verticalEngine_holeCenter_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalEngine_holeCenter_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalEngine_holeCenter_z_Edit->setStyleSheet("color:red");

			ui.StepTwo_verticalEngine_holeCenter_x_Edit->setText("NULL");
			ui.StepTwo_verticalEngine_holeCenter_y_Edit->setText("NULL");
			ui.StepTwo_verticalEngine_holeCenter_z_Edit->setText("NULL");

			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}

	}
	if (num == 2040){
		rightFlangeHoleCenterResult = centerResult;
		rightFlangeHoleCircle_cloudPoints = borderCloud_ptr;
		if (rightCenterResult.status == true){
			//������
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_x_Edit->setStyleSheet("color:black");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_y_Edit->setStyleSheet("color:black");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_z_Edit->setStyleSheet("color:black");

			ui.StepTwo_verticalSyntheticalDriveBox_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepTwo_verticalSyntheticalDriveBox_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepTwo_verticalSyntheticalDriveBox_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));

			rightProcessingStatus = true;
			//��ӵ�������Ϣ
			resultMessage.rightCenter.X = QString::number(centerResult.center.x);
			resultMessage.rightCenter.Y = QString::number(centerResult.center.y);
			resultMessage.rightCenter.Z = QString::number(centerResult.center.z);

		}
		else{
			statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_x_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_y_Edit->setStyleSheet("color:red");
			ui.StepTwo_horizontalFrontDriveBox_holeCenter_z_Edit->setStyleSheet("color:red");
			ui.StepTwo_verticalSyntheticalDriveBox_holeCenter_x_Edit->setText(QString::number(centerResult.center.x));
			ui.StepTwo_verticalSyntheticalDriveBox_holeCenter_y_Edit->setText(QString::number(centerResult.center.y));
			ui.StepTwo_verticalSyntheticalDriveBox_holeCenter_z_Edit->setText(QString::number(centerResult.center.z));

			rightProcessingStatus = false;
			resultMessage.rightCenter.X = "NULL";
			resultMessage.rightCenter.Y = "NULL";
			resultMessage.rightCenter.Z = "NULL";
		}

	}
	if (num == 3010){
		leftCenterResult = centerResult;
		leftCircle_cloudPoints = borderCloud_ptr;
		leftPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (leftCenterResult.status == true){
				//������
				ui.StepThree_verticalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_center_x_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_center_y_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_center_z_Edit->setStyleSheet("color:black");

				ui.StepThree_verticalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_verticalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_verticalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_verticalEngine_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepThree_verticalEngine_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepThree_verticalEngine_center_z_Edit->setText(QString::number(centerResult.center.z));

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = QString::number(centerResult.center.x);
				resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
				resultMessage.leftCenter.Z = QString::number(centerResult.center.z);
			}
			else{
				statusUpdate(QStringLiteral("���Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepThree_verticalEngine_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalEngine_center_x_Edit->setStyleSheet("color:red");
				ui.StepThree_verticalEngine_center_y_Edit->setStyleSheet("color:red");
				ui.StepThree_verticalEngine_center_z_Edit->setStyleSheet("color:red");

				ui.StepThree_verticalEngine_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_verticalEngine_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_verticalEngine_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_verticalEngine_center_x_Edit->setText("NULL");
				ui.StepThree_verticalEngine_center_y_Edit->setText("NULL");
				ui.StepThree_verticalEngine_center_z_Edit->setText("NULL");

				leftProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = "NULL";
				resultMessage.leftCenter.Y = "NULL";
				resultMessage.leftCenter.Z = "NULL";

			}
		}
		else
		{
			statusUpdate(QStringLiteral("���ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepThree_verticalEngine_normal_x_Edit->setStyleSheet("color:red");
			ui.StepThree_verticalEngine_normal_y_Edit->setStyleSheet("color:red");
			ui.StepThree_verticalEngine_normal_z_Edit->setStyleSheet("color:red");
			ui.StepThree_verticalEngine_center_x_Edit->setStyleSheet("color:red");
			ui.StepThree_verticalEngine_center_y_Edit->setStyleSheet("color:red");
			ui.StepThree_verticalEngine_center_z_Edit->setStyleSheet("color:red");

			ui.StepThree_verticalEngine_normal_x_Edit->setText("NULL");
			ui.StepThree_verticalEngine_normal_y_Edit->setText("NULL");
			ui.StepThree_verticalEngine_normal_z_Edit->setText("NULL");

			ui.StepThree_verticalEngine_center_x_Edit->setText("NULL");
			ui.StepThree_verticalEngine_center_y_Edit->setText("NULL");
			ui.StepThree_verticalEngine_center_z_Edit->setText("NULL");

			leftProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";
		}

	}
	if (num == 3020){
		rightCenterResult = centerResult;
		rightCircle_cloudPoints = borderCloud_ptr;
		rightPlaneNormal = planeparam;
		if (planeparamStatus == true)
		{
			if (rightCenterResult.status == true){
				ui.StepThree_verticalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:black");

				ui.StepThree_verticalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_verticalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_verticalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_verticalSyntheticalDriveBox_center_x_Edit->setText(QString::number(centerResult.center.x));
				ui.StepThree_verticalSyntheticalDriveBox_center_y_Edit->setText(QString::number(centerResult.center.y));
				ui.StepThree_verticalSyntheticalDriveBox_center_z_Edit->setText(QString::number(centerResult.center.z));

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = QString::number(centerResult.center.x);
				resultMessage.leftCenter.Y = QString::number(centerResult.center.y);
				resultMessage.leftCenter.Z = QString::number(centerResult.center.z);
			}
			else{
				statusUpdate(QStringLiteral("�Ҳ�Բ�����ʧ�ܣ����²ɼ���"), 1);
				ui.StepThree_verticalSyntheticalDriveBox_normal_x_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_normal_y_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_normal_z_Edit->setStyleSheet("color:black");
				ui.StepThree_verticalSyntheticalDriveBox_center_x_Edit->setStyleSheet("color:red");
				ui.StepThree_verticalSyntheticalDriveBox_center_y_Edit->setStyleSheet("color:red");
				ui.StepThree_verticalSyntheticalDriveBox_center_z_Edit->setStyleSheet("color:red");

				ui.StepThree_verticalSyntheticalDriveBox_normal_x_Edit->setText(QString::number(planeparam.A));
				ui.StepThree_verticalSyntheticalDriveBox_normal_y_Edit->setText(QString::number(planeparam.B));
				ui.StepThree_verticalSyntheticalDriveBox_normal_z_Edit->setText(QString::number(planeparam.C));

				ui.StepThree_verticalSyntheticalDriveBox_center_x_Edit->setText("NULL");
				ui.StepThree_verticalSyntheticalDriveBox_center_y_Edit->setText("NULL");
				ui.StepThree_verticalSyntheticalDriveBox_center_z_Edit->setText("NULL");

				rightProcessingStatus = true;
				//��ӵ�������Ϣ
				resultMessage.leftNormal.A = QString::number(planeparam.A);
				resultMessage.leftNormal.B = QString::number(planeparam.B);
				resultMessage.leftNormal.C = QString::number(planeparam.C);
				resultMessage.leftCenter.X = "NULL";
				resultMessage.leftCenter.Y = "NULL";
				resultMessage.leftCenter.Z = "NULL";
			}
		}
		else
		{
			statusUpdate(QStringLiteral("�Ҳ�ɼ��������⣬�����λ�����²ɼ���"), 1);
			ui.StepThree_verticalSyntheticalDriveBox_normal_x_Edit->setText("NULL");
			ui.StepThree_verticalSyntheticalDriveBox_normal_y_Edit->setText("NULL");
			ui.StepThree_verticalSyntheticalDriveBox_normal_z_Edit->setText("NULL");

			ui.StepThree_verticalSyntheticalDriveBox_center_x_Edit->setText("NULL");
			ui.StepThree_verticalSyntheticalDriveBox_center_y_Edit->setText("NULL");
			ui.StepThree_verticalSyntheticalDriveBox_center_z_Edit->setText("NULL");

			rightProcessingStatus = false;
			//��ӵ�������Ϣ
			resultMessage.leftNormal.A = "NULL";
			resultMessage.leftNormal.B = "NULL";
			resultMessage.leftNormal.C = "NULL";
			resultMessage.leftCenter.X = "NULL";
			resultMessage.leftCenter.Y = "NULL";
			resultMessage.leftCenter.Z = "NULL";

		}

	}
}

void DisplayGUI::receiveStepParam(double CamNoScanSpeed, double CamScanDistance, double CamFirstNoScanDistance, double CamSecondNoScanDistance)
{
	if (CamNoScanSpeed == -1 && CamScanDistance == -1){
		cout << "param error" << endl;
		CameraNoScanSpeed = 20;
		CameraScanDistance = 50;
		CameraFirstNoScanDistance = 20;
		CameraSecondNoScanDistance = 30;
	}
	else{
		CameraNoScanSpeed = CamNoScanSpeed;
		CameraScanDistance = CamScanDistance;
		CameraFirstNoScanDistance = CamFirstNoScanDistance;
		CameraSecondNoScanDistance = CamSecondNoScanDistance;
	}
	cout << "������Ϣ��" << endl;
	cout << "	��ͷ��ɨ��ʱ�˶��ٶ�" << CameraNoScanSpeed << endl;
	cout << "	��ͷɨ��ʱ�˶�����" << CameraScanDistance << endl;
	cout << "	��ͷ�˶�����һ����λ�õ��˶�����" << CameraFirstNoScanDistance << endl;
	cout << "	��ͷ�˶����ڶ�����λ�õ��˶�����" << CameraSecondNoScanDistance << endl;
}

void DisplayGUI::resolveSignal(QString info)
{
	if (info.mid(0, 3) == "hor" || info.mid(0, 3) == "ver"){
		vp->engineNumFileName = "E://engineFile/" + info;
		engineNumFileName = "E://engineFile/" + info;
		QDir dir;
		bool ok = dir.mkdir("E://engineFile");
		ok= dir.mkdir(vp->engineNumFileName);
		socketNum = 0;
		//���䱣�浽TXT�ļ���
		QString fileName = "ConfigFile/engineNumFileName.txt";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
		QTextStream out(&file);		
		out << vp->engineNumFileName << "\n";
		file.close();
		//�����ļ���
		createProjectDirectory(info);
		createParamFirle(info);
		//���淢�����ͺ�
		QString engineTypeStr;
		//�ֽ���Ϣ��ȡ���������ͺ�
		for (int i = 0; i < info.size(); i++){
			if (info[i] == 'Y' || info[i] == 'W')
			{
				engineTypeStr = info.mid(i, info.size() - 1);
				break;
			}
		}
		//���淢�����ͺ�
		fileName = "ConfigFile/engineTypeName.txt";//д���ļ���Ŀ¼
		QFile file1(fileName);
		file1.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
		QTextStream out1(&file1);
		out1 << engineTypeStr << "\n";
		file1.close();
		//��Ϊ��ͨ��logind����������vp�������Ա���ͨ��login�����޸�vp���ֵ��ֱ������vp�޷��޸Ľ���
		emit sendStepConfig();
	}
	else if (info.mid(0, 1) == "H" || info.mid(0, 1)=="V"){
		int infoSize = info.size();
		cout << "infoSize = " << infoSize << endl;
		engineVersion = info.mid(0, 1);
		socketNum = info.mid(1, 3).toInt();
		QString robotPoseStr;
		QString midStr[6];
		int midStrNum = 0;
		robotPoseStr = info.mid(5);
		VisualProcessing::RobotPose robotPose;
		cout << "robotPoseStr = " << robotPoseStr.toStdString() << endl;
		for (int i = 0; i < robotPoseStr.size(); i++)
		{
			if (robotPoseStr.mid(i, 1) != ","){
				midStr[midStrNum].append(robotPoseStr.mid(i, 1));
			}
			else{
				midStrNum++;
			}
		}
		robotPose.X = midStr[0].toDouble();
		robotPose.Y = midStr[1].toDouble();
		robotPose.Z = midStr[2].toDouble();
		robotPose.A = midStr[3].toDouble();
		robotPose.B = midStr[4].toDouble();
		robotPose.C = midStr[5].toDouble();
		cout << "engineVersion:" << engineVersion.toStdString() << endl;
		cout << "socketNum:" << socketNum << endl;
		cout << "robotPose.X:" << robotPose.X << endl;
		cout << "robotPose.Y:" << robotPose.Y << endl;
		cout << "robotPose.Z:" << robotPose.Z << endl;
		cout << "robotPose.A:" << robotPose.A << endl;
		cout << "robotPose.B:" << robotPose.B << endl;
		cout << "robotPose.C:" << robotPose.C << endl;
		emit sendRobotPose(robotPose);
	}
	else{
		socketNum = 999;//��������
	}
	

}

void DisplayGUI::saveInformationToJournalFile(QString info)
{
	QDateTime CurrentTime = QDateTime::currentDateTime();
	QString Timestr = CurrentTime.toString("yyyy-MM-dd hh:mm:ss"); // ������ʾ�ĸ�ʽ
	QString statusStr = Timestr + ": " + info;
	QString fileName = engineNumFileName + "/JournalFile.txt";//д���ļ���Ŀ¼
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);
	out << statusStr << "\n";
	file.close();
}

void DisplayGUI::createProjectDirectory(QString info)
{
	engineNumFileName = "E://engineFile/" + info;
	QDir dir;
	//��������ͼƬ�ļ���
	bool ok = dir.mkdir(engineNumFileName+"/JournalImage");
	//��������ļ��ļ���
	ok = dir.mkdir(engineNumFileName + "/outputFile");
	
	if (info.mid(0, 3) == "hor"){
		//��������һ�ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxFinalBorderPoints");
		//�ڶ�����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxFinalBorderPoints");
		//�����������ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxFinalBorderPoints");
		//�ڶ�����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleFinalBorderPoints");
		//�����������ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxFinalBorderPoints");
		//�ڶ�����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleFinalBorderPoints");
		//�����������ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxFinalBorderPoints");
		//�ڶ�����

		//�����������ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/engineUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/engineCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/engineFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/engineFinalBorderPoints");
		
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxFinalBorderPoints");
	}
	if (info.mid(0, 3) == "ver"){
		//��������һ�ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxFinalBorderPoints");
		//�ڶ�����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxFinalBorderPoints");
		//�����������ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/engineFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxFinalBorderPoints");
		//�ڶ�����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleFinalBorderPoints");
		//�����������ļ���
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree");
		//��һ����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/engineUnifyCirclePoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/engineCirclePointsThroughDistanceThreshold");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/engineFilterBorderPoints");
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/firstPart/engineFinalBorderPoints");
		//�ڶ�����
		ok = dir.mkdir(engineNumFileName + "/outputFile/stepThree/secondPart");
	}
	
}

void DisplayGUI::createParamFirle(QString info)
{
	engineNumFileName = "E://engineFile/" + info;

	QString fileName;
	ofstream outfile;
	if (info.mid(0, 3) == "hor")
	{
		//���÷�����
		//��λ1
		//��դ�߷��������ƶ�����ͷ
		fileName = engineNumFileName + "/outputFile/stepOne/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//ǰ������ֹ��Բ�ĵ����β����ķ���������ͷ
		fileName = engineNumFileName + "/outputFile/stepOne/frontDriveBoxDirectionVector.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "����ʸ��X" << "," << "����ʸ��Y" << "," << "����ʸ��Z" << "\n";
		outfile.close();
		//��λ1��һ����
		fileName = engineNumFileName + "/outputFile/stepOne/firstPart/engineParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" << "\n";
		outfile.close();
		//��λ1�ڶ�����
		fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" << "\n";
		outfile.close();
		//��λ2
		fileName = engineNumFileName + "/outputFile/stepTwo/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//��λ2��һ����
		fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ2�ڶ�����
		fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ3
		fileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//��λ3��һ����
		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ3�ڶ�����
		fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ4
		fileName = engineNumFileName + "/outputFile/stepFour/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//��λ4��һ����
		fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ4�ڶ�����

		//��λ5
		fileName = engineNumFileName + "/outputFile/stepFive/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//��λ5��һ����
		fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ5�ڶ�����
	}
	
	if (info.mid(0, 3) == "ver")
	{
		//���÷�����
		//��λ1
		//��դ�߷��������ƶ�����ͷ
		fileName = engineNumFileName + "/outputFile/stepOne/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//�ۺϴ�����ֹ��Բ�ĵ����β����ķ���������ͷ
		fileName = engineNumFileName + "/outputFile/stepOne/syntheticalDriveBoxDirectionVector.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "����ʸ��X" << "," << "����ʸ��Y" << "," << "����ʸ��Z" << "\n";
		outfile.close();
		//��λ1��һ����
		fileName = engineNumFileName + "/outputFile/stepOne/firstPart/engineParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" << "\n";
		outfile.close();
		//��λ1�ڶ�����
		fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" << "\n";
		outfile.close();
		//��λ2
		fileName = engineNumFileName + "/outputFile/stepTwo/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//��λ2��һ����
		fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ2�ڶ�����
		fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ3
		fileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "��һ�βɼ�ʱ��ԭ�����" << "," << "�ڶ��βɼ�ʱ��ԭ�����" << "\n";
		outfile.close();
		//��λ3��һ����
		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();

		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxParam.csv";
		outfile.open(fileName.toStdString());
		outfile << "ʱ��" << "," << "��λ" << "," << "Բ��X" << "," << "Բ��Y" << "," << "Բ��Z" << "," << "Բ�����״̬" << ","
			<< "��ʸA" << "," << "��ʸB" << "," << "��ʸC" << "," << "��Ե�㵽Բ�ĵ�ƽ������" "\n";
		outfile.close();
		//��λ3�ڶ�����
	}
	
}

/**
*���÷������ź�Ϊ101 102��  
*���÷������ź��ں��÷������źŻ�������ĩβ����0����1010 1020
*/
void DisplayGUI::VStepOneFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	this->close();
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	LeftScannerRun(1010, leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(1020, transmissionCaseCircleFirst_cloudPoints, transmissionCaseCenterResultFirst);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP
	cout << "Բ��: " << endl;
	/*cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;*/
	cout << transmissionCaseCenterResultFirst.center.x
		<< " " << transmissionCaseCenterResultFirst.center.y
		<< " " << transmissionCaseCenterResultFirst.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("V101,%1,%2,%3,%4,%5,%6").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B).arg(resultMessage.leftNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (transmissionCaseCenterResultFirst.status == true){
			displayCloud(transmissionCaseCircleFirst_cloudPoints, transmissionCaseCenterResultFirst);
		}
	}
	else{
		socket->send("ERROR101");
	}
#endif//��PROGRESS
	statusUpdate(QStringLiteral("��λ1��һ�����������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	this->show();
	app->close();
	delete app;
}

void DisplayGUI::VStepOneSecond()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif

	RightScannerRun(1040, transmissionCaseCircleSecond_cloudPoints, transmissionCaseCenterResultSecond);
	Sleep(200);

#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);

	cout << "Բ��: " << endl;
	cout << transmissionCaseCenterResultSecond.center.x << " " << transmissionCaseCenterResultSecond.center.y << " " << transmissionCaseCenterResultSecond.center.z << endl;

#ifndef PROGRESS

	//��ʾ��Ե���Ƽ�Բ��
	if (rightProcessingStatus == true)
	{
		QString sendInfo = QString("V102,%1,%2,%3,%4,%5,%6").arg(resultMessage.rightCenter.X)
			.arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z)
			.arg(resultMessage.directionVector.A).arg(resultMessage.directionVector.B).arg(resultMessage.directionVector.C);

		socket->send(sendInfo);
		if (transmissionCaseCenterResultSecond.status == true){
			displayCloud(transmissionCaseCircleFirst_cloudPoints, transmissionCaseCenterResultFirst,
				transmissionCaseCircleSecond_cloudPoints, transmissionCaseCenterResultSecond);
		}
		
	}
	else{
		socket->send("ERROR102");
	}
#endif//��PROGRESS
	statusUpdate(QStringLiteral("��λ1�������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::VStepTwoFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	//�Ȳ�ֹ�ڷ�ʸ�ͱ�ԵԲ��
	LeftScannerRun(2010, leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(2020, rightCircle_cloudPoints, rightCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;
	cout << rightCenterResult.center.x << " " << rightCenterResult.center.y << " " << rightCenterResult.center.z << endl;
	Sleep(2000);
	//����
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("V201,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B)
			.arg(resultMessage.leftNormal.C)
			.arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y).arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightNormal.A).arg(resultMessage.rightNormal.B).arg(resultMessage.rightNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftCenterResult.status == true && rightCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult);
		}
		if (rightCenterResult.status == true && leftCenterResult.status == false)
		{
			displayCloud(rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftCenterResult.status == true && rightCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
	}
	else{
		socket->send("ERROR201");
	}
#endif // !PROGRESS
	statusUpdate(QStringLiteral("��λ2��һ�����������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::VStepTwoSecond()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	start = clock();
	Sleep(100);
	//�ٲⷨ����Բ��
	LeftScannerRun(2030, leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(2040, rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftFlangeHoleCenterResult.center.x << " " << leftFlangeHoleCenterResult.center.y << " " << leftFlangeHoleCenterResult.center.z << endl;
	cout << rightFlangeHoleCenterResult.center.x << " " << rightFlangeHoleCenterResult.center.y << " " << rightFlangeHoleCenterResult.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("V202,%1,%2,%3,%4,%5,%6").arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y)
			.arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftFlangeHoleCenterResult.status == true && rightFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult);
		}
		if (rightFlangeHoleCenterResult.status == true && leftFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
		}
		if (leftFlangeHoleCenterResult.status == false && rightFlangeHoleCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftFlangeHoleCenterResult.status == true && rightFlangeHoleCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult,
				leftFlangeHoleCircle_cloudPoints, leftFlangeHoleCenterResult, rightFlangeHoleCircle_cloudPoints, rightFlangeHoleCenterResult);
		}

	}
	else{
		socket->send("ERROR202");
	}
#endif // !PROGRESS
	statusUpdate(QStringLiteral("��λ2�ڶ������������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::VStepThreeFirst()
{
	QProcess *app = new QProcess;
	QStringList strList;//����list
	strList << "";//���贫��������в���������������ÿռ���
	app->start("QtGuiApplication2.exe", strList);
	//ͳ�Ƴ�������ʱ��
	clock_t start, finish;
	double totaltime;
	////�����Ե�����
	//pcl::PointCloud<pcl::PointXYZ>::Ptr leftCircle_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	//pcl::PointCloud<pcl::PointXYZ>::Ptr rightCircle_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
	////����ռ�Բ��
	//CoreAlgorithm::StereoCircle leftCenterResult;
	//CoreAlgorithm::StereoCircle rightCenterResult;
	start = clock();
	Sleep(100);
	LeftScannerRun(3010, leftCircle_cloudPoints, leftCenterResult);
	Sleep(500);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
#endif
	statusUpdate(QStringLiteral("���ͷɨ�����"), 0);
	Sleep(100);
#ifndef SETUP
	//ת̨��ת180��
	motion->platformLtoRRotate();
#endif
	Sleep(500);
	RightScannerRun(3020, rightCircle_cloudPoints, rightCenterResult);
	statusUpdate(QStringLiteral("�Ҳ�ͷɨ�����"), 0);
#ifndef SETUP
	motion->CheckRun();
	Sleep(100);
	motion->platformRtoLRotate();
#endif // !SETUP

	cout << "Բ��: " << endl;
	cout << leftCenterResult.center.x << " " << leftCenterResult.center.y << " " << leftCenterResult.center.z << endl;
	cout << rightCenterResult.center.x << " " << rightCenterResult.center.y << " " << rightCenterResult.center.z << endl;
	//��ʾ��Ե���Ƽ�Բ��
#ifndef PROGRESS
	displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
	if (leftProcessingStatus == true || rightProcessingStatus == true)
	{
		QString sendInfo = QString("V301,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12").arg(resultMessage.leftNormal.A).arg(resultMessage.leftNormal.B)
			.arg(resultMessage.leftNormal.C)
			.arg(resultMessage.leftCenter.X).arg(resultMessage.leftCenter.Y).arg(resultMessage.leftCenter.Z)
			.arg(resultMessage.rightNormal.A).arg(resultMessage.rightNormal.B).arg(resultMessage.rightNormal.C)
			.arg(resultMessage.rightCenter.X).arg(resultMessage.rightCenter.Y).arg(resultMessage.rightCenter.Z);
		socket->send(sendInfo);
		if (leftCenterResult.status == true && rightCenterResult.status == false)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult);
		}
		if (rightCenterResult.status == true && leftCenterResult.status == false)
		{
			displayCloud(rightCircle_cloudPoints, rightCenterResult);
		}
		if (leftCenterResult.status == true && rightCenterResult.status == true)
		{
			displayCloud(leftCircle_cloudPoints, leftCenterResult, rightCircle_cloudPoints, rightCenterResult);
		}
	}
	else{
		socket->send("ERROR401");
	}
#endif// !PROGRESS
	statusUpdate(QStringLiteral("��λ4�������"), 0);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

	app->close();
	delete app;
}

void DisplayGUI::VStepThreeSecond()
{

}

void DisplayGUI::createTheoryCircle(CoreAlgorithm::StereoCircle theroyCircle, pcl::PointCloud<pcl::PointXYZ>::Ptr theroyCirclePoints)
{
	double nx = theroyCircle.NormalVector.A, ny = theroyCircle.NormalVector.B, nz = theroyCircle.NormalVector.C;
	double cx = theroyCircle.center.x, cy = theroyCircle.center.y, cz = theroyCircle.center.z;
	double r = theroyCircle.radius;

	double ux = ny, uy = -nx, uz = 0;
	double vx = nx*nz,
		vy = ny*nz,
		vz = -nx*nx - ny*ny;

	double sqrtU = sqrt(ux*ux + uy*uy + uz*uz);
	double sqrtV = sqrt(vx*vx + vy*vy + vz*vz);

	double ux_ = (1 / sqrtU)*ux;
	double uy_ = (1 / sqrtU)*uy;
	double uz_ = (1 / sqrtU)*uz;

	double vx_ = (1 / sqrtV)*vx;
	double vy_ = (1 / sqrtV)*vy;
	double vz_ = (1 / sqrtV)*vz;

	double xi, yi, zi;
	double t = 0;
	double angle = (t / 180.0)*Pi;
	vector<double> x, y, z;

	while (t < 360.0)
	{
		xi = cx + r*(ux_*cos(angle) + vx_*sin(angle));
		yi = cy + r*(uy_*cos(angle) + vy_*sin(angle));
		zi = cz + r*(uz_*cos(angle) + vz_*sin(angle));
		x.push_back(xi);
		y.push_back(yi);
		z.push_back(zi);

		t = t + 1;
		angle = (t / 180.0)*Pi;
	}
	

	//����cloudPoints��С,�������
	theroyCirclePoints->resize(x.size());
	for (int i = 0; i < x.size(); i++){
		//����ά���긳ֵ��PCL��������
		(*theroyCirclePoints)[i].x = x[i];
		(*theroyCirclePoints)[i].y = y[i];
		(*theroyCirclePoints)[i].z = z[i];
	}

	
}
















