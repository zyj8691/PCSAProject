#define SETUP
#include "visualprocessing.h"

VisualProcessing::VisualProcessing(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	alg = new CoreAlgorithm(0);
	lineEdit[lineEditNum] = new QLineEdit();
	//��ʼ������
	engineTypeArray << "YZ1003A" << "YS3003" << "WZ1001" << "WS3001" << "WZ1019" << "WS3005" << "WZ218" << "WS301" << "WZ1008A" << "WS3005A";
	engineTypeFileNameArray << "YZ1003A_YS3003_ConfigInSteps" << "WZ1001_WS3001_ConfigInSteps" << "WZ1019_WS3005_ConfigInSteps" << "WZ218_WS301_ConfigInSteps" << "WZ1008A_WS3005A_ConfigInSteps";
	//��ʼ��lineEdit�ؼ�
	InitializationLineEdit();
	//��ȡ�����������궨����
	ReadConfigParameterFile();
	for (int i = 0; i < lineEditNum; i++){
		lineEdit[i]->setEnabled(false);
	}
	ui.tabWidget->setCurrentIndex(0);
	ui.toolBox->setCurrentIndex(0);
	ui.tabWidget_2->setCurrentIndex(0);
	ui.tabWidget_3->setCurrentIndex(0);

	

}
	
VisualProcessing::~VisualProcessing()
{
	delete alg;
	alg = nullptr;
}

void VisualProcessing::receiveInformation(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	cout << "engineNumFileName;	" << engineNumFileName.toStdString() << endl;
	switch (num)
	{
	case 101:
		stepOneFirstLeftProcessing(num,inputImageFirst, inputImageSecond);
		break;
	case 102:
		stepOneSecondRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 104:
		stepOneSecondRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 201:
		stepTwoLeftProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 202:
		stepTwoRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 203:
		stepTwoLeftFlangeHoleProcessing(num, inputImageFirst);
		break;
	case 204:
		stepTwoRightFlangeHoleProcessing(num, inputImageFirst);
		break;
	case 301:
		stepThreeLeftProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 302:
		stepThreeRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 303:
		stepThreeLeftFlangeHoleProcessing(num, inputImageFirst);
		break;
	case 304:
		stepThreeRightFlangeHoleProcessing(num,inputImageFirst);
		break;
	case 401:{
				 stepFourLeftProcessing(num, inputImageFirst, inputImageSecond);
		break;
	}		
	case 402:{
				 stepFourRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	}		
	case 501:{
				 stepFiveLeftProcessing(num, inputImageFirst, inputImageSecond);
	    break;
	}
	case 502:{
				 stepFiveRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	}
	case 1010:
		stepOneFirstLeftProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 1020:
		stepOneSecondRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 1040:
		stepOneSecondRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 2010:
		stepTwoLeftProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 2020:
		stepTwoRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 2030:
		stepTwoLeftFlangeHoleProcessing(num, inputImageFirst);
		break;
	case 2040:
		stepTwoRightFlangeHoleProcessing(num, inputImageFirst);
		break;
	case 3010:
		stepFourLeftProcessing(num, inputImageFirst, inputImageSecond);
		break;
	case 3020:
		stepFourRightProcessing(num, inputImageFirst, inputImageSecond);
		break;
	default:
		break;
	}
}

void VisualProcessing::InitializationLineEdit()
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

	//��ͷ��ɨ��ʱ�ƶ��ٶ�
	lineEdit[72] = ui.LeftCameraNoScanSpeed_StepOne;
	lineEdit[73] = ui.RightCameraNoScanSpeed_StepOne;
	lineEdit[74] = ui.LeftCameraNoScanSpeed_StepTwo;
	lineEdit[75] = ui.RightCameraNoScanSpeed_StepTwo;
	lineEdit[76] = ui.LeftCameraNoScanSpeed_StepThree;
	lineEdit[77] = ui.RightCameraNoScanSpeed_StepThree;
	lineEdit[78] = ui.LeftCameraNoScanSpeed_StepFour;
	lineEdit[79] = ui.RightCameraNoScanSpeed_StepFour;
	lineEdit[80] = ui.LeftCameraNoScanSpeed_StepFive;
	lineEdit[81] = ui.RightCameraNoScanSpeed_StepFive;

	//���ò��������׵����ƶ�����
	lineEdit[82] = ui.LeftFangleHoleRun_StepTwo;
	lineEdit[83] = ui.RightFangleHoleRun_StepTwo;
	lineEdit[84] = ui.LeftFangleHoleRun_StepThree;
	lineEdit[85] = ui.RightFangleHoleRun_StepThree;

	//���÷�����Լ���뾶
	lineEdit[86] = ui.LeftFlangeHoleRestrictRadius_StepTwo;
	lineEdit[87] = ui.RightFlangeHoleRestrictRadius_StepTwo;
	lineEdit[88] = ui.LeftFlangeHoleRestrictRadius_StepThree;
	lineEdit[89] = ui.RightFlangeHoleRestrictRadius_StepThree;

	//���÷�������Ϣ
	//��λ1
	lineEdit[90] = ui.VerticalLeftRunFirst_StepOne;
	lineEdit[91] = ui.VerticalLeftRunSecond_StepOne;

	lineEdit[92] = ui.VerticalRightRunFirst_StepOne;
	lineEdit[93] = ui.VerticalRightRunSecond_StepOne;

	lineEdit[94] = ui.VerticalLeftRestrictRadius_StepOne;
	lineEdit[95] = ui.VerticalRightRestrictRadius_StepOne;

	lineEdit[96] = ui.VerticalLeftCameraScan_StepOne;
	lineEdit[97] = ui.VerticalRightCameraScan_StepOne;


	lineEdit[98] = ui.VerticalLeftCameraNoScanSpeed_StepOne;
	lineEdit[99] = ui.VerticalRightCameraNoScanSpeed_StepOne;


	//��λ2
	lineEdit[100] = ui.VerticalLeftRunFirst_StepTwo;
	lineEdit[101] = ui.VerticalLeftRunSecond_StepTwo;

	lineEdit[102] = ui.VerticalRightRunFirst_StepTwo;
	lineEdit[103] = ui.VerticalRightRunSecond_StepTwo;

	lineEdit[104] = ui.VerticalLeftRestrictRadius_StepTwo;
	lineEdit[105] = ui.VerticalRightRestrictRadius_StepTwo;

	lineEdit[106] = ui.VerticalLeftCameraScan_StepTwo;
	lineEdit[107] = ui.VerticalRightCameraScan_StepTwo;

	lineEdit[108] = ui.VerticalLeftCameraNoScanSpeed_StepTwo;
	lineEdit[109] = ui.VerticalRightCameraNoScanSpeed_StepTwo;

	lineEdit[110] = ui.VerticalLeftFangleHoleRun_StepTwo;
	lineEdit[111] = ui.VerticalRightFangleHoleRun_StepTwo;

	lineEdit[112] = ui.VerticalLeftFlangeHoleRestrictRadius_StepTwo;
	lineEdit[113] = ui.VerticalRightFlangeHoleRestrictRadius_StepTwo;

	//��λ3
	lineEdit[114] = ui.VerticalLeftRunFirst_StepThree;
	lineEdit[115] = ui.VerticalLeftRunSecond_StepThree;

	lineEdit[116] = ui.VerticalRightRunFirst_StepThree;
	lineEdit[117] = ui.VerticalRightRunSecond_StepThree;

	lineEdit[118] = ui.VerticalLeftRestrictRadius_StepThree;
	lineEdit[119] = ui.VerticalRightRestrictRadius_StepThree;

	lineEdit[120] = ui.VerticalLeftCameraScan_StepThree;
	lineEdit[121] = ui.VerticalRightCameraScan_StepThree;

	lineEdit[122] = ui.VerticalLeftCameraNoScanSpeed_StepThree;
	lineEdit[123] = ui.VerticalRightCameraNoScanSpeed_StepThree;

}

void VisualProcessing::ReadConfigParameterFile()
{
	//��ѡ���ȡ�ĸ������ļ�
	QString fileName = "ConfigFile/engineTypeName.txt";
	QFile file_et(fileName);
	if (!file_et.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Warnning", "engineTypeNameFile can't open", QMessageBox::Yes);
	}
	QTextStream in_et(&file_et);
	QString FName;
	FName = in_et.readLine();
	for (int i = 0; i < engineTypeArray.size(); i++)
	{
		if (FName == engineTypeArray[i]){
			cout << i / 2 << endl;
			FName = engineTypeFileNameArray[i / 2];
			ui.comboBox->setCurrentIndex(i / 2);
			cout << ui.comboBox->currentText().toStdString() << endl;
			cout << FName.toStdString() << endl;
			break;
		}
	}
	//��ָ���ļ�
	fileName = "ConfigFile/" + FName + ".txt";
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Warnning", "configFile can't open", QMessageBox::Yes);
	}
	QTextStream in(&file);
	double variable;
	for (int i = 0; i < lineEditNum; i++){
		variable = in.readLine().toDouble();
		lineEdit[i]->setText(QString::number(variable));
	}
	for (int i = 0; i < lineEditNum; i++){
		lineEdit[i]->setEnabled(false);
	}
}

