#include "corealgorithm.h"

CoreAlgorithm::CoreAlgorithm(QObject *parent)
	: QObject(parent)
{

}

CoreAlgorithm::~CoreAlgorithm()
{

}

HObject CoreAlgorithm::getPlaneImage(HObject &inputImage, bool& status)
{
	// Local iconic variables
	HObject  ho_Regions, ho_ImageReduced;
	HObject  ho_ImageScale, ho_Region, ho_ImageX, ho_ImageY;
	HObject  ho_ImageZ_, ho_ImageZ;
	HObject  ho_ROI_0, ho_ROI_1;
	HObject  ho_RegionUnion, ho_ROI_3, ho_ROI_4, ho_ROI_5, ho_ROI_6;
	HObject  ho_ROI_7, ho_RegionIntersection;
	// Local control variables
	HTuple  hv_WindowHandle, hv_xResolution, hv_yResolution;
	HTuple  hv_ZoomScale, hv_CalibFile, hv_Homography, hv_gFactor;
	HTuple  hv_NumCoefficients, hv_Tinv1, hv_Tinv2, hv_savefile;
	HTuple  hv_Width, hv_Height, hv_Rows, hv_Columns, hv_Grayval;
	HTuple  hv_X, hv_Y_, hv_Z, hv_Qx, hv_Qy, hv_Qw, hv_X_, hv_Z_;
	HTuple  hv_HomographyInvert, hv_Ox, hv_Oz, hv_Ow, hv_X_NEW;
	HTuple  hv_Z_NEW, hv_ObjectModel3D, hv_ObjectModel3D_NEW;
	HTuple  hv_PoseVis, hv_i, hv_xcd, hv_ycd, hv_zcd, hv_num;
	HTuple  hv_Seconds1, hv_FileHandle, hv_Seconds2, hv_j, hv_Time;
	HTuple  hv_Area, hv_Row, hv_Area2, hv_Row2, hv_Column2;
	HTuple  hv_Column, hv_status, hv_Exception;
	//***********************************************************************************
	try
	{
		//ͼ����
		GetImageSize(inputImage, &hv_Width, &hv_Height);
		Threshold(inputImage, &ho_Regions, 1, 65535);
		AreaCenter(ho_Regions, &hv_Area, &hv_Row, &hv_Column);

		GenCircle(&ho_ROI_0, hv_Row, hv_Column, 200);
		GenCircle(&ho_ROI_1, hv_Row / 2, hv_Column, 200);
		Union2(ho_ROI_0, ho_ROI_1, &ho_RegionUnion);
		GenCircle(&ho_ROI_3, hv_Row + 400, hv_Column, 200);
		Union2(ho_RegionUnion, ho_ROI_3, &ho_RegionUnion);
		GenCircle(&ho_ROI_4, hv_Row, hv_Column / 2, 200);
		Union2(ho_RegionUnion, ho_ROI_4, &ho_RegionUnion);
		GenCircle(&ho_ROI_5, hv_Row, hv_Column + ((hv_Width - hv_Column) / 2), 200);
		Union2(ho_RegionUnion, ho_ROI_5, &ho_RegionUnion);
		GenCircle(&ho_ROI_6, hv_Row + ((hv_Height - hv_Row) / 2), hv_Column + ((hv_Width - hv_Column) / 4),
			200);
		Union2(ho_RegionUnion, ho_ROI_6, &ho_RegionUnion);
		GenCircle(&ho_ROI_7, hv_Row + ((hv_Height - hv_Row) / 2), hv_Column - ((hv_Width - hv_Column) / 4),
			200);
		Union2(ho_RegionUnion, ho_ROI_7, &ho_RegionUnion);
		Intersection(ho_Regions, ho_RegionUnion, &ho_RegionIntersection);

		AreaCenter(ho_RegionIntersection, &hv_Area2, &hv_Row2, &hv_Column2);
		if (0 != (hv_Area2 == 0))
		{
			status = false;
		}
		else{
			status = true;
		}



		ReduceDomain(inputImage, ho_RegionIntersection, &ho_ImageReduced);
	}
	// catch (Exception) 
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		status = false;
	}
	
	return ho_ImageReduced;	
}

void CoreAlgorithm::ReadC5CSCalibrationFile(HTuple hv_CalibFileName, HTuple *hv_Homography, HTuple *hv_gFactor, HTuple *hv_NumCoefficients, HTuple *hv_Tinv1, HTuple *hv_Tinv2)
{
	// Local iconic variables

	// Local control variables
	HTuple  hv_FileHandle, hv_OutLine, hv_IsEOF, hv_Substrings;
	HTuple  hv_HigherOrderCoefficients, hv_CoefficientsTemp;
	HTuple  hv_MatrixID, hv_MatrixTransposedID, hv_Tinv;

	//TODO!: Read additonal polynomial correction coefficients from tuple file
	//Tinv = n-by-2 coefficient matrix (n=10 for 3rd order | n=15 for 4th order)
	//Tinv =  Coefficients[11:end] -> split into n-by2 matrix
	//
	OpenFile(hv_CalibFileName, "input", &hv_FileHandle);
	FreadLine(hv_FileHandle, &hv_OutLine, &hv_IsEOF);
	CloseFile(hv_FileHandle);
	TupleRegexpReplace(hv_OutLine, "\\n", "", &hv_OutLine);
	TupleSplit(hv_OutLine, " ", &hv_Substrings);
	TupleNumber(hv_Substrings, &hv_HigherOrderCoefficients);
	//
	//�궨�ļ���0-8λΪ��Ӧ�Ծ���*
	hv_CoefficientsTemp = hv_HigherOrderCoefficients.TupleSelectRange(0, 8);
	CreateMatrix(3, 3, hv_CoefficientsTemp, &hv_MatrixID);
	TransposeMatrix(hv_MatrixID, &hv_MatrixTransposedID);
	GetFullMatrix(hv_MatrixTransposedID, &(*hv_Homography));
	//
	//�궨�ļ���9λΪ  1/������λ��*
	(*hv_gFactor) = ((const HTuple&)hv_HigherOrderCoefficients)[9];
	//
	//�궨�ļ���10λΪ����ʽϵ������*
	(*hv_NumCoefficients) = ((const HTuple&)hv_HigherOrderCoefficients)[10];
	//
	//�궨�ļ���11-40λΪ����ʽϵ����15��ΪX�ģ�15��ΪY��*
	hv_Tinv = hv_HigherOrderCoefficients.TupleSelectRange(11, (hv_HigherOrderCoefficients.TupleLength()) - 1);
	CreateMatrix(15, 1, 0, &(*hv_Tinv1));
	CreateMatrix(15, 1, 0, &(*hv_Tinv2));
	SetFullMatrix((*hv_Tinv1), hv_HigherOrderCoefficients.TupleSelectRange(11, (11 + ((*hv_NumCoefficients) / 2)) - 1));
	SetFullMatrix((*hv_Tinv2), hv_HigherOrderCoefficients.TupleSelectRange(11 + ((*hv_NumCoefficients) / 2), (hv_HigherOrderCoefficients.TupleLength()) - 1));
	return;
}

