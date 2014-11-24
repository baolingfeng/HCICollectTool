#pragma once
#include<Windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<hash_map>
#include<hash_set>
#include<set>
#include<algorithm>
#include<qimage.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
using namespace std;
using namespace cv;

 
const Mat MOUSEICON = imread("../icons/1.png");
const Mat MOUSEMASK = imread("../icons/1_msk.png", 0);

void trim(string& str);
vector<string> split(string& str, string delim);
string join(vector<string> sarray,string sep);

extern void convolveDFT(Mat& A, Mat& B, Mat& C, int flag=1);

extern int myMatchAlphaTemplate(Size imgSize, Size tplSize, Mat& rst, 
	Mat& F_a2, Mat& F_a2T, double aT, 
	Mat& F_I, Mat& F_I2, int method=0);
extern int myMatchAlphaTemplate(Mat& img, Mat& tpl, Mat& msk, Mat& rst,  
	Mat& F_a2, Mat& F_a2T, double aT, Mat& F_I, Mat& F_I2, int method=0);
extern int myMatchAlphaTemplate(Mat& img, Mat& tpl, Mat& msk, Mat& rst,  
	Mat& F_a2, Mat& F_a2T, double aT, int method=0);
extern int myMatchAlphaTemplate(Mat& img, Mat& tpl, Mat& msk, Mat& rst, 
	int method=0, int dftWidth=0, int dftHeight=0);

Rect_<float> getOverlap( Rect_<float> rc1, Rect_<float> rc2 );
Rect         getOverlap( Rect         rc1, Rect         rc2 );
void getJetColors( vector<Scalar>& colors, int sz);

cv::Rect fromWindowRECT(RECT w_r);
void addImageOnFront(cv::Point pt, cv::Mat& bg, cv::Mat fg, cv::Mat mask);

template<typename T>
T stringToNumber(const string& numberAsString)
{
    T val;

    std::stringstream stream(numberAsString);
    stream >> val;
    if (stream.fail()) {
        std::runtime_error e(numberAsString);
        throw e;
    }

    return val;
}

template<typename T>
vector<T> subvector(vector<T> v, int first, int last)
{
	vector<T>::const_iterator first_it = v.begin() + first;
	vector<T>::const_iterator last_it = v.begin() + last;

	vector<T> newV(first_it,last_it);
	return newV;
}

void replaceAll(string& str, string from,string to);

SYSTEMTIME toSystemTime(string timestamp);
SYSTEMTIME covertStringToSystime(string timestamp); //for excel log time format

double GetTimeDifference( SYSTEMTIME &st1, SYSTEMTIME &st2 );

template<typename T>
double calcMedianOfVector(vector<T> v)
{
  T median;
  size_t size = v.size();

  sort(v.begin(), v.end());

  if (size  % 2 == 0)
  {
      median = (v[size / 2 - 1] + v[size / 2]) / 2;
  }
  else 
  {
      median = v[size / 2];
  }

  return median;
}

inline extern bool im2float(Mat& img_u, Mat& img_f)
{
	if(img_u.depth()!=CV_8U){
		return false;
	}
	int type_f = CV_32FC3;
	//int type_u = img_u.type();
	//if( type_u!=CV_8UC1 && type_u!=CV_8UC3){
	//	return false;
	//}
	if(img_u.channels() == 1)
		type_f = CV_32FC1;
	img_u.convertTo(img_f, type_f);
	img_f *= 1.0/255;
	return true;
}

inline extern bool im2double(Mat& img_u, Mat& img_lf)
{
	if(img_u.depth()!=CV_8U){
		return false;
	}
	int type_lf = CV_64FC3;

	if(img_u.channels() == 1)
		type_lf = CV_64FC1;

	img_u.convertTo(img_lf, type_lf);
	img_lf *= 1.0/255;
	return true;
}

template< class T > int vecMskErase(vector<T>& obj, vector<int> msk)
{
	if( msk.size() > obj.size())
	{
		return -1;
	}

	unsigned int i;
	int cnt=0;

	vector<int> msk_id(msk.size()+1);
	msk_id[0] = 0;
	for( i=0; i<msk.size(); i++ )
	{
		msk_id[i+1] = msk[i] + msk_id[i];
		if(msk[i]==0)
		{
			cnt++;
			obj.erase( obj.begin() + msk_id[i] );
		}
	}

	return cnt;
}


inline QImage Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation 
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

inline cv::Mat QImage2Mat(QImage const& src)
{
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,CV_BGR2RGB);
     return result;
}
