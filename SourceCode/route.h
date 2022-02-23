#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <math.h>
#include <algorithm>
#include "mapping.h"
#include "price.h"

using namespace std;

struct Route {
	int price;
	double dist;
	int time_sec;//考虑拥挤后的时间
	int transfer_times;//换乘次数
    int s2s_traffic[3];//区间拥挤情况
    double overall_traffic;//路线综合拥挤程度
	double weightedScore;//加权分数
	vector<StNum> way;
};
struct TransferNode {
	StNum st;
	bool isTransferEdge;
	//isTransferEdge==false
	int count_station;//两站间站数(包括这两站中的一站)
	double dist;//两站间距
	int time_sec;//两站间耗时(原始时间)
	//isTransferEdge==true
	int transfer_time_sec;//换乘时间
	TransferNode* next;
};

extern bool vis[MAX_LINE_NUM][MAX_STN_NUM];//DFS访问表
extern vector<Route> R;
extern vector<TransferNode> TN;
extern vector<Route> rank_list;

extern status route_main(TimeHMS now, int Sline, int SID, int Dline, int DID, int max_search_depth);
//extern status ShowStartStation(TimeHMS now, int Sline, int SID);
//extern status ShowDestStation(int Dline, int DID);
extern status BuildTransferGraph(int Sline, int SID, int Dline, int DID);
extern status PrintTransferAdjList(void);
extern void DFS(Route nowR, StNum Destination, int depth, int max_depth);
extern vector<TransferNode>::iterator FindTransferInAdjList(int lineid, int stid);
extern status PrintDFSResult(void);
extern bool isInArray(Route nowR, StNum s);
extern status RouteEvaluation(Route& nowR);
extern double ComprehensiveEvaluation(Route nowR);
extern bool comprehensive_cmp(Route a, Route b);
extern bool time_cmp(Route a, Route b);
extern bool transfer_cmp(Route a, Route b);
extern bool price_cmp(Route a, Route b);
extern bool traffic_cmp(Route a, Route b);
extern status ProcessingRoute(void);
extern status PrintResult(void);
extern int JudgeDirection(int a, int b);
extern bool isTrafficEqual(double a, double b);
