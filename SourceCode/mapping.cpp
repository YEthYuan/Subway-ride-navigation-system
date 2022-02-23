#include "define.h"
#include "mapping.h"
Line line[MAX_LINE_NUM];
bool openline[MAX_LINE_NUM];
vector<StArray> AdjLst;

//函数名：  input
//作者：    袁也
//日期：    2021/02/16
//功能：    从文件中将数据读入内存
//输入参数： path 数据文件所在路径
//返回值：  状态值(int)
//         返回INFEASTABLE表示文件打开失败；
//         返回OK表示文件读取成功。
//修改记录：
status input(const char* path)
{
	ifstream in;
	in.open(path);
	if (in.is_open()==false)
	{
		return INFEASTABLE;
	}
	else
	{
		int line_id;
		while (!in.eof())//遍历存储全部线路
		{
			in >> line_id;
			openline[line_id] = true;
            line[line_id].cong_flag=0;
			in >> line[line_id].fullNum >> line[line_id].length >> line[line_id].St_Num;//从文件读取线路信息
			for (int i = 1; i <= line[line_id].St_Num; i++)//遍历该线路的全部车站
			{
				Station st;//创建Station结构体临时存放车站信息
				st.id.line = line_id;
				in >> st.id.station_number >> st.name;
				int temp;//暂存输入的可换乘线路编号
				in >> temp;
				if (temp == 0)//该车站不是换乘站，没有其他线路，直接压入Vector
				{
					st.transfer = false;//该站不是换乘站
					line[line_id].st_list.push_back(st);
				}
				else//该站是换乘车站
				{
					st.transfer = true;//该站是换乘站
					st.TransTo.push_back(temp);
					for (;;)//遍历该站可换乘的全部线路
					{
						in >> temp;
						if (temp != 0)
							st.TransTo.push_back(temp);
						else
							break;
					}
					line[line_id].st_list.push_back(st);
				}
			}
		}
		in.close();
		return OK;
	}
}

//函数名：  Test_Output
//作者：    袁也
//日期：    2021/02/16
//功能：    该函数仅用于测试！
//			将内存中存储的全部信息按照特定的的格式打印出来
//输入参数： 无
//返回值：  状态值(int)
//         返回OK表示打印成功。
//修改记录：
status Test_Output(void)
{
	for (int i = 1; i < MAX_LINE_NUM; i++)
	{
		if (!openline[i])
			continue;
        printf("武汉地铁%d号线:\n", i);
        printf("线路信息:\n");
        printf("\t载客量:%d;\n", line[i].fullNum);
        printf("\t线路全长:%gkm;\n", line[i].length);
        printf("\t车站数目:%d;\n\n", line[i].St_Num);
		vector<Station>::iterator itSt;
        printf("\t车站编号\t名称\t\t是否为换乘站\t换乘线路\n");
		for (itSt = line[i].st_list.begin(); itSt != line[i].st_list.end(); itSt++)
		{
            printf("\t%d%02d\t", itSt->id.line, itSt->id.station_number);
            cout << itSt->name;
            printf("\t\t");
			if (itSt->transfer)
			{
                printf("是\t\t");
				for (vector<int>::iterator itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
				{
                    printf("%d号线 ", *itInt);
				}
				putchar('\n');
			}
			else
			{
                printf("否\t\t-------\n");
			}
		}
	}
	return OK;
}

//函数名：  CreateStationNode
//作者：    袁也
//日期：    2021/02/17
//功能：    建立邻接表中的车站节点
//输入参数： 车站信息结构体
//返回值：  车站节点指针
//修改记录：
StNode* CreateStationNode(Station s)
{
	StNode* ret = new StNode;
	ret->next = NULL;
	ret->station = s;
	return ret;
}

//函数名：  BuildMap
//作者：    袁也
//日期：    2021/02/16
//功能：    通过内存中的数据建立无向图
//输入参数： 无
//返回值：  状态值(int)
//         返回OK表示转换成功。
//修改记录：
status BuildMap(void)
{
	for (int i = 1; i < MAX_LINE_NUM; i++)
	{
		if (!openline[i])
			continue;
		vector<Station>::iterator itSt;
		for (itSt = line[i].st_list.begin(); itSt != line[i].st_list.end(); itSt++)
		{
			vector<Station>::iterator it_temp;//临时迭代器
			int count = 0;//邻接结点计数器
			StArray nowinArray;
			nowinArray.station = *itSt;
			StNode* FirstStNode = new StNode;
			StNode* tail = FirstStNode;
			if (itSt != line[i].st_list.begin())//向邻接表中链入上一站数据（如果有）
			{
				it_temp = itSt - 1;
				count++;
				tail->next = CreateStationNode(*it_temp);
				tail = tail->next;
			}
			if (itSt != line[i].st_list.end() - 1)//向邻接表中链入下一站数据（如果有）
			{
				it_temp = itSt + 1;
				count++;
				tail->next = CreateStationNode(*it_temp);
				tail = tail->next;
			}
			if (itSt->transfer == true)//向邻接表中链入换乘车站数据（如果有）
			{
				vector<int>::iterator itInt;
				for (itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
				{
					vector<Station>::iterator temp_itSt;
					for (temp_itSt = line[*itInt].st_list.begin(); temp_itSt != line[*itInt].st_list.end(); temp_itSt++)
					{
						if (temp_itSt->name == itSt->name)
						{
							count++;
							tail->next = CreateStationNode(*temp_itSt);
							tail = tail->next;
						}
					}
				}
			}
			tail->next = NULL;
			nowinArray.numOfAdjacentNodes = count;
			nowinArray.next = FirstStNode->next;
			AdjLst.push_back(nowinArray);
		}
	}
	return OK;
}

vector<Station>::iterator FindStation(int lineid, int stid)
{
	vector<Station>::iterator ret;
	for (ret = line[lineid].st_list.begin(); ret != line[lineid].st_list.end(); ret++)
	{
		if (ret->id.station_number == stid)
			break;
	}
	return ret;
}

//函数名：  PrintAdjList
//作者：    袁也
//日期：    2021/02/18
//功能：    #该功能仅为测试使用！#
//			打印内存中的邻接表
//输入参数： 无
//返回值：  状态值(int)
//         返回OK表示邻接表打印成功。
//修改记录：
status PrintAdjList(void)
{
	vector<StArray>::iterator itAdjLst;
	for (itAdjLst = AdjLst.begin(); itAdjLst != AdjLst.end(); itAdjLst++)
	{
        printf("\t%d%02d ", itAdjLst->station.id.line, itAdjLst->station.id.station_number);
        cout << itAdjLst->station.name;
		StNode* p;//邻接车站遍历指针
		putchar('\t');
		for (p = itAdjLst->next; p != NULL; p = p->next)
		{
            printf("\t%d%02d ", p->station.id.line, p->station.id.station_number);
            cout << p->station.name;
		}
		putchar('\n');
	}
	return OK;
}
