#include "rawdatafileprocessing.h"

#include <QDebug>

namespace Related {

	RawDataFileProcessing::RawDataFileProcessing(QObject *parent)
		: QObject(parent)
	{

	}

	RawDataFileProcessing::~RawDataFileProcessing()
	{

	}

	/*!
	 * @brief  处理指定文件夹下的数据文件
	 * @details 
	 * @param dirPath 原始数据文件夹
	 * @param savePath 处理完成保存的书文件夹
	 * @return
	 */
	bool RawDataFileProcessing::dealDirDataFilePath(QString dirPath, QString savePath)
	{
		//[1] 创建缓存书文件夹
		Base::RUtil::createDir(savePath);
		//[2] 遍历文件夹，处理单个数据文件
		QDir dir(dirPath);
		if (!dir.exists()) {
			return false;
		}
		QStringList filters;
		filters << QString("*.dat");
		dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		dir.setSorting(QDir::Size | QDir::Reversed);
		dir.setNameFilters(filters);

		QFileInfoList list = dir.entryInfoList();
		QStringList string_list;

		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);
			QString absolute_file_path = fileInfo.absoluteFilePath();
			//[3] 处理单个文件
			dealSingleDataFilePath(fileInfo.filePath(), savePath);
		}
		return true;
	}

	/*!
	 * @brief  处理指定数据文件
	 * @details
	 * @param filePath 原始数据文件路径
	 * @param savePath 处理完成保存的书文件夹
	 * @return
	 */
	bool RawDataFileProcessing::dealSingleDataFilePath(QString filePath, QString savePath)
	{
		QDateTime start_date_time = QDateTime::currentDateTime();
		QString t_strStart_date = start_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");


		int t_totalframe = 0;                                   /*!< 文件总帧数 */
		int t_tempbufLength = 512 + 320 * 1024 + 1536;          /*!< 缓存区数据长度 */

		QString fnf;
		QString fns;

		HeadBufStruct *pheadbuf;
		int *ptr;
		unsigned char *presult;
		int frame;

		FILE * fid2 = NULL;                     /*!< 结果数据文件 */
		FILE * fid3 = NULL;                     /*!< 结果音频文件 */

		// 信号处理机时间
		unsigned int DSP_Second;
		unsigned int DSP_Minute;
		unsigned int DSP_Hour;
		unsigned int DSP_Day;
		unsigned int DSP_Mouth;
		unsigned int DSP_Year;
		QString DSP_Time;

		//滑翔机位置级时间
		unsigned short HXJ_WaterDepth;                              //滑翔机深度数据
		//滑翔机经度数据
		unsigned short HXJ_LongD;
		unsigned short HXJ_LongM;
		unsigned short HXJ_LongS;
		//滑翔机纬度数据
		unsigned short HXJ_LatD;
		unsigned short HXJ_LatM;
		unsigned short HXJ_LatS;
		//滑翔机GPS时间数据
		unsigned short HXJ_Year;
		unsigned short HXJ_Month;
		unsigned short HXJ_Day;
		unsigned short HXJ_Hour;
		unsigned short HXJ_Minute;
		unsigned short HXJ_Second;
		float HXJ_Long;
		float HXJ_Lat;
		QString HXJ_Time;																// 滑翔机时间

		float FangWeiJiao;				//方位角
		float HengGunJiao;				//横滚角
		float FuYangJiao;				//俯仰角
		float Heading;
		float Roll;
		float Pitch;

		// 四路水声信号
		float ap[fs], ax[fs], ay[fs], az[fs];
		float tx, ty, tz;

		//方位历程信息
		float RR[3][3];

		//中间变量
		int snap = 5 * fs;
		int prop = 4;

		int *doa = (int *)malloc(360 * sizeof(int));
		float  * buind_filer = (float *)malloc(1025 * sizeof(float));
		float  * longP = (float *)malloc(snap * sizeof(float));
		float  * longX = (float *)malloc(snap * sizeof(float));
		float  * longY = (float *)malloc(snap * sizeof(float));
		float  * longZ = (float *)malloc(snap * sizeof(float));

		float  * Ix = (float *)malloc(fs * sizeof(float));
		float  * Iy = (float *)malloc(fs * sizeof(float));

		for (int i = 0; i < 1025; i++) {
			buind_filer[i] = prepro_fir_highpass(i, 513, 100.0 / sr);
		}

		for (int i = 0; i < snap; i++) {
			longP[i] = 0;
			longX[i] = 0;
			longY[i] = 0;
			longZ[i] = 0;
		}

		//[] 读取文件
		QFile t_file(filePath);
		bool open = t_file.open(QFile::ReadOnly);
		if (!t_file.exists() && !open) {
			qDebug() << "open file Fail!";
			return  false;
		}

		qint64 t_fileLength = t_file.size();
		t_totalframe = t_fileLength / (322 * 1024);

		//[] 处理每一帧数据
		for (int index = 0; index < t_totalframe; index++) {

			m_cacheBuff = new char[t_tempbufLength];

			t_file.read(m_cacheBuff, t_tempbufLength);
			pheadbuf = (HeadBufStruct *)m_cacheBuff;

			ptr = (int *)(m_cacheBuff + 512);
			presult = (unsigned char *)(m_cacheBuff + 512 + 320 * 1024);
			frame = pheadbuf->CurrentFlameNum;

			delete[] m_cacheBuff;
			//读取时间数据
			DSP_Second = ((pheadbuf->TimeBuf[0]) & 0x0f) + ((((pheadbuf->TimeBuf[0]) + 0x70) >> 4) * 10);
			DSP_Minute = ((pheadbuf->TimeBuf[1]) & 0x0f) + ((((pheadbuf->TimeBuf[1]) + 0x70) >> 4) * 10);
			DSP_Hour = ((pheadbuf->TimeBuf[2]) & 0x0f) + ((((pheadbuf->TimeBuf[2]) + 0x30) >> 4) * 10);
			DSP_Day = ((pheadbuf->TimeBuf[4]) & 0x0f) + ((((pheadbuf->TimeBuf[4]) + 0x30) >> 4) * 10);
			DSP_Mouth = ((pheadbuf->TimeBuf[5]) & 0x0f) + ((((pheadbuf->TimeBuf[5]) + 0x10) >> 4) * 10);
			DSP_Year = ((pheadbuf->TimeBuf[6]) & 0x0f) + ((((pheadbuf->TimeBuf[6]) + 0xf0) >> 4) * 10) + 2000;

			DSP_Time = QString("%1%2%3%4%5%6").arg(QString::number(DSP_Year))
				.arg(QString::number(DSP_Mouth)).arg(QString::number(DSP_Day))
				.arg(QString::number(DSP_Hour)).arg(QString::number(DSP_Minute))
				.arg(QString::number(DSP_Second));


			//读取滑翔机数据
			HXJ_WaterDepth = *((unsigned short *) &(presult[1488]));	// 10倍深度
			HXJ_LongD = *((unsigned short *) &(presult[1490]));
			HXJ_LongM = *((unsigned short *) &(presult[1492]));
			HXJ_LongS = *((unsigned short *) &(presult[1494]));
			HXJ_LatD = *((unsigned short *) &(presult[1496]));
			HXJ_LatM = *((unsigned short *) &(presult[1498]));
			HXJ_LatS = *((unsigned short *) &(presult[1500]));

			HXJ_Year = presult[1502];
			HXJ_Month = presult[1503];
			HXJ_Day = presult[1504];
			HXJ_Hour = presult[1505];
			HXJ_Minute = presult[1506];
			HXJ_Second = presult[1507];
			HXJ_Long = HXJ_LongD + (float)HXJ_LongM / 60.0 + (float)HXJ_LongS / 3600;
			HXJ_Lat = HXJ_LatD + (float)HXJ_LatM / 60.0 + (float)HXJ_LatS / 3600;
			HXJ_Time = QString("%1%2%3%4%5%6").arg(QString::number(HXJ_Year))
				.arg(QString::number(HXJ_Month)).arg(QString::number(HXJ_Day))
				.arg(QString::number(HXJ_Hour)).arg(QString::number(HXJ_Minute))
				.arg(QString::number(HXJ_Second));

			//读取姿态数据（弧度）
			FangWeiJiao = pheadbuf->Heading;
			HengGunJiao = pheadbuf->Roll;
			FuYangJiao = pheadbuf->Pitch;
			Heading = FangWeiJiao * 180 / PI;
			Roll = HengGunJiao * 180 / PI;
			Pitch = FuYangJiao * 180 / PI;

			//坐标系转化矩阵
			RR[0][0] = cos(HengGunJiao) * cos(FuYangJiao) - sin(HengGunJiao) *sin(FuYangJiao) *sin(FangWeiJiao);
			RR[0][1] = -cos(HengGunJiao) * cos(FuYangJiao);
			RR[0][2] = sin(HengGunJiao) * cos(FuYangJiao) + cos(HengGunJiao) *sin(FuYangJiao) *sin(FangWeiJiao);

			RR[1][0] = cos(HengGunJiao) * cos(FangWeiJiao) + sin(HengGunJiao) *sin(FuYangJiao) *sin(FangWeiJiao);
			RR[1][1] = cos(FuYangJiao) * cos(FangWeiJiao);
			RR[1][2] = sin(HengGunJiao) * sin(FangWeiJiao) - cos(HengGunJiao) *sin(FuYangJiao) *cos(FangWeiJiao);

			RR[2][0] = -sin(HengGunJiao) * cos(FuYangJiao);
			RR[2][1] = sin(FuYangJiao);
			RR[2][2] = cos(HengGunJiao) * cos(FuYangJiao);

			// 			/************************* 每百帧数据存储音频文件 ********************************/
			// 			if (index == 0) {
			// 				int mvFrame = frame / 100;
			// 				fnf = QString("%1").arg(QString::number(mvFrame * 100));
			// 
			// 				QString t_strWavFileName = QString("%1/signal/%2.wav").arg(t_tempSaveDir).arg(fnf);
			// 				QString  t_strW = QString("w");
			// 				fid3 = fopen(t_strWavFileName.toLocal8Bit().data(), t_strW.toLocal8Bit().data());
			// 			}
			// 			else
			// 			{
			// 				int daTaindex = index % 100;
			// 				if (daTaindex == 0) {
			// 					//[]
			// 					fclose(fid3);
			// 
			// 					int mvFrame = frame / 100;
			// 					fnf = QString("%1").arg(QString::number(mvFrame));
			// 
			// 					QString t_strWavFileName = QString("%1/signal/%2.wav").arg(t_tempSaveDir).arg(fnf);
			// 					QString  t_strW = QString("w");
			// 					fid3 = fopen(t_strWavFileName.toLocal8Bit().data(), t_strW.toLocal8Bit().data());
			// 				}
			// 			}
			// 
						//四路通道数据读取
			for (int i = 0; i < fs; i++) {
				tx = ptr[4 * i + 1] * COFF;
				ty = ptr[4 * i + 2] * COFF;
				tz = ptr[4 * i + 3] * COFF;

				ap[i] = ptr[4 * i + 0] * COFF;
				ax[i] = RR[0][0] * tx + RR[0][1] * ty + RR[0][2] * tz;
				ay[i] = RR[1][0] * tx + RR[1][1] * ty + RR[1][2] * tz;
				az[i] = RR[2][0] * tx + RR[2][1] * ty + RR[2][2] * tz;

				// 				if (fid3 != NULL) {
				// 					QString wavdata = QString("%f %f %f %f\n");
				// 					fprintf(fid3, wavdata.toLocal8Bit().data(), ap[i], ax[i], ay[i], az[i]);
				// 				}
			}
		 
			//[] 高通过滤
			prepro_channalfilter(ap, ap, buind_filer, fs, 1025);
			prepro_channalfilter(ax, ax, buind_filer, fs, 1025);
			prepro_channalfilter(ay, ay, buind_filer, fs, 1025);
			prepro_channalfilter(az, az, buind_filer, fs, 1025);

			//积分
			for (int i = 0; i < fs; i++) {
				ax[i] = ax[i] + ax[i - 1];
				ay[i] = ay[i] + ay[i - 1];
				az[i] = az[i] + az[i - 1];
			}

			// 去直流
			prepro_steady(ax, ax, fs);
			prepro_steady(ay, ay, fs);
			prepro_steady(az, az, fs);

			// 快拍更新
			prepro_channalupdate(longP, longP, ap, snap, fs);
			prepro_channalupdate(longX, longX, ax, snap, fs);
			prepro_channalupdate(longY, longY, ay, snap, fs);
			prepro_channalupdate(longZ, longZ, az, snap, fs);

			//复声强
			prepro_mulpower(Ix, longP, longX, snap, fs);
			prepro_mulpower(Iy, longP, longY, snap, fs);

			//互谱求解方位角
			prepro_crosspectrum(doa, Ix, Iy);
 
			//写入文件
			if (index == 0) {
				QString savefilePath;
				savefilePath = QString("%1/%2").arg(savePath).arg(DSP_Time);

				// 检测文件是否存在
				int  judge = _access(savefilePath.toLocal8Bit().data(), 0);
				if (judge == 0) {

				}
				QString savefileName;
				savefileName.append(savefilePath).append(QString(".dat"));
				QString strW = QString("w");
				fid2 = fopen(savefileName.toLocal8Bit().data(), strW.toLocal8Bit().data());
			}

			QString data = QString("%d %s %s %f  %f  %d  %f %f %f");
			fprintf(fid2, data.toLocal8Bit().data(),
				pheadbuf->CurrentFlameNum,
				DSP_Time.toLocal8Bit().data(),
				HXJ_Time.toLocal8Bit().data(),
				HXJ_Long,
				HXJ_Lat,
				HXJ_WaterDepth,
				Heading,
				Roll,
				Pitch);
			for (int i = 0; i < 360; i++) {
				QString data2 = QString("%d");
				fprintf(fid2, data2.toLocal8Bit().data(), doa[i]);
			}
			QString data3 = QString("\n");
			fprintf(fid2, data3.toLocal8Bit().data());
		}

		//    fclose(fid1);
		fclose(fid2);

		free(Ix);
		free(Iy);
		free(longP);
		free(longX);
		free(longY);
		free(longZ);
		free(doa);
		free(buind_filer);

		QDateTime end_date_time = QDateTime::currentDateTime();
		QString t_strend_date = end_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");

		return true;
	}