void CoreAlgorithm::PolynomialCorrection(HTuple hv_X_, HTuple hv_Z_, HTuple hv_NumCoefficients, HTuple hv_Tinv1, HTuple hv_Tinv2, HTuple *hv_X_NEW, HTuple *hv_Z_NEW)
{
	// Local iconic variables

	// Local control variables
	HTuple  hv_X_Z_, hv_X_X_, hv_Z_Z_, hv_X_X_Z_;
	HTuple  hv_X_Z_Z_, hv_X_X_X_, hv_Z_Z_Z_, hv_AllData, hv_X_X_X_Z_;
	HTuple  hv_X_X_Z_Z_, hv_X_Z_Z_Z_, hv_X_X_X_X_, hv_Z_Z_Z_Z_;
	HTuple  hv_MatrixID, hv_MatrixTransposedID, hv_MatrixProd1;
	HTuple  hv_MatrixProd2;

	//TODO!: Add Polynomial correction -> 3rd/4th order polynomial transformation [u v] = [1 x y ...] * Tinv
	//with Tinv = n-by-2 coefficient matrix (n=10 for 3rd order | n=15 for 4th order)
	//
	//Example for 4th order correction
	//X_NEW = [1 x y x*y x^2 y^2 y*x^2 x*y^2 x^3 y^3 x^3*y x^2*y^2 x*y^3 x^4 y^4] * Tinv(:,1)
	//Z_NEW = [1 x y x*y x^2 y^2 y*x^2 x*y^2 x^3 y^3 x^3*y x^2*y^2 x*y^3 x^4 y^4] * Tinv(:,2)
	//
	//
	hv_X_ = hv_X_;
	hv_Z_ = hv_Z_;
	hv_X_Z_ = hv_X_*hv_Z_;
	hv_X_X_ = hv_X_*hv_X_;
	hv_Z_Z_ = hv_Z_*hv_Z_;
	hv_X_X_Z_ = (hv_X_*hv_X_)*hv_Z_;
	hv_X_Z_Z_ = (hv_X_*hv_Z_)*hv_Z_;
	hv_X_X_X_ = (hv_X_*hv_X_)*hv_X_;
	hv_Z_Z_Z_ = (hv_Z_*hv_Z_)*hv_Z_;
	hv_AllData.Clear();
	hv_AllData.Append(HTuple(hv_X_.TupleLength(), 1));
	hv_AllData.Append(hv_X_);
	hv_AllData.Append(hv_Z_);
	hv_AllData.Append(hv_X_Z_);
	hv_AllData.Append(hv_X_X_);
	hv_AllData.Append(hv_Z_Z_);
	hv_AllData.Append(hv_X_X_Z_);
	hv_AllData.Append(hv_X_Z_Z_);
	hv_AllData.Append(hv_X_X_X_);
	hv_AllData.Append(hv_Z_Z_Z_);
	if (0 != (hv_NumCoefficients == 30))
	{
		hv_X_X_X_Z_ = ((hv_X_*hv_X_)*hv_X_)*hv_Z_;
		hv_X_X_Z_Z_ = ((hv_X_*hv_X_)*hv_Z_)*hv_Z_;
		hv_X_Z_Z_Z_ = ((hv_X_*hv_Z_)*hv_Z_)*hv_Z_;
		hv_X_X_X_X_ = ((hv_X_*hv_X_)*hv_X_)*hv_X_;
		hv_Z_Z_Z_Z_ = ((hv_Z_*hv_Z_)*hv_Z_)*hv_Z_;
		hv_AllData = ((((hv_AllData.TupleConcat(hv_X_X_X_Z_)).TupleConcat(hv_X_X_Z_Z_)).TupleConcat(hv_X_Z_Z_Z_)).TupleConcat(hv_X_X_X_X_)).TupleConcat(hv_Z_Z_Z_Z_);
	}
	//
	CreateMatrix((hv_NumCoefficients / 2).TupleInt(), hv_X_.TupleLength(), 0, &hv_MatrixID);
	SetFullMatrix(hv_MatrixID, hv_AllData);
	TransposeMatrix(hv_MatrixID, &hv_MatrixTransposedID);
	//
	MultMatrix(hv_MatrixTransposedID, hv_Tinv1, "AB", &hv_MatrixProd1);
	MultMatrix(hv_MatrixTransposedID, hv_Tinv2, "AB", &hv_MatrixProd2);
	//
	GetFullMatrix(hv_MatrixProd1, &(*hv_X_NEW));
	GetFullMatrix(hv_MatrixProd2, &(*hv_Z_NEW));
	//
	ClearMatrix(((hv_MatrixID.TupleConcat(hv_MatrixProd1)).TupleConcat(hv_MatrixProd2)).TupleConcat(hv_MatrixTransposedID));
	return;
}

void CoreAlgorithm::threeDimensionToPCLPoints(Vector<Point3f>&points, pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPoints)
{
	//����cloudPoints��С,�������
	cloudPoints->resize(points.size());
	/*cloudPoints->width = points.size();
	cloudPoints->height = 1;*/
	//����ά���긳ֵ��PCL��������
	for (int i = 0; i < points.size(); i++){
		(*cloudPoints)[i].x = points[i].x;
		(*cloudPoints)[i].y = points[i].y;
		(*cloudPoints)[i].z = points[i].z;
	}

}

void CoreAlgorithm::displayCloude(pcl::PointCloud<pcl::PointXYZ>::Ptr Cloud_ptr)
{
	pcl::visualization::PCLVisualizer p;
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_h(Cloud_ptr, 255, 255, 255);
	p.setWindowName("��ʾ����");
	//p.addCoordinateSystem(2.0);
	p.addPointCloud(Cloud_ptr, tgt_h, "target");//��ɶ���ɫ����������Ĵ���
	p.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "target");
	p.spin();
}

