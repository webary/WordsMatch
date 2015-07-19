#include "stdafx.h"
#include "imgMatch.h"
#include "AdvancedSet.h"


AdvancedSet::AdvancedSet(CWnd* pParent)	: CDialog(AdvancedSet::IDD, pParent)
{
	bg2.LoadResource(IDB_BITMAP2);
	waitTime = 5;
	permitError = 8;
	EvoCol = 8;
	absolut = 0.65;
	balance_ratio = 0.5;
	double ssum=0;
	for(int i=0;i<ratio_num-1;i++)
	{
		ratio[i] = int(1000/ratio_num+.5)/1000.0;
		ssum += ratio[i];
	}
	ratio[ratio_num-1] = 1-ssum;
}

void AdvancedSet::DoDataExchange(CDataExchange* pDX){
	DDX_Text(pDX, IDC_wt, waitTime);
	DDV_MinMaxInt(pDX, waitTime, 1, 100000000);
	DDX_Text(pDX, IDC_EDIT1, permitError);
	DDV_MinMaxDouble(pDX, permitError, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, EvoCol);
	DDV_MinMaxInt(pDX, EvoCol, 5, 25);
	DDX_Text(pDX, IDC_abs, absolut);
	DDV_MinMaxDouble(pDX, absolut, 0.01, 0.99);
	DDX_Text(pDX, IDC_ratio1, ratio[0]);
	DDX_Text(pDX, IDC_ratio2, ratio[1]);
	DDX_Text(pDX, IDC_ratio3, ratio[2]);
	DDX_Text(pDX, IDC_ratio4, ratio[3]);
	DDX_Text(pDX, IDC_ratio5, ratio[4]);
	DDX_Text(pDX, IDC_ratio6, ratio[5]);
	DDX_Text(pDX, IDC_ratio7, ratio[6]);
	for(int i=0;i<ratio_num;++i)
		DDV_MinMaxDouble(pDX, ratio[i], 0, 1);
	DDX_Text(pDX, IDC_balance_r, balance_ratio);
	DDV_MinMaxDouble(pDX, balance_ratio, 0, 1);
}

BEGIN_MESSAGE_MAP(AdvancedSet, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_help_abs, &AdvancedSet::OnHelpAbs)
	ON_EN_CHANGE(IDC_ratio1, &AdvancedSet::OnEnChangeRatio)
	ON_EN_CHANGE(IDC_ratio2, &AdvancedSet::OnEnChangeRatio)
	ON_EN_CHANGE(IDC_ratio3, &AdvancedSet::OnEnChangeRatio)
	ON_EN_CHANGE(IDC_ratio4, &AdvancedSet::OnEnChangeRatio)
	ON_EN_CHANGE(IDC_ratio5, &AdvancedSet::OnEnChangeRatio)
	ON_EN_CHANGE(IDC_ratio6, &AdvancedSet::OnEnChangeRatio)
	ON_BN_CLICKED(IDC_help_balance, &AdvancedSet::OnBnClickedhelpbalance)
END_MESSAGE_MAP()


BOOL AdvancedSet::OnEraseBkgnd(CDC* pDC) 
{
	int x,y;
	CRect rc;
	GetClientRect(&rc);
	x=0;y=0;
	while(y < rc.Height())   //rc表示对话框的大小尺寸
	{
		while(x < rc.Width()) 
		{
			bg2.DrawBMP(pDC, x, y); 
			x += bg2.GetWidth(); //得到位图的宽度
		}
		x = 0;
		y += bg2.GetHeight();  //得到位图的高度
	}
	return TRUE;
}

HBRUSH AdvancedSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// 对特定的控件做修改
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255,255,0)); //设置字体颜色
		pDC->SetBkMode(TRANSPARENT);//设置背景为透明
		static HBRUSH B = (HBRUSH)GetStockObject(NULL_BRUSH);//获取画笔颜色混合后的画笔，完成透明
		return B; //返回画刷句柄
	}
	return CDialog::OnCtlColor(pDC,pWnd, nCtlColor);
}

