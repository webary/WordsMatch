#ifndef  _TARGETMATCH_H_
#define  _TARGETMATCH_H_

const int Ch_Num = 11;	//定义特征个数

#include "imgPro.h"
#include "md5.h"
class Target {
private:
    Target* p_model;
    CvSeq *contour;
    IplImage* tar_img;
    typedef float type;
	static const int MAXSIZE = 1000;	//图片最大大小（宽、高）
    uchar R[MAXSIZE][MAXSIZE],G[MAXSIZE][MAXSIZE],B[MAXSIZE][MAXSIZE];
    type p_u[MAXSIZE], p_v[MAXSIZE];
    int x, y, w, h, ww, hh;	//目标在图像中最左上方的点的坐标；目标的宽度和高度;有效宽度和高度
	DB_table db_t;
	MD5 Md5;
#define targetIsBlack 1

    void getSum(type *vec, int &len);
    void Normalize(type *vec, int &len, int toLength);
    void saveRGB(IplImage* img,cchar* name="");
    bool isTargetPoint(const int& i,const int& j);
    void projectionUV();
    void Normalize(int toLength_h,int toLength_w);
    void matchWidthHeight();
    void matchProjection();
	void matchContours();
	double matchBias();
	void Bias(uchar pgray[MAXSIZE][MAXSIZE]);//倾斜向量数组
	void controlWhtieNoise(int th);
public:
	char fileName[500];
	double sum, entropy, whError, comError[2], huError, surfError,
		weightError[4], entrError,bias[3],biasError, Error;//添加倾斜向量的3个分量
    Target(int xx=-1,int yy=-1,int ww=-1,int hh=-1);
    ~Target();
    void showUVWH(string ps="");
    void getTargetInfo(cchar* path,int ask = 1,int show = 1);
#ifdef __AFXWIN_H__
	void getTargetInfo(CString& path,int ask=0,int show=0);
#endif
    void getUVFromFile(cchar* filein);
    double compare(Target &model,cchar *saveError=0,int same=-1,int re=0,double *ratio=0,int ww=0,int hh=0);
    void saveUVToFile(cchar* fileout,cchar* ps=0,int hh=0,int ww=0);
	void saveIplImage();
};

#endif // _TARGETMATCH_H_