HObject CoreAlgorithm::getEngineSplineEdgeImage(HObject &inputImage, bool& status)
{
	// Local iconic variables
	HObject  ho_Image, ho_Regions;
	HObject  ho_RegionErosion, ho_RegionDilation, ho_RegionTrans;
	HObject  ho_RegionErosion1, ho_RegionDifference, ho_ImageReduced;
	HObject  ho_Regions1, ho_intersectio, ho_ImageScale, ho_Rintersectioegion;
	HObject  ho_ImageX, ho_ImageY, ho_ImageZ_, ho_ImageZ, ho_ConnectedRegions, ho_SelectedRegions;
	HObject	ho_ConnectedRegions1, ho_SelectedRegions1, ho_RegionUnion, ho_Skeleton, ho_SelectedRegions2, ho_ConnectedRegions2;
	HObject ho_SelectedRegions3, ho_RegionIntersection, ho_RegionIntersection1, ho_RegionIntersection2, ho_intersect;
	// Local control variables
	HTuple  hv_WindowHandle, hv_xResolution, hv_yResolution;
	HTuple  hv_ZoomScale, hv_CalibFile, hv_Homography, hv_gFactor;
	HTuple  hv_NumCoefficients, hv_Tinv1, hv_Tinv2, hv_Width;
	HTuple  hv_Height, hv_Rows, hv_Columns, hv_Grayval, hv_X;
	HTuple  hv_Y_, hv_Z, hv_Qx, hv_Qy, hv_Qw, hv_X_, hv_Z_;
	HTuple  hv_HomographyInvert, hv_Ox, hv_Oz, hv_Ow, hv_X_NEW;
	HTuple  hv_Z_NEW, hv_ObjectModel3D, hv_ObjectModel3D_NEW;
	HTuple  hv_PoseVis, hv_sampleRate, hv_GenParamValue, hv_SampledObjectModel3D;
	HTuple  hv_PoseOut, hv_i, hv_xcd, hv_ycd, hv_zcd, hv_num;
	HTuple  hv_Seconds1, hv_FileHandle, hv_Seconds2, hv_Time;
	HTuple	hv_Area, hv_Row, hv_Column, hv_Area1, hv_Row1, hv_Column1, hv_Max, hv_Exception, hv_status;
	//Initialize HDevelop
	// Local iconic variables
	HObject  ho_ModelContours;
	HObject  ho_TransContours, ho_EmptyRegion, ho_ModelRegion;
	HObject  ho_RegionsReduced;
	HObject  ho_ImageReduced1, ho_ImageMean;
	HObject  ho_Edges, ho_ContoursSplit, ho_Lines, ho_Contour;
	HObject  ho_RegionLines;
	HObject    ho_Rectangle4;
	HObject  ho_Cross, ho_ROI_0, ho_EmptyRegion1, ho_ObjectSelected;

	// Local control variables
	HTuple   hv_ModelID, hv_ModelRow;
	HTuple  hv_ModelColumn, hv_ModelAngle, hv_ModelScale, hv_ModelScore;
	HTuple  hv_MatchingObjIdx, hv_HomMat,  hv_Phi, hv_Number;
	HTuple  hv_I, hv_Type, hv_RowBegin, hv_ColBegin, hv_RowEnd;
	HTuple  hv_ColEnd, hv_Nr, hv_Nc, hv_Dist;
	HTuple  hv_Length1, hv_Length2,  hv_Col1;
	HTuple  hv_Distance, hv_Sorted, hv_b, hv_a, hv_dis;

	
	//***********************************************************************************
	//ͼ����

	ho_Image = inputImage;

	GetImageSize(ho_Image, &hv_Width, &hv_Height);

	try
	{
		GetImageSize(ho_Image, &hv_Width, &hv_Height);
		Threshold(ho_Image, &ho_Regions, 1, 65535);
		ReadShapeModel("leftgear1.0.shm", &hv_ModelID);
		GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);
		//ģ��ƥ��
		FindScaledShapeModel(ho_Image, hv_ModelID, HTuple(0).TupleRad(), HTuple(360).TupleRad(),
			0.98, 1.02, 0.4, 5, 0.5, "least_squares", (HTuple(7).Append(1)), 0.75, &hv_ModelRow,
			&hv_ModelColumn, &hv_ModelAngle, &hv_ModelScale, &hv_ModelScore);

		{
			HTuple end_val8 = (hv_ModelScore.TupleLength()) - 1;
			HTuple step_val8 = 1;
			for (hv_MatchingObjIdx = 0; hv_MatchingObjIdx.Continue(end_val8, step_val8); hv_MatchingObjIdx += step_val8)
			{
				HomMat2dIdentity(&hv_HomMat);
				HomMat2dScale(hv_HomMat, HTuple(hv_ModelScale[hv_MatchingObjIdx]), HTuple(hv_ModelScale[hv_MatchingObjIdx]),
					0, 0, &hv_HomMat);
				HomMat2dRotate(hv_HomMat, HTuple(hv_ModelAngle[hv_MatchingObjIdx]), 0, 0, &hv_HomMat);
				HomMat2dTranslate(hv_HomMat, HTuple(hv_ModelRow[hv_MatchingObjIdx]), HTuple(hv_ModelColumn[hv_MatchingObjIdx]),
					&hv_HomMat);
				AffineTransContourXld(ho_ModelContours, &ho_TransContours, hv_HomMat);
				if (HDevWindowStack::IsOpen())
					DispObj(ho_TransContours, HDevWindowStack::GetActive());
				//stop ()
			}
		}
		// stop(); only in hdevelop
		GenEmptyRegion(&ho_EmptyRegion);

		{
			HTuple end_val20 = (hv_ModelScale.TupleLength()) - 1;
			HTuple step_val20 = 1;
			for (hv_i = 0; hv_i.Continue(end_val20, step_val20); hv_i += step_val20)
			{

				GenRectangle2(&ho_ModelRegion, HTuple(hv_ModelRow[hv_i]), HTuple(hv_ModelColumn[hv_i]),
					(HTuple(-87.8317).TupleRad()) + HTuple(hv_ModelAngle[hv_i]), 259, 280);

				OrientationRegion(ho_ModelRegion, &hv_Phi);
				ReduceDomain(ho_Image, ho_ModelRegion, &ho_ImageReduced);
				Threshold(ho_ImageReduced, &ho_RegionsReduced, 1, 65535);
				ErosionRectangle1(ho_RegionsReduced, &ho_RegionErosion, 10, 10);
				DilationRectangle1(ho_RegionErosion, &ho_RegionDilation, 10, 10);
				ReduceDomain(ho_Image, ho_RegionDilation, &ho_ImageReduced1);
				MeanImage(ho_ImageReduced1, &ho_ImageMean, 3, 3);
				EdgesSubPix(ho_ImageMean, &ho_Edges, "canny", 1, 40, 80);
				SegmentContoursXld(ho_Edges, &ho_ContoursSplit, "lines", 5, 4, 2);
				CountObj(ho_ContoursSplit, &hv_Number);
				GenEmptyObj(&ho_Lines);
				{
					HTuple end_val35 = hv_Number;
					HTuple step_val35 = 1;
					for (hv_I = 1; hv_I.Continue(end_val35, step_val35); hv_I += step_val35)
					{
						SelectObj(ho_ContoursSplit, &ho_Contour, hv_I);
						GetContourGlobalAttribXld(ho_Contour, "cont_approx", &hv_Type);
						if (0 != (hv_Type == -1))
						{
							ConcatObj(ho_Lines, ho_Contour, &ho_Lines);
						}
					}
				}
				// stop(); only in hdevelop
				FitLineContourXld(ho_Lines, "drop", -1, 0, 5, 2, &hv_RowBegin, &hv_ColBegin,
					&hv_RowEnd, &hv_ColEnd, &hv_Nr, &hv_Nc, &hv_Dist);
				GenRegionLine(&ho_RegionLines, hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd);
				Union1(ho_RegionLines, &ho_RegionUnion);
				Connection(ho_RegionUnion, &ho_ConnectedRegions);
				ErosionRectangle1(ho_ModelRegion, &ho_RegionErosion1, 7, 7);
				Intersection(ho_ConnectedRegions, ho_RegionErosion1, &ho_RegionIntersection);
				AreaCenter(ho_RegionIntersection, &hv_Area, &hv_Row, &hv_Column);
				//**********************************
				hv_Length1 = 259;
				hv_Length2 = 280;
				hv_Phi = (HTuple(-87.8317).TupleRad()) + HTuple(hv_ModelAngle[hv_i]);
				if (HDevWindowStack::IsOpen())
					SetColor(HDevWindowStack::GetActive(), "red");
				GenRectangle2ContourXld(&ho_Rectangle4, HTuple(hv_ModelRow[hv_i]), HTuple(hv_ModelColumn[hv_i]),
					(HTuple(-87.8317).TupleRad()) + HTuple(hv_ModelAngle[hv_i]), 259, 280);
				GetContourXld(ho_Rectangle4, &hv_Row1, &hv_Col1);
				if (HDevWindowStack::IsOpen())
					SetColor(HDevWindowStack::GetActive(), "green");
				GenCrossContourXld(&ho_Cross, hv_Row1, hv_Col1, 6, hv_Phi);
				GenRegionLine(&ho_ROI_0, HTuple(hv_Row1[0]), HTuple(hv_Col1[0]), HTuple(hv_Row1[3]),
					HTuple(hv_Col1[3]));

				//*************************************************
				DistancePl(hv_Row, hv_Column, HTuple(hv_Row1[0]), HTuple(hv_Col1[0]), HTuple(hv_Row1[3]),
					HTuple(hv_Col1[3]), &hv_Distance);

				TupleSort(hv_Distance, &hv_Sorted);
				GenEmptyRegion(&ho_EmptyRegion1);
				hv_b = 0;
				{
					HTuple end_val67 = (hv_Distance.TupleLength()) - 1;
					HTuple step_val67 = 1;
					for (hv_a = 0; hv_a.Continue(end_val67, step_val67); hv_a += step_val67)
					{
						if (0 != (HTuple(hv_Distance[hv_a])<19))
						{
							SelectObj(ho_RegionIntersection, &ho_ObjectSelected, hv_a + 1);
							hv_dis[hv_b] = HTuple(hv_Distance[hv_a]);
							hv_b += 1;
							Union2(ho_ObjectSelected, ho_EmptyRegion1, &ho_EmptyRegion1);
						}

					}
				}
				Union2(ho_EmptyRegion1, ho_EmptyRegion, &ho_EmptyRegion);
				// stop(); only in hdevelop

			}
		}
		Intersection(ho_Regions, ho_EmptyRegion, &ho_EmptyRegion);
		ReduceDomain(ho_Image, ho_EmptyRegion, &ho_ImageReduced);
		status = true;
	}
	// catch (Exception) 
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		status = false;
	}

	return ho_ImageReduced;
}

CoreAlgorithm::PlaneNormal CoreAlgorithm::getPlaneNormal(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_source)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_target(new pcl::PointCloud<pcl::PointXYZ>);
	// ������ڵ�����
	std::vector<int> inliers;
	// ����һ����ģ�Ͷ���
	pcl::SampleConsensusModelPlane<pcl::PointXYZ>::Ptr model_p(new pcl::SampleConsensusModelPlane<pcl::PointXYZ>(cloud_source));
	pcl::RandomSampleConsensus<pcl::PointXYZ> ransac(model_p);
	ransac.setDistanceThreshold(0.01);
	ransac.computeModel();
	ransac.getInliers(inliers);
	cloud_target->resize(inliers.size());
	pcl::copyPointCloud(*cloud_source, inliers, *cloud_target);
	Eigen::VectorXf coef = Eigen::VectorXf::Zero(4, 1);
	ransac.getModelCoefficients(coef);
	PlaneNormal plane;
	plane.A = coef[0];
	plane.B = coef[1];
	plane.C = coef[2];
	plane.D = coef[3];
	if (plane.C < 0){
		plane.A = -coef[0];
		plane.B = -coef[1];
		plane.C = -coef[2];
		plane.D = -coef[3];
	}
	QString fileName = "OutputFile/PlaneNormal.csv";//д���ļ���Ŀ¼
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);
	out << plane.A << "," << plane.B << "," << plane.C << "," << plane.D << "\n";
	file.close();
	return plane;
}

void CoreAlgorithm::setDistanceThresholdAndGetPoints(Vector<Point3f>&inputPoints,/*����� */ PlaneNormal planeparam,/*����ƽ�淨ʸ��Ϣ */ 
	Vector<double>distance,/*�����Ե�㵽��Ͽռ�ƽ��ľ��� */ double lowThreshold,/*����ֵ */ 
	double highThreshold,/*����ֵ */ Vector<Point3f>&outputPoints/*����� */)
{
	double dis;
	for (int i = 0; i < inputPoints.size(); i++)
	{
		dis = fabs(planeparam.A*inputPoints[i].x + planeparam.B*inputPoints[i].y + planeparam.C*inputPoints[i].z + 
			planeparam.D) / sqrt(planeparam.A*planeparam.A + planeparam.B*planeparam.B + planeparam.C*planeparam.C);
		
		distance.push_back(float(dis));

		if (dis<highThreshold&&dis>lowThreshold){
			outputPoints.push_back(inputPoints[i]);
		}
	}
	//���������Ϣ
	ofstream outf;
	outf.open("OutputFile/pointsToPlaneDistance.txt");
	for (int i = 0; i < distance.size(); i++){
		outf << distance[i] << endl;
	}
	outf.close();
}

