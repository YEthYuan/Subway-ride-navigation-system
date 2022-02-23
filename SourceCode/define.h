#pragma once

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

#define MAX_LINE_NUM 10//最大线路数量
#define MAX_STN_NUM 100//每条线路最大车站数量
#define OPEN_TIME 21600//首班车开始时间
#define CLOSE_TIME 82800//末班车时间
#define STOP_TIME 60//停车原始时间
#define TRAVEL_TIME 120//行车原始时间
#define INTERCHANGE_TIME 210//换乘时间

typedef int status;
