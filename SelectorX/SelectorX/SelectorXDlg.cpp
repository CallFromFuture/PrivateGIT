
// SelectorXDlg.cpp : ʵ���ļ�
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








// CSelectorXDlg �Ի���



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


// CSelectorXDlg ��Ϣ�������

BOOL CSelectorXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	progress1Update.SetRange(0, nUpper);
	progress1Update.SetStep(1);
	progress1Update.SetPos(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSelectorXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		;
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSelectorXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSelectorXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CSelectorXDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
