/** 智能自适应机器学习矩阵模版
	最后编辑时间: 2014.8.14  20:00
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
#include "stdafx.h"
#include "adaMachineLearning.h"

#ifndef  _ADAMACHINELEARNING_H_VAR_
#define  _ADAMACHINELEARNING_H_VAR_
char ml_info[10000];
#endif

#ifdef __AFXWIN_H__
int testOut[100];
#endif

#define RAND_DOUBLE (double)rand() / RAND_MAX
int flag_stop = -1;

Ada_ML::Ada_ML(int MLDimension,int _mustEqual,double _absolut,int _col,const int* p_artiTimes) {
    peout = new int*[popsize];
    artiTimes = mustEqual = dataLen = 0;
    mlDimension = MLDimension;
    num_per_col = (mlDimension + 23) / 24 * 8;
	col = _col;
	chromlength = col * 3 * num_per_col + 3;
	memset(data,-1,sizeof(data));
	memset(dataTest,-1,sizeof(dataTest));
    setModel(_absolut,_mustEqual,p_artiTimes);
    b_saveTmp = 0, b_savePre = 0, b_saveBest = 0;
    wait = 5000;
}
void Ada_ML::setML(int MLDimension,int _mustEqual,double _absolut,int _col,const int* p_artiTimes){
    mlDimension = MLDimension;
	num_per_col = (mlDimension + 23) / 24 * 8;
	col = _col;
	chromlength = col * 3 * num_per_col + 3;
    setModel(_absolut,_mustEqual,p_artiTimes);
}
void Ada_ML::setModel(double _absolut,int _mustEqual,const int* p_artiTimes) {
    if(_mustEqual) {
        mustEqual = _mustEqual;
        absolut = _absolut;
    }
    if(p_artiTimes)
        artiTimes = *p_artiTimes;
}
void Ada_ML::setWait_Col(double wt,int _col) {
	wait = wt*1000;
	col = _col?_col:col;
	chromlength = col * 3 * num_per_col + 3;
}
void Ada_ML::init(cchar *readFromFile,bool b_saveData) {
    // readFromFile指向读入一条预存路径的文件
    // b_saveData记录是否保存需要处理的数据集
    int i ,j;
	static int first = 1;
	for(i=0; i<(first?popsize:popsize-1); i++) {
		memset(&pop[i],0,sizeof(pop[i]));
        if(i==0 && readFromFile) {
            if(saveToArr(readFromFile,(int*)bestChrom,chromlength,false))
                for (j=0; j<chromlength; j++)
                    pop[i].chrom[j] = bestChrom[j];
            else {		//如果文件没找到,则不从文件中得读取
                readFromFile = 0;
                i--;
            }
        } else{
            for (j=0; j<chromlength; j++)
                if((j+1)%3 && j<3*num_per_col)
                    pop[i].chrom[j]=rand() % mlDimension;
                else
                    pop[i].chrom[j]=rand() % num_per_col;
        }
    }
	//在后面的演化中把最后一个个体改为前一次演化后的最好值
	if(first)
		first = 0;
	else
		pop[popsize-1] = cBest;
    if(b_saveData) {
        FILE * fp = fopen("data.txt","w");
        for(i = 0; i < dataLen; i++) {
            for(j = 0; j < mlDimension; j++)
                fprintf(fp,"%d ",data[i][j]);
            fprintf(fp,"\t%d\n",data[i][j]);
        }
        fclose(fp);
    }
}
int  Ada_ML::getPEOut(individual * indiv, int *datarow) {
    int x, y, op, rowindex=0, peoutput=0, j, jj;
    int** PE = new int*[num_per_col];
    for(j=0; j<num_per_col; j++)
        PE[j] = new int[col];
    for(j = 0; j < chromlength-3; j+=3) {
        if(j%(3*num_per_col)==0) {	//一列处理完后进入下一列
            rowindex = 0;
            jj = j / (3*num_per_col);
        }
        if(jj==0) {	//才进入模版时从输入数组中取数据
            x = datarow[indiv->chrom[j]];
            y = datarow[indiv->chrom[j+1]];
        } else {	//从前一列取数据填充当前列
            x = PE[indiv->chrom[j]][jj-1];
            y = PE[indiv->chrom[j+1]][jj-1];
        }
        op = indiv->chrom[j+2];
        switch (op%8) {
        case 0:
            PE[rowindex++][jj]=x>>1;//x
            break;
        case 1:
            PE[rowindex++][jj]=(x+y)>>1;
            break;
        case 2:
            PE[rowindex++][jj]=x+y;//
            break;
        case 3:
            PE[rowindex++][jj]=max(x,y);
            break;
        case 4:
            PE[rowindex++][jj]=min(x,y);
            break;
        case 5:
            PE[rowindex++][jj]=x<<1;
            break;
        case 6:
            PE[rowindex++][jj]=y<<1;
            break;
        case 7:
            PE[rowindex++][jj]=y>>1;//
            break;
        }
        if (PE[rowindex-1][jj]>255)	PE[rowindex-1][jj]%=255;
    }
    rowindex=0;
    x=PE[indiv->chrom[j++]][col-1];
    y=PE[indiv->chrom[j++]][col-1];
    op=indiv->chrom[j];
    switch (op%8) {
    case 0:
        peoutput=x>>1;//x
        break;
    case 1:
        peoutput=(x+y)>>1;
        break;
    case 2:
        peoutput=x+y;//(x+y+1)>>1;
        break;
    case 3:
        peoutput=max(x,y);
        break;
    case 4:
        peoutput=min(x,y);
        break;
    case 5:
        peoutput=x<<1;
        break;
    case 6:
        peoutput=y<<1;//x+y;
        break;
    case 7:
        peoutput=y>>1;//y
        break;
    }
    if (peoutput>255)  peoutput%=255;
    for(j=0; j<num_per_col; j++)
        delete[] PE[j];
    delete[] PE;
    return mustEqual ? peoutput>=absolut*255 : peoutput;
}
void Ada_ML::fitnessvalue(individual * indiv, int ii) {	// ii < popsize
    indiv->fitness = 0;
	n_p.p_err = n_p.n_err = 0;
    for(int row = 0; row < dataLen; row++) {
        peout[ii][row] = getPEOut(indiv, &data[row][0]);
		if(mustEqual) {
			//正负训练集个数不相等时根据平衡系数确定权重
			if(peout[ii][row]==0 && data[row][mlDimension]==1)		//应该输出1模版却输出0,即正训练集被判错
				n_p.p_err += ((n_p.n_div_p-1)*n_p.balance_ratio+1) * abs(peout[ii][row]-data[row][mlDimension]);
			else if(peout[ii][row]==1 && data[row][mlDimension]==0)//应该输出0模版却输出1,即负训练集被判错
				n_p.n_err += abs(peout[ii][row]-data[row][mlDimension]);
		}else
            indiv->fitness += fabs(pow(double(peout[ii][row]-data[row][mlDimension]),Exp));
    }
	if(mustEqual)
		indiv->fitness = n_p.p_err*n_p.p_err + n_p.n_err*n_p.n_err;
}
int  Ada_ML::findbest() {
    int tag_best = 0, i;
    for(i = 1; i < popsize; i++)
        if(pop[i].fitness < pop[tag_best].fitness)
            tag_best = i;
    cBest= pop[tag_best];
    return tag_best;
}
void Ada_ML::demute(int i) {	//最少需要3个个体
    if(i>=popsize) return;
    int dis=0,j,j1,j2;
    int r1=rand() % popsize, r2=rand() % popsize;
    while(r1==i)
        r1=rand() % popsize;
    while(r1==r2||r2==i)
        r2=rand()%popsize;
    individual tmp=pop[i];
    for (j=0; j<chromlength; j++)
        if (pop[r1].chrom[j]!=pop[r2].chrom[j])
            dis++;
    int s = int(floor(f * dis));
    int *flag1 = new int[s+1];
    for (j=0; j<s; j++)
        flag1[j] = rand()%chromlength;
    for(j2=0; j2<s; j2++)		//判重
        for (j1=0; j1<s; j1++)
            while(flag1[j2]==flag1[j1]&&j2!=j1) {
                flag1[j2]=rand()%chromlength;
                j1=-1;
            }
    for (j1=0; j1<s; j1++) {
        double p=RAND_DOUBLE;
        if (p <= cr) {
            if ((flag1[j1]+1)%3 && flag1[j1] < 3*num_per_col)
                tmp.chrom[flag1[j1]]=rand() % mlDimension;
            else
                tmp.chrom[flag1[j1]]=rand() % num_per_col;
        }
    }
	delete[] flag1;
    int *tmpPEOut = new int[dataLen];
    memcpy(tmpPEOut,peout[i],dataLen*sizeof(int));
    fitnessvalue(&tmp,i);
    if (tmp.fitness < pop[i].fitness)
        pop[i]=tmp;
    else {
        memcpy(peout[i],tmpPEOut,dataLen*sizeof(int));
        f = RAND_DOUBLE;////(double)10;
        cr = RAND_DOUBLE;
    }
	delete[] tmpPEOut;
}
//创建工作者线程进行后台计算和判断
UINT waiting(void* tt){
	if(flag_stop==1)
		return 0;
    flag_stop = 0;
    if(MessageBox(AfxGetMainWnd()->m_hWnd,"此次演化时间稍长，是否现在停止演化？","温馨提示",MB_YESNO|MB_ICONQUESTION)==IDYES)
        flag_stop = 1;
    else{
        flag_stop = -1;
		*(int*)tt = clock();
	}
	return 0;
}
void Ada_ML::startLearn(int len,double permit_error,double balance_r,cchar *readFromFile,bool b_saveData) {
    if(artiTimes && dataLen==artiTimes)
		return; //若再次调用该函数时数据个数没变则直接返回
	dataLen = len;
	permitError = permit_error>=0 ? permit_error : 8;
	n_p.balance_ratio = balance_r;
	if(dataLen==0)
	{
#ifndef  __AFXWIN_H__
		cout<<"\r数据集（训练集）个数设置不正确，请检查从文件载入是否不正确！"<<endl;
#else
		MessageBox(AfxGetMainWnd()->m_hWnd,"数据集（训练集）个数设置不正确，请检查从文件载入是否不正确！","模板演化出错",0);
		pS->SetPaneText(0,_T("模板演化出错--错误代码为0001"));
		return;
#endif
	}
    int i, gen = 0;
    for(i = 0; i < popsize; i++)
        peout[i] = new int[dataLen];
    FILE * fp, *fptmp, *fppre, *fpbest;
    if(b_saveTmp) fp = fopen("tmp.txt","w");
    srand((unsigned)time(NULL));
    init(readFromFile,b_saveData);
    for(i = 0; i < popsize; i++)
        fitnessvalue(&pop[i],i);
    findbest();
    f = RAND_DOUBLE,cr = RAND_DOUBLE;
    double ind_dif;
#ifndef  __AFXWIN_H__
    cout<<"\r正在进行模板演化以查找最好路径..."<<endl;
#else
	pS->SetPaneText(0,_T("已开始演化过程，请耐心等待..."));
#endif
    long t1 = clock(),tt1 = t1;		//t1控制停顿时间间隔，tt1控制计算演化频率的时间间隔
	flag_stop = -1;
    while (gen++<MaxGen) {
        for (i=0; i<popsize; i++)
            demute(i);
        best = findbest();
        if(clock()-tt1>200 || gen==1) {
			tt1 = clock();
            if(mustEqual)
                ind_dif = sqrt(cBest.fitness) / (n_p.p * n_p.n_div_p * n_p.balance_ratio + n_p.n) * 100;
            else
                ind_dif = pow(cBest.fitness / dataLen,1.0 / Exp) / 2.55;
#ifndef  __AFXWIN_H__
            cout<<"\r\t\t\t\t\t\r已演化至第 "<<gen<<" 代，平均误差为："<<ind_dif<<"   \b\b%%";
#else
			sprintf(ml_info,"已演化至第 %d 代，平均误差为：%.3g %%",gen,ind_dif);
			pS->SetPaneText(0,_T(ml_info));
			Sleep(1);
			MSG  msg;
			if (PeekMessage(&msg,(HWND)NULL, 0, 0, PM_REMOVE))
				::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
#endif
            if(b_saveTmp) {
                fptmp = fopen("tmp.txt","a");
                for(i = 0; i < dataLen; i++)
                    fprintf(fptmp,"%-4d ",peout[best][i]-data[i][mlDimension]);
                fprintf(fptmp,"best = %d\n",best);
                fclose(fptmp);
            }
            if(b_savePre) {
                fppre = fopen("present.pth","w");
                for(i = 0; i < chromlength; i++)
                    fprintf(fppre,"%-4d",cBest.chrom[i]);
                fprintf(fppre,"%.2lf\n",ind_dif);
                fclose(fppre);
            }
            if(ind_dif < permit_error+1e-5)
                break;
			//flag_stop标识用户点击了对话框的哪个值，1表示确定，-1表示取消
            if(clock()-t1>wait && flag_stop==-1)
				AfxBeginThread(waiting,&t1);	//MFC专用线程函数
            if(flag_stop==1)
				break;
		}
		if(gen%300==0)
			Sleep(1);
    }
	flag_stop=1;
#ifdef  __AFXWIN_H__
	sprintf(ml_info,"演化完成 (已演化至第 %d 代，平均误差为：%.3g %%)",gen,ind_dif);
	pS->SetPaneText(0,_T(ml_info));
#endif
#ifndef  __AFXWIN_H__
    cout<<endl<<endl;
    for(i = dataLen; i < 20; i++) {
    	if(i==dataLen)
			cout<<"模版输出结果"<<"\t数据对比结果"<<"\t对比误差\n";
        cout<<"     "<<getPEOut(&cBest,&data[i][0])<<"\t\t     "<<data[i][mlDimension];
        cout<<"\t\t   "<<getPEOut(&cBest,&data[i][0])-data[i][mlDimension]<<endl;
    }
#endif
    if(b_saveBest) {
        fpbest = fopen("best.pth","w");
        fseek(fpbest,0,0);
        for(i = 0; i < chromlength; i++)
            fprintf(fpbest,"%-4d",cBest.chrom[i]);
		if(mustEqual)
			fprintf(fpbest,"\t%.3lf\n%d\n",ind_dif,chromlength);
		else
			fprintf(fpbest,"\n%d\t%.3lf\n",chromlength,pow(cBest.fitness / dataLen,1.0 / Exp));
        fclose(fpbest);
    }
    if(b_saveTmp) fclose(fp);
    for(i = 0; i < popsize; i++)
        delete[] peout[i];
}
void Ada_ML::startLearn(int len,CStatusBar *p,double permit_error,double balance_r,cchar * readFromFile,bool b_saveData){
	pS = p;
	startLearn(len,permit_error,balance_r,readFromFile,b_saveData);
}
int  Ada_ML::loadDataFromFile(const char* fileName, int len, double transFrom1, double transFrom2) {
    FILE *fp = fopen(&fileName[0],"r");
    if(fp==0) {
#ifndef __AFXWIN_H__
        cout<<"从文件载入数据集失败!"<<endl;
#else
		MessageBox(AfxGetMainWnd()->m_hWnd,"从文件载入数据集失败!请确认该程序在一个有完全访问权限的位置运行","出错提示",MB_ICONERROR);
#endif
        return -1;
	}
	n_p.n = n_p.p = 0;
	dataLen = 0;
	while(len--) {
		int i=0;
		double temp=0;
		for(i=0; i<mlDimension; i++) {
			if(fscanf(fp,"%lf",&temp)!=1)
				break;
			data[dataLen][i] = (int)(trans(temp,transFrom1,255));
		}
		if(i<mlDimension)
			break;
		if(fscanf(fp,"%lf",&temp)!=1)
			break;
		if(temp<0 || temp>1)
			break;
		if(mustEqual){
			data[dataLen][i] = (int)temp;
			if(fabs(temp-1)<1e-4)	//最后一维为1表示是正训练集
				++ n_p.p;	//正训练集个数加一
			else
				++ n_p.n;	//负训练集个数加一
		}else
			data[dataLen][i] = (int)(trans(temp,transFrom2,255));
		dataLen++;
	}
	fclose(fp);
	n_p.n_div_p = (double)n_p.n / n_p.p;	//负训练集个数除以正训练集个数
	if(fabs(n_p.n_div_p)<1e-5)
		n_p.n_div_p = 1;	//如果负训练集为0时则不考虑平衡系数
	CString ss;
	ss.Format("%d",dataLen);
	//MessageBox(0,ss,0,0);
	return dataLen;
}
int  Ada_ML::loadTestData(string fileName, int len, double transFrom){
    FILE *fp = fopen(&fileName[0],"r");
    if(fp==0) {
#ifndef __AFXWIN_H__
        cout<<"从文件载入测试集失败!"<<endl;
#else
		MessageBox(0,"从文件载入测试集失败!请确认该程序在一个有完全访问权限的位置运行","出错提示",MB_ICONERROR);
#endif
        return -1;
    }
    int testLen = 0;
    while(len--) {
        int i=0;
        double temp;
        for(i=0; i<mlDimension; i++) {
            if(fscanf(fp,"%lf",&temp)!=1){
				fclose(fp);
				return testLen;
			}
            dataTest[testLen][i] = (int)(trans(temp,transFrom,255));
        }
        testLen++;
    }
	fclose(fp);
	return testLen;
}
void Ada_ML::getTestOut(string fileName, int len, double transFrom){
	int testLen = loadTestData(fileName, len, transFrom), res;
	//CString s;s.Format(_T("%d"),testLen);
	//MessageBox(0,s,"测试集长度",0);
    for(int i = 0; i < testLen; i++){
		res = getPEOut(&cBest,&dataTest[i][0]);
#ifdef __AFXWIN_H__
		testOut[i] = res;
#else
    	if(i==0)
			cout<<"测试数据模版输出:"<<endl<<res<<endl;
        else
			cout<<res<<endl;
#endif
    }
}
// 读入数据并存入到整形数组中
int  Ada_ML::saveToArr(const char *filePath,int* dataarr,int Dimension,bool strict) {
    ifstream in(filePath);			//strict指定读入时是否为严格模式：即是否保存一条的所有信息
    char * arr[1000];
    string s;
    int row,i,j;
	unsigned k;
    row = i = j = k = 0;
	if (!getline(in,s)) {
#ifndef __AFXWIN_H__
        cout<<"\n文件读取失败,请检查文件"<<filePath<<"或数据是否存在!\n"<<endl;
#endif
        return 0;
    }
    arr[row] = new char[2000];					//此处字符数组长度可随时调整
    strcpy(arr[row++],&s[0]);
    while (getline(in,s))
        if(s[0]) {
            arr[row] = new char[2000];
            strcpy(arr[row++],&s[0]);
        } else break;
    int n1 = row, t = 0;
    int value = 0, flag = 0;
    for (j = 0; j < row; j++) {					//将字符串数组的数据转换保存在整形数组中
        for (k = 0,i = 0; k <= strlen(arr[j]); k++) {
            if (k < strlen(arr[j]) && arr[j][k] >= '0' && arr[j][k] <= '9') {
                value = 10 * value + arr[j][k] - '0';
                flag = 1;						// 1 代表当前位为数字
            } else {
                if(!flag)	continue;
                if (i == Dimension && arr[j][k] >= '0' && arr[j][k] <= '9') {
					if(strict) {
#ifndef __AFXWIN_H__
                        cout<<"数据维数设置与文件数据维数不符!\
							   \r请检查文件数据或Dimension的值!\n"<<endl;
                        system("pause");
                        exit(0);
#endif
						return 0;
                    } else
                        break;
                }
                dataarr[i++] = value;
                value = 0;
                flag  = 0;						// 0 代表当前位非数字
            }
        }
    }
    for(j = 0; j < row; j++)
        delete[] arr[j];
    char filename[31] = "re_";
    FILE * fp;
    bool b_saveData = false;
    if(b_saveData) {
        strcat(filename,filePath);
        strcat(filename,".txt");
        fp = fopen(filename,"w+");			//保存读入的数据到txt
        for(i = 0; i < n1; i++) {
            for(j = 0; j < Dimension - 1; j++)
                fprintf(fp,"%-6d",dataarr[j]);
            fprintf(fp,"%d\n",dataarr[j]);
        }
        fclose(fp);
    }
    return 1;
}
/**用法:
    Ada_ML Ada_ML(4,1,1);
    Ada_ML.loadDataFromFile("t.txt",-1,1,1);
    Ada_ML.setWait(4);
    Ada_ML.startLearn(14,1);
    Ada_ML.getTestOut("tt.txt",-1,1);
**/

//#endif // _ADAINTERACTION_H_
