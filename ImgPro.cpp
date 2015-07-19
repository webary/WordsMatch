/**
	该头文件使用OpenCV库实现读取图片并保存像素信息、直接输出一幅图片、对图像进行一些常见处理操作。
	@author : webary
	@time (last update time): 2014.9.2  23:00
**/
#include "stdafx.h"
#ifdef __AFXWIN_H__
#include"ImgPro.h"
#else
#include<windows.h>
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<cv.h>
#include<highgui.h>
#include"opencv2/legacy/legacy.hpp"	//openCV 2.3上的目录,支持函数cvCalcPGH
using namespace std;
typedef const char cchar;
typedef const IplImage CIplImage;

#ifndef _IMGPRO_H_VAR_
#define _IMGPRO_H_VAR_
extern int width, height, depth;
extern IplImage *pimg, *pGray;
extern char info[10000];
extern IplImage* find_obj_match;
#endif

IplImage *loadImg(cchar* tmpimgName,int ask=1,int cls=1,int color=1,int show=1);
#ifdef __AFXWIN_H__
IplImage *loadImg(CString tmpimgstring,int ask=1,int cls=1,int color=1,int show=1);
void Screen(char *filename="");
#endif
void saveRGB( IplImage* img = pimg,cchar *fileOutName = 0) ;
void outputImg(cchar* imgOutName,int readAgain = 1,IplImage* img = pimg) ;
void saveIplImage(IplImage* img = pimg);
void saveImg(cchar* imgSaveName,CIplImage* img = pimg);
int showImg(CIplImage* img = pimg,cchar* str = "图片显示",cchar* ss="显示图像出错",int pause=1) ;
void releaseImg(IplImage** ppimg = &pimg) ;
CvSeq* findContour(CIplImage* src,IplImage** dst=0,int th=100,int filled=0) ;
double find_obj(cchar* imgName,cchar* objName,int showInfo=0
                ,int showMatch=0,int showSurf=0);
void matchWeight(CIplImage* img1, CIplImage* img2,double* wm,int show=0,int n=20,int m=20);

void Hilditch(CIplImage* src,IplImage* dst=0,int th=128,int maxIterations=-1);
float Entropy(CIplImage* img=pimg,int th=1,int w=0,int h=0);
#endif

#if !b_multiThread
uchar R[MAXSIZE][MAXSIZE], G[MAXSIZE][MAXSIZE], B[MAXSIZE][MAXSIZE], **A;
IplImage *pimg=0, *pGray=0;
int width=0, height=0, depth=0;
#endif

char info[10000] = "";

//载入图片
#ifndef _MSC_VER
#include<io.h>			//access()
IplImage *loadImg(cchar* tmpimgName,int ask,int cls,int color,int show)
{
    if(tmpimgName==0 || tmpimgName[0]==0)
        return 0;
    char imgName[200], tempName[200];
    strcpy(imgName,tmpimgName);
    int first = 1,flag;
    IplImage * img=0;
    do
    {
        flag = 0;
        if(first==0)
        {
            cout<<"请输入需要载入的图片: ";
            gets(imgName);
        }
        if(access(imgName,4))  							//打开失败,没有读取权限
        {
            strcpy(tempName,imgName);
            if(strstr(tempName,".jpg")==0)  			//文件名没有以.jpg结尾
            {
                strcat(tempName,".jpg");
                if(access(tempName,1))  				//打开失败,没有读取权限
                {
                    strcpy(tempName,imgName);
                    if(strstr(tempName,".png")==0)  	//文件名没有以.png结尾
                    {
                        strcat(tempName,".png");
                        if(access(tempName,1))  		//打开失败,没有读取权限
                        {
                            strcpy(tempName,imgName);
                            if(strstr(tempName,".bmp")==0)  //文件名没有以.bmp结尾
                            {
                                strcat(tempName,".bmp");
                                if(access(tempName,1)==0)
                                    strcpy(imgName,tempName);
                            }
                        }
                        else
                            strcpy(imgName,tempName);
                    }
                }
                else
                    strcpy(imgName,tempName);
            }
            if(access(imgName,1))  						//打开失败,没有读取权限
            {
                if(first)
                {
                    first = 0;
                    flag = 1;
                    continue;
                }
                cout<<"打开图片 \""<<imgName<<"\" 失败 ！"<<endl;
                flag = 1;
            }
        }
        if(access(imgName,1)==0 && (strstr(imgName,".jpg") || strstr(imgName,".bmp") || strstr(imgName,".png")))  						//图片可以读取
        {
            img = cvLoadImage(imgName,color);
            if(cls) system("cls");
            char tips[200] = "已成功载入图片 ";
            strcat(tips,imgName);
            if(show) cout<<tips<<endl;
            if(first && ask && MessageBox(NULL,"是否重新载入其他图片？",tips,MB_YESNO)==IDYES)
            {
                if(cls) system("cls");
                flag = 1;
            }
        }
        first = 0;
    }
    while(flag);
    if(show)
        cout<<"width(宽) : "<<img->width<<"    height(高) : "<<img->height<<endl;
    //    <<"    depth(位深度) : "<<img->depth
    width  = img->width;
    height = img->height;
    depth  = img->depth;
    pimg = img;
    return img;
}
#else
#include"Shlwapi.h"		//PathFileExists()
#pragma comment(lib,"shlwapi.lib")
IplImage *loadImg(cchar* tmpimgName,int ask/*=1*/,int cls/*=1*/,int color/*=1*/,int show/*=1*/)
{
    if(tmpimgName==0 || tmpimgName[0]==0)
        return 0;
    char imgName[200], tempName[200];
    strcpy(imgName,tmpimgName);
    int first = 1,flag;
    IplImage * img=0;
    do
    {
        flag = 0;
        if(first==0)
        {
            CString warn;
            warn.Format(_T("无法载入\"%s\",请先指定正确的图像路径，然后载入图像"),tmpimgName);
            MessageBox(NULL,warn,"载入图像失败",MB_ICONERROR);
            return 0;
        }
        if(!PathFileExists(imgName))  					//打开失败,没有读取权限
        {
            strcpy(tempName,imgName);
            if(strstr(tempName,".jpg")==0)  			//文件名没有以.jpg结尾
            {
                strcat(tempName,".jpg");
                if(!PathFileExists(tempName))  			//打开失败,没有读取权限
                {
                    strcpy(tempName,imgName);
                    if(strstr(tempName,".png")==0)  	//文件名没有以.png结尾
                    {
                        strcat(tempName,".png");
                        if(!PathFileExists(tempName))  		//打开失败,没有读取权限
                        {
                            strcpy(tempName,imgName);
                            if(strstr(tempName,".bmp")==0)  //文件名没有以.bmp结尾
                            {
                                strcat(tempName,".bmp");
                                if(PathFileExists(tempName))//文件可访问
                                    strcpy(imgName,tempName);
                            }
                        }
                        else
                            strcpy(imgName,tempName);
                    }
                }
                else
                    strcpy(imgName,tempName);
            }
            if(!PathFileExists(imgName))  						//打开失败,没有读取权限
            {
                flag = 1;
                if(first)
                {
                    first = 0;
                    continue;
                }
                sprintf(info,"%s打开图片 \"%s\" 失败 ！\r\n",info,imgName);
                MessageBox(NULL,"打开图像失败","出错提示",0);
            }
        }
        if(PathFileExists(imgName) && (strstr(imgName,".jpg") || strstr(imgName,".bmp") || strstr(imgName,".png")))  	//图片可以读取
        {
            img = cvLoadImage(imgName,color);
            if(show)
            {
                char tips[200] = "已成功载入图像文件 ";
                strcat(tips,imgName);
                sprintf(info,"%s%s\r\n",info,tips);
            }
        }
        else
            flag = 1;
        first = 0;
    }
    while(flag);
    if(show)
		sprintf(info,"%swidth(宽) : %d    height(高) : %d\r\n",info,img->width,img->height);
#if !b_multiThread
    width  = img->width;
    height = img->height;
    depth  = img->depth;
	pimg = img;
#endif
    return img;
}
#ifdef __AFXWIN_H__
IplImage *loadImg(CString tmpimgstring,int ask/*=1*/,int cls/*=1*/,int color/*=1*/,int show/*=1*/)
{
    return loadImg((LPSTR)(LPCTSTR)tmpimgstring,ask,cls,color,show);
}
#endif	//__AFXWIN_H__
#endif	//_MSC_VER
//输出图片到本地文件，第二个参数是选择是否再次读取RGB值---非线程安全（readAgain为真时）
void outputImg(cchar* imgOutName,int readAgain/* = 1*/,IplImage* img/* = pimg*/)
{
	if(imgOutName[0]==0 || img==0)
		return;
	if(readAgain)
#if b_multiThread
		MessageBox(0,"已开启多线程，无法使用线程不安全的全局变量","读取全局变量失败",0);
#else
		saveIplImage(img);
#endif
	if (strstr(imgOutName, ".jpg") == 0 && strstr(imgOutName, ".bmp") == 0 && strstr(imgOutName, ".png") == 0)
	{
		char name[200];
		strcpy(name,imgOutName);
		strcat(name,".jpg");
		cvSaveImage(name,img);
	}
	else
		cvSaveImage(imgOutName,img);
}

