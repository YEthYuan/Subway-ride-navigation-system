#include <stdio.h>
#include <iostream>
#include "define.h"
#include "mapping.h"
#include "route.h"
#include "time.h"
#include "price.h"

int main()
{
	printf("\n\n\n\n\n                   �人������ѯϵͳ\n\n\n\n\n");
	printf("                        ......");
	system("pause");
	int op = 1;
	string path;
	int _h, _m, _s;
	Time tt;
	int lineid_s, stid_s;
	int lindid_d, stid_d;
	status Status;
	double dist = 0.0;
	int depth_m;
	while (op) {
		system("cls");	printf("\n\n");
		printf("             �人������ѯϵͳ \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. ��������         2. ·�߹滮\n");
		printf("    	  3. ����Ʊ��         0. �˳�����\n");
		printf("-------------------------------------------------\n");
		printf("    ��ѡ����Ĳ���[0~24]:");
		scanf("%d", &op);
		getchar();
		//setbuf(stdin, NULL);
		switch (op) {
		case 1:
			printf("�����������ļ�·��:");
			path = "E:\\gbk.in";
			//cin >> path;
			input(path.c_str());
			//Test_Output();
			BuildMap();
			PrintAdjList();
			system("pause");
			break;
		case 2:
		c2:	printf("�����뵱ǰʱ�䣺��ʽ(hh:mm:ss):\n");
			scanf("%d:%d:%d", &_h, &_m, &_s);
			tt = Time(_h, _m, _s);
			printf("�����������·�ź�վ�����:\n");
			scanf("%d%d", &lineid_s, &stid_s);
			Status = ShowStartStation(tt, lineid_s, stid_s);
			if (Status == FALSE)
			{
				system("pause");
				system("cls");
				goto c2;
			}
			printf("�������յ���·�ź�վ�����:\n");
			scanf("%d%d", &lindid_d, &stid_d);
			ShowDestStation(lindid_d, stid_d);
			printf("���������������ȣ�(�Ƽ�12����ֵ�����������������ʱ��)\n");
			scanf("%d", &depth_m);
			route_main(tt, lineid_s, stid_s, lindid_d, stid_d, depth_m);
			system("pause");
			break;
		case 3:
			printf("�������������:");
			scanf("%lf", &dist);
			printf("��Ҫ���ѵļ۸�Ϊ:%dԪ.\n", PriceCal(dist));
			system("pause");
			break;
		case 4:

			system("pause");
			break;
		case 5:

			system("pause");
			break;
		case 6:

			system("pause");
			break;
		case 7:

			system("pause");
			break;
		case 8:

			system("pause");
			break;
		case 9:

			system("pause");
			break;
		case 10:

			system("pause");
			break;
		case 11:

			system("pause");
			break;
		case 12:

			system("pause");
			break;
		case 19:

			system("pause");
			break;
		case 21:

			system("pause");
			break;
		case 22:

			system("pause");
			break;
		case 23:

			system("pause");
			break;
		case 24:

			system("pause");
			break;
		case 0:
			break;
		}//end of switch
		//setbuf(stdin, NULL);
	}//end of while
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
	return 0;
}