CoreAlgorithm::StereoCircle CoreAlgorithm::fitSteroCircle(Vector<Point3f> &subpoints, PlaneNormal&planeparam, double R)
{
	
	//ƽ�淨������������ֵ
	double A = planeparam.A;
	double B = planeparam.B;
	double C = planeparam.C;
	double D = planeparam.D;

	//������Ƶ�һ��Ϊ�ռ�ƽ������ϵԭ��
	double X0 = subpoints[0].x;
	double Y0 = subpoints[0].y;
	double Z0 = subpoints[0].z;

	//����ƽ������ϵ��������
	//X�᷽������
	double x1 = subpoints[1].x - subpoints[0].x;
	double y1 = subpoints[1].y - subpoints[0].y;
	double z1 = subpoints[1].z - subpoints[0].z;

	//Z�᷽������
	double x3 = A;
	double y3 = B;
	double z3 = C;

	//Y�᷽������
	//ʹ�ò�˼���Y�᷽������
	double x2 = y1*z3 - z1*y3;
	double y2 = z1*x3 - x1*z3;
	double z2 = x1*y3 - y1*x3;


	//���㵥λ��������
	//X��
	double sqrt1 = sqrt(x1*x1 + y1*y1 + z1*z1);
	double n1_x = x1 / sqrt1;
	double n1_y = y1 / sqrt1;
	double n1_z = z1 / sqrt1;
	
	//ʹ�ò�˼���Y�ᵥλ��������
	double sqrt2 = sqrt(x2*x2 + y2*y2 + z2*z2);
	double n2_x = x2 / sqrt2;
	double n2_y = y2 / sqrt2;
	double n2_z = z2 / sqrt2;
	
	//Z��
	double sqrt3 = sqrt(x3*x3 + y3*y3 + z3*z3);
	double n3_x = x3 / sqrt3;
	double n3_y = y3 / sqrt3;
	double n3_z = z3 / sqrt3;

	Mat H(4, 4, CV_32FC1, Scalar(0));
	Mat H_inv(4, 4, CV_32FC1, Scalar(0));
	Mat D3(4, 1, CV_32FC1, Scalar(0));
	Mat D2(4, 1, CV_32FC1, Scalar(0));

	H.at<float>(0, 0) = n1_x;
	H.at<float>(0, 1) = n2_x;
	H.at<float>(0, 2) = n3_x;
	H.at<float>(0, 3) = X0;

	H.at<float>(1, 0) = n1_y;
	H.at<float>(1, 1) = n2_y;
	H.at<float>(1, 2) = n3_y;
	H.at<float>(1, 3) = Y0;

	H.at<float>(2, 0) = n1_z;
	H.at<float>(2, 1) = n2_z;
	H.at<float>(2, 2) = n3_z;
	H.at<float>(2, 3) = Z0;

	H.at<float>(3, 0) = 0;
	H.at<float>(3, 1) = 0;
	H.at<float>(3, 2) = 0;
	H.at<float>(3, 3) = 1;

	H_inv = H.inv();

	//�ռ���ά��ת��Ϊ��ά��
	vector<Point2f>planePoints;
	double x;
	double y;
	//����
	for (int i = 0; i < subpoints.size(); i++){

		D3.at<float>(0, 0) = subpoints[i].x;
		D3.at<float>(1, 0) = subpoints[i].y;
		D3.at<float>(2, 0) = subpoints[i].z;
		D3.at<float>(3, 0) = 1;
		D2 = H_inv*D3;
		x = D2.at<float>(0, 0);
		y = D2.at<float>(1, 0);
		planePoints.push_back(Point2f(x, y));
	}
		

	//ʹ��Ransac�㷨
	vector<Point2f>planePoints_Ransac;
	Point2f planeCircleCenter;
	planePoints_Ransac = RansacAlgorithm(planePoints, R);
	cout << "���ս�������ģ�͵����ݸ���Ϊ��" << planePoints_Ransac.size() << endl;

	planeCircleCenter = fitPlaneCircleBaseOnRadius(planePoints_Ransac, R);
	ofstream outf;
	outf.open("OutputFile/planeCircle.txt");
	for (int i = 0; i < planePoints_Ransac.size(); i++){
		outf << planePoints_Ransac[i].x << " " << planePoints_Ransac[i].y << endl;
	}
	outf.close();
	cout << "planeCircleCenter = " << planeCircleCenter.x << " " << planeCircleCenter.y << endl;
	double xc = planeCircleCenter.x;
	double yc = planeCircleCenter.y;

	//ƽ���άת��Ϊ��ά����

	D2.at<float>(0, 0) = xc;
	D2.at<float>(1, 0) = yc;
	D2.at<float>(2, 0) = 0;
	D2.at<float>(3, 0) = 1;

	D3 = H*D2;
	double XC = D3.at<float>(0, 0);
	double YC = D3.at<float>(1, 0);
	double ZC = D3.at<float>(2, 0);


	StereoCircle circleCenter;
	circleCenter.NormalVector.A = A;
	circleCenter.NormalVector.B = B;
	circleCenter.NormalVector.C = C;

	circleCenter.center.x = XC;
	circleCenter.center.y = YC;
	circleCenter.center.z = ZC;
	
	return circleCenter;
}

vector<Point2f> CoreAlgorithm::RansacAlgorithm(vector<Point2f> &SourcePoint, double Radius)
{
	//
	int index1, index2, index3;
	int iterations = 0;//�������
	int flag = 0;//�����жϷ���ģ�͵�ĸ���
	double Len = 0;//�㵽Բ�ĵľ�����뾶�Ĳ�ֵ
	double thre_gap = 0;//������ļ��
	double thre_dif = 0;//��뾶���ֵ���ж�
	double thre_no = 0;//ͨ����ĸ���
	double thre_flat = 0;//�㵽ƽ��ľ�����ֵ
	float a = 0, b = 0, c = 0;//��ϳ���ƽ�淨����
	double Dsum = 0;
	Vector<double> Dist;//�����洢�����㵽Բ�ĵľ���
	Point2f result1;	//���������Բ��ϵĽ��
	vector<Point2f> TestData;//�������ҳ�����������
	vector<Point2f> ReturnPoint;//��ŷ���ģ�͵���ѵ㼯��

	int countnum = SourcePoint.size() - 1;//�㼯�ĳߴ�
	int trytime = 0;//�ۻ������Ĵ���

	//StereoEllipse result11;	
	//result11=fitStereoEllipse(SourcePoint);
	while (!iterations)
	{

		trytime++;
		TestData.clear();

		Dist.clear();

		ReturnPoint.clear();
		//�ҳ���һ������������
		srand(QTime::currentTime().msec());//srand������������������ĳ�ʼ��������
		index1 = rand() % countnum;
		srand(QTime::currentTime().msec());
		index2 = rand() % countnum;
		srand(QTime::currentTime().msec());
		index3 = rand() % countnum;
		thre_gap = 0.1*countnum;//������ļ��
		thre_dif = 0;//��뾶���ֵ���ж�

		thre_no = 0.8*(countnum + 1);//ͨ����ĸ���
		while (abs(index1 - index2) < thre_gap || abs(index1 - index3) < thre_gap || abs(index2 - index3) < thre_gap)
		{
			srand(QTime::currentTime().msec());
			//index1 = rand() % countnum;//Դ�����
			index2 = rand() % countnum;
			index3 = rand() % countnum;

		}

		TestData.push_back(SourcePoint[index1]);
		TestData.push_back(SourcePoint[index2]);
		TestData.push_back(SourcePoint[index3]);


		result1 = fitPlaneCircleBaseOnRadius(TestData, Radius);

		//��뾶���ֵ��ֵ����
		thre_dif = 0.04*Radius;
		for (int i = 0; i <= countnum; i++)//�������е㵽Բ�ĵľ���
		{
			Dist.push_back(cv::sqrt((SourcePoint[i].x - result1.x)*(SourcePoint[i].x - result1.x) +
				(SourcePoint[i].y - result1.y)*(SourcePoint[i].y - result1.y)));
		}

		for (int i = 0; i <= countnum; i++)
		{
			Len = abs(Dist[i] - Radius);
			if (Len < thre_dif)//thre_flat
			{
				ReturnPoint.push_back(SourcePoint[i]);
				flag++;
			}
			else
			{
				int jj = 0;//just fit
			}
		}

		if (flag > thre_no)
		{
			iterations = 1;
		}
		else
		{
			iterations = 0;
			flag = 0;
		}
		if (trytime > 50){
			break;
		}
	}

	return ReturnPoint;
}

