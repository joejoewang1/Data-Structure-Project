//gcc -o elevator main.c include.h passenger.c -lm
#include "include.h"
int CallUp[ElevatorNum][FloorNum], CallDown[ElevatorNum][FloorNum];
int Time;
struct WaitingList *Queue[ElevatorNum][FloorNum];
struct SystemList *System;
struct Elevator *Car[ElevatorNum];
int GenerationKey, GiveupKey, ElevatorKey[ElevatorNum]; //下一个关键时间

void Init(void)
{
    //初始化函数
    srand((unsigned)time(NULL));
    Time = 0;
    System = (SystemList *) malloc(sizeof(SystemList));
    System->p = NULL;
    System->next = System->prev = NULL;
    System->status = -1;
    for (int i = 0; i < ElevatorNum; i++) {
        for (int j = 0; j < FloorNum; j++) {
            Queue[i][j] = (WaitingList *) malloc(sizeof(WaitingList));
            Queue[i][j]->p = NULL;
            Queue[i][j]->next = Queue[i][j]->prev = NULL;
        }
    }
    GenerationKey = 0;
    GiveupKey = -1;
    for (int i = 0; i < ElevatorNum; i++) ElevatorKey[i] = -1;
}
void SystemInsert(Passenger *pcur)
{
    //插入一名新乘客到系统，并按照放弃时间排序
    struct SystemList *new_node = (SystemList *) malloc(sizeof(SystemList));
    SystemList *q = System->next, *q_prev = System;
    new_node->p = pcur;
    new_node->status = Waiting;
    while (q && q->p->GivenupTime < pcur->GivenupTime) {
        q_prev = q;
        q = q->next;
    }
    q_prev->next = new_node;
    if (q) q->prev = new_node;
    new_node->next = q;
    new_node->prev = q_prev;
}
int returnGiveupKey(SystemList *s) {
    SystemList *q = s->next;
    while (q && q->status == NotWaiting) q = q->next; 
    if (q) return q->p->GivenupTime;
    else return -1;
}
int findElevator(Passenger *p) { //对于多电梯，一种简化策略是预先选择好要乘坐的电梯，目前仅实现单电梯因此返回0
    return 0;
}
void QueueInsert(Passenger *pcur, int e)
{
    //插入新乘客到对应的队列末尾
    struct WaitingList *new_node = (WaitingList *) malloc(sizeof(WaitingList));
    new_node->p = pcur;
    WaitingList *q = Queue[e][pcur->InFloor];
    while (q->next) q = q->next;
    q->next = new_node;
    new_node->prev = q;
    new_node->next = NULL;
}
int main()
{
    Init();
    while (Time < SimulationTime) {
        int TimeIncMark = 1;
        //进入系统，为下一个人的出现作准备
        if (Time == GenerationKey) {
            Passenger *p = GenerateNextPassenger(Time, FLOOR1_PROB_FACTOR);
            p->GivenupTime += Time; //实际放弃时间
            GenerationKey = Time + p->InterTime;
            SystemInsert(p); //插入系统
            GiveupKey = returnGiveupKey(System);
            int e = findElevator(p);
            QueueInsert(p, e); //插入选定电梯对应的等待队列
            if (p->InFloor < p->OutFloor) CallUp[e][p->InFloor] = 1;
            else CallDown[e][p->InFloor] = 1;
            printf("%d %d %d %d %d\n", Time, p->InFloor, p->OutFloor, p->GivenupTime, p->InterTime);
            for (int i = 0; i < FloorNum; i++) {
                printf("%d ", CallUp[0][i]);
            }
            printf("\n");
        }
        if (Time == GiveupKey)
        {
            SystemList *q = System->next;
            while (q && q->status == NotWaiting) q = q->next;
            if (canEnter(q->p)) {
                q->status = NotWaiting;
                GiveupKey = returnGiveupTime(System);
                if (Time == GiveupKey) TimeIncMark = 0;
            }
            else {
                QueueDelete(q->p, q->e);
                SystemDelete(q->p);
                GiveupKey = returnGiveupTime(System);
                if (Time == GiveupKey) TimeIncMark = 0;
            }
        }
        if (TimeIncMark) Time = GenerationKey;
    }
    return 0;
}