void VisualProcessing::on_openButton_clicked()
{
	//��ָ���ļ�
	QString fileName = ui.comboBox->currentText(); 
	for (int i = 0; i < fileName.size(); i++)
	{
		if (fileName[i] == QChar('/'))
			fileName[i] = QChar('_');
	}
	fileName = "ConfigFile/"+fileName + "_ConfigInSteps.txt";
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Warnning", "configFile can't open", QMessageBox::Yes);
	}
	QTextStream in(&file);
	double variable;
	for (int i = 0; i < lineEditNum; i++){
		variable = in.readLine().toDouble();
		lineEdit[i]->setText(QString::number(variable));
	}
}

void VisualProcessing::on_setButton_clicked()
{
	QString str = ui.comboBox->currentText();
	cout << str.toStdString() << endl;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == QChar('/'))
			str[i] = QChar('_');
	}
	QMessageBox box;
	box.setWindowTitle(tr("Information"));
	box.setIcon(QMessageBox::Information);
	box.setText(tr("Do you save the datas?"));
	box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	if (box.exec() == QMessageBox::Yes)
	{

		//д���ļ�
		QString fileName = "ConfigFile/" + str + "_ConfigInSteps.txt";//д���ļ���Ŀ¼
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		{
			QMessageBox::warning(this, "ConfigInSteps", "can't open", QMessageBox::Yes);
		}
		QTextStream out(&file);
		for (int i = 0; i < lineEditNum; i++){
			out << lineEdit[i]->text().toDouble() << "\n";
		}
		file.close();
		//�ر�ʹ��
	}
	else
	{

	}
	for (int i = 0; i < lineEditNum; i++){
		lineEdit[i]->setEnabled(false);
	}
}

void VisualProcessing::on_updataButton_clicked()
{
	for (int i = 0; i < lineEditNum; i++){
		lineEdit[i]->setEnabled(true);
	}
}

QString VisualProcessing::ReturnTime()
{
	QDateTime CurrentTime = QDateTime::currentDateTime();
	QString Timestr = CurrentTime.toString("yyyy-MM-dd hh-mm-ss"); // ������ʾ�ĸ�ʽ
	return Timestr;
}

Vector<Point3f> VisualProcessing::RightToLeft(Vector<Point3f> points)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	Vector<Point3f> points_teg;
	double rtol[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			rtol[i][j]=lineEdit[40 + 4*i+j]->text().toDouble();
		}
	}
	Mat RtoL(4, 4, CV_64F, rtol);

	for (int i = 0; i < points.size(); i++){
		ori.at<double>(0, 0) = points[i].x;
		ori.at<double>(1, 0) = points[i].y;
		ori.at<double>(2, 0) = points[i].z;
		ori.at<double>(3, 0) = 1;
		teg = RtoL*ori;
		points_teg.push_back(Point3f(teg.at<double>(0, 0), teg.at<double>(1, 0), teg.at<double>(2, 0)));

	}

	return points_teg;
}

CoreAlgorithm::StereoCircle VisualProcessing::RightToLeft(CoreAlgorithm::StereoCircle centerResult)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	CoreAlgorithm::StereoCircle center_teg;
	double rtol[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			rtol[i][j] = lineEdit[40 + 4 * i + j]->text().toDouble();
		}
	}
	Mat RtoL(4, 4, CV_64F, rtol);

	
	ori.at<double>(0, 0) = centerResult.center.x;
	ori.at<double>(1, 0) = centerResult.center.y;
	ori.at<double>(2, 0) = centerResult.center.z;
	ori.at<double>(3, 0) = 1;
	teg = RtoL*ori;
	center_teg.center.x = teg.at<double>(0, 0);
	center_teg.center.y = teg.at<double>(1, 0);
	center_teg.center.z = teg.at<double>(2, 0);
	return center_teg;
}

CoreAlgorithm::PlaneNormal VisualProcessing::RightToLeft(CoreAlgorithm::PlaneNormal planeparam)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	CoreAlgorithm::PlaneNormal normal_teg;
	double rtol[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			rtol[i][j] = lineEdit[40 + 4 * i + j]->text().toDouble();
		}
	}
	rtol[0][3] = 0;
	rtol[1][3] = 0;
	rtol[2][3] = 0;
	Mat RtoL(4, 4, CV_64F, rtol);

	ori.at<double>(0, 0) = planeparam.A;
	ori.at<double>(1, 0) = planeparam.B;
	ori.at<double>(2, 0) = planeparam.C;
	ori.at<double>(3, 0) = 1;
	teg = RtoL*ori;
	normal_teg.A = teg.at<double>(0, 0);
	normal_teg.B = teg.at<double>(1, 0);
	normal_teg.C = teg.at<double>(2, 0);
	return normal_teg;
}

Vector<Point3f> VisualProcessing::MeasureToRobotTool(Vector<Point3f> points)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	Vector<Point3f> points_teg;
	double rtol[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			rtol[i][j] = lineEdit[56 + 4 * i + j]->text().toDouble();
		}
	}
	Mat RtoL(4, 4, CV_64F, rtol);

	for (int i = 0; i < points.size(); i++){
		ori.at<double>(0, 0) = points[i].x;
		ori.at<double>(1, 0) = points[i].y;
		ori.at<double>(2, 0) = points[i].z;
		ori.at<double>(3, 0) = 1;
		teg = RtoL*ori;
		points_teg.push_back(Point3f(teg.at<double>(0, 0), teg.at<double>(1, 0), teg.at<double>(2, 0)));

	}

	return points_teg;
}

CoreAlgorithm::StereoCircle VisualProcessing::MeasureToRobotTool(CoreAlgorithm::StereoCircle centerResult)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	CoreAlgorithm::StereoCircle center_teg;
	double rtol[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			rtol[i][j] = lineEdit[56 + 4 * i + j]->text().toDouble();
		}
	}
	Mat RtoL(4, 4, CV_64F, rtol);


	ori.at<double>(0, 0) = centerResult.center.x;
	ori.at<double>(1, 0) = centerResult.center.y;
	ori.at<double>(2, 0) = centerResult.center.z;
	ori.at<double>(3, 0) = 1;
	teg = RtoL*ori;
	center_teg.center.x = teg.at<double>(0, 0);
	center_teg.center.y = teg.at<double>(1, 0);
	center_teg.center.z = teg.at<double>(2, 0);
	return center_teg;
}

CoreAlgorithm::PlaneNormal VisualProcessing::MeasureToRobotTool(CoreAlgorithm::PlaneNormal planeparam)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	CoreAlgorithm::PlaneNormal normal_teg;
	double rtol[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			rtol[i][j] = lineEdit[56 + 4 * i + j]->text().toDouble();
		}
	}
	rtol[0][3] = 0;
	rtol[1][3] = 0;
	rtol[2][3] = 0;
	Mat RtoL(4, 4, CV_64F, rtol);

	ori.at<double>(0, 0) = planeparam.A;
	ori.at<double>(1, 0) = planeparam.B;
	ori.at<double>(2, 0) = planeparam.C;
	ori.at<double>(3, 0) = 1;
	teg = RtoL*ori;
	normal_teg.A = teg.at<double>(0, 0);
	normal_teg.B = teg.at<double>(1, 0);
	normal_teg.C = teg.at<double>(2, 0);
	return normal_teg;
}

Vector<Point3f> VisualProcessing::RobotToolToBase(Vector<Point3f> points)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	Vector<Point3f> points_teg;

	for (int i = 0; i < points.size(); i++){
		ori.at<double>(0, 0) = points[i].x;
		ori.at<double>(1, 0) = points[i].y;
		ori.at<double>(2, 0) = points[i].z;
		ori.at<double>(3, 0) = 1;
		teg = robotToolToBaseMatrix*ori;
		points_teg.push_back(Point3f(teg.at<double>(0, 0), teg.at<double>(1, 0), teg.at<double>(2, 0)));

	}

	return points_teg;
}

CoreAlgorithm::StereoCircle VisualProcessing::RobotToolToBase(CoreAlgorithm::StereoCircle centerResult)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	CoreAlgorithm::StereoCircle center_teg;
	
	ori.at<double>(0, 0) = centerResult.center.x;
	ori.at<double>(1, 0) = centerResult.center.y;
	ori.at<double>(2, 0) = centerResult.center.z;
	ori.at<double>(3, 0) = 1;
	teg = robotToolToBaseMatrix*ori;
	center_teg.center.x = teg.at<double>(0, 0);
	center_teg.center.y = teg.at<double>(1, 0);
	center_teg.center.z = teg.at<double>(2, 0);
	return center_teg;
}