cv::Point2f CoreAlgorithm::fitPlaneCircleBaseOnRadius(vector<Point2f>points, double R)
{
	//����Բ������
	double a[2], b[2];
	//Բ�����귵��ֵ
	Point2f center;
	//�����м����
	double A = 0, B = 0, C = 0, D = 0, E = 0, F = 0, G = 0, H = 0, I = 0;
	//����A
	for (int i = 0; i < points.size(); i++){
		A = A + points[i].x*points[i].x;
	}
	//cout << "A = " << A << endl;
	//����B
	for (int i = 0; i < points.size(); i++){
		B = B + points[i].x*points[i].y;
	}
	//����C
	for (int i = 0; i < points.size(); i++){
		C = C + points[i].x;
	}
	//����D
	for (int i = 0; i < points.size(); i++){
		D = D + pow(points[i].x, 3);
	}
	//cout << "D = " << D << endl;
	//����E
	for (int i = 0; i < points.size(); i++){
		E = E + points[i].x*pow(points[i].y, 2);
	}
	//����F
	for (int i = 0; i < points.size(); i++){
		F = F + points[i].y*points[i].y;
	}
	//����G
	for (int i = 0; i < points.size(); i++){
		G = G + points[i].y;
	}
	//����H
	for (int i = 0; i < points.size(); i++){
		H = H + pow(points[i].x, 2)* points[i].y;
	}
	//����I
	for (int i = 0; i < points.size(); i++){
		I = I + pow(points[i].y, 3);
	}

	//����J/K/L
	//����J
	double J = (D + E)*G - (H + I)*C;
	//����K
	double K = -2 * (B*G - F*C);
	//����L
	double L = 2 * (A*G - B*C);

	//����M/N/P Mb2+Nb+P=0 ���b
	//����M
	double M = -C*((K*K) / (L*L) + 1);
	//����N
	double N = 2 * (A*K + B*L) / L - (2 * C*J*K) / (L*L);
	//����P
	double P = (2 * A*J) / L + C*R*R - (C*J*J) / (L*L) - D - E;

	//���Բ������
	if (N*N - 4 * M*P < 0)
	{
		cout << "�޽�" << endl;
	}
	if (N*N - 4 * M*P == 0)
	{
		cout << "һ����" << endl;
	}
	if (N*N - 4 * M*P > 0)
	{
		cout << "������" << endl;
		b[0] = (-N + sqrt(N*N - 4 * M*P)) / (2 * M);
		b[1] = (-N - sqrt(N*N - 4 * M*P)) / (2 * M);

		a[0] = (J + K*b[0]) / L;
		a[1] = (J + K*b[1]) / L;


	}

	cout << a[0] << " " << b[0] << endl;
	cout << a[1] << " " << b[1] << endl;

	double sum1 = 0, sum2 = 0;
	double average1, average2;
	double variance1, variance2;
	double va1 = 0, va2 = 0;
	for (int i = 0; i < points.size(); i++){
		sum1 = sum1 + sqrt((points[i].x - a[0])*(points[i].x - a[0]) + (points[i].y - b[0])*(points[i].y - b[0]));
	}
	for (int i = 0; i < points.size(); i++){
		sum2 = sum2 + sqrt((points[i].x - a[1])*(points[i].x - a[1]) + (points[i].y - b[1])*(points[i].y - b[1]));
	}
	average1 = sum1 / points.size();
	average2 = sum2 / points.size();
	for (int i = 0; i < points.size(); i++){
		va1 = va1 + (sqrt((points[i].x - a[0])*(points[i].x - a[0]) + (points[i].y - b[0])*(points[i].y - b[0])) - average1)*(sqrt((points[i].x - a[0])*(points[i].x - a[0]) + (points[i].y - b[0])*(points[i].y - b[0])) - average1);
	}
	for (int i = 0; i < points.size(); i++){
		va2 = va2 + (sqrt((points[i].x - a[1])*(points[i].x - a[1]) + (points[i].y - b[1])*(points[i].y - b[1])) - average2)*(sqrt((points[i].x - a[1])*(points[i].x - a[1]) + (points[i].y - b[1])*(points[i].y - b[1])) - average2);
	}
	variance1 = va1 / points.size();
	variance2 = va2 / points.size();
	if (variance1<variance2){
		center = (Point2f(a[0], b[0]));
		cout << "��Ե�㵽Բ�ľ��뷽��Ϊ��" << variance1 << endl;

	}
	else{
		center = (Point2f(a[1], b[1]));
		cout << "��Ե�㵽Բ�ľ��뷽��Ϊ��" << variance2 << endl;
	}

	ofstream outf;
	outf.open("center.txt");
	outf << center.x << " " << center.y << " " << R << endl;
	outf.close();

	return center;
}

Vector<Point3f> CoreAlgorithm::createCloud(HObject &inputImage)
{
	// Local iconic variables
	HObject  ho_Regions, ho_ImageReduced;
	HObject  ho_ImageScale, ho_Region, ho_ImageX, ho_ImageY;
	HObject  ho_ImageZ_, ho_ImageZ;

	// Local control variables
	HTuple  hv_WindowHandle, hv_xResolution, hv_yResolution;
	HTuple  hv_ZoomScale, hv_CalibFile, hv_Homography, hv_gFactor;
	HTuple  hv_NumCoefficients, hv_Tinv1, hv_Tinv2, hv_savefile;
	HTuple  hv_Width, hv_Height, hv_Rows, hv_Columns, hv_Grayval;
	HTuple  hv_X, hv_Y_, hv_Z, hv_Qx, hv_Qy, hv_Qw, hv_X_, hv_Z_;
	HTuple  hv_HomographyInvert, hv_Ox, hv_Oz, hv_Ow, hv_X_NEW;
	HTuple  hv_Z_NEW, hv_ObjectModel3D, hv_ObjectModel3D_NEW;
	HTuple  hv_PoseVis, hv_i, hv_xcd, hv_ycd, hv_zcd, hv_num;
	HTuple  hv_Seconds1, hv_FileHandle, hv_Seconds2, hv_j, hv_Time;

	//Initialize HDevelop

	//Set the encoder resolution per Increment in metric unit (mm)
	hv_xResolution = 0.02;
	hv_yResolution = 0.02;
	hv_ZoomScale = 1.5;
	hv_CalibFile = "C5-4090CS39-82_SN21512195.txt";
	//���궨�ļ���������Ӧ�Ծ���Ͷ���ʽУ��ϵ��
	ReadC5CSCalibrationFile(hv_CalibFile, &hv_Homography, &hv_gFactor, &hv_NumCoefficients,
		&hv_Tinv1, &hv_Tinv2);
	// stop(); only in hdevelop
	//***********************************************************************************
	//ͼ����
	Threshold(inputImage, &ho_Regions, 1, 65535); 
	ReduceDomain(inputImage, ho_Regions, &ho_ImageReduced);

	// stop(); only in hdevelop
	//************************************************************************************
	//�궨���ݣ�ת������
	GetImageSize(inputImage, &hv_Width, &hv_Height);
	ZoomImageFactor(ho_ImageReduced, &ho_ImageScale, hv_ZoomScale, 1, "nearest_neighbor");
	//Find valid points (>0) and discard all "Zeros" (use higher threshold to discard specific heights)

	Threshold(ho_ImageScale, &ho_Region, 1, 65535);

	//Get the coordinates of the region
	GetRegionPoints(ho_Region, &hv_Rows, &hv_Columns);

	//Read the gray-values (z-coordinates)
	GetGrayval(ho_ImageScale, hv_Rows, hv_Columns, &hv_Grayval);

	//Convert rangemap data (integer) to real sensor position (float)
	//Attention: Using real data from 3D sensor the gFactor must match with the NumSubPixel setting of the sensor. The gFactor is 1/2^NumSubPixel (e.g. NumSubPixel=6 -> gFactor = 0.015625
	//Define the values
	hv_X = hv_Columns / hv_ZoomScale;
	hv_Y_ = hv_yResolution*hv_Rows;
	hv_Z = hv_Grayval*hv_gFactor;

	//Use the homography to transform (x,z) to (x',z')
	ProjectiveTransPoint2d(hv_Homography, hv_X, hv_Z, HTuple(hv_X.TupleLength(), 1),
		&hv_Qx, &hv_Qy, &hv_Qw);

	//Transform to euclidean coordinates
	hv_X_ = hv_Qx / hv_Qw;
	hv_Z_ = hv_Qy / hv_Qw;

	//����ͶӰԭ�㣨Calculate the projection origin��
	HomMat2dInvert(hv_Homography, &hv_HomographyInvert);
	ProjectiveTransPoint2d(hv_HomographyInvert, 0, 0, 1, &hv_Ox, &hv_Oz, &hv_Ow);
	hv_Ox = hv_Ox / hv_Ow;
	hv_Oz = hv_Oz / hv_Ow;

	//����ʽУ��*
	PolynomialCorrection(hv_X_, hv_Z_, hv_NumCoefficients, hv_Tinv1, hv_Tinv2, &hv_X_NEW,
		&hv_Z_NEW);

	//Create the 3D model from points
	GenObjectModel3dFromPoints(hv_X_, hv_Y_, -hv_Z_, &hv_ObjectModel3D);
	GenObjectModel3dFromPoints(hv_X_NEW, hv_Y_, -hv_Z_NEW, &hv_ObjectModel3D_NEW);


	hv_PoseVis.Clear();
	hv_PoseVis[0] = -2.42076;
	hv_PoseVis[1] = 2.63284;
	hv_PoseVis[2] = 1447.37;
	hv_PoseVis[3] = 69.1049;
	hv_PoseVis[4] = 0.777967;
	hv_PoseVis[5] = 359.478;
	hv_PoseVis[6] = 0;

	ObjectModel3dToXyz(&ho_ImageX, &ho_ImageY, &ho_ImageZ_, hv_ObjectModel3D_NEW, "cartesian",
		((((((HTuple(0).Append(0)).TupleConcat(hv_xResolution)).TupleConcat(hv_yResolution)).TupleConcat(hv_Ox)).TupleConcat(0)).TupleConcat(hv_Width)).TupleConcat(hv_Height),
		((((((HTuple(0).Append(0)).Append(0)).Append(0)).Append(0)).Append(0)).Append(0)));
	ScaleImage(ho_ImageZ_, &ho_ImageZ, -1, 0);

	//**ȡ����ά�����
	GetObjectModel3dParams(hv_ObjectModel3D_NEW, "point_coord_x", &hv_xcd);
	GetObjectModel3dParams(hv_ObjectModel3D_NEW, "point_coord_y", &hv_ycd);
	GetObjectModel3dParams(hv_ObjectModel3D_NEW, "point_coord_z", &hv_zcd);
	//fit_surface_first_order (Regions, Image, 'regression', 5, 2, Alpha, Beta, Gamma)
	//fit_surface_second_order (Regions, Image_real, 'regression', 5, 2, Alpha1, Beta1, Gamma1, Delta, Epsilon, Zeta)
	//����������д��������
	Vector<Point3f>TDPlanePoints;
	hv_num = hv_xcd.TupleLength();
	CountSeconds(&hv_Seconds1);

	for (int i = 0; i < hv_num.I(); i++){
		TDPlanePoints.push_back(Point3f(hv_xcd[i].D(), hv_ycd[i].D(), hv_zcd[i].D()));
	}

	CountSeconds(&hv_Seconds2);

	//��λms
	hv_Time = 1000.0*(hv_Seconds2 - hv_Seconds1);

	return TDPlanePoints;
}

