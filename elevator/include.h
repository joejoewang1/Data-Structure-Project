#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define FloorNum 5
#define ElevatorNum 1
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
#define Idle 3       //电梯三种状态

#define FLOOR1_PROB_FACTOR 0.5 //起终点在一层的概率因子
#define MODE 0 //模拟模式，目前默认为单梯随机模拟
#define UPToleranceBase 50 //上行忍耐基准时间
#define DOWNToleranceBase 40 //下行忍耐基准时间
#define MaxInterTime 200

#define UP 1
#define DOWN -1
#define NONE 0
#define Open 1
#define Close 0

extern int CallUp[ElevatorNum][FloorNum], CallDown[ElevatorNum][FloorNum]; //每层上下两个按钮
extern int Time;

typedef struct Passenger {
    int InFloor;
    int OutFloor;
    int GivenupTime; //生成时是最大容忍时间，后续模拟中为实际放弃时间
    int InterTime; //下一人出现时间间隔
}Passenger;

typedef struct WaitingList { //每一层的排队情况，采用双向链表
    Passenger *p;
    struct WaitingList *next, *prev;
}WaitingList, PassengerList;

typedef struct Elevator {
    int CallCar[FloorNum]; //电梯内楼层按钮
    int State;
    int Floor;
    int PassengerNum; //记录电梯内人数
    PassengerList *plist; //乘客列表
    int ExpectedDir; //期望运行方向，写可视化函数不用管
    int WaitingLimit; //非一层等待极限时间
    int DoorState; //门状态
    
}Elevator;
extern struct Elevator *Car[ElevatorNum];

extern struct WaitingList *Queue[ElevatorNum][FloorNum];
extern struct WaitingList *System; //系统，按照放弃时间升序排序

//函数声明
extern Passenger *GenerateNextPassenger(int curTime, double prob_fac);