//保存图片到文件---此函数线程安全（默认readAgain为假）
void saveImg(cchar* imgSaveName,IplImage* img)
{
	outputImg(imgSaveName,0,img);
}

//保存图片像素信息到文件或数组---非线程安全（使用了全局变量RGB）
void saveRGB( CIplImage* img/* = pimg*/,cchar *fileOutName/* = 0*/)
{
    if(img==0)
        return;
    uchar* data = (uchar*)img->imageData;
    if(fileOutName && fileOutName[0])
    {
        FILE *fp1;
        if(fp1 = fopen(fileOutName,"w"))
        {
			if(img->nChannels==3)
			{
				for( int y = 0; y < img->height; y++ )
					for( int x = 0,yy = y * img->widthStep; x < img->width; x++ )
						fprintf(fp1,"%d\t%d\t%d\n",data[yy+3*x+2],data[yy+3*x+1],data[yy+3*x]);
			}
			else if(img->nChannels==1)
			{
				for( int y = 0; y < img->height; y++ )
					for( int x = 0,yy = y * img->widthStep; x < img->width; x++ )
						fprintf(fp1,"%d\n",data[yy+x]);
			}
			fclose(fp1);
        }
    }
    else
	{
#if b_multiThread
		MessageBox(0,"已开启多线程，无法使用线程不安全的全局变量","读取全局变量失败",0);
#else
        if(img->nChannels==3)
        {
            for( int y = 0; y < img->height; y++ )
                for( int x = 0,yy = y * img->widthStep; x < img->width; x++ )
                {
                    R[y][x] = data[yy+3*x+2];
                    G[y][x] = data[yy+3*x+1];
                    B[y][x] = data[yy+3*x];
                }
        }
        else if(img->nChannels==1)
        {
            for( int y = 0; y < img->height; y++ )
                for( int x = 0,yy = y * img->widthStep; x < img->width; x++ )
                    R[y][x] = data[yy+x];
		}
#endif
    }
}

//保存当前的RGB数组到IplImage指针中---非线程安全
void saveIplImage(IplImage* img/* = pimg*/)
{
#if b_multiThread
	MessageBox(0,"已开启多线程，无法使用线程不安全的全局变量","读取全局变量失败",0);
#else
	uchar* data = (uchar*)img->imageData;
	if(img->nChannels==3)
	{
		for( int y = 0; y < img->height; y++ )
			for( int x = 0,yy = y * img->widthStep; x < img->width; x++ )
			{
				data[yy+3*x+2] = R[y][x];
				data[yy+3*x+1] = G[y][x];
				data[yy+3*x]   = B[y][x];
			}
	}
	else if(img->nChannels==1)
	{
		for( int y = 0; y < img->height; y++ )
			for( int x = 0,yy = y * img->widthStep; x < img->width; x++ )
				data[yy+x] = R[y][x];
	}
#endif
}

//得到某一个像素点的灰度值---非线程安全
inline unsigned char getGray(int i,int j)
{
#if b_multiThread
	MessageBox(0,"已开启多线程，无法使用线程不安全的全局变量","读取全局变量失败",0);
	return 0;
#else
	//return (.3*R[i][j]+.6*G[i][j]+.1*B[i][j]+1)/3;
	if(pimg->nChannels==3)
		return (R[i][j]+G[i][j]+B[i][j]+1)/3;
	else
		return R[i][j];
#endif
}

//像素取反---非线程安全
void reverseRGB(int th=255)
{
#if b_multiThread
	MessageBox(0,"已开启多线程，无法使用线程不安全的全局变量","读取全局变量失败",0);
#else
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
		{
			R[i][j] = R[i][j]>th?0 : 255 - R[i][j];
			G[i][j] = G[i][j]>th?0 : 255 - G[i][j];
			B[i][j] = B[i][j]>th?0 : 255 - B[i][j];
		}
#endif
}

