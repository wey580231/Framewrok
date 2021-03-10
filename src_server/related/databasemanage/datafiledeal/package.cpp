#include "package.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

Package::Package(QObject *parent)
    : QObject(parent)
{
}

Package::~Package()
{
}

/**
 * @brief Package::batchfile
 * @param indir  : 原始数据文件路径
 * @param savedir
 */
void Package::batchfile(QString indir, QString savedir)
{
    //[] 遍历文件夹
    //要判断路径是否存在
    QDir dir(indir);
    if(!dir.exists())  {
        qDebug() << "it is not true path";
        return ;
    }
    QStringList filters;
    filters<<QString("*.dat");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);//实现对文件的过滤
    dir.setSorting(QDir::Size | QDir::Reversed);//实现对文件输出的排序
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    QStringList string_list;

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString absolute_file_path = fileInfo.absoluteFilePath();
        // 处理单个文件
        embedfile(indir, savedir, fileInfo.fileName());
    }
}

/**
 * @brief embedfile
 * @param infi1e
 * @param savedir
 * @param fi1ename
 * @return
 */
int Package::embedfile(QString infi1e, QString savedir, QString fi1ename)
{
    QString t_tempSaveDir = savedir;
    QString fnf;
    QString fns;
    int Totalframe;

    HeadBufStruct *pheadbuf;
    int *ptr;
    unsigned char *presult;
    int frame;

    FILE * fid1 = NULL;                     /*!< 原始文件 */
    FILE * fid2 = NULL;                     /*!< 结果数据文件 */
    FILE * fid3 = NULL;                     /*!< 结果音频文件 */

    // 信号处理机时间
    unsigned int DSP_Second;
    unsigned int DSP_Minute;
    unsigned int DSP_Hour ;
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

    int *doa = (int *)malloc(360* sizeof(int));
    float  * buind_filer = (float *)malloc(1025 * sizeof(float));
    float  * longP = (float *)malloc(snap * sizeof(float));
    float  * longX = (float *)malloc(snap * sizeof(float));
    float  * longY = (float *)malloc(snap * sizeof(float));
    float  * longZ = (float *)malloc(snap * sizeof(float));

    float  * Ix = (float *)malloc(fs * sizeof(float));
    float  * Iy = (float *)malloc(fs * sizeof(float));

    for (int i = 0; i< 1025; i++) {
        buind_filer[i] = prepro_fir_highpass(i, 513, 100.0 / sr);
    }

    for (int  i = 0; i< snap; i++) {
        longP[i] = 0;
        longX[i] = 0;
        longY[i] = 0;
        longZ[i] = 0;
    }

    QString strRb = QStringLiteral("rb");
    QString filepath = infi1e+ QString("/") + fi1ename;
    QFile t_file(filepath);
    bool open = t_file.open(QFile::ReadOnly);

    if(!t_file.exists() && !open){
        qDebug() << "open file Fail!" ;
        return  0;
    }

    fid1 = fopen(filepath.toLocal8Bit().data(), strRb.toLocal8Bit().data());
    if (fid1 == NULL) {
        cout << "Cannot open" <<endl ;
    }
    //[] 获取总帧数
    Totalframe = checkframes(fid1);

    //处理每一帧数据
    for (int ii = 0; ii < Totalframe-1; ii++)
    {
        qDebug() << "_________file____1____" << ii;

        int bufLength = 512 + 320 * 1024 + 1536;
        char * t_buff = new char[512 + 320 * 1024 + 1536];
        t_file.read(t_buff, bufLength);
        pheadbuf = (HeadBufStruct *)t_buff;

        ptr = (int *)(t_buff + 512);
        presult = (unsigned char *)(t_buff + 512 + 320 * 1024);
        frame = pheadbuf->CurrentFlameNum;

        //读取时间数据
        DSP_Second	= ((pheadbuf->TimeBuf[0]) & 0x0f) + ((((pheadbuf->TimeBuf[0]) + 0x70) >> 4) * 10);
        DSP_Minute	= ((pheadbuf->TimeBuf[1]) & 0x0f) + ((((pheadbuf->TimeBuf[1]) + 0x70) >> 4) * 10);
        DSP_Hour	= ((pheadbuf->TimeBuf[2]) & 0x0f) + ((((pheadbuf->TimeBuf[2]) + 0x30) >> 4) * 10);
        DSP_Day		= ((pheadbuf->TimeBuf[4]) & 0x0f) + ((((pheadbuf->TimeBuf[4]) + 0x30) >> 4) * 10);
        DSP_Mouth	= ((pheadbuf->TimeBuf[5]) & 0x0f) + ((((pheadbuf->TimeBuf[5]) + 0x10) >> 4) * 10);
        DSP_Year	= ((pheadbuf->TimeBuf[6]) & 0x0f) + ((((pheadbuf->TimeBuf[6]) + 0xf0) >> 4) * 10) +2000;

        DSP_Time = QString("%1%2%3%4%5%6").arg(QString::number(DSP_Year))
                .arg(QString::number(DSP_Mouth)).arg(QString::number(DSP_Day))
                .arg(QString::number(DSP_Hour)).arg(QString::number(DSP_Minute))
                .arg(QString::number(DSP_Second));

        //读取滑翔机数据
        HXJ_WaterDepth	= *((unsigned short *) &(presult[1488]));	// 10倍深度
        HXJ_LongD		= *((unsigned short *) &(presult[1490]));
        HXJ_LongM		= *((unsigned short *) &(presult[1492]));
        HXJ_LongS		= *((unsigned short *) &(presult[1494]));
        HXJ_LatD		= *((unsigned short *) &(presult[1496]));
        HXJ_LatM		= *((unsigned short *) &(presult[1498]));
        HXJ_LatS		= *((unsigned short *) &(presult[1500]));

        HXJ_Year		= presult[1502];
        HXJ_Month		= presult[1503];
        HXJ_Day			= presult[1504];
        HXJ_Hour		= presult[1505];
        HXJ_Minute		= presult[1506];
        HXJ_Second      = presult[1507];
        HXJ_Long = HXJ_LongD + (float)HXJ_LongM / 60.0 + (float)HXJ_LongS / 3600;
        HXJ_Lat = HXJ_LatD + (float)HXJ_LatM / 60.0 + (float)HXJ_LatS / 3600;
        HXJ_Time  = QString("%1%2%3%4%5%6").arg(QString::number(HXJ_Year))
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
        RR[2][1] = sin(FuYangJiao) ;
        RR[2][2] = cos(HengGunJiao) * cos(FuYangJiao);

        /************************* 每百帧数据存储音频文件 ********************************/
        if(ii == 0){
            int mvFrame = frame/100;
            fnf = QString("%1").arg(QString::number(mvFrame*100));

            QString t_strWavFileName = QString("%1/signal/%2.wav").arg(t_tempSaveDir).arg(fnf);
            QString  t_strW = QString("w");
            fid3 = fopen(t_strWavFileName.toLocal8Bit().data(), t_strW.toLocal8Bit().data());
        }
        else
        {
            int index = ii%100;
            if(index == 0){
                //[]
                fclose(fid3);

                int mvFrame = frame/100;
                fnf = QString("%1").arg(QString::number(mvFrame));

                QString t_strWavFileName = QString("%1/signal/%2.wav").arg(t_tempSaveDir).arg(fnf);
                QString  t_strW = QString("w");
                fid3 = fopen(t_strWavFileName.toLocal8Bit().data(), t_strW.toLocal8Bit().data());
            }
        }

        //四路通道数据读取
        for (int i = 0; i < fs; i++) {
            tx = ptr[4 * i + 1] * COFF;
            ty = ptr[4 * i + 2] * COFF;
            tz = ptr[4 * i + 3] * COFF;

            ap[i] = ptr[4 * i + 0] * COFF;
            ax[i] = RR[0][0] * tx + RR[0][1] * ty + RR[0][2] * tz;
            ay[i] = RR[1][0] * tx + RR[1][1] * ty + RR[1][2] * tz;
            az[i] = RR[2][0] * tx + RR[2][1] * ty + RR[2][2] * tz;

            if (fid3 != NULL) {
                QString wavdata = QString("%f %f %f %f\n");
                fprintf(fid3, wavdata.toLocal8Bit().data(), ap[i], ax[i], ay[i], az[i]);
            }
        }

        //高通过滤
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
        if (ii == 0) {
            QString savefilePath;
            savefilePath = QString("%1/%2").arg(t_tempSaveDir).arg(DSP_Time);

            // 检测文件是否存在
            int  judge  = _access(savefilePath.toLocal8Bit().data(), 0);
            if (judge == 0) {

            }
            QString savefileName;
            savefileName.append(savefilePath).append(QString(".dat"));
            QString strW = QString("w");
            fid2 = fopen(savefileName.toLocal8Bit().data(), strW.toLocal8Bit().data());
        }

        QString data = QString("%d %s %s %f  %f  %d  %f %f %f");
        fprintf(fid2, data.toLocal8Bit().data(), pheadbuf->CurrentFlameNum, DSP_Time.toLocal8Bit().data(), HXJ_Time.toLocal8Bit().data(),
                HXJ_Long, HXJ_Lat,HXJ_WaterDepth, Heading, Roll, Pitch);
        for(int i = 0; i< 360; i++){
            QString data2 = QString("%d");
            fprintf(fid2, data2.toLocal8Bit().data(), doa[i]);
        }
        QString data3 = QString("\n");
        fprintf(fid2, data3.toLocal8Bit().data());
    }
    fclose(fid1);
    fclose(fid2);

    free(Ix);
    free(Iy);
    free(longP);
    free(longX);
    free(longY);
    free(longZ);
    free(doa);
    free(buind_filer);
    return 0;
}

