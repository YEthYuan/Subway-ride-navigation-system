#include "price.h"

//函数名：  PriceCal
//作者：    袁也
//日期：    2021/02/19
//功能：    计算票价
//输入参数： 乘车里程(double)
//返回值：   花费票价(int)
//修改记录：
int PriceCal(double dist)
{
	int price = 0;
	if (dist <= 4.0)
	{
		price = 2;
	}
	else if (dist > 4.0 && dist <= 12.0)
	{
		price = 2 + 1 * (int)ceil((dist - 4) / 4);
	}
	else if (dist > 12.0 && dist <= 24.0)
	{
		price = 4 + 1 * (int)ceil((dist - 12) / 6);
	}
	else if (dist > 24.0 && dist <= 40.0)
	{
		price = 6 + 1 * (int)ceil((dist - 24) / 8);
	}
	else if (dist > 40.0 && dist <= 50.0)
	{
		price = 8 + 1 * (int)ceil((dist - 40) / 10);
	}
	else
	{
		price = 9 + 1 * (int)ceil((dist - 50) / 20);
	}
	return price;
}

//函数名：  CongestionCal
//作者：    袁也
//日期：    2021/02/19
//功能：    计算拥挤度
//输入参数： 线路编号、当前时间
//返回值：   拥挤度(double)
//修改记录：
double CongestionCal(int lineID, int t_sec)
{
	double ret = 0.0;
    if(line[lineID].cong_flag==0)//默认模式下，根据预设进行判断
    {
        if (lineID == 1 || lineID == 3)//上下班类型
        {
            if (Time2Sec(TimeHMS(7, 30, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(9, 0, 0)))
                ret = 0.8;//7:30-9:00，拥挤度80%
            else if (Time2Sec(TimeHMS(16, 30, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(18, 30, 0)))
                ret = 0.75;//16:30-18:30，拥挤度75%
            else
                ret = 0.4;//其余时间拥挤度40%
        }
        else if (lineID == 8)//购物类型
        {
            if (Time2Sec(TimeHMS(9, 30, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(15, 0, 0)))
                ret = 0.65;//9:30-15:00，拥挤度65%
            else
                ret = 0.2;//其余时间拥挤度20%
        }
        else if (lineID == 6 || lineID == 7)//娱乐类型
        {
            if (Time2Sec(TimeHMS(19, 0, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(22, 0, 0)))
                ret = 0.65;//19:00-22:00，拥挤度65%
            else
                ret = 0.15;//其余时间拥挤度15%
        }
        else if (lineID == 2 || lineID == 4)
        {
            ret = 0.5;//全天拥挤度50%
        }
    }
    else if(line[lineID].cong_flag==1)//1~手动宽松
    {
        ret=0.0;
    }
    else if(line[lineID].cong_flag==2)//2~手动一般
    {
        ret=0.5;
    }
    else if(line[lineID].cong_flag==3)//3~手动拥挤
    {
        ret=1.0;
    }
	return ret;
}

//函数名：  StationDist
//作者：    袁也
//日期：    2021/02/20
//功能：    计算某条线路的站间距
//输入参数： 线路编号
//返回值：   站间距(double)
//修改记录：
double StationDist(int lineID)
{
	double ret = 0.0;
	ret = line[lineID].length / ((double)line[lineID].St_Num - 1.0);
	return ret;
}

int TrafficJudge(int lineID, int nowsec)
{
    double cong = CongestionCal(lineID, nowsec);
    if (0.0 <= cong && cong <= 0.2)
    {
        return 0;//宽松区间
    }
    else if (0.2 < cong && cong <= 0.62)
    {
        return 1;//正常区间
    }
    else if (0.62 < cong && cong <= 1.0)
    {
        return 2;//拥挤区间
    }
}
