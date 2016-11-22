
// SelectorXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SelectorX.h"
#include "SelectorXDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
long randomDataA(315);
bool beStarted(0);
int nUpper(5000);
CString randomData;
long temp;
int countA[10]{ 0 };
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSelectorXDlg 对话框



CSelectorXDlg::CSelectorXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SELECTORX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSelectorXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edit1Update);
	DDX_Control(pDX, IDC_PROGRESS1, progress1Update);
}

BEGIN_MESSAGE_MAP(CSelectorXDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_EN_CHANGE(IDC_EDIT1, &CSelectorXDlg::OnEnChangeEdit1)
ON_WM_MOUSEMOVE()
ON_BN_CLICKED(IDC_BUTTON1, &CSelectorXDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSelectorXDlg 消息处理程序

BOOL CSelectorXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	progress1Update.SetRange(0, nUpper);
	progress1Update.SetStep(1);
	progress1Update.SetPos(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSelectorXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSelectorXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSelectorXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CSelectorXDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(beStarted == true) {


	temp = (((point.x + point.y) * point.x * point.y) ^ ((point.x - point.y) / (point.x+1) / (point.y +1)));
	randomDataA ^= temp;
	randomDataA = (randomDataA + 1) / 2;
	countA[(randomDataA % 10)]++;
	randomData.Format(_T("%ld,%ld format:%ld All:%ld\r\n0:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d 8:%d 9:%d"), point.x, point.y, temp,randomDataA,countA[0], countA[1], countA[2], countA[3], countA[4], countA[5], countA[6], countA[7], countA[8], countA[9]);
	edit1Update.SetWindowText(randomData);
	progress1Update.StepIt();
	if (progress1Update.GetPos() == nUpper) {
		AfxMessageBox(_T("Done"), MB_OK| MB_ICONINFORMATION);
		beStarted = !beStarted;
	}
	

	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CSelectorXDlg::OnBnClickedButton1()
{
	beStarted = !beStarted;
	for (auto& i : countA)
		i = 0;
	progress1Update.SetPos(0);
	// TODO: 在此添加控件通知处理程序代码
}