BOOL AdvancedSet::OnInitDialog(){
	UpdateData(FALSE);
	CDialog::OnInitDialog();
	return TRUE;
}

int  AdvancedSet::check_redo(int check,int redo)
{
	static int lastWT, lastEC;
	static double lastPE, lastAB;
	static double r[ratio_num];
	static double lastBR;
	int i;
	if(check==0 && redo==0){		//都为0
		lastWT = waitTime, lastEC = EvoCol;
		lastPE = permitError, lastAB = absolut;
		lastBR = balance_ratio;
		for(i=0;i<ratio_num;++i)
			r[i] = ratio[i];
	}else if(check){				//check不为0
		if( lastWT !=waitTime || lastPE !=permitError || lastEC != EvoCol  || lastAB != absolut || lastBR != balance_ratio)
			return 1;
		for(i=0;i<ratio_num;i++)
			if(fabs(r[i]-ratio[i])>1e-3)
				return 1;
		return 0;
	}else{							//redo不为0
		waitTime = lastWT;
		EvoCol = lastEC;
		permitError = lastPE;
		absolut = lastAB;
		balance_ratio = lastBR;
		for(i=0;i<ratio_num;i++)
			ratio[i] = r[i];
		UpdateData(0);
		RedrawWindow();
	}
	return 0;
}

void AdvancedSet::OnOK(){
	check_redo(0,0);
	if(UpdateData(1)==0){
		check_redo(0,1);
		return;
	}
	if(check_redo(1,0)){
		int re = MessageBox("检测到输入了新值，是否修改数据？","确认修改",MB_YESNOCANCEL);
		if(re==IDCANCEL){
			check_redo(0,1);
			return;
		}else if(re==IDNO){
			MessageBox("未做出任何修改","返回提醒");
			check_redo(0,1);
		}else if(re==IDYES)
			MessageBox("数据修改成功","返回提醒");
	}else
		MessageBox("未做出任何修改","返回提醒");
	CDialog::OnOK();
}

void AdvancedSet::OnHelpAbs()
{
	MessageBox("在误差允许的一定范围内，该系数越接近1则把不匹配判为匹配的可能性越小，匹配判为不匹配可能性越大；反之亦然","偏向性系数");
}

void AdvancedSet::OnBnClickedhelpbalance()
{
	MessageBox("控制演化过程中正负训练集在个数相差较大时平衡误差的系数。该值为1则表示使正负训练集占有同等影响力，为0表示按原有个数计算误差！","正负平衡系数",1);
}

void AdvancedSet::OnEnChangeRatio()
{
	double ra[ratio_num];
	CString temp;
	GetDlgItem(IDC_ratio1)->GetWindowText(temp);
	ra[0] = _ttof(temp.GetBuffer());
	GetDlgItem(IDC_ratio2)->GetWindowText(temp);
	ra[1] = _ttof(temp.GetBuffer());
	GetDlgItem(IDC_ratio3)->GetWindowText(temp);
	ra[2] = _ttof(temp.GetBuffer());
	GetDlgItem(IDC_ratio4)->GetWindowText(temp);
	ra[3] = _ttof(temp.GetBuffer());
	GetDlgItem(IDC_ratio5)->GetWindowText(temp);
	ra[4] = _ttof(temp.GetBuffer());
	GetDlgItem(IDC_ratio6)->GetWindowText(temp);
	ra[5] = _ttof(temp.GetBuffer());
	ra[ratio_num-1] = 1;
	for(int i=0;i<ratio_num-1;i++)
		ra[ratio_num-1] -= ra[i];
	temp.Format(_T("%.3g"),ra[ratio_num-1]);
	GetDlgItem(IDC_ratio7)->SetWindowText(0);
	GetDlgItem(IDC_ratio7)->SetWindowText(temp);
	RedrawWindow();
}