CoreAlgorithm::PlaneNormal VisualProcessing::RobotToolToBase(CoreAlgorithm::PlaneNormal planeparam)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	CoreAlgorithm::PlaneNormal normal_teg;

	robotToolToBaseMatrix.at<double>(0, 3) = 0;
	robotToolToBaseMatrix.at<double>(1, 3) = 0;
	robotToolToBaseMatrix.at<double>(2, 3) = 0;
	ori.at<double>(0, 0) = planeparam.A;
	ori.at<double>(1, 0) = planeparam.B;
	ori.at<double>(2, 0) = planeparam.C;
	ori.at<double>(3, 0) = 1;
	teg = robotToolToBaseMatrix*ori;
	normal_teg.A = teg.at<double>(0, 0);
	normal_teg.B = teg.at<double>(1, 0);
	normal_teg.C = teg.at<double>(2, 0);
	return normal_teg;
}

void VisualProcessing::receiveStepNum(int num)
{
	/**
	*���÷������ź�Ϊ101 102��
	*���÷������ź��ں��÷������źŻ�������ĩβ����0����1010 1020
	*/

	double CameraNoScanSpeed;//��ͷ��ɨ��ʱ�˶��ٶ�
	double CameraScanDistance;//��ͷɨ��ʱ�˶�����
	double CameraFirstNoScanDistance;//��ͷ�˶�����һ����λ�õ��˶�����
	double CameraSecondNoScanDistance;//��ͷ�˶����ڶ�����λ�õ��˶�����
	switch (num)
	{
	case 101:
	{
				CameraNoScanSpeed = ui.LeftCameraNoScanSpeed_StepOne->text().toDouble();
				CameraScanDistance = ui.LeftCameraScan_StepOne->text().toDouble();
				CameraFirstNoScanDistance = ui.LeftRunFirst_StepOne->text().toDouble();
				CameraSecondNoScanDistance = ui.LeftRunSecond_StepOne->text().toDouble();
				break;
	}
	case 102:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepOne->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepOne->text().toDouble();
				CameraFirstNoScanDistance = ui.RightRunFirst_StepOne->text().toDouble();
				CameraSecondNoScanDistance = ui.RightRunSecond_StepOne->text().toDouble();
				break;
	}
	case 104:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepOne->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepOne->text().toDouble();
				CameraFirstNoScanDistance = ui.RightRunFirst_StepOne->text().toDouble();
				CameraSecondNoScanDistance = ui.RightRunSecond_StepOne->text().toDouble();
				break;
	}
	case 201:
	{
				CameraNoScanSpeed = ui.LeftCameraNoScanSpeed_StepTwo->text().toDouble();
				CameraScanDistance = ui.LeftCameraScan_StepTwo->text().toDouble();
				CameraFirstNoScanDistance = ui.LeftRunFirst_StepTwo->text().toDouble();
				CameraSecondNoScanDistance = ui.LeftRunSecond_StepTwo->text().toDouble();
				break;
	}
	case 202:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepTwo->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepTwo->text().toDouble();
				CameraFirstNoScanDistance = ui.RightRunFirst_StepTwo->text().toDouble();
				CameraSecondNoScanDistance = ui.RightRunSecond_StepTwo->text().toDouble();
				break;
	}
	case 203:
	{
				CameraNoScanSpeed = ui.LeftCameraNoScanSpeed_StepTwo->text().toDouble();
				CameraScanDistance = ui.LeftCameraScan_StepTwo->text().toDouble();
				CameraFirstNoScanDistance = ui.LeftFangleHoleRun_StepTwo->text().toDouble();
				CameraSecondNoScanDistance = 0;
				break;
	}
	case 204:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepTwo->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepTwo->text().toDouble();
				CameraFirstNoScanDistance = ui.RightFangleHoleRun_StepTwo->text().toDouble();
				CameraSecondNoScanDistance = 0;
				break;
	}

	case 301:
	{
				CameraNoScanSpeed = ui.LeftCameraNoScanSpeed_StepThree->text().toDouble();
				CameraScanDistance = ui.LeftCameraScan_StepThree->text().toDouble();
				CameraFirstNoScanDistance = ui.LeftRunFirst_StepThree->text().toDouble();
				CameraSecondNoScanDistance = ui.LeftRunSecond_StepThree->text().toDouble();
				break;
	}
	case 302:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepThree->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepThree->text().toDouble();
				CameraFirstNoScanDistance = ui.RightRunFirst_StepThree->text().toDouble();
				CameraSecondNoScanDistance = ui.RightRunSecond_StepThree->text().toDouble();
				break;
	}
	case 303:
	{
				CameraNoScanSpeed = ui.LeftCameraNoScanSpeed_StepThree->text().toDouble();
				CameraScanDistance = ui.LeftCameraScan_StepThree->text().toDouble();
				CameraFirstNoScanDistance = ui.LeftFangleHoleRun_StepThree->text().toDouble();
				CameraSecondNoScanDistance = 0;
				break;
	}
	case 304:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepThree->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepThree->text().toDouble();
				CameraFirstNoScanDistance = ui.RightFangleHoleRun_StepThree->text().toDouble();
				CameraSecondNoScanDistance = 0;
				break;
	}
	case 401:
	{
				CameraNoScanSpeed = ui.LeftCameraNoScanSpeed_StepFour->text().toDouble();
				CameraScanDistance = ui.LeftCameraScan_StepFour->text().toDouble();
				CameraFirstNoScanDistance = ui.LeftRunFirst_StepFour->text().toDouble();
				CameraSecondNoScanDistance = ui.LeftRunSecond_StepFour->text().toDouble();
				break;
	}
	case 402:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepFour->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepFour->text().toDouble();
				CameraFirstNoScanDistance = ui.RightRunFirst_StepFour->text().toDouble();
				CameraSecondNoScanDistance = ui.RightRunSecond_StepFour->text().toDouble();
				break;
	}
	case 501:
	{
				CameraNoScanSpeed = ui.LeftCameraNoScanSpeed_StepFive->text().toDouble();
				CameraScanDistance = ui.LeftCameraScan_StepFive->text().toDouble();
				CameraFirstNoScanDistance = ui.LeftRunFirst_StepFive->text().toDouble();
				CameraSecondNoScanDistance = ui.LeftRunSecond_StepFive->text().toDouble();
				break;
	}
	case 502:
	{
				CameraNoScanSpeed = ui.RightCameraNoScanSpeed_StepFive->text().toDouble();
				CameraScanDistance = ui.RightCameraScan_StepFive->text().toDouble();
				CameraFirstNoScanDistance = ui.RightRunFirst_StepFive->text().toDouble();
				CameraSecondNoScanDistance = ui.RightRunSecond_StepFive->text().toDouble();
				break;
	}
	//���÷�����
	case 1010:
	{
				 CameraNoScanSpeed = ui.VerticalLeftCameraNoScanSpeed_StepOne->text().toDouble();
				 CameraScanDistance = ui.VerticalLeftCameraScan_StepOne->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalLeftRunFirst_StepOne->text().toDouble();
				 CameraSecondNoScanDistance = ui.VerticalLeftRunSecond_StepOne->text().toDouble();
				break;
	}
	case 1020:
	{
				 CameraNoScanSpeed = ui.VerticalRightCameraNoScanSpeed_StepOne->text().toDouble();
				 CameraScanDistance = ui.VerticalRightCameraScan_StepOne->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalRightRunFirst_StepOne->text().toDouble();
				 CameraSecondNoScanDistance = ui.VerticalRightRunSecond_StepOne->text().toDouble();
				break;
	}
	case 1040:
	{
				 CameraNoScanSpeed = ui.VerticalRightCameraNoScanSpeed_StepOne->text().toDouble();
				 CameraScanDistance = ui.VerticalRightCameraScan_StepOne->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalRightRunFirst_StepOne->text().toDouble();
				 CameraSecondNoScanDistance = ui.VerticalRightRunSecond_StepOne->text().toDouble();
				break;
	}
	case 2010:
	{
				 CameraNoScanSpeed = ui.VerticalLeftCameraNoScanSpeed_StepTwo->text().toDouble();
				 CameraScanDistance = ui.VerticalLeftCameraScan_StepTwo->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalLeftRunFirst_StepTwo->text().toDouble();
				 CameraSecondNoScanDistance = ui.VerticalLeftRunSecond_StepTwo->text().toDouble();
				break;
	}
	case 2020:
	{
				 CameraNoScanSpeed = ui.VerticalRightCameraNoScanSpeed_StepTwo->text().toDouble();
				 CameraScanDistance = ui.VerticalRightCameraScan_StepTwo->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalRightRunFirst_StepTwo->text().toDouble();
				 CameraSecondNoScanDistance = ui.VerticalRightRunSecond_StepTwo->text().toDouble();
				break;
	}
	case 2030:
	{
				 CameraNoScanSpeed = ui.VerticalLeftCameraNoScanSpeed_StepTwo->text().toDouble();
				 CameraScanDistance = ui.VerticalLeftCameraScan_StepTwo->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalLeftFangleHoleRun_StepTwo->text().toDouble();
				CameraSecondNoScanDistance = 0;
				break;
	}
	case 2040:
	{
				 CameraNoScanSpeed = ui.VerticalRightCameraNoScanSpeed_StepTwo->text().toDouble();
				 CameraScanDistance = ui.VerticalRightCameraScan_StepTwo->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalRightFangleHoleRun_StepTwo->text().toDouble();
				CameraSecondNoScanDistance = 0;
				break;
	}
	case 3010:
	{
				 CameraNoScanSpeed = ui.VerticalLeftCameraNoScanSpeed_StepThree->text().toDouble();
				 CameraScanDistance = ui.VerticalLeftCameraScan_StepThree->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalLeftRunFirst_StepThree->text().toDouble();
				 CameraSecondNoScanDistance = ui.VerticalLeftRunSecond_StepThree->text().toDouble();
				break;
	}
	case 3020:
	{
				 CameraNoScanSpeed = ui.VerticalRightCameraNoScanSpeed_StepThree->text().toDouble();
				 CameraScanDistance = ui.VerticalRightCameraScan_StepThree->text().toDouble();
				 CameraFirstNoScanDistance = ui.VerticalRightRunFirst_StepThree->text().toDouble();
				 CameraSecondNoScanDistance = ui.VerticalRightRunSecond_StepThree->text().toDouble();
				break;
	}
	default:
	{
			   CameraNoScanSpeed = -1;
			   CameraScanDistance = -1;
			   CameraFirstNoScanDistance = -1;
			   CameraSecondNoScanDistance = -1;
			   break;
	}
		
	}

	emit sendStepParam(CameraNoScanSpeed, CameraScanDistance, CameraFirstNoScanDistance, CameraSecondNoScanDistance);

}

