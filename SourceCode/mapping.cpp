#include "define.h"
#include "mapping.h"
Line line[MAX_LINE_NUM];
bool openline[MAX_LINE_NUM];
vector<StArray> AdjLst;

//��������  input
//���ߣ�    ԬҲ
//���ڣ�    2021/02/16
//���ܣ�    ���ļ��н����ݶ����ڴ�
//��������� path �����ļ�����·��
//����ֵ��  ״ֵ̬(int)
//         ����INFEASTABLE��ʾ�ļ���ʧ�ܣ�
//         ����OK��ʾ�ļ���ȡ�ɹ���
//�޸ļ�¼��
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
		while (!in.eof())//�����洢ȫ����·
		{
			in >> line_id;
			openline[line_id] = true;
            line[line_id].cong_flag=0;
			in >> line[line_id].fullNum >> line[line_id].length >> line[line_id].St_Num;//���ļ���ȡ��·��Ϣ
			for (int i = 1; i <= line[line_id].St_Num; i++)//��������·��ȫ����վ
			{
				Station st;//����Station�ṹ����ʱ��ų�վ��Ϣ
				st.id.line = line_id;
				in >> st.id.station_number >> st.name;
				int temp;//�ݴ�����Ŀɻ�����·���
				in >> temp;
				if (temp == 0)//�ó�վ���ǻ���վ��û��������·��ֱ��ѹ��Vector
				{
					st.transfer = false;//��վ���ǻ���վ
					line[line_id].st_list.push_back(st);
				}
				else//��վ�ǻ��˳�վ
				{
					st.transfer = true;//��վ�ǻ���վ
					st.TransTo.push_back(temp);
					for (;;)//������վ�ɻ��˵�ȫ����·
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

//��������  Test_Output
//���ߣ�    ԬҲ
//���ڣ�    2021/02/16
//���ܣ�    �ú��������ڲ��ԣ�
//			���ڴ��д洢��ȫ����Ϣ�����ض��ĵĸ�ʽ��ӡ����
//��������� ��
//����ֵ��  ״ֵ̬(int)
//         ����OK��ʾ��ӡ�ɹ���
//�޸ļ�¼��
status Test_Output(void)
{
	for (int i = 1; i < MAX_LINE_NUM; i++)
	{
		if (!openline[i])
			continue;
        printf("�人����%d����:\n", i);
        printf("��·��Ϣ:\n");
        printf("\t�ؿ���:%d;\n", line[i].fullNum);
        printf("\t��·ȫ��:%gkm;\n", line[i].length);
        printf("\t��վ��Ŀ:%d;\n\n", line[i].St_Num);
		vector<Station>::iterator itSt;
        printf("\t��վ���\t����\t\t�Ƿ�Ϊ����վ\t������·\n");
		for (itSt = line[i].st_list.begin(); itSt != line[i].st_list.end(); itSt++)
		{
            printf("\t%d%02d\t", itSt->id.line, itSt->id.station_number);
            cout << itSt->name;
            printf("\t\t");
			if (itSt->transfer)
			{
                printf("��\t\t");
				for (vector<int>::iterator itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
				{
                    printf("%d���� ", *itInt);
				}
				putchar('\n');
			}
			else
			{
                printf("��\t\t-------\n");
			}
		}
	}
	return OK;
}

//��������  CreateStationNode
//���ߣ�    ԬҲ
//���ڣ�    2021/02/17
//���ܣ�    �����ڽӱ��еĳ�վ�ڵ�
//��������� ��վ��Ϣ�ṹ��
//����ֵ��  ��վ�ڵ�ָ��
//�޸ļ�¼��
StNode* CreateStationNode(Station s)
{
	StNode* ret = new StNode;
	ret->next = NULL;
	ret->station = s;
	return ret;
}

//��������  BuildMap
//���ߣ�    ԬҲ
//���ڣ�    2021/02/16
//���ܣ�    ͨ���ڴ��е����ݽ�������ͼ
//��������� ��
//����ֵ��  ״ֵ̬(int)
//         ����OK��ʾת���ɹ���
//�޸ļ�¼��
status BuildMap(void)
{
	for (int i = 1; i < MAX_LINE_NUM; i++)
	{
		if (!openline[i])
			continue;
		vector<Station>::iterator itSt;
		for (itSt = line[i].st_list.begin(); itSt != line[i].st_list.end(); itSt++)
		{
			vector<Station>::iterator it_temp;//��ʱ������
			int count = 0;//�ڽӽ�������
			StArray nowinArray;
			nowinArray.station = *itSt;
			StNode* FirstStNode = new StNode;
			StNode* tail = FirstStNode;
			if (itSt != line[i].st_list.begin())//���ڽӱ���������һվ���ݣ�����У�
			{
				it_temp = itSt - 1;
				count++;
				tail->next = CreateStationNode(*it_temp);
				tail = tail->next;
			}
			if (itSt != line[i].st_list.end() - 1)//���ڽӱ���������һվ���ݣ�����У�
			{
				it_temp = itSt + 1;
				count++;
				tail->next = CreateStationNode(*it_temp);
				tail = tail->next;
			}
			if (itSt->transfer == true)//���ڽӱ������뻻�˳�վ���ݣ�����У�
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

//��������  PrintAdjList
//���ߣ�    ԬҲ
//���ڣ�    2021/02/18
//���ܣ�    #�ù��ܽ�Ϊ����ʹ�ã�#
//			��ӡ�ڴ��е��ڽӱ�
//��������� ��
//����ֵ��  ״ֵ̬(int)
//         ����OK��ʾ�ڽӱ��ӡ�ɹ���
//�޸ļ�¼��
status PrintAdjList(void)
{
	vector<StArray>::iterator itAdjLst;
	for (itAdjLst = AdjLst.begin(); itAdjLst != AdjLst.end(); itAdjLst++)
	{
        printf("\t%d%02d ", itAdjLst->station.id.line, itAdjLst->station.id.station_number);
        cout << itAdjLst->station.name;
		StNode* p;//�ڽӳ�վ����ָ��
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
