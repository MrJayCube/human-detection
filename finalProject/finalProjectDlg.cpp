
// finalProjectDlg.cpp: archivo de implementación
//

#include "pch.h"
#include "framework.h"
#include "finalProject.h"
#include "finalProjectDlg.h"
#include "afxdialogex.h"
#include <opencv2/video/background_segm.hpp>

//#include <opencv2/bgsegm.hpp>

Mat fgMask, fgMask2, fgMask3, fgMask4;
Ptr<BackgroundSubtractor> pBackSub;
Ptr<BackgroundSubtractor> pBackSubAdaptative;
Ptr<BackgroundSubtractor> pBackSubBayesianSegmentation;
Ptr<BackgroundSubtractor> pBackSubKNN;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cuadro de diálogo de CfinalProjectDlg



CfinalProjectDlg::CfinalProjectDlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_FINALPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfinalProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfinalProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CfinalProjectDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CfinalProjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CfinalProjectDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CfinalProjectDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Controladores de mensajes de CfinalProjectDlg

BOOL CfinalProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Establecer el icono para este cuadro de diálogo.  El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	// TODO: agregar aquí inicialización adicional

	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}

void CfinalProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Si agrega un botón Minimizar al cuadro de diálogo, necesitará el siguiente código
//  para dibujar el icono.  Para aplicaciones MFC que utilicen el modelo de documentos y vistas,
//  esta operación la realiza automáticamente el marco de trabajo.

void CfinalProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CfinalProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


class Detector
{
	HOGDescriptor hog;
public:
	Detector()
	{
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	}
	vector<Rect> detect(InputArray img)
	{
		// Run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		vector<Rect> found;
		hog.detectMultiScale(img, found, 0, Size(8, 8), Size(), 1.1, 1, false);
		return found;
	}
	void adjustRect(Rect& r) const
	{
		// The HOG detector returns slightly larger rectangles than the real objects,
		// so we slightly shrink the rectangles to get a nicer output.
		r.x += cvRound(r.width * 0.1);
		r.width = cvRound(r.width * 0.8);
		r.y += cvRound(r.height * 0.07);
		r.height = cvRound(r.height * 0.8);
	}
};


void CfinalProjectDlg::OnBnClickedButton1()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	UpdateData(TRUE);
	CString nombreArchivo;
	GetDlgItemText(IDC_EDIT1, nombreArchivo);
	std::string ss;
	ss = CW2A((LPCTSTR)nombreArchivo);

	if (!m_bCapturando) {
		if (nombreArchivo.IsEmpty()) {
			//cap.open("C:\\Users\\Joan\\Videos\\vtest.avi");
			//cap.open("C:\\Users\\Joan\\Videos\\nubesyPersonas.mp4");
			cap.open("C:\\Users\\Joan\\Videos\\videoPruebaDeteccion.mp4");
			//cap.open("C:\\Users\\Joan\\Videos\\untitled.mp4");
			//cap.open("http://kona2.alc.upv.es/temporal/VideosMenejador/CAM0_2021-04-06.mp4");
			//nombreArchivo = "C:\\Users\\Joan\\Videos\\Camara";
			pBackSubKNN = createBackgroundSubtractorKNN(500, 400, false);
			kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		}
		else {
			cap.open(ss);
			cout << ss;
		}
		if (!cap.isOpened()) {
			AfxMessageBox(_T("Imposible abrir camara o video"), MB_OK);
			return;
		}
		SetTimer(1, (uint)(10000.0 / fps), NULL);
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("Parar"));
		m_bCapturando = true;
	}
	else {
		KillTimer(1);
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("Capturar"));
		m_bCapturando = false;
		cap.release();
		videoOut.release();
		destroyAllWindows();
	}
}

void onChange(int pos, void* obj) {
	CfinalProjectDlg* dlg = (CfinalProjectDlg*)obj;
}

void CfinalProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	if (cap.isOpened()) {
		cap.read(imagen);
		imshow("imagen Image 2", imagen);

	}
	if (imagen.channels() != 3) {
		return;
	}

	Mat dst, gray;
	Size tam;
	int width, height;
	tam = imagen.size();

	width = tam.width;
	height = tam.height;

	Rect roi(0, height / 2, width, height / 2);
	Mat image_roi = imagen(roi);

	resize(image_roi, image_roi, Size(), 1.5, 1.5);

	cvtColor(image_roi, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, dst);
	//imshow("equalizeHist", gray);
	GaussianBlur(dst, dst, Size(19, 19), 0, BORDER_DEFAULT);
	//imshow("GaussianBlur", dst);

	Mat imagen2;
	cvtColor(dst, imagen2, COLOR_GRAY2BGR);
	pBackSubKNN->apply(imagen2, fgMask4);

	Mat stat, cent, ilabel;
	int nobjetos;

	nobjetos = connectedComponentsWithStats(fgMask4, ilabel, stat, cent, 8);

	for (int i = 1; i < nobjetos; i++) {
		if (stat.at<int>(i, CC_STAT_AREA) > 2000 && stat.at<int>(i, CC_STAT_AREA) < 200000) {
			Mat mask = ilabel == i;
			Rect roi(stat.at<int>(i, CC_STAT_LEFT), stat.at<int>(i, CC_STAT_TOP), stat.at<int>(i, CC_STAT_WIDTH), stat.at<int>(i, CC_STAT_HEIGHT));
			Mat object = image_roi(roi);
			resize(object, object, Size(), 3, 3);
			Mat gray2;
			cvtColor(object, gray2, COLOR_BGR2GRAY);
			imshow("testing", object);

			vector<cv::Rect> faces;
			vector<int> rejectLevels;
			vector<double> levelWeights;

			Detector detector;
			vector<Rect> found = detector.detect(gray2);
			for (vector<Rect>::iterator i = found.begin(); i != found.end(); ++i)
			{
				Rect& r = *i;
				detector.adjustRect(r);
				rectangle(object, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
				personas++;
			}

			imshow("testing", object);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CfinalProjectDlg::OnBnClickedButton2()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	if (first_time) {
		captures.open("C:\\Users\\Joan\\Videos\\videoPruebaDeteccion.mp4");
		first_time = false;
	}

	if (captures.isOpened()) {
		captures.read(imagen);
	}
	training = true;
}


void CfinalProjectDlg::OnBnClickedButton3()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	positive = true;
	selected = true;
	chose();
}


void CfinalProjectDlg::OnBnClickedButton4()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	positive = false;
	selected = true;
	chose();
}