// 	/**********************     数据处理    ********************************/
	/*!
	 * @brief 单帧滤波
	 * @details 
	 */
	void RawDataFileProcessing::prepro_channalfilter(float * result, float * channal, float * build_filter, int N1, int N2)
	{
		float  * windowadd = (float *)malloc(N1 * sizeof(float));
		float  * convresult = (float *)malloc((N1 + N2 - 1) * sizeof(float));

// 		prepro_hamming(windowadd, channal, N1);
// 		prepro_conv(convresult, windowadd, build_filter, N1, N2);
// 		prepro_truncation(result, convresult, N1 + N2 - 1, N1);

		free(windowadd);
		free(convresult);
	}

	/*!
	 * @brief 快拍更新
	 * @details
	 */
	void RawDataFileProcessing::prepro_channalupdate(float * result, float * longchannal, float * channal, int N1, int N2)
	{
		for (int i = 0; i < N1 - N2; i++) {
			result[i] = longchannal[i + N2];
		}
		for (int i = N1 - N2; i < N1; i++) {
			result[i] = channal[i - N1 + N2];
		}
	}

	/*!
	 * @brief 复声强
	 * @details
	 */
	void RawDataFileProcessing::prepro_mulpower(float * II, float * P, float * D, int N1, int N2)
	{
		int Prop = 4;
		int frames = (N1 - N2) * Prop / fs + 1;
		int ns;
		float * shortP = (float *)malloc(N2 * sizeof(float));
		float * shortD = (float *)malloc(N2 * sizeof(float));

		complex<float> * wp = (complex<float> *)malloc(N2 * sizeof(complex<float>));
		complex<float> * wd = (complex<float> *)malloc(N2 * sizeof(complex<float>));

		for (int j = 0; j< int(N2 / 2); j++) {
			II[j] = 0;
		}

		for (int i = 0; i < frames; i++) {
			ns = i * N2 / 4;
			for (int j = 0; j < N2; j++) {
				shortP[j] = P[ns + j];
				shortD[j] = D[ns + j];
			}

// 			prepro_fft(wp, shortP, N2, N2);
// 			prepro_fft(wd, shortD, N2, N2);

			for (int j = 0; j < int(N2 / 2); j++) {
				II[j] = II[j] + wp[j].real() *wd[j].real() + wp[j].imag() *wd[j].imag();
			}
		}

		free(shortP);
		free(shortD);
		free(wp);
		free(wd);
	}

	/*!
	 * @brief 互谱法
	 * @details
	 */
	void RawDataFileProcessing::prepro_crosspectrum(int * doa, float * Ix, float * Iy)
	{
		int N1 = int(200 * fs / sr);
		int N2 = int(3000 * fs / sr);
		float *theta = (float*)malloc((N2 - N1) * sizeof(float));

		for (int j = N1; j < N2; j++) {
			theta[j - N1] = 90 - atan2(Iy[j], Ix[j]) / PI * 180;
			if (theta[j - N1] < 0) {
				theta[j - N1] = theta[j - N1] + 360;
			}
		}

		//prepro_histgram(doa, theta, N2 - N1);
		free(theta);
	}

	/*!
	 * @brief 滤波器设计
	 * @details
	 */
	void RawDataFileProcessing::prepro_fir(float * coff, int N, float flp, float fhp, int mode)
	{
		int M;
		if (N % 2 != 0) {
			M = (N - 1) / 2;
		}
		else {
			M = N / 2;
		}

		if (mode < 1 || mode > 4) {
			printf("%s\n", "");
		}
		else {
			switch (mode) {
				//低通
			case 1: {
				for (int i = 0; i < M + 1; i++) {
					coff[i] = prepro_fir_lowpass(i, M, fhp);
					coff[N - 1 - i] = coff[i];
				}
			}
					break;
					//高通
			case 2: {
				for (int i = 0; i < M + 1; i++) {
					coff[i] = prepro_fir_highpass(i, M, flp);
					coff[N - 1 - i] = coff[i];
				}
			}
					break;
					//带通
			case 3: {
				for (int i = 0; i < M + 1; i++) {
					coff[i] = prepro_fir_bandpass(i, M, flp, fhp);
					coff[N - 1 - i] = coff[i];
				}
			}
					break;
					//带阻
			case 4: {
				for (int i = 0; i < M + 1; i++) {
					coff[i] = prepro_fir_bandlock(i, M, flp, fhp);
					coff[N - 1 - i] = coff[i];
				}
			}
					break;
			default:
				break;
			}
		}
	}

	/*!
	 * @brief fourier变换
	 * @details
	 */
	void RawDataFileProcessing::prepro_fft(complex<float>* result, float * x, int N, int M)
	{
		//fftw_complex
	}

	/*!
	 * @brief  
	 * @details
	 */
	void RawDataFileProcessing::prepro_ifft(float * result, complex<float>* x, int N, int M)
	{
	}

	/*!
	 * @brief 快速卷积
	 * @details
	 */
	void RawDataFileProcessing::prepro_conv(float * z, float * x, float * y, int N1, int N2)
	{
		int L = N1 + N2 - 1;
		float wzreal, wzimag;
		float *xexpand = (float *)malloc(L * sizeof(float));
		float *yexpand = (float *)malloc(L * sizeof(float));

		complex<float> * wx = (complex<float> *)malloc(L * sizeof(complex<float>));
		complex<float> * wy = (complex<float> *)malloc(L * sizeof(complex<float>));
		complex<float> * wz = (complex<float> *)malloc(L * sizeof(complex<float>));

		prepro_zeropad(xexpand, x, N1, L);
		prepro_zeropad(yexpand, y, N2, L);

		prepro_fft(wx, xexpand, L, L);
		prepro_fft(wy, yexpand, L, L);

		for (int i = 0; i < L; i++) {
			wzreal = wx[i].real()*wy[i].real() - wx[i].imag()*wy[i].imag();
			wzimag = wx[i].real()*wy[i].imag() - wx[i].imag()*wy[i].real();
			wz[i] = complex<float>(wzreal, wzimag);
		}

		prepro_ifft(z, wz, L, L);

		free(xexpand);
		free(yexpand);
		free(wx);
		free(wy);
		free(wz);
	}

	/*!
	 * @brief  中心阶段
	 * @details
	 */
	void RawDataFileProcessing::prepro_truncation(float * y, float * x, int N, int M)
	{
		if (N % 2 != 0 && M % 2 != 0) {
			for (int i = 0; i < (N - M) / 2; i++) {
				y[i] = x[i + (N - M) / 2];
				y[M - 1 - i] = x[N - 1 - (N - M) / 2 - i];
			}
		}

		if (N % 2 != 0 && M % 2 == 0) {
			for (int i = 0; i < M / 2; i++) {
				y[i] = x[i + (N - M - 1) / 2];
				y[M - 1 - i] = x[N - 1 - (N - M - 1) / 2 - i];
			}
		}

		if (N % 2 == 0 && M % 2 == 0) {
			for (int i = 0; i < M / 2; i++) {
				y[i] = x[i + (N - M) / 2];
				y[M - 1 - i] = x[N - 1 - (N - M) / 2 - i];
			}
		}

		if (N % 2 == 0 && M % 2 != 0) {
			for (int i = 0; i < (M - 1) / 2; i++) {
				y[i] = x[i + (N - M + 1) / 2];
				y[M - 1 - i] = x[N - 1 - (N - M + 1) / 2 - i];
			}
			y[(M - 1) / 2] = (y[(M + 1) / 2] + y[(M - 3) / 2]) / 2;
		}
	}

	/*!
	 * @brief  直方图
	 * @details
	 */
	void RawDataFileProcessing::prepro_histgram(int * doa, float * mu, int N)
	{
		float * mur = (float *)malloc(N * sizeof(float));
		prepro_round(mur, mu, N);

		for (int i = 0; i < 360; i++) {
			doa[i] = 0;
			for (int j = 0; j < N; j++) {
				if (mur[j] == i) {
					doa[i] = doa[i] + 1;
				}
			}
		}
		free(mur);
	}
 
	/*!
	 * @brief  累积高斯分布图
	 * @details
	 */
	void RawDataFileProcessing::prepro_gaussgram(float * doa, float * mu, int N)
	{
		float muvar = 0;
		float mumean = 0;

		for (int i = 0; i < N; i++) {
			mumean = mumean + mu[i] / N;
		}

		for (int i = 0; i < N; i++) {
			muvar = muvar + pow(mu[i] - mumean, 2) / N;
		}

		float mustd = sqrt(muvar);
		float coff1 = sqrt(2 * PI) * mustd / N;
		float coff2 = -2 * muvar;
		float * doaexpand = (float *)malloc(720 * sizeof(float));

		for (int i = -180; i < 540; i++) {
			doaexpand[i + 180] = 0;
			for (int j = 0; j < N; j++) {
				doaexpand[i + 180] = doaexpand[i + 180] + coff1 * exp(coff2 * pow(i - mu[j], 2));
			}
		}

		for (int i = 0; i < 360; i++) {
			if (i < 180) {
				doa[i] = doaexpand[i + 180] + doaexpand[i + 540];
			}
			if (i >= 180) {
				doa[i] = doaexpand[i + 180] + doaexpand[i - 180];
			}
		}
		free(doaexpand);
	}

	/*!
	 * @brief  汉明窗
	 * @details
	 */
	void RawDataFileProcessing::prepro_hamming(float * xh, float * x, int N)
	{
		int M;
		float * w = (float *)malloc(N * sizeof(float));

		if (N % 2 != 0) {
			M = (N + 1) / 2;
		}
		else {
			M = N / 2;
		}
		for (int i = 0; i < M; i++) {
			w[i] = 0.54 - 0.46*cos(2 * PI * i / (N - 1));
			w[N - 1 - i] = w[i];
		}
		prepro_mul(xh, x, w, N);
		free(w);
	}

	/*!
	 * @brief  低通滤波器设计
	 * @details
	 */
	float RawDataFileProcessing::prepro_fir_lowpass(int i, int M, float fhp)
	{
		float coff;
		int delte = i - M;
		if (delte == 0) {
			coff = fhp;
		}
		else {
			coff = sin(fhp * PI *delte) / (PI *delte);
		}
		return coff;
	}

	/*!
	 * @brief  高通滤波器设计
	 * @details 
	 */
	float RawDataFileProcessing::prepro_fir_highpass(int i, int M, float flp)
	{
		float coff;
		int delte = i - M;
		if (delte == 0) {
			coff = 1 - flp;
		}
		else {
			coff = (sin(PI * delte) - sin(flp * PI *delte)) / (PI *delte);
		}
		return coff;
	}

	/*!
	 * @brief  带通滤波器设计
	 * @details
	 */
	float RawDataFileProcessing::prepro_fir_bandpass(int i, int M, float flp, float fhp)
	{
		float coff;
		int delte = i - M;
		if (delte == 0) {
			coff = fhp - flp;
		}
		else {
			coff = (sin(fhp * PI * delte) - sin(flp * PI *delte)) / (PI *delte);
		}
		return coff;
	}

	/*!
	 * @brief  带阻滤波器设计
	 * @details
	 */
	float RawDataFileProcessing::prepro_fir_bandlock(int i, int M, float flp, float fhp)
	{
		float coff;
		int delte = i - M;
		if (delte == 0) {
			coff = 1 + flp - fhp;
		}
		else {
			coff = (sin(flp *PI *delte) + sin(PI *delte) - sin(fhp *PI *delte)) / (PI *delte);
		}
		return coff;
	}

	/*!
	 * @brief  右补零
	 * @details
	 */
	void RawDataFileProcessing::prepro_zeropad(float * y, float * x, int N, int M)
	{
		for (int i = 0; i < M; i++) {
			if (i < N) {
				y[i] = x[i];
			}
			else {
				y[i] = 0;
			}
		}
	} 

	/*!
	 * @brief  向量乘法
	 * @details
	 */
	void RawDataFileProcessing::prepro_mul(float * z, float * x, float * y, int N)
	{
		for (int i = 0; i < N; i++) {
			z[i] = x[i] * y[i];
		}
	}

	/*!
	 * @brief  向量与常数乘
	 * @details
	 */
	void  RawDataFileProcessing::prepro_constrantmul(float * z, float * x, float a, int N)
	{
		for (int i = 0; i < N; i++) {
			z[i] = x[i] * a;
		}
	}

	/*!
	 * @brief  寻找序列中最接近某值的位置
	 * @details 
	 */
	int RawDataFileProcessing::prepro_min(float * x, int N, float a)
	{
		int p;
		float yp;
		float *y = (float *)malloc(N * sizeof(float));
		for (int i = 0; i < N; i++) {
			y[i] = abs(x[i] - a);
			if (i == 0 || yp > y[i]) {
				p = i;
				yp = y[i];
			}
		}
		return p;
	}

	/*!
	 * @brief  向量取整
	 * @details 
	 */
	void RawDataFileProcessing::prepro_round(float *y, float *x, int N)
	{
		float delta;
		for (int i = 0; i < N; i++) {
			delta = x[i] - int(x[i]);
			if (delta < 0.5) {
				y[i] = int(x[i]);
			}
			else {
				y[i] = int(x[i]) + 1;
			}
		}
	}

	/*!
	 * @brief  最小二乘拟合去趋势
	 * @details 
	 */
	void RawDataFileProcessing::prepro_steady(float * z, float * y, int N)
	{
		float sumx = 0;
		float sumy = 0;
		float sumxx = 0;
		float sumxy = 0;

		for (int i = 0; i < N; i++) {
			sumx = sumx + i;
			sumy = sumy + y[i];
			sumxx = sumxx + i * i;
			sumxy = sumxy + i * y[i];
		}

		float k = (sumxy - sumx * sumy) / (sumxx - sumx * sumx);
		float b = (sumxx * sumy - sumx * sumxy) / (sumxx - sumx * sumx);
		for (int i = 0; i < NULL; i++) {
			z[i] = y[i] - (k *i + b);
		}
	}

	/*!
	 * @brief  按坐标截断
	 * @details 
	 */
	void RawDataFileProcessing::prepro_cutarray(float * y, float * x, int N1, int N2)
	{
		for (int i = N1; i < N2; i++) {
			y[i] = x[i - N1];
		}
	}

}//namespace Related 
