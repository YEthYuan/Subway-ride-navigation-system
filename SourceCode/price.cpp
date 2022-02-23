#include "price.h"

//��������  PriceCal
//���ߣ�    ԬҲ
//���ڣ�    2021/02/19
//���ܣ�    ����Ʊ��
//��������� �˳����(double)
//����ֵ��   ����Ʊ��(int)
//�޸ļ�¼��
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

//��������  CongestionCal
//���ߣ�    ԬҲ
//���ڣ�    2021/02/19
//���ܣ�    ����ӵ����
//��������� ��·��š���ǰʱ��
//����ֵ��   ӵ����(double)
//�޸ļ�¼��
double CongestionCal(int lineID, int t_sec)
{
	double ret = 0.0;
    if(line[lineID].cong_flag==0)//Ĭ��ģʽ�£�����Ԥ������ж�
    {
        if (lineID == 1 || lineID == 3)//���°�����
        {
            if (Time2Sec(TimeHMS(7, 30, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(9, 0, 0)))
                ret = 0.8;//7:30-9:00��ӵ����80%
            else if (Time2Sec(TimeHMS(16, 30, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(18, 30, 0)))
                ret = 0.75;//16:30-18:30��ӵ����75%
            else
                ret = 0.4;//����ʱ��ӵ����40%
        }
        else if (lineID == 8)//��������
        {
            if (Time2Sec(TimeHMS(9, 30, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(15, 0, 0)))
                ret = 0.65;//9:30-15:00��ӵ����65%
            else
                ret = 0.2;//����ʱ��ӵ����20%
        }
        else if (lineID == 6 || lineID == 7)//��������
        {
            if (Time2Sec(TimeHMS(19, 0, 0)) <= t_sec && t_sec <= Time2Sec(TimeHMS(22, 0, 0)))
                ret = 0.65;//19:00-22:00��ӵ����65%
            else
                ret = 0.15;//����ʱ��ӵ����15%
        }
        else if (lineID == 2 || lineID == 4)
        {
            ret = 0.5;//ȫ��ӵ����50%
        }
    }
    else if(line[lineID].cong_flag==1)//1~�ֶ�����
    {
        ret=0.0;
    }
    else if(line[lineID].cong_flag==2)//2~�ֶ�һ��
    {
        ret=0.5;
    }
    else if(line[lineID].cong_flag==3)//3~�ֶ�ӵ��
    {
        ret=1.0;
    }
	return ret;
}

//��������  StationDist
//���ߣ�    ԬҲ
//���ڣ�    2021/02/20
//���ܣ�    ����ĳ����·��վ���
//��������� ��·���
//����ֵ��   վ���(double)
//�޸ļ�¼��
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
        return 0;//��������
    }
    else if (0.2 < cong && cong <= 0.62)
    {
        return 1;//��������
    }
    else if (0.62 < cong && cong <= 1.0)
    {
        return 2;//ӵ������
    }
}
