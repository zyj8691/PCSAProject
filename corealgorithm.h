#ifndef COREALGORITHM_H
#define COREALGORITHM_H

#include <QObject>
#include<iostream>
#include <fstream>
#include"math.h"
#include <iomanip>
////PCL���ͷ�ļ�
#include <pcl/io/pcd_io.h> //PCL��PCD��ʽ�ļ����������ͷ�ļ�  
#include <pcl/point_types.h> //PCL�Ը��ָ�ʽ�ĵ��֧��ͷ�ļ�  
#include <pcl/visualization/cloud_viewer.h>  
#include <pcl/filters/voxel_grid.h>
#include <pcl/segmentation/sac_segmentation.h>  
#include <pcl/search/kdtree.h>  
#include <pcl/filters/passthrough.h>  
#include <pcl/filters/extract_indices.h>  
//
#include <pcl/features/boundary.h>
#include <pcl/features/integral_image_normal.h>  //���߹�����ͷ�ļ�
#include <pcl/features/normal_3d.h>

#include <pcl/range_image/range_image.h>
#include <pcl/io/png_io.h>
#include <pcl/visualization/common/float_image_utils.h>

//���ƿռ�����
#include <boost/thread/thread.hpp>  
#include <pcl/common/common_headers.h>  
#include <pcl/features/normal_3d.h>  

//10.18ƽ�����
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/sample_consensus/ransac.h>
//QT �����
#include<QTime>
//���OpenCV
#include "cv.h"
#include "opencv.hpp"
//���halcon
#include "HalconCpp.h"
//����
#include<QFile>
#include <QTextStream>

//�ռ�����
using  std::cout;
using  std::endl;
using namespace cv;
using namespace HalconCpp;

class CoreAlgorithm : public QObject
{
	Q_OBJECT


public:
	CoreAlgorithm(QObject *parent);
	~CoreAlgorithm();
	//����ƽ�淨ʸ���ݽṹ
	struct PlaneNormal
	{
		double A;//������x
		double B;//������y
		double C;//������z
		double D;//����
	};
	//����ռ�Բ���ݽṹ
	struct StereoCircle
	{
		PlaneNormal NormalVector;//ƽ�淨ʸ
		Point3f center;//�ռ�Բ������
		double radius;//�ռ�ԲԲ��
		bool status;//���Բ���Ƿ�ɹ�
	};
	//���ͼ�궨��ת��Ϊ��������ϵ�µ�
	Vector<Point3f> depthMapCalibration(HObject &inputImage,bool status);
	//�����ͼ�л�ȡƽ��
	HObject getPlaneImage(HObject &inputImage, bool& status);
	//�����ͼ�л�ȡ���ͻ�������Ե
	HObject getEngineSplineEdgeImage(HObject &inputImage, bool& status);
	//�����ͼ�л�ȡǰ�����仨����Ե
	HObject getfrontDriveBoxSplineEdgeImage(HObject &inputImage, bool& status);
	//�����ͼ�л�ȡֹ�ڱ�Ե
	HObject getFlangeEdgeImage(HObject &inputImage, bool& status);
	//�����ͼ�л�ȡ�����ױ�Ե
	HObject getFlangeHoleEdgeImage(HObject &inputImage, bool& status);
	//��ȡƽ�淨ʸ
	PlaneNormal getPlaneNormal(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_source);
	//�����Ե�㵽��Ͽռ�ƽ��ľ��룬�����þ�����ֵ��ȡ������ֵ�ĵ�
	void setDistanceThresholdAndGetPoints(Vector<Point3f>&inputPoints,//�����
										  PlaneNormal planeparam,//����ƽ�淨ʸ��Ϣ
										  Vector<double>distance,//�����Ե�㵽��Ͽռ�ƽ��ľ���
										  double lowThreshold,//����ֵ
										  double highThreshold,//����ֵ
										  Vector<Point3f>&outputPoints//�����
		);
	//�����ԵͶӰ��
	Vector<Point3f> ComputedEdgeProjectionPoints(Vector<Point3f> &points, PlaneNormal&planeparam);
	//���ڰ뾶Լ���Ŀռ�Բ���
	StereoCircle fitSteroCircle(Vector<Point3f> &subpoints, PlaneNormal&planeparam, double R);
	//Ransac�㷨
	vector<Point2f> RansacAlgorithm(vector<Point2f> &SourcePoint, double Radius);
	//Լ���뾶��ƽ��Բ���
	Point2f fitPlaneCircleBaseOnRadius(vector<Point2f>points, double R);
	//����ά���긳ֵ��PCL��������
	void threeDimensionToPCLPoints(Vector<Point3f>&points, pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPoints);
	//��ʾ����
	void displayCloude(pcl::PointCloud<pcl::PointXYZ>::Ptr Cloud_ptr);
	//����ʹ�� ���ɵ���
	Vector<Point3f> createCloud(HObject &inputImage);
	//�Ҳ�ͷ����ת�������ͷ����ϵ��
	Vector<Point3f> RightToLeft(Vector<Point3f> points);
	//�����Ե�㵽Բ�ĵľ��룬��������Ϣ,����ƽ������
	double CalculateBorderPointsToCenterAverageDistance(Vector<Point3f> points, StereoCircle centerPoint);
	//�����Ե�㵽Բ�ĵľ���
	void CalculateBorderPointsToCenterDistance(Vector<Point3f> points, StereoCircle centerPoint, Vector<double> &distances);
	//���ñ�Ե�㵽Բ�ĵľ�����ֵ��ɾ����Բ�Ĺ�Զ�͹�����
	Vector<Point3f> filterBorderPointsOnDistanceThrehold(Vector<Point3f> borderPoints, Vector<double> distances, double R,double threholdRange);

private:
	//halcon�ڲ��Զ��庯��
	void ReadC5CSCalibrationFile(HTuple hv_CalibFileName, HTuple *hv_Homography, HTuple *hv_gFactor,
		HTuple *hv_NumCoefficients, HTuple *hv_Tinv1, HTuple *hv_Tinv2);
	void PolynomialCorrection(HTuple hv_X_, HTuple hv_Z_, HTuple hv_NumCoefficients,
		HTuple hv_Tinv1, HTuple hv_Tinv2, HTuple *hv_X_NEW, HTuple *hv_Z_NEW);
};

#endif // COREALGORITHM_H