Vector<Point3f> CoreAlgorithm::RightToLeft(Vector<Point3f> points)
{
	Mat ori(4, 1, CV_64F);
	Mat teg(4, 1, CV_64F);
	Vector<Point3f> points_teg;
	double rtol[4][4] = {
		1, 0, 0, 0,
		0, -1, 0, 147.8,
		0, 0, -1, 665.6,
		0, 0, 0, 1
	};
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

double CoreAlgorithm::CalculateBorderPointsToCenterAverageDistance(Vector<Point3f> points, StereoCircle centerPoint)
{
	Vector<double> distances;
	double singledistance;
	double AverageDistance;
	double SumDistance = 0;
	for (int i = 0; i < points.size(); i++){
		singledistance = sqrt((points[i].x - centerPoint.center.x)*(points[i].x - centerPoint.center.x) + (points[i].y - centerPoint.center.y)*(points[i].y - centerPoint.center.y)
			+ (points[i].z - centerPoint.center.z)*(points[i].z - centerPoint.center.z));
		distances.push_back(singledistance);
	}
	for (int i = 0; i < distances.size(); i++){
		SumDistance = SumDistance + distances[i];
	}
	AverageDistance = SumDistance / (distances.size());

	ofstream out;
	out.open("OutputFile/BorderPointsToCenterDistance.csv");
	for (int i = 0; i < points.size(); i++){
		out << distances[i] << "\n";
	}
	out.close();

	return AverageDistance;
}

void CoreAlgorithm::CalculateBorderPointsToCenterDistance(Vector<Point3f> points, StereoCircle centerPoint, Vector<double> &distances)
{
	double singledistance;
	for (int i = 0; i < points.size(); i++){
		singledistance = sqrt((points[i].x - centerPoint.center.x)*(points[i].x - centerPoint.center.x) + (points[i].y - centerPoint.center.y)*(points[i].y - centerPoint.center.y)
			+ (points[i].z - centerPoint.center.z)*(points[i].z - centerPoint.center.z));
		distances.push_back(singledistance);
	}
}

Vector<Point3f> CoreAlgorithm::ComputedEdgeProjectionPoints(Vector<Point3f> &points, PlaneNormal&planeparam)
{
	//ƽ�淨������������ֵ
	double A = planeparam.A;
	double B = planeparam.B;
	double C = planeparam.C;
	double D = planeparam.D;
	//��ƽ��ͶӰ��
	//����ͶӰ��
	Vector<Point3f> subpoints;
	double k;
	double xi, yi, zi;
	//������ͶӰ��
	for (int i = 0; i < points.size(); i++){
		k = -(A*points[i].x + B*points[i].y + C*points[i].z + D) / (A*A + B*B + C*C);
		xi = A*k + points[i].x;
		yi = B*k + points[i].y;
		zi = C*k + points[i].z;
		//cout << i + 1 << ": " << xi << " " << yi << " " << zi << endl;
		subpoints.push_back(Point3f(xi, yi, zi));
	}

	ofstream outff;
	outff.open("OutputFile/planetoutingTDpoints.txt");
	for (int i = 0; i < subpoints.size(); i++){
		outff << subpoints[i].x << " " << subpoints[i].y << " " << subpoints[i].z << endl;
	}
	outff.close();
	return subpoints;
}

Vector<Point3f> CoreAlgorithm::filterBorderPointsOnDistanceThrehold(Vector<Point3f> borderPoints, Vector<double> distances, double R, double threholdRange)
{
	Vector<Point3f>filterBorderPoints;
	for (int i = 0; i < borderPoints.size(); i++){
		if (distances[i]>(R - threholdRange) &&distances[i] < (R + threholdRange)){
			filterBorderPoints.push_back(borderPoints[i]);
		}
	}
	return filterBorderPoints;
}

Vector<Point3f> CoreAlgorithm::depthMapCalibration(HObject &inputImage, bool status)
{
	// Local iconic variables
	HObject  ho_Regions, ho_ImageReduced;
	HObject  ho_ImageScale, ho_Region, ho_ImageX, ho_ImageY;
	HObject  ho_ImageZ_, ho_ImageZ;
	HObject  ho_ROI_0, ho_ROI_1;
	HObject  ho_RegionUnion, ho_ROI_3, ho_ROI_4, ho_ROI_5, ho_ROI_6;
	HObject  ho_ROI_7, ho_RegionIntersection;
	// Local control variables
	HTuple  hv_WindowHandle, hv_xResolution, hv_yResolution;
	HTuple  hv_ZoomScale, hv_CalibFile, hv_Homography, hv_gFactor;
	HTuple  hv_NumCoefficients, hv_Tinv1, hv_Tinv2, hv_savefile;
	HTuple  hv_Width, hv_Height, hv_Rows, hv_Columns, hv_Grayval;
	HTuple  hv_X, hv_Y_, hv_Z, hv_Qx, hv_Qy, hv_Qw, hv_X_, hv_Z_;
	HTuple  hv_HomographyInvert, hv_Ox, hv_Oz, hv_Ow, hv_X_NEW;
	HTuple  hv_Z_NEW, hv_ObjectModel3D, hv_ObjectModel3D_NEW;
	HTuple  hv_PoseVis, hv_i, hv_xcd, hv_ycd, hv_zcd, hv_num;
	HTuple  hv_Seconds1, hv_FileHandle, hv_Seconds2, hv_j, hv_Time;
	HTuple  hv_Area, hv_Row, hv_Area2, hv_Row2, hv_Column2;
	HTuple  hv_Column, hv_status, hv_Exception;
	//Initialize HDevelop

	//Set the encoder resolution per Increment in metric unit (mm)
	hv_xResolution = 0.02;
	hv_yResolution = 0.02;
	hv_ZoomScale = 1.5;
	hv_CalibFile = "C5-4090CS39-82_SN21512195.txt";
	//���궨�ļ���������Ӧ�Ծ���Ͷ���ʽУ��ϵ��
	ReadC5CSCalibrationFile(hv_CalibFile, &hv_Homography, &hv_gFactor, &hv_NumCoefficients,
		&hv_Tinv1, &hv_Tinv2);
	// stop(); only in hdevelop
	//***********************************************************************************
	

	if (status==true)
	{
		//************************************************************************************
		//�궨���ݣ�ת������
		GetImageSize(inputImage, &hv_Width, &hv_Height);
		ZoomImageFactor(inputImage, &ho_ImageScale, hv_ZoomScale, 1, "nearest_neighbor");
		//Find valid points (>0) and discard all "Zeros" (use higher threshold to discard specific heights)

		Threshold(ho_ImageScale, &ho_Region, 1, 65535);

		//Get the coordinates of the region
		GetRegionPoints(ho_Region, &hv_Rows, &hv_Columns);

		//Read the gray-values (z-coordinates)
		GetGrayval(ho_ImageScale, hv_Rows, hv_Columns, &hv_Grayval);

		//Convert rangemap data (integer) to real sensor position (float)
		//Attention: Using real data from 3D sensor the gFactor must match with the NumSubPixel setting of the sensor. The gFactor is 1/2^NumSubPixel (e.g. NumSubPixel=6 -> gFactor = 0.015625
		//Define the values
		hv_X = hv_Columns / hv_ZoomScale;
		hv_Y_ = hv_yResolution*hv_Rows;
		hv_Z = hv_Grayval*hv_gFactor;

		//Use the homography to transform (x,z) to (x',z')
		ProjectiveTransPoint2d(hv_Homography, hv_X, hv_Z, HTuple(hv_X.TupleLength(), 1),
			&hv_Qx, &hv_Qy, &hv_Qw);

		//Transform to euclidean coordinates
		hv_X_ = hv_Qx / hv_Qw;
		hv_Z_ = hv_Qy / hv_Qw;

		//����ͶӰԭ�㣨Calculate the projection origin��
		HomMat2dInvert(hv_Homography, &hv_HomographyInvert);
		ProjectiveTransPoint2d(hv_HomographyInvert, 0, 0, 1, &hv_Ox, &hv_Oz, &hv_Ow);
		hv_Ox = hv_Ox / hv_Ow;
		hv_Oz = hv_Oz / hv_Ow;

		//����ʽУ��*
		PolynomialCorrection(hv_X_, hv_Z_, hv_NumCoefficients, hv_Tinv1, hv_Tinv2, &hv_X_NEW,
			&hv_Z_NEW);

		//Create the 3D model from points
		GenObjectModel3dFromPoints(hv_X_, hv_Y_, -hv_Z_, &hv_ObjectModel3D);
		GenObjectModel3dFromPoints(hv_X_NEW, hv_Y_, -hv_Z_NEW, &hv_ObjectModel3D_NEW);


		hv_PoseVis.Clear();
		hv_PoseVis[0] = -2.42076;
		hv_PoseVis[1] = 2.63284;
		hv_PoseVis[2] = 1447.37;
		hv_PoseVis[3] = 69.1049;
		hv_PoseVis[4] = 0.777967;
		hv_PoseVis[5] = 359.478;
		hv_PoseVis[6] = 0;

		ObjectModel3dToXyz(&ho_ImageX, &ho_ImageY, &ho_ImageZ_, hv_ObjectModel3D_NEW, "cartesian",
			((((((HTuple(0).Append(0)).TupleConcat(hv_xResolution)).TupleConcat(hv_yResolution)).TupleConcat(hv_Ox)).TupleConcat(0)).TupleConcat(hv_Width)).TupleConcat(hv_Height),
			((((((HTuple(0).Append(0)).Append(0)).Append(0)).Append(0)).Append(0)).Append(0)));
		ScaleImage(ho_ImageZ_, &ho_ImageZ, -1, 0);

		//**ȡ����ά�����
		GetObjectModel3dParams(hv_ObjectModel3D_NEW, "point_coord_x", &hv_xcd);
		GetObjectModel3dParams(hv_ObjectModel3D_NEW, "point_coord_y", &hv_ycd);
		GetObjectModel3dParams(hv_ObjectModel3D_NEW, "point_coord_z", &hv_zcd);
		//fit_surface_first_order (Regions, Image, 'regression', 5, 2, Alpha, Beta, Gamma)
		//fit_surface_second_order (Regions, Image_real, 'regression', 5, 2, Alpha1, Beta1, Gamma1, Delta, Epsilon, Zeta)
		//����������д��������
		Vector<Point3f>TDPlanePoints;
		hv_num = hv_xcd.TupleLength();
		CountSeconds(&hv_Seconds1);

		for (int i = 0; i < hv_num.I(); i++){
			TDPlanePoints.push_back(Point3f(hv_xcd[i].D(), hv_ycd[i].D(), hv_zcd[i].D()));
		}

		CountSeconds(&hv_Seconds2);

		//��λms
		hv_Time = 1000.0*(hv_Seconds2 - hv_Seconds1);

		return TDPlanePoints;
	}
	else{
		Vector<Point3f>TDPlanePoints;
		return TDPlanePoints;
	}
}

HalconCpp::HObject CoreAlgorithm::getFlangeEdgeImage(HObject &inputImage, bool& status)
{
	// Local iconic variables
	HObject  ho_Regions, ho_ImageReduced;
	HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_RegionTrans;
	HObject  ho_RegionErosion, ho_RegionDifference, ho_RegionTrans1;
	HObject  ho_Rectangle, ho_RegionIntersection, ho_EndRegionIntersection;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_Area, hv_Row;
	HTuple  hv_Column, hv_rectangleWidth, hv_rectangleHeight;
	HTuple  hv_rectanglePhi, hv_Exception;

	try{
		//��ȡͼƬ�ߴ�
		GetImageSize(inputImage, &hv_Width, &hv_Height);
		//��ֵ�ָ�
		Threshold(inputImage, &ho_Regions, 1, 65535);
		//������ͨ��
		Connection(ho_Regions, &ho_ConnectedRegions);
		//�������ѡ����Ҫ������
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 1e+04, 5e+06);
		//����͹���任
		ShapeTrans(ho_SelectedRegions, &ho_RegionTrans, "convex");
		//��������
		ErosionCircle(ho_RegionTrans, &ho_RegionErosion, 3.5);
		//���������ȡ��Χ����
		Difference(ho_RegionTrans, ho_RegionErosion, &ho_RegionDifference);
		//��ȡ�������Ӿ���
		ShapeTrans(ho_RegionDifference, &ho_RegionTrans1, "rectangle1");
		//��ȡ��Ӿ��ε���������
		AreaCenter(ho_RegionTrans1, &hv_Area, &hv_Row, &hv_Column);
		//��ȡ��Ӿ��εĳ�
		RegionFeatures(ho_RegionTrans1, "width", &hv_rectangleWidth);
		//��ȡ��Ӿ��εĿ�
		RegionFeatures(ho_RegionTrans1, "height", &hv_rectangleHeight);
		//��ȡ��Ӿ��ε���ת��
		RegionFeatures(ho_RegionTrans1, "phi", &hv_rectanglePhi);
		//������Ӿ��εĲ��������ƿ�������ľ���
		GenRectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_rectanglePhi, (hv_rectangleWidth - 10) / 2,
			(hv_rectangleHeight - 10) / 2);
		//��������ȥ���±�ֱ��
		Intersection(ho_RegionDifference, ho_Rectangle, &ho_RegionIntersection);
		//��ͼƬ�л�ȡ��Ե
		Intersection(ho_RegionIntersection, ho_SelectedRegions, &ho_EndRegionIntersection);
		ReduceDomain(inputImage, ho_RegionIntersection, &ho_ImageReduced);
		status = true;
	}
	// catch (Exception) 
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		status = false;
	}
	return ho_ImageReduced;
}

