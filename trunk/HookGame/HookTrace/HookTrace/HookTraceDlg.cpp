// HookTraceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HookTrace.h"
#include "HookTraceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHookTraceDlg �Ի���




CHookTraceDlg::CHookTraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHookTraceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_pDlg = this;
}

void CHookTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lbHook);
}

BEGIN_MESSAGE_MAP(CHookTraceDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CHookTraceDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHookTraceDlg ��Ϣ�������

BOOL CHookTraceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHookTraceDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHookTraceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void WINAPI sysTrace(LPCSTR lpOutputString)
{
	AfxMessageBox(lpOutputString);
}

/*����ļ�����ʱ��   //���һ���޸�ʱ��*/
#ifdef _DEBUG
	#define HDumpInfo(info)\
	{\
		CString strInfo;\
		strInfo.Format(_T("%s\n%s\n%d\n%s\n%s\n%s\n"),__FILE__,__FUNCTION__,__LINE__,__DATE__,__TIME__,__TIMESTAMP__);\
		strInfo += info;\
		strInfo += _T("\n");\
		OutputDebugStringA(strInfo);\
	}
#else
	#define HDumpInfo OutputDebugStringA
#endif



class Base;
typedef void (Base::*AOut)();
AOut func;

class Base
{
public:
	virtual void Output(){}
};

class A: public Base
{
public:
	void Output()
	{
		TRACE("Output class member\n");
	}
};

void newOutput()
{
	TRACE("hooked class member\n");
}

void CHookTraceDlg::OnBnClickedOk()
{
	HHOOK hk;
	static bool bStart = false;
	if (!bStart)
	{
		hk = m_hookKB.StartHook(&CHookTraceDlg::APICallBack);
		SetDlgItemText(IDOK,_T("ж�ع���"));
	}
	else
	{
		m_hookKB.StopHook();
		SetDlgItemText(IDOK,_T("��������"));
	}
	bStart = !bStart;



	m_hookKB.InterceptDllMember(GetModuleHandle(NULL),"Kernel32.dll","OutputDebugStringA",(DWORD)&sysTrace);
	HDumpInfo("dll������ע��ɹ���");

	HookMemberFunc(A::Output, newOutput, func);
	Base *a = new A();
	a->Output();  // ��ʱ, ������佫�����newOutput
	//(a->*func)(); // ����ǵ���ԭ����A::Output

	delete a;
}

int CALLBACK CHookTraceDlg::APICallBack( WPARAM wParam,LPARAM lParam )
{
	switch (wParam)
	{
//�������
	case VK_F5:
		{
			g_pDlg->m_lbHook.InsertString(0,_T("F5������"));
			break;
		}

//�������
	case WM_RBUTTONDBLCLK:
		{
			g_pDlg->m_lbHook.InsertString(0,_T("����Ҽ�������"));
			break;
		}
	default:break;
	}
	return 0;
}