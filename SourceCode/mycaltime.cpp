#include "define.h"
#include "route.h"
#include "mycaltime.h"

//函数名：  Sec2Time
//作者：    袁也
//日期：    2021/02/18
//功能：    将时间戳（从0：00开始的秒数）转化为时间(hh:mm:ss)
//输入参数： 时间戳
//返回值：  时间结构体(Time)
//         返回OK表示时间戳转换成功。
//修改记录：
TimeHMS Sec2Time(int n)
{
    TimeHMS ret = TimeHMS(n / 3600, n % 3600 / 60, n % 60);
	return ret;
}

//函数名：  Time2Sec
//作者：    袁也
//日期：    2021/02/18
//功能：    将时间(hh:mm:ss)转化为时间戳（从0：00开始的秒数）
//输入参数： 时间结构体(Time)
//返回值：  时间戳(int)
//         返回OK表示时间转换成功。
//修改记录：
int Time2Sec(TimeHMS n)
{
	return n.h * 3600 + n.m * 60 + n.s;
}

//函数名：  NextTrainTime
//作者：    袁也
//日期：    2021/02/19
//功能：    计算下一趟列车还有多久到站
//输入参数： 时间结构体(Time)
//返回值：  时间结构体(Time)
//修改记录：
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
        printf("到站时间：%02d:%02d:%02d\t\t", a.h, a.m, a.s);
		t += 60;
		a = Sec2Time(t);
        printf("发车时间：%02d:%02d:%02d\n", a.h, a.m, a.s);
	}
	return;
}

//函数名：  CalRealTime
//作者：    袁也
//日期：    2021/02/19
//功能：    将原始时间秒数转化为实际拥挤程度时间秒数
//输入参数： 当前线路、当前时刻秒数、需要转换的时间长度秒数
//返回值：   转换后的时间长度秒数
//修改记录：
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