HalconCpp::HObject CoreAlgorithm::getFlangeHoleEdgeImage(HObject &inputImage, bool& status)
{
	// Local iconic variables
	HObject  ho_ModelContours, ho_TransContours;
	HObject  ho_Circle, ho_ImageReduced, ho_Regions, ho_Region1;
	HObject  ho_RegionDilation, ho_RegionIntersection, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_ImageReduced1;

	// Local control variables
	HTuple  hv_ModelId, hv_ModelRow, hv_ModelColumn;
	HTuple  hv_ModelAngle, hv_ModelScale, hv_ModelScore, hv_Exception;
	HTuple  hv_MatchingObjIdx, hv_HomMat, hv_Area, hv_Row, hv_Column;

	//��ȡͼƬ
	//��ȡģ��
	ReadShapeModel("flangeHole.shm", &hv_ModelId);
	//��ȡģ������
	GetShapeModelContours(&ho_ModelContours, hv_ModelId, 1);
	//ģ��ƥ��
	try
	{
		FindScaledShapeModel(inputImage, hv_ModelId, HTuple(0).TupleRad(), HTuple(360).TupleRad(),
			0.95, 1.05, 0.5, 1, 0.5, "least_squares", (HTuple(7).Append(1)), 0.75, &hv_ModelRow,
			&hv_ModelColumn, &hv_ModelAngle, &hv_ModelScale, &hv_ModelScore);
	}
	// catch (Exception) 
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		status = false;
	}

	{
		HTuple end_val13 = (hv_ModelScore.TupleLength()) - 1;
		HTuple step_val13 = 1;
		for (hv_MatchingObjIdx = 0; hv_MatchingObjIdx.Continue(end_val13, step_val13); hv_MatchingObjIdx += step_val13)
		{
			//�޶�ģ������
			GenCircle(&ho_Circle, hv_ModelRow, hv_ModelColumn, 220);
			//��ȡͼƬ����������
			ReduceDomain(inputImage, ho_Circle, &ho_ImageReduced);
			//��ֵ�ָ�
			Threshold(ho_ImageReduced, &ho_Regions, 0, 1);
			//��ֵ�ָ�
			Threshold(ho_ImageReduced, &ho_Region1, 1, 65535);
			//��������
			DilationCircle(ho_Regions, &ho_RegionDilation, 1.5);
			//��������
			Intersection(ho_Region1, ho_RegionDilation, &ho_RegionIntersection);
			//������ͨ��
			Connection(ho_RegionIntersection, &ho_ConnectedRegions);
			//���������ֵѡ������
			SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 800, 99999);
			//��ȡSelectedRegions���
			AreaCenter(ho_SelectedRegions, &hv_Area, &hv_Row, &hv_Column);
			if (hv_Area != 0)
			{
				status = true;
				ReduceDomain(inputImage, ho_SelectedRegions, &ho_ImageReduced1);
			}
			else
			{
				status = false;
				ReduceDomain(inputImage, ho_SelectedRegions, &ho_ImageReduced1);
			}


		}
	}
	ClearShapeModel(hv_ModelId);
	return ho_ImageReduced1;
}

