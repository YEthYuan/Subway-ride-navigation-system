#include "define.h"
#include "route.h"
#include "mycaltime.h"
#include <cstring>
#include <QString>

vector<TransferNode> TN;
bool vis[MAX_LINE_NUM][MAX_STN_NUM];//DFS���ʱ�
vector<Route> R;
vector<Route> rank_list;
int nowt_sec;
//Line line[MAX_LINE_NUM];

//��������  route_main
//���ߣ�    ԬҲ
//���ڣ�    2021/02/19
//���ܣ�    ·���滮������
//��������� ��ǰʱ�䡢��ʼ��վ��·����ʼ��վ��š����ﳵվ��·�����ﳵվ���
//����ֵ��  ״ֵ̬(int)
//         
//�޸ļ�¼��
status route_main(TimeHMS now, int Sline, int SID, int Dline, int DID, int max_search_depth)
{
	Route nowR;
	nowt_sec = Time2Sec(now);
	R.clear();
	TN.clear();
	rank_list.clear();
	memset(vis, false, sizeof(bool) * MAX_LINE_NUM * MAX_STN_NUM);
	BuildTransferGraph(Sline, SID, Dline, DID);//��������ͼ
	//PrintTransferAdjList();
	nowR.way.push_back(FindStation(Sline, SID)->id);
	DFS(nowR, FindStation(Dline, DID)->id, 0, max_search_depth);
	//PrintDFSResult();
    //ProcessingRoute();
    //PrintResult();
	return OK;
}

//��������  ShowStartStation
//���ߣ�    ԬҲ
//���ڣ�    2021/02/19
//���ܣ�    ���벢��ʾ��ʼ��վ�������Ϣ
//��������� ��ǰʱ�䡢��ʵ��վ��·����ʼ��վ���
//����ֵ��  ״ֵ̬(int)
//         ����OK��ʾʱ���趨�ɹ���
//			����FLASE��ʾʱ�䲻������Ӫʱ���ڣ���Ҫ�������úϷ�ʱ��
//�޸ļ�¼��
//status ShowStartStation(TimeHMS now, int Sline, int SID)
//{
//	vector<Station>::iterator itSt;
//	for (itSt = line[Sline].st_list.begin(); itSt != line[Sline].st_list.end(); itSt++)
//	{
//		if (itSt->id.station_number == SID)
//			break;
//	}

//    QString outp;
//    outp=QString::fromLocal8Bit("������ʼվ���ǣ�");
//    printf("\n������ʼվ���ǣ�");
//    cout << itSt->name << endl;
//	if (itSt->transfer == true)
//	{
//        printf("��վ�ǻ���վ���ɻ���:");
//		vector<int>::iterator itInt;
//		for (itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
//		{
//            printf(" �人����%d����,", *itInt);
//		}
//		putchar('\n');
//	}
//	else
//	{
//        printf("��վ���ǻ���վ.\n");
//	}
//    TimeHMS result = NextTrainTime(now);
//	if (result.h == -1)
//	{
//        printf("��ǰ���ڵ�������Ӫʱ���ڣ���ı����ʱ�䣡\n");
//		return FALSE;
//	}
//	else if (result.m >= 2)
//	{
//        printf("�г��Ѿ���վ������%d��%d��󷢳�����ץ���ϳ���\n", result.m - 2, result.s);
//        printf("�����г�����%d��%d��󵽴����%d��%d�����վ.\n", result.m, result.s, result.m + 1, result.s);
//		return OK;
//	}
//	else
//	{
//        printf("�����г�����%d��%d��󵽴����%d��%d�����վ.\n", result.m, result.s, result.m + 1, result.s);
//		return OK;
//	}
//}

