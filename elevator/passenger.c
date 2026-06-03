#include "include.h"

int GenerateGivenUpTime(int in, int out)
{
    //楼层相距越远，乘客忍耐时间越长，上行乘客忍耐时间比下行普遍长，且尽量均匀分布
    //目前采用的公式: Tolerance = Base * (|in - out| ^ 2 + (-1, 1) * |in - out|)
    double random_num = (double) rand() / RAND_MAX * 2.0 - 1.0;
    int span = abs(in - out);
    double value;
    if (in > out) value = DOWNToleranceBase * (span * span + random_num * span); //下行
    else value = UPToleranceBase * (span * span + random_num * span); //上行
    int result = (int) round(value);
    if (result < 10) result = 10; //防止等待时间过短
    return result;
}

int GenerateInterTime(int mode)
{
    //目前暂采取随机生成(mode=0)，其他未来可能使用的生成方法包括预生成所有时间+返回相邻两个时间的差值
    if (mode == 0)
    {
        int random_num = rand() % (MaxInterTime - 10) + 10; //间隔时间不要太短
        return random_num;
    }
}
Passenger *GenerateNextPassenger(int curTime, double prob_fac)
{
    double random_num = (double) rand() / RAND_MAX;
    int floor_num = rand() % (FloorNum - 1);
    Passenger *new_p = (Passenger *) malloc(sizeof(Passenger));

    //楼层生成逻辑目前只考虑一层需求比其他层多的情况，其他需求（如远距离需求更频繁）还未考虑，这一部分有待改进
    if (random_num <= prob_fac / 2.0) { //1层到其他层
        new_p->InFloor = 1;
        new_p->OutFloor = !floor_num ? floor_num : floor_num + 1;
    }
    else if (random_num <= prob_fac) { //其他层到1层
        new_p->OutFloor = 1;
        new_p->InFloor = !floor_num ? floor_num : floor_num + 1;
    }
    else { //其他层之间流动
        int floor_num2 = rand() % (FloorNum - 2), i = 0;
        new_p->InFloor = !floor_num ? floor_num : floor_num + 1;
        while (floor_num2 >= 0) { //保证两个楼层不重复
            if (i == new_p->InFloor || i == 1) i++;
            else {
                if (floor_num2 == 0) {
                    new_p->OutFloor = i;
                    break;
                }
                else {
                    i++;
                    floor_num2--;
                }
            }
        }
    }
    
    //忍耐时间生成
    new_p->GivenupTime = GenerateGivenUpTime(new_p->InFloor, new_p->OutFloor);

    //时间间隔生成
    new_p->InterTime = GenerateInterTime(MODE);
    return new_p;
}