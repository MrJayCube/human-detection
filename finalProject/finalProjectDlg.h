
// finalProjectDlg.h: archivo de encabezado
//

#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/objdetect.hpp"
#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/bgsegm.hpp>


using namespace std;
using namespace cv;
using namespace dnn;


// Cuadro de diálogo de CfinalProjectDlg
class CfinalProjectDlg : public CDialogEx
{
// Construcción
public:
	CfinalProjectDlg(CWnd* pParent = nullptr);	// Constructor estándar

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINALPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	cv::VideoWriter videoOut;
	CString nombreArchivo;
	int identifier = 0;
	double fps = 60;
	cv::VideoCapture cap;
	bool m_bCapturando;
	Mat imagen;
	int fourcc;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void condefects(vector<Vec4i> convexityDefectsSet, vector<Point> mycontour, Mat& original) {
		for (int cDefIt = 0; cDefIt < convexityDefectsSet.size(); cDefIt++) {
			int startIdx = convexityDefectsSet[cDefIt].val[0];
			Point ptStart(mycontour[startIdx]);
			int endIdx = convexityDefectsSet[cDefIt].val[1];
			Point ptEnd(mycontour[endIdx]);
			int farIdx = convexityDefectsSet[cDefIt].val[2];
			Point ptFar(mycontour[farIdx]);
			double depth = static_cast<double>(convexityDefectsSet[cDefIt].val[3]) / 256;
			//display start points
			circle(original, ptStart, 5, CV_RGB(255, 0, 0), 2, 8);
			//display all end points
			circle(original, ptEnd, 5, CV_RGB(255, 255, 0), 2, 8);
			//display all far points
			circle(original, ptFar, 5, CV_RGB(0, 255, 0), 2, 8);
		}
	}
	int contrast = 50;
	int minThresh = 60;

	//Good features
	int maxCorners = 50;
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3, gradientSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	int radius = 4;

	Mat kernel;

	CascadeClassifier face_cascade;

	afx_msg void OnBnClickedButton2();

	boolean training = false;
	boolean selected;
	boolean positive = false;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	int contp = 0;
	int contn = 0;
	std::string imageName;

	boolean first_time = true;
	cv::VideoCapture captures;

	void chose() {
		stringstream ss;
		if (captures.isOpened()) {
			captures.read(imagen);
		}
		if (training) {
			imshow("For training", imagen);
			if (selected) {
				if (positive) {
					ss << contp;
					imageName = "C://Users//Joan//Documents//Asignaturas//VisionPorComputador//ProyectoFinal//clasificador//p//test" + ss.str();
					contp++;
				}
				else {
					ss << contn;
					imageName = "C://Users//Joan//Documents//Asignaturas//VisionPorComputador//ProyectoFinal//clasificador//n//test" + ss.str();
					contn++;
				}
				imageName = imageName + ".jpg";
				cout << imageName;
				imwrite(imageName, imagen);
				selected = false;
			}
		}
	}
	int personas = 0;
};
