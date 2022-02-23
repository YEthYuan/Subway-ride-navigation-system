#ifndef __MYCALTIME_H__
#define __MYCALTIME_H__
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include "price.h"
#include <math.h>
#include <time.h>

#define LOOSE_RATIO 0.6//对应拥挤度0~20%
#define NORMAL_RATIO 1.0//对应拥挤度21%~62%
#define CROWDED_RATIO 1.5//对应拥挤度63%~100%

using namespace std;

struct TimeHMS {
	int h;
	int m;
	int s;
    TimeHMS(){}
    TimeHMS(int _h, int _m, int _s) :h(_h), m(_m), s(_s) {}
};

extern TimeHMS Sec2Time(int n);
extern int Time2Sec(TimeHMS n);
extern TimeHMS NextTrainTime(TimeHMS nowt);
extern void PrintSchedule(void);
extern int CalRealTime(int lineid, int nowsec, int ori_time);

#endif