//灰度化图像---非线程安全
void grayRGB()
{
#if b_multiThread
	MessageBox(0,"已开启多线程，无法使用线程不安全的全局变量","读取全局变量失败",0);
#else
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
			R[i][j]=G[i][j]=B[i][j] = getGray(i,j);
#endif
}

//控制并滤除偏白（亮度较高）的噪声点---非线程安全
void controlWhtieNoise(int th)
{
#if b_multiThread
	MessageBox(0,"已开启多线程，无法使用线程不安全的全局变量","读取全局变量失败",0);
#else
	//另一种思想：if(R[i][j]>th && G[i][j]>th && B[i][j]>th)
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++){
			if(pimg->nChannels==3) {
				int sum = (int)R[i][j]+G[i][j]+B[i][j];
				if(sum>3*th)
					R[i][j] = G[i][j] = B[i][j] = (uchar)255;
				else
					R[i][j] = G[i][j] = B[i][j] = (uchar)((sum+1)/3);
			}else if(R[i][j]>th)
				R[i][j] = (uchar)255;
		}
#endif
}


//显示图片
int showImg(CIplImage* img/* = pimg*/, cchar* str/* = "图片显示"*/, cchar* ss/*"显示图像出错"*/, int pause/*=1*/)
{
	if(img==0)
	{
#ifdef __AFXWIN_H__
		MessageBox(NULL,"图像指针无效，请先载入有效图像后再使用该功能",ss,MB_ICONSTOP);
#endif
		return 0;
	}
	cvNamedWindow(str);
	cvShowImage(str,img);
	if(pause==1)
	{
		cvWaitKey(0);
		cvDestroyWindow(str);//销毁窗口
	}
	return 1;
}

void showImg(cchar *file)
{
	if(strstr(file,".jpg")==0 && strstr(file,".bmp")==0 && strstr(file,".png")==0)
	{
		char name[200];
		strcpy(name,file);
		strcat(name,".jpg");
		system(name);
	}
	else
		system(file);
}

//释放图片内存指针
void releaseImg(IplImage** ppimg/* = &pimg*/)
{
	if(*ppimg)
	{
		cvReleaseImage(ppimg);
		*ppimg = 0;
	}
}

//从文件夹读取所有图片
void readBMPFromFolder(const char* fileFolder,char readPath[100][300],int del=1)
{
	char str[300] = "DIR /s /b /o:e ";	// /s得到并列文件夹以及子文件夹下所有图片
	strcat(str,fileFolder);
	strcat(str," > fileList.txt");
	system(str);				// 保存该文件夹下所有文件到fileList.txt
	int i = 0,flag = 1;			// flag为文件结束标识;
	freopen("fileList.txt","r",stdin);	//输入重定向至文本文件
	while(1)
	{
		char filePath[200] = " ";
		while(strstr(filePath,".bmp")==0
			&&strstr(filePath,".jpg")==0
			&&strstr(filePath,".png")==0)
			if(gets(filePath)==NULL)
			{
				flag = 0;		// 文件已读完，直接中止
				break;
			}
			if(flag==0) break;
			strcpy(readPath[i++],filePath);	//得到该图像的绝对路径
	}
	freopen("CON", "r", stdin );		//输入重定向至控制台
	if(del)
		remove("fileList.txt");
}

