// imgMatchView.h : interface of the CImgMatchView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGMATCHVIEW_H__1407C906_F5BC_4B19_9AEC_CA93337B164B__INCLUDED_)
#define AFX_IMGMATCHVIEW_H__1407C906_F5BC_4B19_9AEC_CA93337B164B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<cv.h>
#include<highgui.h>
#include "AdvancedSet.h"
#include "MyBitamp.h"
#include "targetMatch.h"
#include "adaMachineLearning.h"

class CImgMatchView : public CFormView
{
protected: // create from serialization only
	CImgMatchView();
	DECLARE_DYNCREATE(CImgMatchView)

public:
	enum{ IDD = IDD_IMGMATCH_FORM };

	CImgMatchDoc* GetDocument();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

	virtual ~CImgMatchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	//改变窗口背景图片
	DECLARE_MESSAGE_MAP()
public:
	RECT r;						
    CMyBitmap m_bmpBackground;	//创建画刷变量
	CStatusBar *pStatues;		//状态栏指针
	AdvancedSet Dlg;			//高级设置对象
	static const int log_num = 10;//操作记录数
	CString log, logg[log_num]	//保存操作历史记录
		,filePath1, filePath2	//对比图1和对比图2的路径
		,EvoPath				//演化后的路径
		,matchlog				//匹配日志，用于导出
		,tmp;					//保存一些临时字符串
	struct {
		CString path[10];
		int n;	//当前模板个数
	}model;		//模板图结构
	struct {
		CString p[1000], n[1000];
		int p_num, n_num;
		bool done_p[1000],done_n[1000];
	}train;		//训练集结构
	struct {
		CString p[1000];
		int num;
		int times[10];
		bool done[1000];
	}test;		//测试集结构
	int Evolution;	//当前演化状态
	IplImage * img1,* img2, *tmpimg;//对比图1,2的图像指针
	Ada_ML ml;	//机器自学习模板类对象
	Target t,t2,t3,t4;
	MSG msg;	//全局消息变量，用于更新消息队列
	_CrtMemState cc1, cc2, cc3;//检查内存是否泄漏
	HANDLE hLogFile;//声明日志文件句柄
	bool runing;	//标记当前是否正在进行运算
	int resetP_N;	//标记是否重置了训练集
	double ratio[20];	//当前演化中用到的各特征比例系数
	CString DragImgPath[100];	//拖放进来的图片文件列表
	CString DragEvoPath;		//拖放进来的演化路径
	CString DragModelTrainPath;	//拖放进来的模版训练集路径

	CMainFrame * pMF;	//指向主窗口，用于绘制状态栏的进度条
	RECT myr;			//标识状态栏中进度条的矩形区域
	CREATESTRUCT *pstyle;


	void updatelog();	//更新操作记录
	afx_msg void OnFileopen1();	//打开对比图1
	afx_msg void OnFileopen2();	//打开对比图2
	afx_msg void OnShowObj();	//显示对比图2
	afx_msg void OnMatchAll();	//将对比图1与对比图2对比（所有特征）
	afx_msg void OnSaveMatch();	//保存对比图1与对比图2对比surf角点检测后的匹配图
	afx_msg void OnClearLog();	//情空操作记录
	afx_msg void OnMatchSurf();	//匹配对比图1与对比图2的surf角点
	afx_msg void OnClearAll();	//重置所有配置
	afx_msg void OnTrainP();	//载入正训练集
	afx_msg void OnTrainN();	//载入负训练集
	afx_msg void OnShowTrainP();//显示正训练集
	afx_msg void OnShowTrainN();//显示负训练集
	afx_msg void OnClearP();	//情空正训练集
	afx_msg void OnClearN();	//情空负训练集
	afx_msg void OnShowModel();	//显示模板图
	afx_msg void OnShowObj1();	//显示对比图1
	afx_msg void OnShowObj2();	//显示对比图2
	afx_msg void OnShowTest();	//显示测试集
	afx_msg void OnStartEvolution();//开始演化
	afx_msg void OnAddTest();//增加测试集
	afx_msg void OnClearTest();	//清空测试集
	afx_msg void OnStartMatch();//开始匹配
	afx_msg void OnClearTrain();//清空训练集
	afx_msg void OnUseHelp();	//打开使用帮助
	afx_msg void OnOpenModelMany();//载入多模板
	afx_msg void OnTrainHelp();	//对训练集做出提示
	afx_msg void OnTestHelp();	//对测试集做出提示
	afx_msg void OnAdvancedSet();//调用高级设置界面
	afx_msg void OnSavematch();	//保存匹配结果
	afx_msg void OnSaveEvoPath();//保存演化路径
	afx_msg void OnReadEvoPath();//读取演化路径
	afx_msg void OnUpdateLog();	//打开升级历史记录
	afx_msg void OnReadModelTrain();//读取模板图训练集
	afx_msg void OnSaveModelTrain();//保存模板和训练集
	afx_msg void OnResizeWindow();//更改窗口状态：可更改大小-不可更改大小

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	//对静态文本区域重绘，达到透明效果
	afx_msg void OnEnVscrollLog();
	afx_msg void OnNcLButtonDown(UINT,CPoint);
	afx_msg void OnQuit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

#ifndef _DEBUG  // debug version in imgMatchView.cpp
inline CImgMatchDoc* CImgMatchView::GetDocument()
   { return (CImgMatchDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGMATCHVIEW_H__1407C906_F5BC_4B19_9AEC_CA93337B164B__INCLUDED_)