void VisualProcessing::stepOneFirstLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double LeftIncrementFirst;
	double LeftIncrementSecond;
	LeftIncrementFirst = ui.LeftRunFirst_StepOne->text().toDouble();
	LeftIncrementSecond = ui.LeftRunFirst_StepOne->text().toDouble() + ui.LeftCameraScan_StepOne->text().toDouble()
		+ ui.LeftRunSecond_StepOne->text().toDouble();
#ifndef SETUP
	LeftIncrementFirst = leftRunDis[0];
	LeftIncrementSecond = leftRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/StepOne/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "left"<<"," << LeftIncrementFirst << "," << LeftIncrementSecond << "\n";
	runfile.close();
	
	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	//HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	//Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	//Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	//bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	//circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
	//circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	//circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	//circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	//cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	//cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + LeftIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + LeftIncrementSecond, planePointsSecond[i].z));
	}

	/*
	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + LeftIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + LeftIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;
	
	ofstream file;
	file.open("OutputFile/unifyCirclePoints.txt");
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		file << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	*/
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		//pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		/*
		//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
		//�������������
		Vector<Point3f> out_circlePoints;
		//�����㵽ƽ����뼯��
		Vector<double> distance;
		//��������ֵΪmm����������ֵΪmm
		alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
		cout << "�������������" << out_circlePoints.size() << endl;
		ofstream outfile;
		outfile.open("OutputFile/out_circlePoints.txt");
		for (int i = 0; i < out_circlePoints.size(); i++){
			outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
		}
		outfile.close();
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		//���пռ�Բ���
		//�뾶Լ��
		double R = ui.LeftRestrictRadius_StepOne->text().toDouble();
		Vector<Point3f> subpoints;
		//�����Ե�㵽���ƽ���ͶӰ��
		subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
		centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
		double averageDistance;
		averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
		cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
		cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
		Vector<double>distances;
		alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
		Vector<Point3f>filterBorderPoints;
		filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
		cout << "����ǰ���� = " << subpoints.size() << endl;
		cout << "���˺���� = " << filterBorderPoints.size() << endl;
		centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
		cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
		averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
		cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
		outfile.open("OutputFile/filterBorderPoints.txt");
		for (int i = 0; i < filterBorderPoints.size(); i++){
			outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
		}
		outfile.close();
		*/
		
		//����任
		//filterBorderPoints = MeasureToRobot(filterBorderPoints);
	//	centerResult = MeasureToRobot(centerResult);
		planeparam = MeasureToRobotTool(planeparam);
		planeparam = RobotToolToBase(planeparam);
		cout << "����任��" << endl;
		//cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
		cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;

		QString time = ReturnTime();
		QString fileName = engineNumFileName + "/outputFile/StepOne/firstPart/engineParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," <<num<<","<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "\n";
		file.close();
		bool planeparamStatus = true;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		
	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		QString time = ReturnTime();
		QString fileName = engineNumFileName + "/outputFile/StepOne/firstPart/engineNormal.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num <<","<< "null" << "," << "null" << "," << "null" << "\n";
		file.close();
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepOneSecondRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double RightIncrementFirst;
	double RightIncrementSecond;
	RightIncrementFirst = ui.RightRunFirst_StepOne->text().toDouble();
	RightIncrementSecond = ui.RightRunFirst_StepOne->text().toDouble() + ui.RightCameraScan_StepOne->text().toDouble()
		+ ui.RightRunSecond_StepOne->text().toDouble();