//最大类间方差算法 将灰度图像灰度级分割(注意:传入的需要是灰度图像数据)
int Otsu(CIplImage* src)
{
	int height=src->height, width=src->width, threshold, i, j;
	double histogram[256] = {0};
	for(i=0; i < height; i++)
	{
		uchar* p=(uchar*)src->imageData + src->widthStep * i;
		for(j = 0; j < width; j++)
			histogram[*p++]++;
	}
	int size = height * width;
	for(i = 0; i < 256; i++)
		histogram[i] /= size;
	double avgValue=0, maxVariance=0, w = 0, u = 0;
	for(i=0; i < 256; i++)
		avgValue += i * histogram[i];  //整幅图像的平均灰度
	for(i = 0; i < 256; i++)
	{
		w += histogram[i];
		u += i * histogram[i];
		double t = avgValue * w - u;
		double variance = t * t / (w * (1 - w) );
		if(variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}
	return threshold;
}

//将图片直方图均衡化处理--利用cv库函数--需要释放该指针
IplImage* equalizeHist(CIplImage* img, int saveFile/*=0*/)
{
	if(img==0)
		return 0;
	IplImage* pGray = cvCreateImage(cvGetSize(img),img->depth,1);
	if(img->nChannels==3)
		cvCvtColor(img,pGray,CV_BGR2GRAY);
	else if(img->nChannels==1)
		cvCopy(img,pGray);
	else
	{
		releaseImg(&pGray);
		return 0;
	}
	cvEqualizeHist(pGray,pGray);
	if(saveFile)
		outputImg("均衡化.jpg",0,pGray);
	return pGray;
}

//将图片直方图均衡化处理--利用自编函数--需要释放该指针
IplImage* Histeq(CIplImage* img, int saveFile = 0)
{
	if(img==0)
		return 0;
	IplImage* pGray = cvCreateImage(cvGetSize(img),img->depth,1);
	if(img->nChannels==3)
		cvCvtColor(img,pGray,CV_BGR2GRAY);
	else if(img->nChannels==1)
		cvCopy(img,pGray);
	else
	{
		releaseImg(&pGray);
		return 0;
	}
	double histogram[256] = {0},dTemp;
	int i,j,k;
	uchar* p;
	for(i=0; i < pGray->height; i++)
	{
		p=(uchar*)pGray->imageData + pGray->widthStep * i;
		for(j = 0; j < pGray->width; j++)
			histogram[*p++]++;
	}
	int size = pGray->height * pGray->width;
	for(i = 0; i < 256; i++)
		histogram[i] /= size;
	for(i=0; i < pGray->height; i++)
	{
		p=(uchar*)pGray->imageData + pGray->widthStep * i;
		for(j = 0; j < pGray->width; j++)
		{
			dTemp = 0;
			int gray = getGray(i,j);
			for(k=0; k<gray; ++k)
				dTemp += histogram[k];
			int tar = int(255 * dTemp+.5);
			if(tar<0) tar = 0;
			else if(tar>255) tar = 255;
			*p++ = tar;
		}
	}
	if(saveFile)
		outputImg("均衡化.jpg",0,pGray);
	return pGray;
}

//将图片直方图均衡化处理--结果保存在自身指针中
void Histeq_self(IplImage** ppimg,int save)
{
	IplImage* img = *ppimg;
	saveRGB(img);
	controlWhtieNoise(Otsu(img));//更改RGB：用自动阈值去噪
	reverseRGB();			//取反RGB
	saveIplImage(img);		//将RGB保存到图像指针
	IplImage* gray = equalizeHist(img);	//均衡化
	gray = equalizeHist(gray);//目标是黑色点则再进行一次均衡化
	saveRGB(gray);
	reverseRGB();
	saveIplImage(gray);
	if(save)
		outputImg("histeq",0,gray);
	releaseImg(ppimg);
	*ppimg=gray;
}

//计算图片的轮廓
CvSeq* findContour(CIplImage* src, IplImage** dst, int th, int filled)
{
    int releaseDst = 0;
    if(dst==0)  	// 没有传递二级指针进入该函数
    {
        releaseDst = 1;
        IplImage* ddst = cvCreateImage(cvSize(src->width,src->height),8,1);
        dst = &ddst;
    }
    else if(*dst==0)	// 传递的二级指针没有初始化为图像指针
        *dst = cvCreateImage(cvSize(src->width,src->height),8,1);
    if(src->nChannels==3)
        cvCvtColor(src,*dst,CV_BGR2GRAY);
    else
        cvCopy(src,*dst);
    CvMemStorage *storage = cvCreateMemStorage();
    if(th==-1)
    {
        int getTh(CIplImage*);
        th = getTh(src);
    }
    //cvSmooth(*dst,*dst,CV_MEDIAN,3,3,0,0);		//3x3中值滤波
	cvCanny(*dst,*dst,th,th);
	CvSeq *contour = NULL;
    cvFindContours(*dst,storage,&contour);	        //在二值图像中寻找轮廓
    cvThreshold(*dst,*dst,th,255,CV_THRESH_BINARY);	//对图像进行固定阈值操作
    if(filled)
        cvDrawContours(*dst,contour,cvScalarAll(255),cvScalarAll(200),0,CV_FILLED);
    else
        cvDrawContours(*dst,contour,cvScalarAll(255),cvScalarAll(200),1);
    if(releaseDst)
        releaseImg(dst);
    return contour;
}

//计算图片的7个Hu不变量
double* getHu(CIplImage* src, int th, int printHu, int show, int save, IplImage* dst)
{
    if(src==0)
        return 0;
	int delDst = 0;
    if(dst==0)
	{
        dst = cvCreateImage(cvSize(src->width,src->height),8,1);
		delDst = 1;
	}
    CvSeq *contour = findContour(src,&dst,th);
    CvMoments moments;
    static CvHuMoments hu;          //局部变量作为返回值不安全,故设置为静态变量
    cvMoments(dst,&moments,1);		//计算多边形和光栅形状的最高达三阶的所有矩
    cvGetHuMoments(&moments, &hu);	//计算7个Hu不变量
    cvDrawContours(dst,contour,cvScalar(255),cvScalar(200),1);
    cvReleaseMemStorage(&contour->storage);
    for(int i = 0; i<7; ++i)
        ((double*)&hu)[i] = fabs(log(fabs( ((double*)&hu)[i] )));
    //显示7个矩(取对数后的结果)
    if(printHu)
    {
        cout<<setprecision(4);
        for(int i = 0; i<7; ++i)
            cout<<((double*)&hu)[i]<<"\t";
        cout<<"\n";
    }
    //显示图片
    if(show)
        showImg(dst,"dst");
    //保存图片
    if(save)
        outputImg("轮廓.jpg",0,dst);
    if(delDst)
		cvReleaseImage(&dst);
    return (double*)&hu;
}

//保存得到的Hu矩值
void saveHu(const double* hu,cchar* path,cchar* file = "hu.txt",int th=-1)
{
    FILE* fp = fopen(file,"a");
    fprintf(fp,"%-16s",path);
    for(int i=0; i<7; i++)
        fprintf(fp,"%.2lf\t",hu[i]);
    if(th != -1)
        fprintf(fp,"阈值: %d",th);
    fprintf(fp,"\n");
    fclose(fp);
}

//计算两个Hu矩的距离
double comHu(CIplImage* src, CIplImage* dst, int th = 100)
{
    return cvMatchShapes(findContour(src,0,th),findContour(dst,0,th),CV_CONTOURS_MATCH_I3);
}
double comHu(const CvSeq *contour1,CIplImage* dst,int th=100)
{
    return cvMatchShapes(contour1,findContour(dst,0,th),CV_CONTOURS_MATCH_I3);
}
double comHu(const double* h1,const double* h2)
{
    if(h1==0 || h2==0)
        return 0;
    double dif = 0;
    for(int i=0; i<7; i++)
        dif += fabs(((h1[i]-h2[i]))/ h1[i]);
    return dif;
}

//通过调节滑块得到最好阈值
bool first;
CIplImage* p_image=NULL;
void on_trackbar(int g_thresh)     //回调函数
{
    static IplImage* p_gray=0;
    if(first)
        p_gray = cvCreateImage(cvGetSize(p_image),8,1);
    CvSeq* contours=findContour(p_image,&p_gray,g_thresh);
    cvZero(p_gray);
    if(contours)
        cvDrawContours(p_gray,contours,cvScalarAll(255),cvScalarAll(200),1);//画轮廓
    cvShowImage("调整阈值",p_gray);
    first = 0;
    cvReleaseImage(&p_gray);
}

int getTh(CIplImage* src)
{
    first = 1;
    p_image = src;
    int g_thresh=100;    //初始阈值
    cvNamedWindow("调整阈值",1);
    cvCreateTrackbar("threshold","调整阈值",&g_thresh,255,on_trackbar);
    on_trackbar(g_thresh);
    cvWaitKey();
    return g_thresh;
}

//计算轮廓的pair-wise几何直方图
CvHistogram* gesCalcContoursPGH(const CvSeq* contour)
{
    CvContour* tempCont = (CvContour*)contour;//得到成对几何直方图第二个维度上的范围
    cvBoundingRect(tempCont, 1);
    int sizes[2] = {60, 200};
    float ranges[2][2] = {{0,3.14159f}, {0,200}};
    float** rangesPtr = new float* [2];
    rangesPtr[0] = ranges[0];
    rangesPtr[1] = ranges[1];
    //初始化几何直方图
    CvHistogram* hist = cvCreateHist(2, sizes, CV_HIST_ARRAY, rangesPtr, 1);
    //计算轮廓的成对几何直方图
    cvCalcPGH(contour, hist);//计算轮廓的 pair-wise 几何直方图
    delete []rangesPtr;
    return hist;
}

//对轮廓的pair-wise几何直方图进行匹配
double getMatchContoursPGH(const CvSeq* contour1, const CvSeq* contour2)
{
    //得到轮廓的成对几何直方图
    CvHistogram* hist1 = gesCalcContoursPGH(contour1);
    CvHistogram* hist2 = gesCalcContoursPGH(contour2);
    //归一化直方图
    cvNormalizeHist(hist1,1);
    cvNormalizeHist(hist2,2);
    //直方图匹配
    double result = cvCompareHist(hist1, hist2, CV_COMP_INTERSECT);
    //释放内存
    cvReleaseHist(&hist1);
    cvReleaseHist(&hist2);
    return result;
}

// 将一幅图缩放和旋转变换后保存到另一幅图中
void imRotate(CIplImage *src,IplImage *&dst,double angle,double scale=1,CvPoint2D32f center=cvPoint2D32f(-1,-1))
{
    if(dst==0)
    {
        return;
    }
    if(src->width == dst->width && src->height == dst->height
            &&src->depth == dst->depth &&src->nChannels == dst->nChannels)
    {
        CvMat *mapMatrix = cvCreateMat(2,3,CV_32FC1);
        if(center.x==-1 && center.y==center.x)
            center = cvPoint2D32f(src->width/2,src->height/2);
        cv2DRotationMatrix(center,angle,scale,mapMatrix);//计算二维旋转的仿射变换矩阵
        cvWarpAffine(src,dst,mapMatrix);    //对图像做仿射变换
    }
    else
#ifndef __AFXWIN_H__
        cout<<"两幅图类型或规格不一致,无法进行转化"<<endl;
#else
        sprintf(info,"%s两幅图类型或规格不一致,无法进行转化\r\n",info);
#endif
}

//下面几个函数一起完成surf角点检测功能，并返回匹配对比例数(外部调用find_obj函数)
//比较surf特征
double compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
    double total_cost = 0;
    assert( length % 4 == 0 );
    for( int i = 0; i < length; i += 4 )
    {
        double t0 = d1[i  ] - d2[i  ];
        double t1 = d1[i+1] - d2[i+1];
        double t2 = d1[i+2] - d2[i+2];
        double t3 = d1[i+3] - d2[i+3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if( total_cost > best )
            break;
    }
    return total_cost;
}
//找到单纯最近邻近点
int naiveNearestNeighbor( const float* vec, int laplacian,const CvSeq* model_keypoints,const CvSeq* model_descriptors )
{
    int length = (int)(model_descriptors->elem_size/sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq( model_keypoints, &kreader, 0 );
    cvStartReadSeq( model_descriptors, &reader, 0 );

    for( i = 0; i < model_descriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        if( laplacian != kp->laplacian )
            continue;
        d = compareSURFDescriptors( vec, mvec, dist2, length );
        if( d < dist1 )
        {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        }
        else if ( d < dist2 )
            dist2 = d;
    }
    if ( dist1 < 0.6*dist2 )
        return neighbor;
    return -1;
}
//寻找匹配对
void findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
    int i;
    CvSeqReader reader, kreader;
    cvStartReadSeq( objectKeypoints, &kreader );
    cvStartReadSeq( objectDescriptors, &reader );
    ptpairs.clear();

    for( i = 0; i < objectDescriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* descriptor = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
        if( nearest_neighbor >= 0 )
        {
            ptpairs.push_back(i);
            ptpairs.push_back(nearest_neighbor);
        }
    }
}
//surf匹配
IplImage* find_obj_match = 0;
double find_obj(CIplImage* object, CIplImage* image, int showInfo, int showMatch, int showSurf)
{
    if(object->nChannels!=1)
    {
#ifndef __AFXWIN_H__
        cout<<"#error: 对比图不是单通道图像"<<endl;
#else
        sprintf(info,"%s#error: 对比图不是单通道图像\r\n",info);
#endif
        return -1;
    }
    if(image->nChannels!=1)
    {
#ifndef __AFXWIN_H__
        cout<<"#error: 模板图不是单通道图像"<<endl;
#else
        sprintf(info,"%s#error: 模板图不是单通道图像\r\n",info);
#endif
        return -1;
	}
    CvMemStorage* storage = cvCreateMemStorage(0);
    IplImage* object_color = cvCreateImage(cvGetSize(object), 8, 3);
    cvCvtColor( object, object_color, CV_GRAY2BGR );
    CvSeq* objectKeypoints = 0, *objDescriptors = 0;
    CvSeq* imageKeypoints = 0, *imageDescriptors = 0;
	int i;
	int64 tt = cvGetTickCount();
    CvSURFParams params = cvSURFParams(800, 1);
    cvExtractSURF( object, 0, &objectKeypoints, &objDescriptors, storage, params );
	Sleep(1);
	cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );
    if(showInfo)
    {
#ifndef __AFXWIN_H__
        cout<<"Object Descriptors: "<<objDescriptors->total<<"\nImage Descriptors: "<<imageDescriptors->total<<endl;
        cout<<"Extraction time = "<<(cvGetTickCount()-tt)/(cvGetTickFrequency()*1000.)<<"ms\n";
#else
        sprintf(info,"%sObject Descriptors: %d  \tImage Descriptors: %d  \t",info,objDescriptors->total,imageDescriptors->total);
        sprintf(info,"%sExtraction time = %gms\r\n",info,(cvGetTickCount()-tt)/(cvGetTickFrequency()*1000.));
#endif
    }
    int max_width = image->width>object->width ? image->width : object->width;
    IplImage* correspond = cvCreateImage(cvSize(max_width,object->height+image->height),object->depth,object->nChannels);
    cvSetImageROI( correspond, cvRect( 0, 0, object->width, object->height ) );
    cvCopy( object, correspond );
    cvSetImageROI(correspond,cvRect(0,object->height,image->width,correspond->height));
    cvCopy( image, correspond );
    cvResetImageROI( correspond );

    vector<int> ptpairs;
	findPairs(objectKeypoints,objDescriptors,imageKeypoints,imageDescriptors,ptpairs);
    if(showMatch)
    {
        find_obj_match = cvCreateImage(cvGetSize(correspond), 8, 3);
        cvCvtColor( correspond, find_obj_match, CV_GRAY2BGR );
    }
    srand((unsigned)time(NULL));
    int matchNum = 0;
    double rt_w = object->width/(image->width+.01);
    double rt_h = object->height/(image->height+.01);
    double error = 0.13;
    //cout<<error*rt_w<<" "<<error*rt_h<<endl;
    for( i = 0; i < (int)ptpairs.size(); i += 2 )
    {
        CvScalar cs = {rand()%256,rand()%256,rand()%256};
        CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, ptpairs[i] );
        CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( imageKeypoints, ptpairs[i+1] );
        if( fabs(r1->pt.x/(r2->pt.x+.01)-rt_w) < error*rt_w
                &&fabs(r1->pt.y/(r2->pt.y+.01)-rt_h) < error*rt_h )
        {
            //cout<<fabs(r1->pt.x/(r2->pt.x+.01)-rt_w)<<" "<<fabs(r1->pt.y/(r2->pt.y+.01)-rt_h)<<endl;
            if(showMatch)
                cvLine( find_obj_match, cvPointFrom32f(r1->pt), cvPoint(cvRound(r2->pt.x),
                        cvRound(r2->pt.y+object->height)), cs);
            matchNum++;
        }
	}
    if(showSurf)
    {
        CvScalar red = {0,0,255};
        for( i = 0; i < objectKeypoints->total; i++ )
        {
            CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, i );
            CvPoint center = {cvRound(r->pt.x), cvRound(r->pt.y)};
            int radius = cvRound(r->size*1.2/9.*2);
            cvCircle( object_color, center, radius, red, 1, 8, 0 );
        }
        cvNamedWindow("Object", 1);
        cvShowImage( "Object", object_color );
    }
    int totalNum = objDescriptors->total < imageKeypoints->total?
                   objDescriptors->total:imageKeypoints->total;
    double ratio = pow((double)matchNum/totalNum,1.0/9);
    if(showInfo)
