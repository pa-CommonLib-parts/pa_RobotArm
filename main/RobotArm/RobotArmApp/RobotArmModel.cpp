#include "RobotArmApp.h"
#include "robot_math.h"

// #include "pa_CommonLib/src/util/pa_Math/pa_Math.h"
RobotArmModel::RobotArmModel()
{
}

//detailed model please check model_describe.md
void RobotArmModel::recalcVeticalPlane(float x, float y, float z, int &m1Step, int &m2Step, int &m3Step)
{
    float theata4plus1 = atan2f(y, -x);
    if (theata4plus1 < 0)
    {
        theata4plus1 += PI + PI;
    }
    //之前这里没有加z。所以是有bug的
    float l3Square = x * x + y * y + z * z;
    float l3 = sqrtf(l3Square);
    float theata3plus2 = acosf((l2Square + l1Square - l3Square) / (2 * l1 * l2));
    float theata4 = acosf((l1Square + l3Square - l2Square) / (2 * l1 * l3));
    theata2 = theata3plus2 - theata3;
    theata1 = theata4plus1 - theata4;

    float theataXZ = atan2f(z, x + 43);
    m3Step = theataXZ * RobotArmStepCnt_PI_4_divide8 / PI_4 * 0.8;

    m2Step = theata1 * RobotArmStepCnt_PI_4_divide8 / PI_4;
    m2Step = m2Step - 2 * RobotArmStepCnt_PI_4_divide8 + RobotArmStepCnt_RightArmVertical_divide8;

    m1Step = (theata1 - theata2) * RobotArmStepCnt_PI_4_divide8 / PI_4;
    m1Step = RobotArmStepCnt_LeftArmVertical_divide8 + m1Step - 2 * RobotArmStepCnt_PI_4_divide8;
}

void RobotArmModel::initDatas()
{
    l2Square = l2_y * l2_y + l2_x + l2_x;
    l2 = sqrtf(l2Square);
    l1Square = l1 * l1;
    theata3 = atan2f(l2_x, l2_y);
}