// 查看文件帧数
int Package::checkframes(FILE * fid)
{
    int totalFrame;
    __int64 nFileLen;
    _fseeki64(fid, 0, SEEK_END);
    nFileLen = _ftelli64(fid);
    totalFrame = nFileLen / (322 *1024);
    _fseeki64(fid, 0, SEEK_SET);
    return totalFrame;
}

// 单帧滤波
void Package::prepro_channalfilter(float * result, float * channal, float * build_filter, int N1, int N2)
{
    float  * windowadd = (float *)malloc(N1 * sizeof(float));
    float  * convresult = (float *)malloc((N1+N2-1) * sizeof(float));

    prepro_hamming(windowadd, channal, N1);
    prepro_conv(convresult, windowadd, build_filter, N1, N2);
    prepro_truncation(result, convresult, N1 + N2 - 1, N1);

    free(windowadd);
    free(convresult);
}

// 快拍更新
void Package::prepro_channalupdate(float * result, float * longchannal, float * channal, int N1, int N2)
{
    for (int i = 0; i< N1-N2 ; i++){
        result[i] = longchannal[i + N2];
    }
    for (int i = N1 - N2; i < N1; i++) {
        result[i] = channal[i - N1 + N2];
    }
}

//复声强
void Package::prepro_mulpower(float * II, float * P, float * D, int N1, int N2)
{
    int Prop = 4;
    int frames = (N1 - N2) * Prop / fs + 1;
    int ns;
    float * shortP = (float *)malloc(N2 * sizeof(float));
    float * shortD = (float *)malloc(N2 * sizeof(float));

    complex<float> * wp = (complex<float> *)malloc(N2 * sizeof(complex<float>));
    complex<float> * wd = (complex<float> *)malloc(N2 * sizeof(complex<float>));

    for (int j = 0; j< int(N2 / 2);j++) {
        II[j] = 0;
    }

    for (int i = 0; i < frames; i++) {
        ns = i * N2 / 4;
        for (int j = 0; j < N2; j++) {
            shortP[j] = P[ns + j];
            shortD[j] = D[ns + j];
        }

        prepro_fft(wp, shortP, N2, N2);
        prepro_fft(wd, shortD, N2, N2);

        for (int j = 0; j < int(N2 / 2); j++) {
            II[j] = II[j] + wp[j].real() *wd[j].real() + wp[j].imag() *wd[j].imag();
        }
    }

    free(shortP);
    free(shortD);
    free(wp);
    free(wd);
}

