// #include "pa_Defines.h"

#ifndef __ROBOTARMAPP_H__
#define __ROBOTARMAPP_H__
#include "../_drv_inc.h"
#include "../RobotStepper/RobotStepper.h"
#include "robot_math.h"
// #include "RobotArm_UserInterface.h"
/***********************************************************
 * 
 * direction invert
 * 方向控制引脚翻转标志位，用于解决硬件接线导致实际方向相反的情况
 * 
 * *********************************************************/
#define RobotArmStepper1_Inverted 0
#define RobotArmStepper2_Inverted 0
#define RobotArmStepper3_Inverted 0
/***********************************************************
 * 
 * reference step 
 * 基准步进值，用于运行过程中换算出步进电机需要走的步进值
 * 
 * *********************************************************/
//45度脚，8细分下的步进数（单位基准）
#define RobotArmStepCnt_PI_4_divide8 1340
//左侧控制臂垂直时，8细分下的步进值（坐标基准）
#define RobotArmStepCnt_LeftArmVertical_divide8 2150
//右侧控制臂垂直时，8细分下的步进值（坐标基准）
#define RobotArmStepCnt_RightArmVertical_divide8 1544
#define RobotArmPointBuffLen 200

struct RobotArmPoint3D
{
    float x;
    float y;
    float z;
    RobotArmPoint3D() {}
    RobotArmPoint3D(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
};

class RobotArmPointBuff
{
public:
    bool addPoint(RobotArmPoint3D point);
    bool getNextPoint(RobotArmPoint3D &p);
    void reset();
    bool ifNeedSupply();

private:
    bool needSupplyFlag = true;
    int pointCnt = 0;
    int headIndex = 0;
    RobotArmPoint3D buff[RobotArmPointBuffLen];
};

class RobotArmModel
{
public:
    RobotArmModel();

    void recalcVeticalPlane(float x, float y, float z, int &m1Step, int &m2Step, int &m3Step);
    void initDatas();

private:
    const float l1 = 200;   //left motor connected arm len (arm1)
    const float l2_y = 212; //right motor connectd arm len (arm2)
    const float l2_x = 28;  //the xoffset of head to arm2
    float l2;               //modeled arm len, calc from l2_y & l2_x
    float theata3;          //in model angle fix, calc from 12_y & 12_x
    float theata1;          //(arm1 angle)
    float theata2;          //(arm2 angle)
    float l2Square;
    float l1Square;
};
// class RobotArm_UserInterface;
// class RobotArmApp;
// class RobotArm_UserInterface
// {
// public:
//     RobotArm_UserInterface();

//     void setMotorEnable_do(char enable);
//     void setMotorEnable_setGuiCallback(void (*callback)(char enable));

// private:
//     RobotArmApp &robotArm = RobotArmApp::instance;
//     void (*setMotorEnable_guiCallback)(char enable);
// };
class RobotArmApp
{
private:
    /**************************************
     * 
     *      硬件层
     * 
     **************************************/

    // char getLimitSwitch_LeftArm();
    // char getLimitSwitch_RightArm();

#include "_drv_inline.h"
    /**************************************
     * 
     *      逻辑层
     * 
     **************************************/
public:
    const uint16_t limitSwitchActivatedCnt_MAX = 100; //用于消抖的限位开关计数值
    void setMotorEnable(char enable);
    // RobotArmApp();

    // static RobotArmApp instance;
    // RobotArm_UserInterface userInterface;
    RobotArmPointBuff pointBuff;
    RobotArmPoint3D lastPoint;

    void onTimerTick();

    enum Mode
    {
        mode_backup,
        mode_running
    };
    void setMode(Mode mode);
    void init();
    void parseMsg(uint8_t *data, int len);

private:
    RobotArmModel robotArmModel;

    Mode curMode;
    RobotStepper robotSteppers[3];

    //电机1当前步进数
    int step1 = 0;
    //电机2当前步进数
    int step2 = 0;
    //电机3当前步进数
    int step3 = 0;

    //电机1目标步进数
    int targetStep1 = 0;
    //电机1目标步进数
    int targetStep2 = 0;
    //电机1目标步进数
    int targetStep3 = 0;

    int currentStep1 = 0;

    //电机1 位移所需总步进数
    int totalStep1 = 0;
    //电机2 位移所需总步进数
    int totalStep2 = 0;
    //电机3 位移所需总步进数
    int totalStep3 = 0;

    //决定了完成总的步进需要花费的tick
    int tickCountInOneMove = 0;
    //当前tick计数
    int currentTick = 0;
    int sleepTickCnt = 0;

    void prepareNextMove(bool firstRun);
    void doStepperEvent(RobotStepper &stepper, bool pinStateOnBackupMode);
    void setStep(RobotStepper &stepper, int m1step, int m2step, int m3step);
};

#endif // __ROBOTARMAPP_H__