#ifndef __AFXWIN_H__
        cout<<"match : "<<matchNum<<"("<<totalNum<<")"<<endl<<"ratio: "<<ratio<<endl;
#else
        sprintf(info,"%smatch : %d\tratio: %g\r\n",info,matchNum,ratio);
#endif
    if(showMatch)
    {
        cvNamedWindow("Object Correspond", 1);
        cvShowImage( "Object Correspond", find_obj_match );
    }
    cvWaitKey(0);
    if(showSurf)
        cvDestroyWindow("Object");
    if(showMatch)
        cvDestroyWindow("Object Correspond");
    releaseImg(&object_color);
    releaseImg(&correspond);
    if(showMatch)
        releaseImg(&find_obj_match);
    cvReleaseMemStorage(&storage);
#ifndef __AFXWIN_H__
    cout<<"surf角点匹配率为:\t"<<ratio<<endl;
#else
    sprintf(info,"%ssurf角点匹配率为:\t%g\r\n",info,ratio);
#endif
    return ratio;
}

double find_obj(cchar* imgName,cchar* objName,int showInfo,int showMatch,int showSurf)
{
#ifndef __AFXWIN_H__
    IplImage* object = loadImg( objName, 0,0,0,showInfo );
    IplImage* image = loadImg( imgName, 0,0,0,showInfo );
#else
    IplImage* object = loadImg( objName, 0,0,0,0 );
    IplImage* image = loadImg( imgName, 0,0,0,0 );
#endif
    if( !object || !image )
    {
#ifndef __AFXWIN_H__
        fprintf( stderr, "Can not load %s and/or %s\n",objName, imgName );
        exit(-1);
#else
        sprintf(info,"%sCan not load %s and/or %s\r\n",objName, imgName );
        return -1;
#endif
    }
    double ratio = find_obj(object, image, showInfo, showMatch, showSurf);
    releaseImg(&object);
    releaseImg(&image);
    return ratio;
}

