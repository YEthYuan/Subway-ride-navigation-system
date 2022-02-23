#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include "define.h"

using namespace std;

struct StNum {
	int line;
	int station_number;
};
struct Station {
	StNum id;
	string name;
	bool transfer;
	vector<int> TransTo;
};
struct StNode {
	Station station;
	StNode* next;
};
struct StArray {
	Station station;
	int numOfAdjacentNodes;//邻接节点数量
	StNode* next;
};
struct Line {
	int id;
	int fullNum;
	double length;
	int St_Num;
    int cong_flag;//手动拥挤度设置模式：0~预设拥挤度，1~手动宽松, 2~手动一般, 3~手动拥挤
	vector<Station> st_list;
};
extern Line line[MAX_LINE_NUM];
extern bool openline[MAX_LINE_NUM];
extern vector<StArray> AdjLst;

extern status input(const char* path);
extern status Test_Output(void);
extern StNode* CreateStationNode(Station s);
extern status BuildMap(void);
extern status PrintAdjList(void);
extern vector<Station>::iterator FindStation(int lineid, int stid);
