/*!
 *  @brief     原始数据文件处理
 *  @details   对原始数据文件进行解析
 *  @author    yzg
 *  @version   v1.0
 *  @date      2021.03.09
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include <base/util/rutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <iostream>
#include <complex>

using namespace std;

#define PI		3.1415926
#define sr		20000
#define fs		20480
#define NFFT	32768
#define COFF	1/1000000.0

namespace Related {

	/*!
	 * @brief    数据帧描述头
	 * @details 
	 */
	struct HeadBufStruct	{
		unsigned short HeadValue;
		unsigned short CmdType;
		unsigned int   CurrentFlameNum;
		unsigned char  TimeBuf[8];
		unsigned short ReservedBuf2[4];
		unsigned short AttitudeFlag;
		unsigned short AttitudeReservel;
		float Heading;
		float Roll;
		float Pitch;
		unsigned short AttitudeReserwe[4];
		unsigned short DynamiclessageF1ag;
		unsigned short StaticMessageFlag;
		unsigned char DynamieMessageBuf[456];
		unsigned short ReservedBuf;
		unsigned short EndValue;
	};


	class RawDataFileProcessing : public QObject
	{
		Q_OBJECT

	public:
		RawDataFileProcessing(QObject *parent = nullptr);
		~RawDataFileProcessing();

		bool dealDirDataFilePath(QString dirPath, QString savePath);
		bool dealSingleDataFilePath(QString filePath, QString savePath);


		// 		/************** 预处理 *************/
		void prepro_channalfilter(float *result, float *channal, float *build_filter, int N1, int N2);
		void prepro_channalupdate(float *result, float *longchannal, float *channal, int N1, int N2);
		void prepro_mulpower(float *II, float *P, float *D, int N1, int N2);
		void prepro_crosspectrum(int *doa, float *Ix, float *Iy);
		void prepro_fir(float *coff, int N, float flp, float fhp, int mode);
		
		void prepro_fft(complex<float> *result, float *x, int N, int M);
		void prepro_ifft(float *result, complex<float> *x, int N, int M);
		
				void prepro_conv(float *z, float *x, float *y, int N1, int N2);
		void prepro_truncation(float *y, float *x, int N, int M);
		void prepro_histgram(int *doa, float *mu, int N);
		void prepro_gaussgram(float *doa, float *mu, int N);
		void prepro_hamming(float *xh, float *x, int N);
		 
		float prepro_fir_lowpass(int i, int M, float fhp);
		float prepro_fir_highpass(int i, int M, float flp);
		float prepro_fir_bandpass(int i, int M, float flp, float fhp);
		float prepro_fir_bandlock(int i, int M, float flp, float fhp);
		 
		void prepro_zeropad(float *y, float *x, int N, int M);
		 
	 	void prepro_mul(float *z, float *x, float *y, int N);
	
 		void prepro_constrantmul(float *z, float *x, float a, int N);
 
		int prepro_min(float *x, int N, float a);

		void prepro_round(float *y, float *x, int N);
		
		void prepro_steady(float *z, float *y, int N);

		void prepro_cutarray(float *y, float *x, int N1, int N2);

	private:
		char * m_cacheBuff;
	};

}//namespace Related 