//surf角点检测并标记
IplImage* surfDetector(CIplImage* img,cchar* saveSurf="")
{
    CvMemStorage* storage=cvCreateMemStorage(0);
    cvNamedWindow("Image");
    IplImage* img1=cvCreateImage(cvSize(img->width,img->height),img->depth,3);
    cvCopy(img,img1);
    IplImage* image=cvCreateImage(cvSize(img->width,img->height),8,1);
    cvCvtColor(img1,image,CV_BGR2GRAY);
    CvSeq *imageKeypoints = 0, *imageDescriptors=0;
    CvSURFParams params=cvSURFParams(800, 1);
    double tt = (double)cvGetTickCount();
    cvExtractSURF(image, 0, &imageKeypoints, &imageDescriptors, storage, params);
#ifndef __AFXWIN_H__
    cout<<"Image Descriptors: "<<imageDescriptors->total<<endl;
#else
    sprintf(info,"%sImage Descriptors: %d\r\n",info,imageDescriptors->total);
#endif
    CvSURFPoint* r;
    for(int i=0; i<imageKeypoints->total; i++)
    {
        r=(CvSURFPoint*)cvGetSeqElem(imageKeypoints,i);
        CvPoint center;
        center.x = cvRound(r->pt.x);
        center.y = cvRound(r->pt.y);
        int radius = cvRound(r->size*1.2/9*2);
        cvCircle(img1, center, radius, cvScalar(0,0,255), 1, 8, 0);
    }
#ifndef __AFXWIN_H__
    cout<<"last time is "<<(cvGetTickCount()-tt)/(cvGetTickFrequency()*1000.)<<"ms\n";
#else
    sprintf(info,"%slast time is %gms\r\n",info,(cvGetTickCount()-tt)/(cvGetTickFrequency()*1000.));
#endif
    if(saveSurf[0])
        saveImg(saveSurf,img1);
    cvShowImage("Image", img1);
    cvWaitKey(-1);
    cvDestroyWindow("Image");
    releaseImg(&image);
    cvReleaseMemStorage(&storage);
    return img1;
}

