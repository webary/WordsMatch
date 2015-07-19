/**
	该头文件使用OpenCV库实现读取图片并保存像素信息、直接输出一幅图片、
	对图像进行一些常见处理操作。
	@author : webary
	@time (last update time): 2014.7.12  19:00
**/
#pragma once
#ifndef _IMGPRO_H_
#define _IMGPRO_H_

//标记是否使用多线程加速
#define b_multiThread 1

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
const int MAXSIZE = 1000;//支持图片最大大小

#ifndef _IMGPRO_H_VAR_
#define _IMGPRO_H_VAR_
#if !b_multiThread
	extern IplImage *pimg, *pGray;
	#define _InitWith_pimg =pimg
#else
	#define _InitWith_pimg =0
#endif
extern IplImage* find_obj_match;
extern char info[10000];
#endif

IplImage *loadImg(cchar* tmpimgName,int ask=1,int cls=1,int color=1,int show=1);
#ifdef __AFXWIN_H__
IplImage *loadImg(CString tmpimgstring,int ask=1,int cls=1,int color=1,int show=1);
#endif
void saveRGB(CIplImage* img _InitWith_pimg, cchar *fileOutName = 0);
void outputImg(cchar* imgOutName, int readAgain = 1, IplImage* img _InitWith_pimg);
void saveIplImage(IplImage* img _InitWith_pimg);
void saveImg(cchar* imgSaveName, IplImage* img _InitWith_pimg);
int  showImg(CIplImage* img _InitWith_pimg, cchar* str = "图片显示", cchar* ss = "显示图像出错", int pause = 1);
void releaseImg(IplImage** ppimg) ;
CvSeq* findContour(CIplImage* src, IplImage** dst = 0, int th = 100, int filled = 0);
double find_obj(cchar* imgName,cchar* objName,int showInfo=0,int showMatch=0,int showSurf=0);
double find_obj(CIplImage* object,CIplImage* image,int showInfo=0,int showMatch=0,int showSurf=0);
void matchWeight(CIplImage* img1, CIplImage* img2, double* wm, int show = 0, int n = 20, int m = 20);
void Screen(char *filename="");

void Hilditch(CIplImage* src, IplImage* dst = 0, int th = 128, int maxIterations = -1);
float Entropy(CIplImage* img _InitWith_pimg, int th = 1, int w = 0, int h = 0);
IplImage* equalizeHist(CIplImage* img _InitWith_pimg, int saveFile = 0);
void Histeq_self(IplImage** ppimg,int save=0);
int Otsu(CIplImage* src);
#endif // _IMGPRO_H_
