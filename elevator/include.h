#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define FloorNum 5
#define EvelatorNum 1
#define CheckInterval 40
#define OpenTime 20
#define CloseTime 20
#define EnterTime 25
#define ExitTime 25
#define MaxWaitingTime 300
#define AwaitingFloor 1 //一层待命，为方便起见假设地下一层到四层分别对应数字0到4
#define SimulationTime 1000
#define Capacity 15 //电梯容量

#define GoingUp 1
#define GoingDown 2
#define Idel 3       //电梯三种状态

#define FLOOR1_PROB_FACTOR 0.5 //起终点在一层的概率因子
#define MODE 0 //模拟模式，目前默认为单梯随机模拟
#define UPToleranceBase 50 //上行忍耐基准时间
#define DOWNToleranceBase 40 //下行忍耐基准时间
#define MaxInterTime 200

extern int CallUp[FloorNum], CallDown[FloorNum]; //每层上下两个按钮
extern int Time;

typedef struct Elevator {
    int CallCar[FloorNum]; //电梯内楼层按钮
    int State;
    int Floor;
    int D1, D2, D3;
}Elevator;

typedef struct Passenger {
    int InFloor;
    int OutFloor;
    int GivenupTime;
    int InterTime; //下一人出现时间间隔
}Passenger;

//函数声明
extern Passenger *GenerateNextPassenger(int curTime, double prob_fac);