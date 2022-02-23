#include <stdio.h>
#include <iostream>
#include "define.h"
#include "mapping.h"
#include "route.h"
#include "time.h"
#include "price.h"

int main()
{
	printf("\n\n\n\n\n                   武汉地铁查询系统\n\n\n\n\n");
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
		printf("             武汉地铁查询系统 \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. 导入数据         2. 路线规划\n");
		printf("    	  3. 计算票价         0. 退出程序\n");
		printf("-------------------------------------------------\n");
		printf("    请选择你的操作[0~24]:");
		scanf("%d", &op);
		getchar();
		//setbuf(stdin, NULL);
		switch (op) {
		case 1:
			printf("请输入数据文件路径:");
			path = "E:\\gbk.in";
			//cin >> path;
			input(path.c_str());
			//Test_Output();
			BuildMap();
			PrintAdjList();
			system("pause");
			break;
		case 2:
		c2:	printf("请输入当前时间：格式(hh:mm:ss):\n");
			scanf("%d:%d:%d", &_h, &_m, &_s);
			tt = Time(_h, _m, _s);
			printf("请输入起点线路号和站点序号:\n");
			scanf("%d%d", &lineid_s, &stid_s);
			Status = ShowStartStation(tt, lineid_s, stid_s);
			if (Status == FALSE)
			{
				system("pause");
				system("cls");
				goto c2;
			}
			printf("请输入终点线路号和站点序号:\n");
			scanf("%d%d", &lindid_d, &stid_d);
			ShowDestStation(lindid_d, stid_d);
			printf("请输入最大搜索深度：(推荐12，数值过大会严重增加运算时间)\n");
			scanf("%d", &depth_m);
			route_main(tt, lineid_s, stid_s, lindid_d, stid_d, depth_m);
			system("pause");
			break;
		case 3:
			printf("请输入您的里程:");
			scanf("%lf", &dist);
			printf("需要花费的价格为:%d元.\n", PriceCal(dist));
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
	printf("欢迎下次再使用本系统！\n");
	return 0;
}