//��������  ShowDestStation
//���ߣ�    ԬҲ
//���ڣ�    2021/02/19
//���ܣ�    ���벢��ʾ���ﳵվ�������Ϣ
//��������� ���ﳵվ��·�����ﳵվ���
//����ֵ��  ״ֵ̬(int)
//         
//�޸ļ�¼��
//status ShowDestStation(int Dline, int DID)
//{
//	vector<Station>::iterator itSt;
//	for (itSt = line[Dline].st_list.begin(); itSt != line[Dline].st_list.end(); itSt++)
//	{
//		if (itSt->id.station_number == DID)
//			break;
//	}
//    printf("\n���ĵ���վ���ǣ�");
//    cout << itSt->name << endl;
//	if (itSt->transfer == true)
//	{
//        printf("��վ�ǻ���վ���ɻ���:");
//		vector<int>::iterator itInt;
//		for (itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
//		{
//            printf(" �人����%d����,", *itInt);
//		}
//		putchar('\n');
//	}
//	else
//	{
//        printf("��վ���ǻ���վ.\n");
//	}
//	return OK;
//}

status BuildTransferGraph(int Sline, int SID, int Dline, int DID)
{
	for (int i = 1; i < MAX_LINE_NUM; i++)
	{
		if (!openline[i])
			continue;
		vector<Station>::iterator itSt;
		for (itSt = line[i].st_list.begin(); itSt != line[i].st_list.end(); itSt++)//�ҵ�����վ
		{
			if (itSt->transfer == false && !(itSt->id.line == Sline && itSt->id.station_number == SID) && !(itSt->id.line == Dline && itSt->id.station_number == DID))
				continue;//������ǻ���վ����ĩվ��������һվ
			TransferNode NowInArray;
			NowInArray.st = itSt->id;
			TransferNode* tail = new TransferNode;
			NowInArray.next = tail;
			tail->next = NULL;
			vector<Station>::iterator itTemp;//��ʱ������
			int count = 0;//��վ������
			if (itSt != line[i].st_list.begin())//�����ǰ��վ������ս����ǰ�һ���վ
			{
				for (itTemp = itSt - 1; itTemp >= line[i].st_list.begin(); itTemp--)
				{
					count++;
					if (itTemp->transfer == false && !(itTemp->id.line == Sline && itTemp->id.station_number == SID) && !(itTemp->id.line == Dline && itTemp->id.station_number == DID))
					{
						if (itTemp == line[i].st_list.begin())
							break;
						else
							continue;
					}
					else
					{
						tail->count_station = count;
						tail->dist = count * line[i].length / line[i].St_Num;
						tail->isTransferEdge = false;
						tail->next = new TransferNode;
						tail->st = itTemp->id;
						tail->time_sec = (STOP_TIME + TRAVEL_TIME) * count;
						tail = tail->next;
						tail->next = NULL;
						break;
					}
					if (itTemp == line[i].st_list.begin())
						break;
				}
			}
			count = 0;
			if (itSt != line[i].st_list.end() - 1)//�����ǰ��վ����ĩս������һ���վ
			{
				for (itTemp = itSt + 1; itTemp < line[i].st_list.end(); itTemp++)
				{
					count++;
					if (itTemp->transfer == false && !(itTemp->id.line == Sline && itTemp->id.station_number == SID) && !(itTemp->id.line == Dline && itTemp->id.station_number == DID))
						continue;
					else
					{
						tail->count_station = count;
						tail->dist = count * line[i].length / line[i].St_Num;
						tail->isTransferEdge = false;
						tail->next = new TransferNode;
						tail->st = itTemp->id;
						tail->time_sec = (STOP_TIME + TRAVEL_TIME) * count;
						tail = tail->next;
						tail->next = NULL;
						break;
					}
				}
			}
			//�����˳�վ�����ڽӱ�
			if (itSt->transfer == true)
			{
				vector<int>::iterator itInt;
				for (itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
				{
					vector<Station>::iterator temp_itSt;
					for (temp_itSt = line[*itInt].st_list.begin(); temp_itSt != line[*itInt].st_list.end(); temp_itSt++)
					{
						if (temp_itSt->name == itSt->name)
						{
							tail->st = temp_itSt->id;
							tail->isTransferEdge = true;
							tail->transfer_time_sec = INTERCHANGE_TIME;
							tail->next = new TransferNode;
							tail = tail->next;
							tail->next = NULL;
						}
					}
				}
			}
			tail = NULL;
			TN.push_back(NowInArray);
		}
	}
	//����������ĩվҲ��ӵ�����վͼ��
	//if (FindStation(Sline, SID)->transfer == false)
	//{
	//	TransferNode frstStop;//�����վ
	//	frstStop.st.line = Sline;
	//	frstStop.st.station_number = SID;
	//	vector<Station>::iterator itSt = FindStation(Sline, SID);
	//	TransferNode* tail = new TransferNode;
	//	frstStop.next = tail;
	//	tail->next = NULL;
	//	vector<Station>::iterator itTemp;//��ʱ������
	//	int count = 0;//��վ������
	//	if (itSt != line[Sline].st_list.begin())//�����ǰ��վ������ս����ǰ�һ���վ
	//	{
	//		for (itTemp = itSt - 1; itTemp >= line[Sline].st_list.begin(); itTemp--)
	//		{
	//			count++;
	//			if (itTemp->transfer == false)
	//			{
	//				if (itTemp == line[Sline].st_list.begin())
	//					break;
	//				else
	//					continue;
	//			}
	//			else
	//			{
	//				tail->count_station = count;
	//				tail->dist = count * line[Sline].length / line[Sline].St_Num;
	//				tail->isTransferEdge = false;
	//				tail->next = new TransferNode;
	//				tail->st = itTemp->id;
	//				tail->time_sec = (STOP_TIME + TRAVEL_TIME) * count;
	//				vector<TransferNode>::iterator itTN = FindTransferInAdjList(tail->st.line, tail->st.station_number);
	//				TransferNode* q = itTN->next;
	//				for (;;)
	//				{
	//					if (q->st.line == Sline && q->st.station_number > SID)
	//						break;
	//				}
	//				if (q == itTN->next)
	//				{
	//					itTN->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				else
	//				{
	//					TransferNode* p = itTN->next;
	//					for (; p->next != q; p = p->next);
	//					p->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				for (q = itTN->next; q->next != NULL; q = q->next);
	//				q->count_station = count;
	//				q->dist = tail->dist;
	//				q->isTransferEdge = false;
	//				q->next = new TransferNode;
	//				q->st.line = Sline;
	//				q->st.station_number = SID;
	//				q->time_sec = tail->time_sec;
	//				q = q->next;
	//				q->next = NULL;
	//				tail = tail->next;
	//				tail->next = NULL;
	//				break;
	//			}
	//			if (itTemp == line[Sline].st_list.begin())
	//				break;
	//		}
	//	}
	//	count = 0;
	//	if (itSt != line[Sline].st_list.end() - 1)//�����ǰ��վ����ĩս������һ���վ
	//	{
	//		for (itTemp = itSt + 1; itTemp < line[Sline].st_list.end(); itTemp++)
	//		{
	//			count++;
	//			if (itTemp->transfer == false)
	//				continue;
	//			else
	//			{
	//				tail->count_station = count;
	//				tail->dist = count * line[Sline].length / line[Sline].St_Num;
	//				tail->isTransferEdge = false;
	//				tail->next = new TransferNode;
	//				tail->st = itTemp->id;
	//				tail->time_sec = (STOP_TIME + TRAVEL_TIME) * count;
	//				vector<TransferNode>::iterator itTN = FindTransferInAdjList(tail->st.line, tail->st.station_number);
	//				TransferNode* q = itTN->next;
	//				for (;;)
	//				{
	//					if (q->st.line == Sline && q->st.station_number < SID)
	//						break;
	//				}
	//				if (q == itTN->next)
	//				{
	//					itTN->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				else
	//				{
	//					TransferNode* p = itTN->next;
	//					for (; p->next != q; p = p->next);
	//					p->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				for (q = itTN->next; q->next != NULL; q = q->next);
	//				q->count_station = count;
	//				q->dist = tail->dist;
	//				q->isTransferEdge = false;
	//				q->next = new TransferNode;
	//				q->st.line = Sline;
	//				q->st.station_number = SID;
	//				q->time_sec = tail->time_sec;
	//				q = q->next;
	//				q->next = NULL;
	//				tail = tail->next;
	//				tail->next = NULL;
	//				break;
	//			}
	//		}
	//	}
	//	tail = NULL;
	//	TN.push_back(frstStop);
	//}

	//if (FindStation(Dline, DID)->transfer == false)
	//{
	//	//���ĩվ
	//	TransferNode lastStop;
	//	lastStop.st.line = Dline;
	//	lastStop.st.station_number = DID;
	//	vector<Station>::iterator itSt = FindStation(Dline, DID);
	//	TransferNode* tail = new TransferNode;
	//	lastStop.next = tail;
	//	tail->next = NULL;
	//	vector<Station>::iterator itTemp;//��ʱ������
	//	int count = 0;//��վ������
	//	if (itSt != line[Dline].st_list.begin())//�����ǰ��վ������ս����ǰ�һ���վ
	//	{
	//		for (itTemp = itSt - 1; itTemp >= line[Dline].st_list.begin(); itTemp--)
	//		{
	//			count++;
	//			if (itTemp->transfer == false)
	//			{
	//				if (itTemp == line[Dline].st_list.begin())
	//					break;
	//				else
	//					continue;
	//			}
	//			else
	//			{
	//				tail->count_station = count;
	//				tail->dist = count * line[Dline].length / line[Dline].St_Num;
	//				tail->isTransferEdge = false;
	//				tail->next = new TransferNode;
	//				tail->st = itTemp->id;
	//				tail->time_sec = (STOP_TIME + TRAVEL_TIME) * count;
	//				vector<TransferNode>::iterator itTN = FindTransferInAdjList(tail->st.line, tail->st.station_number);
	//				TransferNode* q = itTN->next;
	//				for (;;)
	//				{
	//					if (q->st.line == Dline && q->st.station_number > DID)
	//						break;
	//				}
	//				if (q == itTN->next)
	//				{
	//					itTN->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				else
	//				{
	//					TransferNode* p = itTN->next;
	//					for (; p->next != q; p = p->next);
	//					p->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				for (q = itTN->next; q->next != NULL; q = q->next);
	//				q->count_station = count;
	//				q->dist = tail->dist;
	//				q->isTransferEdge = false;
	//				q->next = new TransferNode;
	//				q->st.line = Dline;
	//				q->st.station_number = DID;
	//				q->time_sec = tail->time_sec;
	//				q = q->next;
	//				q->next = NULL;
	//				tail = tail->next;
	//				tail->next = NULL;
	//				break;
	//			}
	//			if (itTemp == line[Dline].st_list.begin())
	//				break;
	//		}
	//	}
	//	count = 0;
	//	if (itSt != line[Dline].st_list.end() - 1)//�����ǰ��վ����ĩս������һ���վ
	//	{
	//		for (itTemp = itSt + 1; itTemp < line[Dline].st_list.end(); itTemp++)
	//		{
	//			count++;
	//			if (itTemp->transfer == false)
	//				continue;
	//			else
	//			{
	//				tail->count_station = count;
	//				tail->dist = count * line[Dline].length / line[Dline].St_Num;
	//				tail->isTransferEdge = false;
	//				tail->next = new TransferNode;
	//				tail->st = itTemp->id;
	//				tail->time_sec = (STOP_TIME + TRAVEL_TIME) * count;
	//				vector<TransferNode>::iterator itTN = FindTransferInAdjList(tail->st.line, tail->st.station_number);
	//				TransferNode* q = itTN->next;
	//				for (;;)
	//				{
	//					if (q->st.line == Dline && q->st.station_number < DID)
	//						break;
	//				}
	//				if (q == itTN->next)
	//				{
	//					itTN->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				else
	//				{
	//					TransferNode* p = itTN->next;
	//					for (; p->next != q; p = p->next);
	//					p->next = q->next;
	//					free(q);
	//					q = NULL;
	//				}
	//				for (q = itTN->next; q->next != NULL; q = q->next);
	//				q->count_station = count;
	//				q->dist = tail->dist;
	//				q->isTransferEdge = false;
	//				q->next = new TransferNode;
	//				q->st.line = Dline;
	//				q->st.station_number = DID;
	//				q->time_sec = tail->time_sec;
	//				q = q->next;
	//				q->next = NULL;
	//				tail = tail->next;
	//				tail->next = NULL;
	//				break;
	//			}
	//		}
	//	}
	//	tail = NULL;
	//	TN.push_back(lastStop);
	//}
	return OK;
}

status PrintTransferAdjList(void)
{
	vector<TransferNode>::iterator itTN;
	for (itTN = TN.begin(); itTN != TN.end(); itTN++)
	{
        printf("\t%d%02d ", itTN->st.line, itTN->st.station_number);
		TransferNode* p;//�ڽӳ�վ����ָ��
		putchar('\t');
		for (p = itTN->next; p->next != NULL; p = p->next)
		{
            printf("\t%d%02d ", p->st.line, p->st.station_number);
			if (p->isTransferEdge == false)
			{
                printf("dist:%g, stnum:%d, time:%ds.", p->dist, p->count_station, p->time_sec);
			}
		}
		putchar('\n');
	}
	return OK;
}

void DFS(Route nowR, StNum Destination, int depth, int max_depth)
{
	if (depth > max_depth) return;
	StNum nowSt = nowR.way.back();
	vis[nowSt.line][nowSt.station_number] = true;
	//printf("%d%02d  ", nowSt.line, nowSt.station_number);
	if (nowSt.line == Destination.line && nowSt.station_number == Destination.station_number)
	{
		RouteEvaluation(nowR);//������·��
		R.push_back(nowR);
		//PrintDFSResult();
		vis[nowSt.line][nowSt.station_number] = false;
		return;
	}
	vector<TransferNode>::iterator itTN = FindTransferInAdjList(nowSt.line, nowSt.station_number);
	for (TransferNode* p = itTN->next; p->next != NULL; p = p->next)
	{
		if (vis[p->st.line][p->st.station_number] == false && !isInArray(nowR, p->st))
		{
			nowR.way.push_back(p->st);
			DFS(nowR, Destination, depth + 1, max_depth);
			nowR.way.pop_back();
		}
	}
	vis[nowSt.line][nowSt.station_number] = false;
	return;
}

vector<TransferNode>::iterator FindTransferInAdjList(int lineid, int stid)
{
	vector<TransferNode>::iterator ret;
	for (ret = TN.begin(); ret != TN.end(); ret++)
	{
		if (lineid != ret->st.line)
			continue;
		if (stid == ret->st.station_number)
			break;
	}
	return ret;
}

status PrintDFSResult(void)
{
	vector<Route>::iterator itR;
	for (itR = R.begin(); itR != R.end(); itR++)
	{
		vector<StNum>::iterator itSN;
		for (itSN = itR->way.begin(); itSN != itR->way.end(); itSN++)
		{
			if (itSN == itR->way.end() - 1)
                printf("%d%02d.\n", itSN->line, itSN->station_number);
			else
                printf("%d%02d-->", itSN->line, itSN->station_number);
		}
        TimeHMS t = Sec2Time(itR->time_sec);
        printf("ʱ�䣺%02d:%02d:%02d, ����:%g, Ʊ��:%d, ���˴���:%d, ��Ȩ�ɼ�:%g.\n", t.h, t.m, t.s, itR->dist, itR->price, itR->transfer_times, itR->weightedScore);
	}
	return OK;
}

bool isInArray(Route nowR, StNum s)
{
	vector<StNum>::iterator itSN;
	for (itSN = nowR.way.begin(); itSN != nowR.way.end(); itSN++)
	{
		if (itSN->line == s.line && itSN->station_number == s.station_number)
			return true;
	}
	return false;
}

status RouteEvaluation(Route& nowR)
{
	nowR.dist = 0.0;
	nowR.price = 0;
	nowR.time_sec = 0;
	nowR.transfer_times = 0;
    nowR.overall_traffic=0.0;
    memset(nowR.s2s_traffic, 0, 3*sizeof(int));//����ӵ����ͳ�ƣ�0~��������������1~һ������������2~ӵ����������
	nowR.weightedScore = 0.0;//��ʼ������
	vector<StNum>::iterator itSN;
	int then_time_sec = nowt_sec;
	for (itSN = nowR.way.begin(); itSN != nowR.way.end() - 1; itSN++)
	{
		vector<StNum>::iterator nxtNode = itSN + 1;
		vector<TransferNode>::iterator n = FindTransferInAdjList(itSN->line, itSN->station_number);
		TransferNode* p = n->next;
		for (; p->next != NULL; p = p->next)
		{
			if (p->st.line == nxtNode->line && p->st.station_number == nxtNode->station_number)
				break;
		}
		if (itSN->line == nxtNode->line)//ͬһ�����ϣ��˳���Ϊ
		{
			for (int count = p->count_station; count != 0; count--)
			{
                nowR.s2s_traffic[TrafficJudge(p->st.line, then_time_sec)]++;
				int real_time_sec = CalRealTime(p->st.line, then_time_sec, STOP_TIME + TRAVEL_TIME);
				then_time_sec += real_time_sec;
			}
			nowR.dist += p->dist;
		}
		else//ͬվ��ͬ�ߣ�������Ϊ
		{
			then_time_sec += INTERCHANGE_TIME;
			nowR.transfer_times++;
		}
	}
    nowR.overall_traffic=(double)(nowR.s2s_traffic[2]*1.0+nowR.s2s_traffic[1]*0.2)/(nowR.s2s_traffic[0]+nowR.s2s_traffic[1]+nowR.s2s_traffic[2]);
	nowR.price = PriceCal(nowR.dist);
	nowR.time_sec = then_time_sec - nowt_sec;
	nowR.weightedScore = ComprehensiveEvaluation(nowR);
	return OK;
}

double ComprehensiveEvaluation(Route nowR)
{
//    nowR.dist;    ·�����
//    nowR.price;   ·�߼۸�
//    nowR.time_sec;·��ʱ��
//    nowR.transfer_times;  ���˴���
//    nowR.overall_traffic; ����ӵ����
	double ret = 0.0;
    ret = nowR.dist * 2.25 + nowR.price * 10.0 + nowR.time_sec * 0.12 + (double)nowR.transfer_times * 15.0+(1-nowR.overall_traffic)*80.0;
	return ret;
}

bool comprehensive_cmp(Route a, Route b)
{
	return a.weightedScore < b.weightedScore;
}

bool time_cmp(Route a, Route b)
{
	return a.time_sec < b.time_sec;
}

bool transfer_cmp(Route a, Route b)
{
    if(a.transfer_times!=b.transfer_times)
        return a.transfer_times < b.transfer_times;
    else
        return a.time_sec<b.time_sec;
}

bool price_cmp(Route a, Route b)
{
    if(a.price!=b.price)
        return a.price < b.price;
    else
        return a.time_sec<b.time_sec;
}

bool traffic_cmp(Route a, Route b)
{
    if(isTrafficEqual(a.overall_traffic,b.overall_traffic)==false)
        return a.overall_traffic < b.overall_traffic;
    else
        return a.time_sec<b.time_sec;
}

status ProcessingRoute(void)
{
	sort(R.begin(), R.end(), comprehensive_cmp);
	rank_list.push_back(R.front());//ranklist[0]
	rank_list.push_back(R[1]);//ranklist[1]
	rank_list.push_back(R[2]);//ranklist[2]
	rank_list.push_back(R[3]);//ranklist[3]
	sort(R.begin(), R.end(), time_cmp);
	rank_list.push_back(R.front());//ranklist[4]
	sort(R.begin(), R.end(), transfer_cmp);
	rank_list.push_back(R.front());//ranklist[5]
	return OK;
}

status PrintResult(void)
{
	vector<StNum>::iterator itSN;
	Route Ro = rank_list[0];
    TimeHMS begintime = Sec2Time(nowt_sec);
    TimeHMS tt = Sec2Time(Ro.time_sec);
	int forecast_sec = nowt_sec + Ro.time_sec;
    TimeHMS forecast = Sec2Time(forecast_sec);
    printf("\n\n=======================·���Ƽ�=======================\n\n");
    printf("\t����ʱ��:  %02d:%02d\t", begintime.h, begintime.m);
    cout << line[Ro.way.begin()->line].st_list[Ro.way.begin()->station_number - 1].name;
    cout << " -------> ";
    cout << line[Ro.way.back().line].st_list[Ro.way.back().station_number - 1].name << endl;
    printf("\t�ۺ��Ƽ�:  ��ʱ%02dʱ%02d��%02d�룬Ʊ��%dԪ������%d�Σ������%g���\n", tt.h, tt.m, tt.s, Ro.price, Ro.transfer_times, Ro.dist);
	if (forecast_sec > CLOSE_TIME)
	{
        printf("\t������Ӫʱ��Ԥ������ѡ������·�ߣ��������޷�����Ŀ�ĵأ�������ѡ��������ͨ��ʽ��\n");
	}
	else
	{
        printf("\tԤ�Ƶ���ʱ��:  %02d:%02d\n", forecast.h, forecast.m);
	}
    printf("���-(%d����--", Ro.way.begin()->line);
	for (itSN = Ro.way.begin(); itSN != Ro.way.end() - 1; itSN++)
	{
		vector<StNum>::iterator nxtNode = itSN + 1;
		if (itSN->line != nxtNode->line)
		{
            printf("-->");
            cout << line[itSN->line].st_list[itSN->station_number - 1].name;
            printf("--%d����)-{����%d����}-(%d����--", itSN->line, nxtNode->line, nxtNode->line);
		}
		else
		{
			int delta = JudgeDirection(itSN->station_number, nxtNode->station_number);
			for (int i = itSN->station_number - 1; i != nxtNode->station_number; i += delta)
			{
				if (i == nxtNode->station_number - delta || i == nxtNode->station_number)
					break;
                printf("-->");
                cout << line[itSN->line].st_list[i].name;
			}
		}
	}
    printf("-->");
    cout << line[itSN->line].st_list[itSN->station_number - 1].name;
    printf("--%d����)-�յ�\n\n", itSN->line);
	Ro = rank_list[4];
	tt=Sec2Time(Ro.time_sec);
	forecast_sec = nowt_sec + Ro.time_sec;
	forecast = Sec2Time(forecast_sec);
    printf("\t��ʱ����:  ��ʱ%02dʱ%02d��%02d�룬Ʊ��%dԪ������%d�Σ������%g���\n", tt.h, tt.m, tt.s, Ro.price, Ro.transfer_times, Ro.dist);
	if (forecast_sec > CLOSE_TIME)
	{
        printf("\t������Ӫʱ��Ԥ������ѡ������·�ߣ��������޷�����Ŀ�ĵأ�������ѡ��������ͨ��ʽ��\n");
	}
	else
	{
        printf("\tԤ�Ƶ���ʱ��:  %02d:%02d\n", forecast.h, forecast.m);
	}
    printf("���-(%d����--", Ro.way.begin()->line);
	for (itSN = Ro.way.begin(); itSN != Ro.way.end() - 1; itSN++)
	{
		vector<StNum>::iterator nxtNode = itSN + 1;
		if (itSN->line != nxtNode->line)
		{
            printf("-->");
            cout << line[itSN->line].st_list[itSN->station_number - 1].name;
            printf("--%d����)-{����%d����}-(%d����--", itSN->line, nxtNode->line, nxtNode->line);
		}
		else
		{
			int delta = JudgeDirection(itSN->station_number, nxtNode->station_number);
			for (int i = itSN->station_number - 1; i != nxtNode->station_number; i += delta)
			{
				if (i == nxtNode->station_number - delta || i == nxtNode->station_number)
					break;
                printf("-->");
                cout << line[itSN->line].st_list[i].name;
			}
		}
	}
    printf("-->");
    cout << line[itSN->line].st_list[itSN->station_number - 1].name;
    printf("--%d����)-�յ�\n\n", itSN->line);
	Ro = rank_list[5];
	tt = Sec2Time(Ro.time_sec);
	forecast_sec = nowt_sec + Ro.time_sec;
	forecast = Sec2Time(forecast_sec);
    printf("\t���˴�������:  ��ʱ%02dʱ%02d��%02d�룬Ʊ��%dԪ������%d�Σ������%g���\n", tt.h, tt.m, tt.s, Ro.price, Ro.transfer_times, Ro.dist);
	if (forecast_sec > CLOSE_TIME)
	{
        printf("\t������Ӫʱ��Ԥ������ѡ������·�ߣ��������޷�����Ŀ�ĵأ�������ѡ��������ͨ��ʽ��\n");
	}
	else
	{
        printf("\tԤ�Ƶ���ʱ��:  %02d:%02d\n", forecast.h, forecast.m);
	}
    printf("���-(%d����--", Ro.way.begin()->line);
	for (itSN = Ro.way.begin(); itSN != Ro.way.end() - 1; itSN++)
	{
		vector<StNum>::iterator nxtNode = itSN + 1;
		if (itSN->line != nxtNode->line)
		{
            printf("-->");
            cout << line[itSN->line].st_list[itSN->station_number - 1].name;
            printf("--%d����)-{����%d����}-(%d����--", itSN->line, nxtNode->line, nxtNode->line);
		}
		else
		{
			int delta = JudgeDirection(itSN->station_number, nxtNode->station_number);
			for (int i = itSN->station_number - 1; i != nxtNode->station_number; i += delta)
			{
				if (i == nxtNode->station_number - delta || i == nxtNode->station_number)
					break;
                printf("-->");
                cout << line[itSN->line].st_list[i].name;
			}
		}
	}
    printf("-->");
    cout << line[itSN->line].st_list[itSN->station_number - 1].name;
    printf("--%d����)-�յ�\n\n", itSN->line);
    printf("=========================================================================\n");
	for (int i = 1; i <= 3; i++)
	{
		Ro = rank_list[i];
		tt = Sec2Time(Ro.time_sec);
		forecast_sec = nowt_sec + Ro.time_sec;
		forecast = Sec2Time(forecast_sec);
        printf("\t��ѡ·��%d:  ��ʱ%02dʱ%02d��%02d�룬Ʊ��%dԪ������%d�Σ������%g���\n", i, tt.h, tt.m, tt.s, Ro.price, Ro.transfer_times, Ro.dist);
		if (forecast_sec > CLOSE_TIME)
		{
            printf("\t������Ӫʱ��Ԥ������ѡ������·�ߣ��������޷�����Ŀ�ĵأ�������ѡ��������ͨ��ʽ��\n");
		}
		else
		{
            printf("\tԤ�Ƶ���ʱ��:  %02d:%02d\n", forecast.h, forecast.m);
		}
        printf("���-(%d����--", Ro.way.begin()->line);
		for (itSN = Ro.way.begin(); itSN != Ro.way.end() - 1; itSN++)
		{
			vector<StNum>::iterator nxtNode = itSN + 1;
			if (itSN->line != nxtNode->line)
			{
                printf("-->");
                cout << line[itSN->line].st_list[itSN->station_number - 1].name;
                printf("--%d����)-{����%d����}-(%d����--", itSN->line, nxtNode->line, nxtNode->line);
			}
			else
			{
				int delta = JudgeDirection(itSN->station_number, nxtNode->station_number);
				for (int j = itSN->station_number - 1; j != nxtNode->station_number; j += delta)
				{
					if (j == nxtNode->station_number - delta || j == nxtNode->station_number)
						break;
                    printf("-->");
                    cout << line[itSN->line].st_list[j].name;
				}
			}
		}
        printf("-->");
        cout << line[itSN->line].st_list[itSN->station_number - 1].name;
        printf("--%d����)-�յ�\n\n", itSN->line);
	}
	return OK;
}

int JudgeDirection(int a, int b)
{
	if (a <= b) return 1;
	else return -1;
}

bool isTrafficEqual(double a, double b)
{
    const double VERY_LITTLE=0.01;
    if(fabs(a-b)<VERY_LITTLE)
        return true;
    else
        return false;
}
