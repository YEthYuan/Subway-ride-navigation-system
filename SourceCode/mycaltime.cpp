#include "define.h"
#include "route.h"
#include "mycaltime.h"

//��������  Sec2Time
//���ߣ�    ԬҲ
//���ڣ�    2021/02/18
//���ܣ�    ��ʱ�������0��00��ʼ��������ת��Ϊʱ��(hh:mm:ss)
//��������� ʱ���
//����ֵ��  ʱ��ṹ��(Time)
//         ����OK��ʾʱ���ת���ɹ���
//�޸ļ�¼��
TimeHMS Sec2Time(int n)
{
    TimeHMS ret = TimeHMS(n / 3600, n % 3600 / 60, n % 60);
	return ret;
}

//��������  Time2Sec
//���ߣ�    ԬҲ
//���ڣ�    2021/02/18
//���ܣ�    ��ʱ��(hh:mm:ss)ת��Ϊʱ�������0��00��ʼ��������
//��������� ʱ��ṹ��(Time)
//����ֵ��  ʱ���(int)
//         ����OK��ʾʱ��ת���ɹ���
//�޸ļ�¼��
int Time2Sec(TimeHMS n)
{
	return n.h * 3600 + n.m * 60 + n.s;
}

//��������  NextTrainTime
//���ߣ�    ԬҲ
//���ڣ�    2021/02/19
//���ܣ�    ������һ���г����ж�õ�վ
//��������� ʱ��ṹ��(Time)
//����ֵ��  ʱ��ṹ��(Time)
//�޸ļ�¼��
TimeHMS NextTrainTime(TimeHMS nowt)
{
	int now_time = Time2Sec(nowt);
	int ret_sec = now_time - OPEN_TIME;
    TimeHMS ret = TimeHMS(0, 0, 0);
	if (ret_sec < 0 || ret_sec>61080)
	{
		ret.h = -1;
		ret.m = -1;
		ret.s = -1;
	}
	else
	{
		ret_sec %= 180;
		ret_sec = 180 - ret_sec;
		ret = Sec2Time(ret_sec);
	}
	return ret;
}

void PrintSchedule(void)
{
	int t;
	for (t = OPEN_TIME; t < CLOSE_TIME; t += 120)
	{
        TimeHMS a = Sec2Time(t);
        printf("��վʱ�䣺%02d:%02d:%02d\t\t", a.h, a.m, a.s);
		t += 60;
		a = Sec2Time(t);
        printf("����ʱ�䣺%02d:%02d:%02d\n", a.h, a.m, a.s);
	}
	return;
}

//��������  CalRealTime
//���ߣ�    ԬҲ
//���ڣ�    2021/02/19
//���ܣ�    ��ԭʼʱ������ת��Ϊʵ��ӵ���̶�ʱ������
//��������� ��ǰ��·����ǰʱ����������Ҫת����ʱ�䳤������
//����ֵ��   ת�����ʱ�䳤������
//�޸ļ�¼��
int CalRealTime(int lineid, int nowsec, int ori_time)
{
	int ret = 0;
	double cong = CongestionCal(lineid, nowsec);
	if (0.0 <= cong && cong <= 0.2)
	{
		ret = (int)round(LOOSE_RATIO * ori_time);
	}
	else if (0.2 < cong && cong <= 0.62)
	{
		ret = (int)round(NORMAL_RATIO * ori_time);
	}
	else if (0.62 < cong && cong <= 1.0)
	{
		ret = (int)round(CROWDED_RATIO * ori_time);
	}
	return ret;
}
