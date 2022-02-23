#pragma once
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "mycaltime.h"
#include "define.h"
#include "mapping.h"

using namespace std;

extern int PriceCal(double dist);
extern double CongestionCal(int lineID, int t_sec);
extern double StationDist(int lineID);
extern int TrafficJudge(int lineID, int nowsec);
