//gcc -o elevator main.c include.h passenger.c -lm
#include "include.h"
int CallUp[FloorNum], CallDown[FloorNum];
int Time;
int main()
{
    srand((unsigned)time(NULL));
    Time = 0;
    while (Time < SimulationTime) {
        Passenger *p = GenerateNextPassenger(Time, FLOOR1_PROB_FACTOR);
        printf("%d %d %d %d %d\n", Time, p->InFloor, p->OutFloor, p->GivenupTime, p->InterTime);
        Time += p->InterTime;
    }
    return 0;
}