#ifndef SETUP
	RightIncrementFirst = rightRunDis[0];
	RightIncrementSecond = rightRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepOne/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "right" << "," << RightIncrementFirst << "," << RightIncrementSecond << "\n";
	runfile.close();
	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	if (num == 102||num==104){
		circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	}//���÷�����
	if (num == 1020 || num == 1040){
		circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	}//���÷�����
	
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + RightIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + RightIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + RightIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + RightIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;
	/*unifyPlanePoints = alg->RightToLeft(unifyPlanePoints);
	unifyCirclePoints = alg->RightToLeft(unifyCirclePoints);
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;*/

	QString time = ReturnTime();
	QString fileName;
	if (num == 102)
	{
		fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxUnifyCirclePoints/" + time + " frontDriveBoxUnifyCirclePoints.txt";
	}
	if (num == 1020){
		fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxUnifyCirclePoints/" + time + " syntheticalDriveBoxUnifyCirclePoints.txt";
	}
	if (num == 104 )
	{
		fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxUnifyCirclePoints/" + time + " frontDriveBoxUnifyCirclePoints.txt";
	}
	if (num == 1040){
		fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxUnifyCirclePoints/" + time + " syntheticalDriveBoxUnifyCirclePoints.txt";
	}
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out <<unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();	
	if (unifyPlanePoints.size() != 0)//�ȼ���Ƿ���Խ���ƽ�����
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)//����Ƿ���Խ���Բ�����
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			if (num == 102)
			{
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " frontDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			if (num == 1020){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " syntheticalDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			if (num == 104){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " frontDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			if (num == 1040){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " syntheticalDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}

			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();

			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.RightRestrictRadius_StepOne->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			if (num == 102 || num == 1020){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxFilterBorderPoints/" + time
					+ " frontDriveBoxFilterBorderPoints.txt";
			}
			if (num == 1020){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxFilterBorderPoints/" + time
					+ " syntheticalDriveBoxFilterBorderPoints.txt";
			}
			if (num == 104 || num == 1040){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxFilterBorderPoints/" + time
					+ " frontDriveBoxFilterBorderPoints.txt";
			}
			if (num == 1040){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxFilterBorderPoints/" + time
					+ " syntheticalDriveBoxFilterBorderPoints.txt";
			}

			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			//�Ҳ�ͷ����ϵ�����ͷ����ϵ
			filterBorderPoints = RightToLeft(filterBorderPoints);
			centerResult = RightToLeft(centerResult);
			planeparam = RightToLeft(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//��ͷ����ϵ�������˹�������ϵ
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//�����˹�������ϵ�������˻�������ϵ
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����
			time = ReturnTime();
			if (num == 102){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 1020){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 104){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 1040){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			QString ccircleStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				ccircleStatusStr = "true";
			}
			else{
				centerResult.status = false;
				ccircleStatusStr = "false";
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << ccircleStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//�����������
			if (num == 102){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxFinalBorderPoints/" + time
					+ " frontDriveBoxFinalBorderPoints.txt";
			}
			if (num == 1020){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxFinalBorderPoints/" + time
					+ " syntheticalDriveBoxFinalBorderPoints.txt";
			}
			if (num == 104){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxFinalBorderPoints/" + time
					+ " frontDriveBoxFinalBorderPoints.txt";
			}
			if (num == 1040){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxFinalBorderPoints/" + time
					+ " syntheticalDriveBoxFinalBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;

			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus=true;
			if (num == 102){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 1020){
				fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 104){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 1040){
				fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" <<"\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		if (num == 102){
			fileName = engineNumFileName + "/outputFile/stepOne/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
		}
		if (num == 1020){
			fileName = engineNumFileName + "/outputFile/stepOne/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
		}
		if (num == 104){
			fileName = engineNumFileName + "/outputFile/stepOne/secondPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
		}
		if (num == 1040){
			fileName = engineNumFileName + "/outputFile/stepOne/secondPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
		}
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepTwoLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double LeftIncrementFirst;
	double LeftIncrementSecond;
	LeftIncrementFirst = ui.LeftRunFirst_StepTwo->text().toDouble();
	LeftIncrementSecond = ui.LeftRunFirst_StepTwo->text().toDouble() + ui.LeftCameraScan_StepTwo->text().toDouble()
		+ ui.LeftRunSecond_StepTwo->text().toDouble();
#ifndef SETUP
	LeftIncrementFirst = leftRunDis[0];
	LeftIncrementSecond = leftRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepTwo/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "left" << "," << LeftIncrementFirst << "," << LeftIncrementSecond << "\n";
	runfile.close();

	
	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	if (num == 201 ){
		circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	}
	if (num == 2010 ){
		circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	}
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + LeftIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + LeftIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + LeftIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + LeftIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;
	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineUnifyCirclePoints/" + time + " engineUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << num << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineCirclePointsThroughDistanceThreshold/" + time
				+ " engineCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.LeftRestrictRadius_StepOne->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineFilterBorderPoints/" + time
				+ " engineFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			//����任
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//����任
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������
			fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineFinalBorderPoints/" + time
				+ " engineFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/engineParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepTwoRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double RightIncrementFirst;
	double RightIncrementSecond;
	RightIncrementFirst = ui.RightRunFirst_StepTwo->text().toDouble();
	RightIncrementSecond = ui.RightRunFirst_StepTwo->text().toDouble() + ui.RightCameraScan_StepTwo->text().toDouble()
		+ ui.RightRunSecond_StepTwo->text().toDouble();
#ifndef SETUP
	RightIncrementFirst = rightRunDis[0];
	RightIncrementSecond = rightRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepTwo/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "right" << "," << RightIncrementFirst << "," << RightIncrementSecond << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	if (num == 202){
		circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	}
	if (num == 2020){
		circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	}
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + RightIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + RightIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + RightIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + RightIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;


	QString time = ReturnTime();
	QString fileName;
	if (num == 202){
		fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxUnifyCirclePoints/" + time + " frontDriveBoxUnifyCirclePoints.txt";
	}
	if (num==2020){
		fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxUnifyCirclePoints/" + time + " syntheticalDriveBoxUnifyCirclePoints.txt";
	}
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();

	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			if (num == 202){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " frontDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			if (num == 2020){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " syntheticalDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.RightRestrictRadius_StepOne->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			if (num == 202){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxFilterBorderPoints/" + time
					+ " frontDriveBoxFilterBorderPoints.txt";
			}
			if (num == 2020){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxFilterBorderPoints/" + time
					+ " syntheticalDriveBoxFilterBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			//����任
			//�Ҳ�ͷ����ϵ�����ͷ����ϵ
			filterBorderPoints = RightToLeft(filterBorderPoints);
			centerResult = RightToLeft(centerResult);
			planeparam = RightToLeft(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//��ͷ����ϵ�������˹�������ϵ
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//�����˹�������ϵ�������˻�������ϵ
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			if (num == 202){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 2020){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << ","<<num<<"," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������
			if (num == 202){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxFinalBorderPoints/" + time
					+ " frontDriveBoxFinalBorderPoints.txt";
			}
			if (num == 2020){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxFinalBorderPoints/" + time
					+ " syntheticalDriveBoxFinalBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			time = ReturnTime();
			if (num == 202){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 2020){
				fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< "null" << "\n";
			file.close();
			bool planeparamStatus = true;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		if (num == 202){
			fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
		}
		if (num == 2020){
			fileName = engineNumFileName + "/outputFile/stepTwo/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
		}
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepTwoLeftFlangeHoleProcessing(int num, HObject inputImage)
{
	//�����ͷ����ϵ��Y������
	double LeftIncrementFirst;
	LeftIncrementFirst = ui.LeftFangleHoleRun_StepTwo->text().toDouble();
#ifndef SETUP
	LeftIncrementFirst = leftRunDis[0];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepTwo/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "left" << "," << LeftIncrementFirst << "\n";
	runfile.close();
	
	//����ƽ�洦��ͼƬ
	HObject planeFirstImage;
	//������Ե����ͼƬ
	HObject circleFirstImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst;
	bool status_circleFirst;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImage, status_planeFirst);
	//��ȡ��Ե����ͼƬ
	if (num == 203){
		circleFirstImage = alg->getFlangeHoleEdgeImage(inputImage, status_circleFirst);
	}
	if (num == 2030){
		circleFirstImage = alg->getFlangeHoleEdgeImage(inputImage, status_circleFirst);
	}
	
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	cout << "ƽ��㼯������" << planePointsFirst.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	cout << "��Ե�㼯������" << circlePointsFirst.size() << endl;

	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + LeftIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + LeftIncrementFirst, circlePointsFirst[i].z));
	}

	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleUnifyCirclePoints/" + time + " engineFlangeHoleUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleCirclePointsThroughDistanceThreshold/" + time
				+ " engineFlangeHoleCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.LeftFlangeHoleRestrictRadius_StepTwo->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.2);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleFilterBorderPoints/" + time
				+ " engineFlangeHoleFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//����任
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������
			fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleFinalBorderPoints/" + time
				+ " engineFlangeHoleFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/engineFlangeHoleParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepTwoRightFlangeHoleProcessing(int num, HObject inputImage)
{
	//�����ͷ����ϵ��Y������
	double RightIncrementFirst;
	RightIncrementFirst = ui.RightFangleHoleRun_StepTwo->text().toDouble();
#ifndef SETUP
	RightIncrementFirst = rightRunDis[0];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepTwo/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "right" << "," << RightIncrementFirst << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage;
	//������Ե����ͼƬ
	HObject circleFirstImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst;
	bool status_circleFirst;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImage, status_planeFirst);
	//��ȡ��Ե����ͼƬ
	if (num == 204){
		circleFirstImage = alg->getFlangeHoleEdgeImage(inputImage, status_circleFirst);
	}
	if (num == 2040){
		circleFirstImage = alg->getFlangeHoleEdgeImage(inputImage, status_circleFirst);
	}
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	cout << "ƽ��㼯������" << planePointsFirst.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	cout << "��Ե�㼯������" << circlePointsFirst.size() << endl;

	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + RightIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + RightIncrementFirst, circlePointsFirst[i].z));
	}


	QString time = ReturnTime();
	QString fileName;
	if (num == 204){
		fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleUnifyCirclePoints/" + time
			+ " frontDriveBoxFlangeHoleUnifyCirclePoints.txt";
	}
	if (num == 2040){
		fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleUnifyCirclePoints/" + time
			+ " syntheticalDriveBoxFlangeHoleUnifyCirclePoints.txt";
	}
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out<< unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyPlanePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			if (num == 204){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold/" + time
					+ " frontDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold.txt";
			}
			if (num == 2040){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold/" + time
					+ " syntheticalDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.RightFlangeHoleRestrictRadius_StepTwo->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.2);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			if (num == 204){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleFilterBorderPoints/" + time
					+ " frontDriveBoxFlangeHoleFilterBorderPoints.txt";
			}
			if (num == 2040){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleFilterBorderPoints/" + time
					+ " syntheticalDriveBoxFlangeHoleFilterBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			//����任
			//�Ҳ�ͷ����ϵ�����ͷ����ϵ
			filterBorderPoints = RightToLeft(filterBorderPoints);
			centerResult = RightToLeft(centerResult);
			planeparam = RightToLeft(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			if (num == 204){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 2040){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������
			if (num == 204){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleFinalBorderPoints/" + time
					+ " frontDriveBoxFlangeHoleFinalBorderPoints.txt";
			}
			if (num == 2040){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleFinalBorderPoints/" + time
					+ " syntheticalDriveBoxFlangeHoleFinalBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			if (num == 204){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 2040){
				fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		if (num == 204){
			fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/frontDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
		}
		if (num == 2040){
			fileName = engineNumFileName + "/outputFile/stepTwo/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
		}
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepThreeLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double LeftIncrementFirst;
	double LeftIncrementSecond;
	LeftIncrementFirst = ui.LeftRunFirst_StepThree->text().toDouble();
	LeftIncrementSecond = ui.LeftRunFirst_StepThree->text().toDouble() + ui.LeftCameraScan_StepThree->text().toDouble()
		+ ui.LeftRunSecond_StepThree->text().toDouble();
#ifndef SETUP
	LeftIncrementFirst = leftRunDis[0];
	LeftIncrementSecond = leftRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "left" << "," << LeftIncrementFirst << "," << LeftIncrementSecond << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
	circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + LeftIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + LeftIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + LeftIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + LeftIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;
	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxUnifyCirclePoints/" + time + " syntheticalDriveBoxUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold/" + time
				+ " syntheticalDriveBoxCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.LeftRestrictRadius_StepOne->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFilterBorderPoints/" + time
				+ " syntheticalDriveBoxFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//����任
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints/" + time
				+ " syntheticalDriveBoxFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints/";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints/";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepThreeRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double RightIncrementFirst;
	double RightIncrementSecond;
	RightIncrementFirst = ui.RightRunFirst_StepThree->text().toDouble();
	RightIncrementSecond = ui.RightRunFirst_StepThree->text().toDouble() + ui.RightCameraScan_StepThree->text().toDouble()
		+ ui.RightRunSecond_StepThree->text().toDouble();
#ifndef SETUP
	RightIncrementFirst = rightRunDis[0];
	RightIncrementSecond = rightRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "right" << "," << RightIncrementFirst << "," << RightIncrementSecond << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	circleFirstImage = alg->getFlangeEdgeImage(inputImageFirst, status_circleFirst);
	circleSecondImage = alg->getFlangeEdgeImage(inputImageSecond, status_circleSecond);
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + RightIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + RightIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + RightIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + RightIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;


	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxUnifyCirclePoints/" + time + " frontDriveBoxUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();

	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyPlanePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold/" + time
				+ " frontDriveBoxCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.RightRestrictRadius_StepOne->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxFilterBorderPoints/" + time
				+ " frontDriveBoxFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();


			//����任
			//�Ҳ�ͷ����ϵ�����ͷ����ϵ
			filterBorderPoints = RightToLeft(filterBorderPoints);
			centerResult = RightToLeft(centerResult);
			planeparam = RightToLeft(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//��ͷ����ϵ�������˹�������ϵ
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//�����˹�������ϵ�������˻�������ϵ
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxFinalBorderPoints/" + time
				+ " frontDriveBoxFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepThreeLeftFlangeHoleProcessing(int num, HObject inputImage)
{
	//�����ͷ����ϵ��Y������
	double LeftIncrementFirst;
	LeftIncrementFirst = ui.LeftFangleHoleRun_StepThree->text().toDouble();
#ifndef SETUP
	LeftIncrementFirst = leftRunDis[0];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "left" << "," << LeftIncrementFirst << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage;
	//������Ե����ͼƬ
	HObject circleFirstImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst;
	bool status_circleFirst;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImage, status_planeFirst);
	//��ȡ��Ե����ͼƬ
	circleFirstImage = alg->getFlangeHoleEdgeImage(inputImage, status_circleFirst);
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	cout << "ƽ��㼯������" << planePointsFirst.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	cout << "��Ե�㼯������" << circlePointsFirst.size() << endl;

	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + LeftIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + LeftIncrementFirst, circlePointsFirst[i].z));
	}

	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleUnifyCirclePoints/" + time
		+ " syntheticalDriveBoxFlangeHoleUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyCirclePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold/" + time
				+ " syntheticalDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.LeftFlangeHoleRestrictRadius_StepTwo->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.2);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleFilterBorderPoints/" + time
				+ " syntheticalDriveBoxFlangeHoleFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//����任
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleFinalBorderPoints/" + time
				+ " syntheticalDriveBoxFlangeHoleFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepThree/secondPart/syntheticalDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepThreeRightFlangeHoleProcessing(int num, HObject inputImage)
{
	//�����ͷ����ϵ��Y������
	double RightIncrementFirst;
	RightIncrementFirst = ui.RightFangleHoleRun_StepThree->text().toDouble();;
#ifndef SETUP
	RightIncrementFirst = rightRunDis[0];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "right" << "," << RightIncrementFirst << "\n";
	runfile.close();
	//����ƽ�洦��ͼƬ
	HObject planeFirstImage;
	//������Ե����ͼƬ
	HObject circleFirstImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst;
	bool status_circleFirst;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImage, status_planeFirst);
	//��ȡ��Ե����ͼƬ
	circleFirstImage = alg->getFlangeHoleEdgeImage(inputImage, status_circleFirst);
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	cout << "ƽ��㼯������" << planePointsFirst.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	cout << "��Ե�㼯������" << circlePointsFirst.size() << endl;

	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + RightIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + RightIncrementFirst, circlePointsFirst[i].z));
	}


	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleUnifyCirclePoints/" + time + " frontDriveBoxFlangeHoleUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.7, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold/" + time
				+ " frontDriveBoxFlangeHoleCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.RightFlangeHoleRestrictRadius_StepTwo->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.2);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleFilterBorderPoints/" + time
				+ " frontDriveBoxFlangeHoleFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			//����任
			//�Ҳ�ͷ����ϵ�����ͷ����ϵ
			filterBorderPoints = RightToLeft(filterBorderPoints);
			centerResult = RightToLeft(centerResult);
			planeparam = RightToLeft(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleFinalBorderPoints/" + time
				+ " frontDriveBoxFlangeHoleFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepThree/secondPart/frontDriveBoxFlangeHoleParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepFourLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)

{
	//�����ͷ����ϵ��Y������
	double LeftIncrementFirst;
	double LeftIncrementSecond;
	LeftIncrementFirst = ui.LeftRunFirst_StepFour->text().toDouble();
	LeftIncrementSecond = ui.LeftRunFirst_StepFour->text().toDouble() + ui.LeftCameraScan_StepFour->text().toDouble()
		+ ui.LeftRunSecond_StepFour->text().toDouble();
#ifndef SETUP
	LeftIncrementFirst = leftRunDis[0];
	LeftIncrementSecond = leftRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName;
	if (num == 401){
		runfileName = engineNumFileName + "/outputFile/stepFour/GratingData.csv";//д���ļ���Ŀ¼
	}
	if (num == 3010){
		runfileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";//д���ļ���Ŀ¼
	}
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "left" << "," << LeftIncrementFirst << "," << LeftIncrementSecond << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	if (num == 401){
		circleFirstImage = alg->getEngineSplineEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getEngineSplineEdgeImage(inputImageSecond, status_circleSecond);
	}
	if (num == 3010){
		circleFirstImage = alg->getEngineSplineEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getEngineSplineEdgeImage(inputImageSecond, status_circleSecond);
	}

	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + LeftIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + LeftIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + LeftIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + LeftIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;
	QString time = ReturnTime();
	QString fileName;
	if (num == 401){
		fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxUnifyCirclePoints/" + time + " syntheticalDriveBoxUnifyCirclePoints.txt";
	}
	if (num == 3010){
		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineUnifyCirclePoints/" + time + " engineUnifyCirclePoints.txt";
	}
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyPlanePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.2, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			if (num == 401){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " syntheticalDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			if (num == 3010){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineCirclePointsThroughDistanceThreshold/" + time
					+ " engineCirclePointsThroughDistanceThreshold.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.LeftRestrictRadius_StepFour->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			if (num == 401){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxFilterBorderPoints/" + time
					+ " syntheticalDriveBoxFilterBorderPoints.txt";
			}
			if (num == 3010){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineFilterBorderPoints/" + time
					+ " engineFilterBorderPoints.txt";
			}

			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//����任
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			if (num == 401){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 3010){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineParam.csv";//д���ļ���Ŀ¼
			}

			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			if (num == 401){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxFinalBorderPoints/" + time
					+ " syntheticalDriveBoxFinalBorderPoints.txt";
			}
			if (num == 3010){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineFinalBorderPoints/" + time
					+ " engineFinalBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			if (num == 401){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 3010){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineParam.csv";//д���ļ���Ŀ¼
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		if (num == 401){
			fileName = engineNumFileName + "/outputFile/stepFour/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
		}
		if (num == 3010){
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/engineParam.csv";//д���ļ���Ŀ¼
		}
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}

}

void VisualProcessing::stepFourRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double RightIncrementFirst;
	double RightIncrementSecond;
	RightIncrementFirst = ui.RightRunFirst_StepFour->text().toDouble();
	RightIncrementSecond = ui.RightRunFirst_StepFour->text().toDouble() + ui.RightCameraScan_StepFour->text().toDouble()
		+ ui.RightRunSecond_StepFour->text().toDouble();
#ifndef SETUP
	RightIncrementFirst = rightRunDis[0];
	RightIncrementSecond = rightRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName;
	if (num == 402){
		runfileName = engineNumFileName + "/outputFile/stepFour/GratingData.csv";//д���ļ���Ŀ¼
	}
	if (num == 3020){
		runfileName = engineNumFileName + "/outputFile/stepThree/GratingData.csv";//д���ļ���Ŀ¼
	}
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "right" << "," << RightIncrementFirst << "," << RightIncrementSecond << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	if (num == 402){
		circleFirstImage = alg->getfrontDriveBoxSplineEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getfrontDriveBoxSplineEdgeImage(inputImageSecond, status_circleSecond);
	}
	if (num == 3020){
		circleFirstImage = alg->getfrontDriveBoxSplineEdgeImage(inputImageFirst, status_circleFirst);
		circleSecondImage = alg->getfrontDriveBoxSplineEdgeImage(inputImageSecond, status_circleSecond);
	}

	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + RightIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + RightIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + RightIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + RightIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;


	QString time = ReturnTime();
	QString fileName;
	if (num == 402){
		fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxUnifyCirclePoints/" + time
			+ " frontDriveBoxUnifyCirclePoints.txt";
	}
	if (num == 3020){
		fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxUnifyCirclePoints/" + time
			+ " syntheticalDriveBoxUnifyCirclePoints.txt";
	}
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.2, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			if (num == 402){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " frontDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			if (num == 3020){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxCirclePointsThroughDistanceThreshold/" + time
					+ " syntheticalDriveBoxCirclePointsThroughDistanceThreshold.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.RightRestrictRadius_StepFour->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.2);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			if (num == 402){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxFilterBorderPoints/" + time
					+ " frontDriveBoxFilterBorderPoints.txt";
			}
			if (num == 3020){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFilterBorderPoints/" + time
					+ " syntheticalDriveBoxFilterBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();


			//����任
			//�Ҳ�ͷ����ϵ�����ͷ����ϵ
			filterBorderPoints = RightToLeft(filterBorderPoints);
			centerResult = RightToLeft(centerResult);
			planeparam = RightToLeft(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//��ͷ����ϵ�������˹�������ϵ
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//�����˹�������ϵ�������˻�������ϵ
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			if (num == 402){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			if (num == 3020){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxParam.csv";//д���ļ���Ŀ¼
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			if (num == 402){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxFinalBorderPoints/" + time
					+ " frontDriveBoxFinalBorderPoints.txt";
			}
			if (num == 3020){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints/" + time
					+ " frontDriveBoxFinalBorderPoints.txt";
			}
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);

		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			if (num == 402){
				fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxFinalBorderPoints/" + time
					+ " frontDriveBoxFinalBorderPoints.txt";
			}
			if (num == 3020){
				fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints/" + time
					+ " frontDriveBoxFinalBorderPoints.txt";
			}
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		if (num == 402){
			fileName = engineNumFileName + "/outputFile/stepFour/firstPart/frontDriveBoxFinalBorderPoints/" + time
				+ " frontDriveBoxFinalBorderPoints.txt";
		}
		if (num == 3020){
			fileName = engineNumFileName + "/outputFile/stepThree/firstPart/syntheticalDriveBoxFinalBorderPoints/" + time
				+ " frontDriveBoxFinalBorderPoints.txt";
		}
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::stepFiveLeftProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double LeftIncrementFirst;
	double LeftIncrementSecond;
	LeftIncrementFirst = ui.LeftRunFirst_StepFive->text().toDouble();
	LeftIncrementSecond = ui.LeftRunFirst_StepFive->text().toDouble() + ui.LeftCameraScan_StepFive->text().toDouble()
		+ ui.LeftRunSecond_StepFive->text().toDouble();
#ifndef SETUP
	LeftIncrementFirst = leftRunDis[0];
	LeftIncrementSecond = leftRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepFive/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "left" << "," << LeftIncrementFirst << "," << LeftIncrementSecond << "\n";
	runfile.close();


	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	circleFirstImage = alg->getEngineSplineEdgeImage(inputImageFirst, status_circleFirst);
	circleSecondImage = alg->getEngineSplineEdgeImage(inputImageSecond, status_circleSecond);
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + LeftIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + LeftIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + LeftIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + LeftIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;
	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineUnifyCirclePoints/" + time + " engineUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyCirclePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.2, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineCirclePointsThroughDistanceThreshold/" + time
				+ " engineCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.LeftRestrictRadius_StepFour->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.05);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineFilterBorderPoints/" + time
				+ " engineFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();

			//����任
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//����任
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineFinalBorderPoints/" + time
				+ " engineFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepFive/firstPart/engineParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}

}

void VisualProcessing::stepFiveRightProcessing(int num, HObject inputImageFirst, HObject inputImageSecond)
{
	//�����ͷ����ϵ��Y������
	double RightIncrementFirst;
	double RightIncrementSecond;
	RightIncrementFirst = ui.RightRunFirst_StepFive->text().toDouble();
	RightIncrementSecond = ui.RightRunFirst_StepFive->text().toDouble() + ui.RightCameraScan_StepFive->text().toDouble()
		+ ui.RightRunSecond_StepFive->text().toDouble();
#ifndef SETUP
	RightIncrementFirst = rightRunDis[0];
	RightIncrementSecond = rightRunDis[1];
#endif//SETUP
	QString runtime = ReturnTime();
	QString runfileName = engineNumFileName + "/outputFile/stepFive/GratingData.csv";//д���ļ���Ŀ¼
	QFile runfile(runfileName);
	runfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream runout(&runfile);
	runout << runtime << "," << "right" << "," << RightIncrementFirst << "," << RightIncrementSecond << "\n";
	runfile.close();

	//����ƽ�洦��ͼƬ
	HObject planeFirstImage, planeSecondImage;
	//������Ե����ͼƬ
	HObject circleFirstImage, circleSecondImage;
	//����ƽ��㼯
	Vector<Point3f> planePointsFirst, planePointsSecond;
	//����ͳһ����ϵ��ƽ��㼯
	Vector<Point3f> unifyPlanePoints;
	//������Ե�㼯
	Vector<Point3f> circlePointsFirst, circlePointsSecond;
	//����ͳһ����ϵ�±�Ե�㼯
	Vector<Point3f> unifyCirclePoints;
	//����ͼ������ true or false
	bool status_planeFirst, status_planeSecond;
	bool status_circleFirst, status_circleSecond;
	//��ȡƽ�洦��ͼƬ
	planeFirstImage = alg->getPlaneImage(inputImageFirst, status_planeFirst);
	planeSecondImage = alg->getPlaneImage(inputImageSecond, status_planeSecond);
	//��ȡ��Ե����ͼƬ
	circleFirstImage = alg->getfrontDriveBoxSplineEdgeImage(inputImageFirst, status_circleFirst);
	circleSecondImage = alg->getfrontDriveBoxSplineEdgeImage(inputImageSecond, status_circleSecond);
	//��ȡƽ��㼯
	planePointsFirst = alg->depthMapCalibration(planeFirstImage, status_planeFirst);
	planePointsSecond = alg->depthMapCalibration(planeSecondImage, status_planeSecond);
	cout << "1ƽ��㼯������" << planePointsFirst.size() << endl;
	cout << "2ƽ��㼯������" << planePointsSecond.size() << endl;
	//��ȡ��Ե�㼯
	circlePointsFirst = alg->depthMapCalibration(circleFirstImage, status_circleFirst);
	circlePointsSecond = alg->depthMapCalibration(circleSecondImage, status_circleSecond);
	cout << "1��Ե�㼯������" << circlePointsFirst.size() << endl;
	cout << "2��Ե�㼯������" << circlePointsSecond.size() << endl;
	//�ɼ��㣬ͳһ����ϵ
	for (int i = 0; i < planePointsFirst.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsFirst[i].x, planePointsFirst[i].y + RightIncrementFirst, planePointsFirst[i].z));
	}

	for (int i = 0; i < planePointsSecond.size(); i++){
		unifyPlanePoints.push_back(Point3f(planePointsSecond[i].x, planePointsSecond[i].y + RightIncrementSecond, planePointsSecond[i].z));
	}

	for (int i = 0; i < circlePointsFirst.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsFirst[i].x, circlePointsFirst[i].y + RightIncrementFirst, circlePointsFirst[i].z));
	}

	for (int i = 0; i < circlePointsSecond.size(); i++){
		unifyCirclePoints.push_back(Point3f(circlePointsSecond[i].x, circlePointsSecond[i].y + RightIncrementSecond, circlePointsSecond[i].z));
	}
	cout << "ͳһ�����ƽ��㼯������" << unifyPlanePoints.size() << endl;
	cout << "ͳһ������Ե�㼯������" << unifyCirclePoints.size() << endl;


	QString time = ReturnTime();
	QString fileName;
	fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxUnifyCirclePoints/" + time + " frontDriveBoxUnifyCirclePoints.txt";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
	QTextStream out(&file);
	for (int i = 0; i < unifyCirclePoints.size(); i++){
		out << unifyCirclePoints[i].x << " " << unifyCirclePoints[i].y << " " << unifyCirclePoints[i].z << endl;
	}
	file.close();
	if (unifyPlanePoints.size() != 0)
	{
		//����ƽ����ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloudPoints(new pcl::PointCloud<pcl::PointXYZ>);
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		//ƽ��㼯ת��ΪPCL����
		alg->threeDimensionToPCLPoints(unifyPlanePoints, plane_cloudPoints);
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//��ȡƽ�淨ʸ
		planeparam = alg->getPlaneNormal(plane_cloudPoints);
		if (unifyPlanePoints.size() != 0)
		{
			//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
			//�������������
			Vector<Point3f> out_circlePoints;
			//�����㵽ƽ����뼯��
			Vector<double> distance;
			//��������ֵΪmm����������ֵΪmm
			alg->setDistanceThresholdAndGetPoints(unifyCirclePoints, planeparam, distance, 0, 0.2, out_circlePoints);
			cout << "�������������" << out_circlePoints.size() << endl;
			ofstream outfile;
			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxCirclePointsThroughDistanceThreshold/" + time
				+ " frontDriveBoxCirclePointsThroughDistanceThreshold.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < out_circlePoints.size(); i++){
				outfile << out_circlePoints[i].x << " " << out_circlePoints[i].y << " " << out_circlePoints[i].z << endl;
			}
			outfile.close();
			//�����ռ�����
			CoreAlgorithm::StereoCircle centerResult;
			//���пռ�Բ���
			//�뾶Լ��
			double R = ui.RightRestrictRadius_StepFour->text().toDouble();
			Vector<Point3f> subpoints;
			//�����Ե�㵽���ƽ���ͶӰ��
			subpoints = alg->ComputedEdgeProjectionPoints(out_circlePoints, planeparam);
			centerResult = alg->fitSteroCircle(subpoints, planeparam, R);
			double averageDistance;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(subpoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			Vector<double>distances;
			alg->CalculateBorderPointsToCenterDistance(subpoints, centerResult, distances);
			Vector<Point3f>filterBorderPoints;
			filterBorderPoints = alg->filterBorderPointsOnDistanceThrehold(subpoints, distances, R, 0.2);
			cout << "����ǰ���� = " << subpoints.size() << endl;
			cout << "���˺���� = " << filterBorderPoints.size() << endl;
			centerResult = alg->fitSteroCircle(filterBorderPoints, planeparam, R);
			cout << "�ռ�Բ�����꣺" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			averageDistance = alg->CalculateBorderPointsToCenterAverageDistance(filterBorderPoints, centerResult);
			cout << endl << "��Ե�㵽�ռ�Բ�ĵ�ƽ������Ϊ�� " << averageDistance << endl << endl;

			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxFilterBorderPoints/" + time
				+ " frontDriveBoxFilterBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();


			//����任
			//�Ҳ�ͷ����ϵ�����ͷ����ϵ
			filterBorderPoints = RightToLeft(filterBorderPoints);
			centerResult = RightToLeft(centerResult);
			planeparam = RightToLeft(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//��ͷ����ϵ�������˹�������ϵ
			filterBorderPoints = MeasureToRobotTool(filterBorderPoints);
			centerResult = MeasureToRobotTool(centerResult);
			planeparam = MeasureToRobotTool(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//�����˹�������ϵ�������˻�������ϵ
			filterBorderPoints = RobotToolToBase(filterBorderPoints);
			centerResult = RobotToolToBase(centerResult);
			planeparam = RobotToolToBase(planeparam);
			cout << "����任��" << endl;
			cout << "centerResult :" << centerResult.center.x << " " << centerResult.center.y << " " << centerResult.center.z << endl;
			cout << "planeparam :" << planeparam.A << " " << planeparam.B << " " << planeparam.C << endl;
			//���洦����Ϣ
			QString cirlceStatusStr;
			if (abs(averageDistance - R) < 0.1){
				centerResult.status = true;
				cirlceStatusStr = "true";
			}
			else{
				centerResult.status = false;
				cirlceStatusStr = "false";
			}
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << centerResult.center.x << "," << centerResult.center.y << "," << centerResult.center.z << "," << cirlceStatusStr << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << ","
				<< averageDistance << "\n";
			file.close();
			//����������

			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxFinalBorderPoints/" + time
				+ " frontDriveBoxFinalBorderPoints.txt";
			outfile.open(fileName.toStdString());
			for (int i = 0; i < filterBorderPoints.size(); i++){
				outfile << filterBorderPoints[i].x << " " << filterBorderPoints[i].y << " " << filterBorderPoints[i].z << endl;
			}
			outfile.close();
			
			bool planeparamStatus = true;
			//��Ե�㼯ת��ΪPCL����
			alg->threeDimensionToPCLPoints(filterBorderPoints, borderCloud_ptr);
			//��ֵcenterResult�ṹֵ
			centerResult.NormalVector.A = planeparam.A;
			centerResult.NormalVector.B = planeparam.B;
			centerResult.NormalVector.C = planeparam.C;
			centerResult.radius = averageDistance;
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}
		else
		{
			CoreAlgorithm::StereoCircle centerResult;
			centerResult.status = false;
			bool planeparamStatus = true;
			time = ReturnTime();
			fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
			QFile file(fileName);
			file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
			QTextStream out(&file);
			out << time << "," << num << "," << "null" << "," << "null" << "," << "null" << "," << "false" << ","
				<< planeparam.A << "," << planeparam.B << "," << planeparam.C << "," << "null" << "\n";
			file.close();
			emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
		}

	}
	else
	{
		bool planeparamStatus = false;
		//�����ռ�ƽ�淨ʸ���ݽṹ
		CoreAlgorithm::PlaneNormal planeparam;
		//�����ռ�����
		CoreAlgorithm::StereoCircle centerResult;
		centerResult.status = false;
		//������Ե���ƣ�PCL��
		pcl::PointCloud<pcl::PointXYZ>::Ptr borderCloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
		time = ReturnTime();
		fileName = engineNumFileName + "/outputFile/stepFive/firstPart/frontDriveBoxParam.csv";//д���ļ���Ŀ¼
		QFile file(fileName);
		file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);
		out << time << "," << num << "," << "null" /*Բ��X*/ << "," << "null" /*Բ��Y*/ << "," << "null"/*Բ��Z*/ << "," << "false"/*Բ�����״̬*/ << ","
			<< "null"/*��ʸA*/ << "," << "null"/*��ʸB*/ << "," << "null"/*��ʸC*/ << "," << "null" /*��Ե�㵽Բ�ĵ�ƽ������*/ << "\n";
		file.close();
		emit sendProcessingResult(planeparamStatus, planeparam, centerResult, borderCloud_ptr, num);
	}
}

void VisualProcessing::receiveLeftGratingData(vector<double>left)
{
	//�����
	leftRunDis.clear();
	//�ٸ�ֵ
	leftRunDis = left;
}

void VisualProcessing::receiveRightGratingData(vector<double>right)
{
	//�����
	rightRunDis.clear();
	//�ٸ�ֵ
	rightRunDis = right;
}

void VisualProcessing::receiveRobotPose(RobotPose robotPo)
{
	robotPose.X = robotPo.X;
	robotPose.Y = robotPo.Y;
	robotPose.Z = robotPo.Z;
	robotPose.A = robotPo.A;
	robotPose.B = robotPo.B;
	robotPose.C = robotPo.C;
	calculateRobotToolToBaseMatrix(robotPose);
}

void VisualProcessing::calculateRobotToolToBaseMatrix(RobotPose robotPose)
{
	robotToolToBaseMatrix.release();
	robotToolToBaseMatrix = Mat(4, 4, CV_64F);

	//������ת����
	double a = deg2rad(robotPose.A);//��z����ת
	double b = deg2rad(robotPose.B);
	double c = deg2rad(robotPose.C);

	double rotz[3][3] = {
		cos(a), -sin(a), 0,
		sin(a), cos(a), 0,
		0, 0, 1 };
	double roty[3][3] = {
		cos(b), 0, sin(b),
		0, 1, 0,
		-sin(b), 0, cos(b)

	};
	double rotx[3][3] = {
		1, 0, 0,
		0, cos(c), -sin(c),
		0, sin(c), cos(c)

	};
	Mat RZ(3, 3, CV_64F, rotz);
	Mat RY(3, 3, CV_64F, roty);
	Mat RX(3, 3, CV_64F, rotx);
	Mat MR(3, 3, CV_64F);
	MR = RZ*RY*RX;
	
	//����ƽ�ƾ���
	double trans[3][1] =
	{ robotPose.X, robotPose.Y, robotPose.Z };
	Mat MT(3, 1, CV_64F, trans);

	//�ϲ�����
	double aa[4] = { 0, 0, 0, 1 };
	Mat mat03(1, 4, CV_64F, aa);
	Mat R0T0;
	hconcat(MR, MT, R0T0);
	vconcat(R0T0, mat03, robotToolToBaseMatrix);
	cout << robotToolToBaseMatrix << endl;
}

double VisualProcessing::deg2rad(double deg)
{
	cout << "Pi = " << Pi << endl;
	double rad = (deg / 180.0)*Pi;
	return rad;
}

void VisualProcessing::readStepConfigSlot()
{
	ReadConfigParameterFile();
}









Login::Login()
{
	ui.setupUi(this);
	vp = new VisualProcessing;

}

Login::~Login()
{
	delete vp;
	vp = nullptr;
}

void Login::on_loginButton_clicked()
{
	if (ui.passwordEdit->text() == "Astropulsion_SHU"){
		this->close();
		vp->show();
	}
	else{
		QMessageBox msgBox;
		msgBox.setText("The password is wrong!");
		msgBox.exec();
	}
}

void Login::readStepConfigSlot()
{
	vp->ReadConfigParameterFile();
}