//轻重比比较类
class WM
{
	int _n_,_m_,each_part,tran,th_,width,height;
	uchar pp[MAXSIZE*MAXSIZE];  //用来读取数组的模板
	uchar V[MAXSIZE*MAXSIZE];  //灰度数组
public:
	WM(int n=20,int m=20)
	{
		_n_ = n;
		_m_ = m;
		each_part = tran=0;
	}
	// 将像素数据存入到一个整形数组中
	int saveToArr(CIplImage * img,int th = 180)
	{
		if(img){
			width  = img->width;
			height = img->height;
		}else{
			MessageBox(0,"传入轻重比类的图像指针有误","轻重比计算失败",0);
			return 0;
		}
		if(th>0 && th<255)
			th_ = th;
		memset(V,0,sizeof(V));
		uchar* data = (uchar*)img->imageData;
		for( int y = 0; y < img->height; y++ )
			for( int x = 0,yy = y * img->widthStep; x < img->width; x++ ){
				if(img->nChannels==3)
					V[y*img->width+x] = (data[yy+3*x+2]+data[yy+3*x+1]+data[yy+3*x]+1)/3;
				else if(img->nChannels==1)
					V[y*img->width+x] = data[yy+x];
				if(V[y*img->width+x]>th_)
					V[y*img->width+x] = 255;
			}
		return 1;
	}
	//轮廓跟踪之后3*3矩阵平滑处理，做灰度直方图，灰度值V是max（R,G,B）,对原图的数值进行处理
	void binaryzation(int a,int b,int KY)   //寻找点
	{
		int mat=0;
		for(int i=0; i<each_part; i++)
			for(int j=0; j<tran; j++)
			{
				int zz = a*each_part*width+b*tran+i*width+j;
				if(V[zz]>KY)	//轻笔变为非笔迹
					V[zz]=255;
				else			//重笔则统计数目
					mat++;
			}
	}
	//统计笔迹点数目
	int mark(int a,int b)
	{
		int mat=0;
		for(int i=0; i<each_part; i++)
			for(int j=0; j<tran; j++)
				if(V[a*each_part*width+b*tran+i*width+j]!=255)
					mat++;
		return mat;
	}
	template<class T1>
	int Otsu(T1* arr,int size,int size2)
	{
		double histogram[256] = {0};
		int i,threshold;
		for(i=0; i < size; i++)
			if(arr[i]<=th_)
				histogram[arr[i]]++;
		for(i = 0; i < th_; i++)
			if(histogram[i]>.99)
				histogram[i] /= size2;
		double avgValue=0, maxVariance=0, w = 0, u = 0;
		for(i=0; i < th_; i++)
			avgValue += i * histogram[i];
		for(i = 0; i < th_; i++)
		{
			w += histogram[i];
			u += i * histogram[i];
			double t = avgValue * w - u;
			double variance = t * t / (w * (1 - w) );
			if(variance > maxVariance)
			{
				maxVariance = variance;
				threshold = i;
			}
		}
		return threshold;
	}

	void Outline_pretreatment(uchar *pp,int a,int b,float *res)
	{
		int before = mark(a,b);
		if(before==0)
		{
			res[a*_m_+b]=0;
			return;
		}
		int KT = Otsu(pp,each_part*tran,before);
		binaryzation(a,b,KT); //将选中的部分进行局域二值化处理
		int after = mark(a,b);
		if(after==0)
			res[a*_m_+b]=0;
		else
			res[a*_m_+b]= (float)after/before;
	}

	void subsection_mactching(float *res)
	{
		each_part = height/_n_; //每一段有几行
		tran = width/_m_; //用来记录横向的分段
		memset(pp,0,sizeof(pp));
		for(int a=0; a<_n_; a++)
			for(int b=0; b<_m_; b++)
			{
				for(int i=0; i<each_part; i++)
					for(int j=0; j<tran; j++)
						pp[i*tran+j]=V[a*each_part*width+b*tran+i*width+j];
				Outline_pretreatment(pp,a,b,res);
			}
	}

	void matching(float *res,float *res2,double* mw)
	{
		int k,j,nm = _n_*_m_/4;
		for(j=0; j<4; j++)
			mw[j] = 0;
		for(k=0; k<_n_/2; k++)
			for(j=0; j<_m_/2; j++)
				if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
					mw[0]++;
		for(k=_n_/2; k<_n_; k++)
			for(j=0; j<_m_/2; j++)
				if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
					mw[1]++;
		for(k=0; k<_n_/2; k++)
			for(j=_m_/2; j<_m_; j++)
				if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
					mw[2]++;
		for(k=_n_/2; k<_n_; k++)
			for(j=_m_/2; j<_m_; j++)
				if(fabs(res[j*_n_+k]-res2[j*_n_+k])>0.3)
					mw[3]++;
		for(j=0; j<4; j++)
			mw[j] = 1-mw[j]/nm;
	}
};

void matchWeight(CIplImage* img1,CIplImage* img2,double* mw, int show, int n, int m)
{
	WM wm1(n,m),wm2(n,m);
	float res[2][10000]= {0};
	if(!wm1.saveToArr(img1))
		return;
	wm1.subsection_mactching(res[0]);
	Sleep(1);
	if(!wm2.saveToArr(img2))
		return;
	wm2.subsection_mactching(res[1]);
	wm1.matching(res[0],res[1],mw);
#ifndef __AFXWIN_H__
	cout<<"轻重比相似概率为 :\t"<<mw[0]<<","<<mw[1]<<","<<mw[2]<<","<<mw[3]<<endl;
#else
	sprintf(info,"%s轻重比相似概率为 :\t%g,%g,%g,%g\r\n",info,mw[0],mw[1],mw[2],mw[3]);
#endif
}

#ifdef __AFXWIN_H__
//将当前屏幕保存到文件
void Screen(char *filename)
{

    if(filename==0 || filename[0]==0)
    {
        srand(unsigned(time(0)));
        int a = rand()%9000+1000 , b = rand()%9000+1000;
        CString s;
        s.Format("%d%d",a,b);
        filename = s.GetBuffer();
    }
    CDC *pDC;//屏幕DC
    pDC = CDC::FromHandle(::GetDC(NULL));//获取当前整个屏幕DC
    int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
    int Width = pDC->GetDeviceCaps(HORZRES);
    int Height = pDC->GetDeviceCaps(VERTRES);

    CDC memDC;//内存DC
    memDC.CreateCompatibleDC(pDC);

    CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
    memBitmap.CreateCompatibleBitmap(pDC, Width, Height);
    oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
    memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

    //以下代码保存memDC中的位图到文件
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);//获得位图信息
    FILE *fp = fopen(filename, "w+b");

    BITMAPINFOHEADER bih = {0};//位图信息头
    bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//高度
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
    bih.biWidth = bmp.bmWidth;//宽度

    BITMAPFILEHEADER bfh = {0};//位图文件头
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
    bfh.bfType = (WORD)0x4d42;

    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据
    GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p,(LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据
    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据
    delete [] p;
    fclose(fp);
    memDC.SelectObject(oldmemBitmap);
}
#endif

#ifdef __AFXWIN_H__
#define LOG2(x) log(x)/log(2)
#else
#define LOG2(x) log10(x)/log10(2)
#endif // __AFXWIN_H__