HalconCpp::HObject CoreAlgorithm::getfrontDriveBoxSplineEdgeImage(HObject &inputImage, bool& status)
{
	// Local iconic variables
	HObject  ho_Image, ho_Regions, ho_ModelContours;
	HObject  ho_TransContours, ho_EmptyRegion, ho_ModelRegion;
	HObject  ho_ImageReduced, ho_RegionsReduced, ho_RegionErosion;
	HObject  ho_RegionDilation, ho_ImageReduced1, ho_ImageMean;
	HObject  ho_Edges, ho_ContoursSplit, ho_Lines, ho_Contour;
	HObject  ho_RegionLines, ho_RegionUnion, ho_ConnectedRegions;
	HObject  ho_RegionErosion1, ho_RegionIntersection, ho_Rectangle4;
	HObject  ho_Cross, ho_ROI_0, ho_EmptyRegion1, ho_ObjectSelected;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_ModelID, hv_ModelRow;
	HTuple  hv_ModelColumn, hv_ModelAngle, hv_ModelScale, hv_ModelScore;
	HTuple  hv_MatchingObjIdx, hv_HomMat, hv_i, hv_Phi, hv_Number;
	HTuple  hv_I, hv_Type, hv_RowBegin, hv_ColBegin, hv_RowEnd;
	HTuple  hv_ColEnd, hv_Nr, hv_Nc, hv_Dist, hv_Area, hv_Row;
	HTuple  hv_Column, hv_Length1, hv_Length2, hv_Row1, hv_Col1;
	HTuple  hv_Distance, hv_Sorted, hv_b, hv_a, hv_dis, hv_Exception;


	//***********************************************************************************
	//ͼ����

	ho_Image = inputImage;

	GetImageSize(ho_Image, &hv_Width, &hv_Height);

	try
	{
		GetImageSize(ho_Image, &hv_Width, &hv_Height);
		Threshold(ho_Image, &ho_Regions, 1, 65535);
		ReadShapeModel("rightgear1.0.shm", &hv_ModelID);
		GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);
		//ģ��ƥ��
		FindScaledShapeModel(ho_Image, hv_ModelID, HTuple(0).TupleRad(), HTuple(360).TupleRad(),
			0.98, 1.02, 0.4, 10, 0.5, "least_squares", (HTuple(7).Append(1)), 0.75, &hv_ModelRow,
			&hv_ModelColumn, &hv_ModelAngle, &hv_ModelScale, &hv_ModelScore);

		{
			HTuple end_val10 = (hv_ModelScore.TupleLength()) - 1;
			HTuple step_val10 = 1;
			for (hv_MatchingObjIdx = 0; hv_MatchingObjIdx.Continue(end_val10, step_val10); hv_MatchingObjIdx += step_val10)
			{
				HomMat2dIdentity(&hv_HomMat);
				HomMat2dScale(hv_HomMat, HTuple(hv_ModelScale[hv_MatchingObjIdx]), HTuple(hv_ModelScale[hv_MatchingObjIdx]),
					0, 0, &hv_HomMat);
				HomMat2dRotate(hv_HomMat, HTuple(hv_ModelAngle[hv_MatchingObjIdx]), 0, 0, &hv_HomMat);
				HomMat2dTranslate(hv_HomMat, HTuple(hv_ModelRow[hv_MatchingObjIdx]), HTuple(hv_ModelColumn[hv_MatchingObjIdx]),
					&hv_HomMat);
				AffineTransContourXld(ho_ModelContours, &ho_TransContours, hv_HomMat);
				if (HDevWindowStack::IsOpen())
					DispObj(ho_TransContours, HDevWindowStack::GetActive());
				//stop ()
			}
		}
		// stop(...); only in hdevelop
		GenEmptyRegion(&ho_EmptyRegion);

		{
			HTuple end_val22 = (hv_ModelScale.TupleLength()) - 1;
			HTuple step_val22 = 1;
			for (hv_i = 0; hv_i.Continue(end_val22, step_val22); hv_i += step_val22)
			{

				GenRectangle2(&ho_ModelRegion, HTuple(hv_ModelRow[hv_i]), HTuple(hv_ModelColumn[hv_i]),
					(HTuple(-87.8317).TupleRad()) + HTuple(hv_ModelAngle[hv_i]), 259, 280);

				OrientationRegion(ho_ModelRegion, &hv_Phi);
				ReduceDomain(ho_Image, ho_ModelRegion, &ho_ImageReduced);
				Threshold(ho_ImageReduced, &ho_RegionsReduced, 1, 65535);
				ErosionRectangle1(ho_RegionsReduced, &ho_RegionErosion, 10, 10);
				DilationRectangle1(ho_RegionErosion, &ho_RegionDilation, 10, 10);
				ReduceDomain(ho_Image, ho_RegionDilation, &ho_ImageReduced1);
				MeanImage(ho_ImageReduced1, &ho_ImageMean, 3, 3);
				EdgesSubPix(ho_ImageMean, &ho_Edges, "canny", 1, 40, 80);
				SegmentContoursXld(ho_Edges, &ho_ContoursSplit, "lines", 5, 4, 2);
				CountObj(ho_ContoursSplit, &hv_Number);
				GenEmptyObj(&ho_Lines);
				{
					HTuple end_val37 = hv_Number;
					HTuple step_val37 = 1;
					for (hv_I = 1; hv_I.Continue(end_val37, step_val37); hv_I += step_val37)
					{
						SelectObj(ho_ContoursSplit, &ho_Contour, hv_I);
						GetContourGlobalAttribXld(ho_Contour, "cont_approx", &hv_Type);
						if (0 != (hv_Type == -1))
						{
							ConcatObj(ho_Lines, ho_Contour, &ho_Lines);
						}
					}
				}
				// stop(...); only in hdevelop
				FitLineContourXld(ho_Lines, "drop", -1, 0, 5, 2, &hv_RowBegin, &hv_ColBegin,
					&hv_RowEnd, &hv_ColEnd, &hv_Nr, &hv_Nc, &hv_Dist);
				GenRegionLine(&ho_RegionLines, hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd);
				Union1(ho_RegionLines, &ho_RegionUnion);
				Connection(ho_RegionUnion, &ho_ConnectedRegions);
				ErosionRectangle1(ho_ModelRegion, &ho_RegionErosion1, 7, 7);
				Intersection(ho_ConnectedRegions, ho_RegionErosion1, &ho_RegionIntersection);
				AreaCenter(ho_RegionIntersection, &hv_Area, &hv_Row, &hv_Column);
				//**********************************
				hv_Length1 = 259;
				hv_Length2 = 280;
				hv_Phi = (HTuple(-87.8317).TupleRad()) + HTuple(hv_ModelAngle[hv_i]);
				if (HDevWindowStack::IsOpen())
					SetColor(HDevWindowStack::GetActive(), "red");
				GenRectangle2ContourXld(&ho_Rectangle4, HTuple(hv_ModelRow[hv_i]), HTuple(hv_ModelColumn[hv_i]),
					(HTuple(-87.8317).TupleRad()) + HTuple(hv_ModelAngle[hv_i]), 259, 280);
				GetContourXld(ho_Rectangle4, &hv_Row1, &hv_Col1);
				if (HDevWindowStack::IsOpen())
					SetColor(HDevWindowStack::GetActive(), "green");
				GenCrossContourXld(&ho_Cross, hv_Row1, hv_Col1, 6, hv_Phi);
				GenRegionLine(&ho_ROI_0, HTuple(hv_Row1[0]), HTuple(hv_Col1[0]), HTuple(hv_Row1[3]),
					HTuple(hv_Col1[3]));

				//*************************************************
				DistancePl(hv_Row, hv_Column, HTuple(hv_Row1[0]), HTuple(hv_Col1[0]), HTuple(hv_Row1[3]),
					HTuple(hv_Col1[3]), &hv_Distance);

				TupleSort(hv_Distance, &hv_Sorted);
				GenEmptyRegion(&ho_EmptyRegion1);
				hv_b = 0;
				{
					HTuple end_val69 = (hv_Distance.TupleLength()) - 1;
					HTuple step_val69 = 1;
					for (hv_a = 0; hv_a.Continue(end_val69, step_val69); hv_a += step_val69)
					{
						if (0 != (HTuple(hv_Distance[hv_a])<104))
						{
							SelectObj(ho_RegionIntersection, &ho_ObjectSelected, hv_a + 1);
							hv_dis[hv_b] = HTuple(hv_Distance[hv_a]);
							hv_b += 1;
							Union2(ho_ObjectSelected, ho_EmptyRegion1, &ho_EmptyRegion1);
						}

					}
				}
				Union2(ho_EmptyRegion1, ho_EmptyRegion, &ho_EmptyRegion);
				// stop(...); only in hdevelop

			}
		}
		Intersection(ho_Regions, ho_EmptyRegion, &ho_EmptyRegion);
		ReduceDomain(ho_Image, ho_EmptyRegion, &ho_ImageReduced);
		status = true;
	}

	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		status = false;
	}

	return ho_ImageReduced;
}

