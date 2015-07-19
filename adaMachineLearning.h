/** 智能自适应机器学习矩阵模版
	最后编辑时间: 2014.6.8  22:00
    文档说明: 对需要处理的数据集进行演化,以找到合适的路径实现机器学习
    变量含义: mlDimension——进入模板的数据维数;num_per_col——模板每列个数,
        根据Dimension的增大自动增长;chromlength——个体长度,即路径长度;
        popsize——同时进行演化的种群大小；MaxChromLength——最大个体长度;
        MaxGen——最大演化代数;Exp——误差计算指数;dataLen数据集个数(长度);
        bestChrom——最好个体;peout——个体在通过路径后的输出值;data——数据集
    本次更新内容:
		(6.08)改为C++类的形式封装
		(5.14)更改需求,以适应不同情况下的机器学习
		(4.10)创建多线程,在出现询问对话框时后台仍然演化
		(3.24)解决演化速度有时过慢,无法由用户决定停止的问题
    @Author: 闻波(webary)
**/
#ifndef  _ADAMACHINELEARNING_H_
#define  _ADAMACHINELEARNING_H_

#include <iostream>
#include <cstdlib>		//rand()函数
#include <cmath>		//数学函数
#include <windows.h>
#include <ctime>
#include <string>		//getline()函数
#include <fstream>		//getline()函数
#include <process.h>	//_beginthread()函数
using namespace std;
typedef const char cchar;

#ifdef __AFXWIN_H__
extern int testOut[100];
extern char ml_info[10000];
#endif

class Ada_ML {
#ifdef __AFXWIN_H__
	CStatusBar *pS;
#endif
    int mlDimension, num_per_col, col, chromlength;
    const static int MaxDimension = 150;	//定义最大数据维数为150维
	const static int MaxCol = 25;
    const static int popsize = 10, MaxChromLength = (MaxDimension+23)/24*8*3*MaxCol+3;
	const static int MaxGen = 300000;
	const static int Exp = 2;	//计算指数
	int best;	//最好个体的索引
	int dataLen,bestChrom[MaxChromLength], **peout;	//数据集个数,最好个体,模板输出数组
	int artiTimes, mustEqual;	//人工打分次数,是否必须相等
	int data[500][151], dataTest[500][150];	//数据集，测试集
	double f,cr,absolut;	//变异系数,交叉因子,偏向性系数:absolut越大,结果为1(匹配)的概率越小
    struct individual {
        int chrom[MaxChromLength];
        double fitness;
    } pop[popsize], cBest;
	struct N_P
	{
		int n, p;			//分别表示正、负训练集个数
		double n_err, p_err;//分别表示正、负训练集被判错的个数（平衡之后的）
		double n_div_p,		//负训练集个数除以正训练集
			balance_ratio;	//平衡系数,标记求取适应值时使正负训练集平衡的系数
	}n_p;
    bool b_saveTmp, b_savePre, b_saveBest;//记录是否保存所有个体的误差，保存当前最好路径，保存最好路径
public:
	double wait,permitError;	//等待时间
	inline individual& getBest(){ return cBest;} 
	inline int& getCol(){ return col; }
	inline int getChromlen()const{ return chromlength; }
    Ada_ML(int MLDimension=5,int _mustEqual=1,double _absolut=.95,int _col=6,const int* p_artiTimes=0);
	void setML(int MLDimension=1,int _mustEqual=1,double _absolut=.95,int _col=6,const int* p_artiTimes=0);
    void setModel(double _absolut=.75,int _mustEqual=1,const int* p_artiTimes=0);
    void setWait_Col(double wt,int _col=0);
    int  saveToArr(const char *filePath,int* dataarr,int Dimension,bool strict = true);
    void init(const char *readFromFile = 0,bool b_saveData = false);
    int  getPEOut(individual * indiv, int *datarow);
    void fitnessvalue(individual * indiv, int ii);
    int  findbest();
    void demute(int i);	//最少需要3个个体
    void startLearn(int len,double permit_error=8,double balance_r=.5,const char *readFromFile=0,bool b_saveData=false);
#ifdef __AFXWIN_H__
    void startLearn(int len,CStatusBar *p,double permit_error=8,double balance_r=.5,cchar* readFromFile="",bool b_saveData=false);
#endif
    int  loadDataFromFile(const char* fileName, int len = -1, double transFrom1=255, double transFrom2=255);
	int  loadTestData(string fileName, int len, double transFrom);
    void getTestOut(string fileName, int len = -1, double transFrom=255);
    template<class T1,class T2>						//转化模版————归一化函数
    T1 trans(T2 num,T2 max_T2,T1 stand) {
        return  T1((double)num / max_T2 * stand + .5);
    }
    void setSave(bool saveBest = 0,bool saveTmp = 0,bool savePre = 0){
		b_saveBest = saveBest, b_saveTmp = saveTmp, b_savePre = savePre;
    }
};

#endif // _ADAMACHINELEARNING_H_