//对二值化后的图像求信息熵
//img为输入图像,w和h为完整区域的宽高,最好是包含骨架的最小矩形
float Entropy(CIplImage* img,int th,int w,int h)
{
    int count=0;
    CvSize size;
    //如果传入了宽高参数则使用传递的参数--一般为该骨架的最小矩形
    if(w>0&&h>0)
        size = cvSize(w,h);
    else
        size = cvGetSize(img);
    for (int i=0; i<size.height; ++i)
        for (int j=0; j<size.width; ++j)
            if(CV_IMAGE_ELEM(img,uchar,i,j)<=th)
                count++;	//统计前景像素点个数
    float ratio1 = (float)count / size.width / size.height;
    //返回信息熵:H(S) = -p0*log2(p0)-p1*log2(p1);
    return float(-ratio1*LOG2(ratio1)-(1-ratio1)*LOG2(1-ratio1));
}

// 对输入图像进行细化
//maxIterations限制迭代次数,默认为-1,代表不限制迭代次数,直到获得最终结果
void Hilditch(CIplImage* src,IplImage* dst,int th,int maxIterations)
{
    int tt=clock();
    int noDst = 0;
    if(!dst)
    {
        dst = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
        noDst = 1;
    }
    //将原图像转换为二值图像，字迹输入时需要反向二值化
    cvThreshold(src,dst,th,1,CV_THRESH_BINARY_INV);
    int count = 0;	//记录迭代次数
    CvSize size = cvGetSize(src);
    while (1)
    {
        if(clock()-tt>50)
        {
            Sleep(1);
            tt=clock();
        }
        count++;
        if(maxIterations!=-1 && count > maxIterations) //限制迭代次数
            break;
        vector<pair<int,int> > mFlag; //用于标记需要删除的点
        //对点标记
        for (int i=0; i<size.height; ++i)
        {
            for (int j=0; j<size.width; ++j)
            {
                //如果满足四个条件，进行标记
                //  p[8] p[1] p[2]
                //  p[7] p[0] p[3]
                //  p[6] p[5] p[4]
                int p[9];
                p[0] = CV_IMAGE_ELEM(dst,uchar,i,j);//取得[i][j]对应的值，前景为1，背景为0
                if(!p[0])
                    continue;
                p[1] = (i==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j);
                p[2] = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j+1);
                p[3] = (j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i,j+1);
                p[4] = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j+1);
                p[5] = (i==size.height-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j);
                p[6] = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j-1);
                p[7] = (j==0)?0:CV_IMAGE_ELEM(dst,uchar,i,j-1);
                p[8] = (i==0 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j-1);
                //条件1:保证p0的8领域内为前景的像素点个数在2~6之间，即p1不为端点
                int pTotal = p[1]+p[2]+p[3]+p[4]+p[5]+p[6]+p[7]+p[8];
                if (pTotal>=2 && pTotal<=6)
                {
                    int ap=0;
                    for(int k=1; k<8; ++k)
                        if (p[k]==0 && p[k+1]) ++ap;
                    if (p[8]==0 && p[1]) ++ap;
                    //条件2:p0的顺时针方向周围的像素点中由背景变前景的次数为1
                    if (ap==1 && p[1]*p[3]*p[5]==0 && p[3]*p[5]*p[7]==0)
                        mFlag.push_back(make_pair(i,j));    //标记
                }
            }
        }

        //将标记的点删除
        for (vector<pair<int,int> >::iterator i=mFlag.begin(); i!=mFlag.end(); ++i)
            CV_IMAGE_ELEM(dst,uchar,i->first,i->second) = 0;
        //直到没有点满足，算法结束
        if (mFlag.size()==0)
            break;
        else
            mFlag.clear();//将mFlag清空

        //第二次对点标记
        for (int i=0; i<size.height; ++i)
        {
            for (int j=0; j<size.width; ++j)
            {
                //如果满足条件，进行标记
                //  p[8] p[1] p[2]
                //  p[7] p[0] p[3]
                //  p[6] p[5] p[4]
                int p[9];
                p[0] = CV_IMAGE_ELEM(dst,uchar,i,j);//取得[i][j]对应的值，前景为1，背景为0
                if(p[0]!=1)
                    continue;
                p[1] = (i==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j);
                p[2] = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j+1);
                p[3] = (j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i,j+1);
                p[4] = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j+1);
                p[5] = (i==size.height-1)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j);
                p[6] = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i+1,j-1);
                p[7] = (j==0)?0:CV_IMAGE_ELEM(dst,uchar,i,j-1);
                p[8] = (i==0 || j==0)?0:CV_IMAGE_ELEM(dst,uchar,i-1,j-1);

                //条件1:保证p0的8领域内为前景的像素点个数在2~6之间，即p1不为端点
                int pTotal = p[1]+p[2]+p[3]+p[4]+p[5]+p[6]+p[7]+p[8];
                if (pTotal>=2 && pTotal<=6)
                {
                    int ap=0;
                    for(int k=1; k<8; ++k)
                        if (p[k]==0 && p[k+1]) ++ap;
                    if (p[8]==0 && p[1]) ++ap;
                    //条件2:p0的顺时针方向周围的像素点中由背景变前景的次数为1
                    if (ap==1 && p[1]*p[3]*p[7]==0 && p[1]*p[5]*p[7]==0)
                        mFlag.push_back(make_pair(i,j));    //标记
                }
            }
        }
        //将标记的点删除
        for (vector<pair<int,int> >::iterator i=mFlag.begin(); i!=mFlag.end(); ++i)
            CV_IMAGE_ELEM(dst,uchar,i->first,i->second) = 0;

        //直到没有点满足，算法结束
        if (mFlag.size()==0)
            break;
        else
            mFlag.clear();//将mFlag清空
    }
    if(noDst)
        releaseImg(&dst);
    else
    {
        for (int i=0; i<size.height; ++i)
            for (int j=0; j<size.width; ++j)
            {
                if(CV_IMAGE_ELEM(dst,uchar,i,j))
                    CV_IMAGE_ELEM(dst,uchar,i,j)=CV_IMAGE_ELEM(src,uchar,i,j);
				else
                    CV_IMAGE_ELEM(dst,uchar,i,j)=255;
            }
    }
}

//中值滤波---//未完结
void midFilter(IplImage* src)
{
    int va[9];
    uchar* data = (uchar*)src->imageData;
    if(src->nChannels==3)
    {
        for( int y = 0; y < src->height; y++ )
            for( int x = 0,yy = y * src->widthStep; x < src->width; x++ )
			{
                data[yy+3*x];
			}
    }
    else if(src->nChannels==1)
    {
        for( int y = 0; y < src->height; y++ )
            for( int x = 0,yy = y * src->widthStep; x < src->width; x++ )
			{
                data[yy+x];
			}
    }
    sort(va,va+8);
    //if(RGB_v(i,j)==va[0] || RGB_v(i,j)==va[8])
    //RGB_v(i,j)=va[4];
}