//互谱法
void Package::prepro_crosspectrum(int * doa, float * Ix, float * Iy)
{
    int N1 = int(200 * fs / sr);
    int N2 = int(3000 * fs / sr);
    float *theta = (float*)malloc((N2 - N1) * sizeof(float));

    for (int j = N1; j< N2; j++)
    {
        theta[j - N1] = 90 - atan2(Iy[j], Ix[j]) / PI * 180;
        if (theta[j - N1] < 0) {
            theta[j - N1] = theta[j - N1] + 360;
        }
    }

    prepro_histgram(doa, theta, N2 - N1);
    free(theta);
}

//滤波器设计
void Package::prepro_fir(float * coff, int N, float flp, float fhp, int mode)
{
    int M;
    if (N %2 != 0) {
        M = (N - 1) / 2;
    }
    else
    {
        M = N / 2;
    }

    if (mode < 1 || mode > 4)
    {
        printf("%s\n", "");
    }
    else
    {
        switch (mode)
        {
        case 1:		//低通
        {
            for (int i = 0; i< M+1; i++)
            {
                coff[i] = prepro_fir_lowpass(i, M, fhp);
                coff[N - 1 - i] = coff[i];
            }
        }
            break;
        case 2: //高通
        {
            for (int i = 0; i < M + 1; i++)
            {
                coff[i] = prepro_fir_highpass(i, M, flp);
                coff[N - 1 - i] = coff[i];
            }
        }
            break;
        case 3: //带通
        {
            for (int i = 0; i < M + 1; i++)
            {
                coff[i] = prepro_fir_bandpass(i,M, flp, fhp);
                coff[N - 1 - i] = coff[i];
            }
        }
            break;
        case 4:		//带阻
        {
            for (int i = 0; i < M + 1; i++)
            {
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

// fourier变换
void Package::prepro_fft(complex<float>* result, float * x, int N, int M)
{
    //fftw_complex
}

void Package::prepro_ifft(float * result, complex<float>* x, int N, int M)
{
}

// 快速卷积
void Package::prepro_conv(float * z, float * x, float * y, int N1, int N2)
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

    for (int i = 0; i< L; i++)
    {
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

//中心阶段
void Package::prepro_truncation(float * y, float * x, int N, int M)
{
    if (N % 2 != 0 && M % 2 != 0) {
        for (int i = 0; i < (N-M)/2; i++) {
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

    if (N % 2  == 0 && M % 2 != 0) {
        for (int i = 0; i < (M - 1) / 2; i++) {
            y[i] = x[i + (N - M + 1) / 2];
            y[M - 1 - i] = x[N - 1 - (N - M + 1) / 2 - i];
        }
        y[(M - 1) / 2] = (y[(M +1) / 2 ] + y[(M - 3) / 2])/2;
    }
}

// 直方图
void Package::prepro_histgram(int * doa, float * mu, int N)
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

//累积高斯分布图
void Package::prepro_gaussgram(float * doa, float * mu, int N)
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

    for (int i = -180; i < 540; i++)
    {
        doaexpand[i + 180] = 0;
        for (int j = 0; j < N; j++)
        {
            doaexpand[i + 180] = doaexpand[i + 180] + coff1*exp(coff2 * pow(i - mu[j], 2));
        }
    }

    for (int i = 0; i < 360; i++)
    {
        if (i < 180) {
            doa[i] = doaexpand[i + 180] + doaexpand[i + 540];
        }
        if (i >= 180) {
            doa[i] = doaexpand[i + 180] + doaexpand[i - 180];
        }
    }

    free(doaexpand);
}

// 汉明窗
void Package::prepro_hamming(float * xh, float * x, int N)
{
    int M;
    float * w = (float *)malloc(N * sizeof(float));

    if (N % 2 != 0) {
        M = (N + 1) / 2;
    }
    else {
        M = N / 2;
    }

    for (int i = 0; i< M; i++)
    {
        w[i] = 0.54 - 0.46*cos(2 * PI * i/(N-1));
        w[N - 1 - i] = w[i];
    }
    prepro_mul(xh, x, w, N);
    free(w);
}


//低通滤波器设计
float Package::prepro_fir_lowpass(int i, int M, float fhp)
{
    float coff;
    int delte = i - M;
    if (delte == 0) {
        coff = fhp;
    }
    else
    {
        coff =  sin(fhp * PI *delte) / (PI *delte);
    }
    return coff;
}

//高通滤波器设计
float Package::prepro_fir_highpass(int i, int M, float flp)
{
    float coff;
    int delte = i - M;
    if (delte == 0) {
        coff = 1 - flp;
    }
    else
    {
        coff = (sin( PI * delte) - sin(flp * PI *delte)) / (PI *delte);
    }
    return coff;
}

//带通滤波器设计
float Package::prepro_fir_bandpass(int i, int M, float flp, float fhp)
{
    float coff;
    int delte = i - M;
    if (delte == 0) {
        coff = fhp - flp ;
    }
    else
    {
        coff = (sin(fhp * PI * delte) - sin(flp * PI *delte)) / (PI *delte);
    }

    return coff;
}

//带阻滤波器设计
float Package::prepro_fir_bandlock(int i, int M, float flp, float fhp)
{
    float coff;
    int delte = i - M;
    if (delte == 0) {
        coff = 1 + flp - fhp;
    }
    else
    {
        coff = (sin(flp *PI *delte) + sin(PI *delte) - sin(fhp *PI *delte))/(PI *delte);
    }

    return coff;
}

//右补零
void Package::prepro_zeropad(float * y, float * x, int N, int M)
{
    for (int i = 0; i < M; i++)
    {
        if (i < N)
        {
            y[i] = x[i];
        }
        else {
            y[i] = 0;
        }
    }
}

//向量乘法
void Package::prepro_mul(float * z, float * x, float * y, int N)
{
    for (int i = 0; i < N; i++)
    {
        z[i] = x[i] * y[i];
    }
}

//向量与常数乘
void  Package::prepro_constrantmul(float * z, float * x, float a, int N)
{
    for (int i = 0; i < N; i++)
    {
        z[i] = x[i] * a;
    }
}

// 寻找序列中最接近某值的位置
int Package::prepro_min(float * x, int N, float a)
{
    int p;
    float yp;
    float *y = (float *)malloc(N * sizeof(float));
    for (int i = 0; i < N; i++)
    {
        y[i] = abs(x[i] - a);
        if (i == 0 || yp > y[i]) {
            p = i;
            yp = y[i];
        }
    }
    return p;
}

// 向量取整
void Package::prepro_round(float *y, float *x, int N)
{
    float delta;
    for (int i = 0; i < N; i++) {
        delta = x[i] - int(x[i]);
        if (delta < 0.5)
        {
            y[i] = int(x[i]);
        }
        else
        {
            y[i] = int(x[i])+1;
        }
    }
}

//最小二乘拟合去趋势
void Package::prepro_steady(float * z, float * y, int N)
{
    float sumx = 0;
    float sumy = 0;
    float sumxx = 0;
    float sumxy = 0;

    for (int i = 0; i < N; i++) {
        sumx = sumx + i;
        sumy = sumy + y[i];
        sumxx = sumxx + i*i;
        sumxy = sumxy + i *y[i];
    }

    float k = (sumxy - sumx * sumy)/(sumxx - sumx* sumx);
    float b = (sumxx * sumy - sumx * sumxy) / (sumxx - sumx * sumx);
    for (int i = 0; i < NULL; i++ ) {
        z[i] = y[i] - (k *i + b);
    }
}

// 按坐标截断
void Package::prepro_cutarray(float * y, float * x, int N1, int N2)
{
    for (int i = N1; i < N2; i++) {
        y[i] = x[i - N1];
    }
}


