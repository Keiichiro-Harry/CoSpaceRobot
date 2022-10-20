////////////////////////////////////////
//
//	File : ai.c
//	CoSpace Robot
//	Version 1.0.0
//	OCT 1 2021
//	Copyright (C) 2021 CoSpace Robot. All Rights Reserved
//
//////////////////////////////////////
//
// ONLY C Code can be compiled.
//
/////////////////////////////////////

#define CsBot_AI_H//DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif
//The robot ID : six chars unique CID.
//Find it from your CoSpace Robot label or CoSpace program download GUI.
//Don't write the below line into two lines or more lines.
char AI_MyID[6] = {'1','2','3','4','5','6'};

int Duration = 0;
int SuperDuration = 0;
int bGameEnd = false;
int CurAction = -1;
int CurGame = 0;
int SuperObj_Num = 0;
int SuperObj_X = 0;
int SuperObj_Y = 0;
int Teleport = 0;
int LoadedObjects = 0;
int US_Front = 0;
int US_Left = 0;
int US_Right = 0;
int CSLeft_R = 0;
int CSLeft_G = 0;
int CSLeft_B = 0;
int CSRight_R = 0;
int CSRight_G = 0;
int CSRight_B = 0;
int PositionX = 0;
int PositionY = 0;
int Compass = 0;
int Time = 0;
int WheelLeft = 0;
int WheelRight = 0;
int LED_1 = 0;
int MyState = 0;
int AI_TeamID = 1;   //Robot Team ID.    1:Blue Ream;    2:Red Team.
int AI_SensorNum = 12;

#define CsBot_AI_C//DO NOT delete this line

DLL_EXPORT void SetGameID(int GameID)
{
    if(CurGame != GameID) LoadedObjects = 0;
    CurGame = GameID;
    bGameEnd = 0;
}


DLL_EXPORT void SetTeamID(int TeamID)
{
    AI_TeamID = TeamID;
}

DLL_EXPORT int GetGameID()
{
    return CurGame;
}

//Only Used by CsBot Dance Platform
DLL_EXPORT int IsGameEnd()
{
    return bGameEnd;
}

#ifndef CSBOT_REAL

char info[3000];
DLL_EXPORT char* GetDebugInfo()
{
    sprintf(info, "Duration=%d;SuperDuration=%d;bGameEnd=%d;CurAction=%d;CurGame=%d;SuperObj_Num=%d;SuperObj_X=%d;SuperObj_Y=%d;Teleport=%d;LoadedObjects=%d;US_Front=%d;US_Left=%d;US_Right=%d;CSLeft_R=%d;CSLeft_G=%d;CSLeft_B=%d;CSRight_R=%d;CSRight_G=%d;CSRight_B=%d;PositionX=%d;PositionY=%d;Compass=%d;Time=%d;WheelLeft=%d;WheelRight=%d;LED_1=%d;MyState=%d;",Duration,SuperDuration,bGameEnd,CurAction,CurGame,SuperObj_Num,SuperObj_X,SuperObj_Y,Teleport,LoadedObjects,US_Front,US_Left,US_Right,CSLeft_R,CSLeft_G,CSLeft_B,CSRight_R,CSRight_G,CSRight_B,PositionX,PositionY,Compass,Time,WheelLeft,WheelRight,LED_1,MyState);
    return info;
}
 
DLL_EXPORT char* GetTeamName()
{
     return "TJI22JP7034";
}

DLL_EXPORT int GetCurAction()
{
    return CurAction;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT int GetTeleport()
{
    return Teleport;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT void SetSuperObj(int X, int Y, int num)
{
    SuperObj_X = X;
    SuperObj_Y = Y;
    SuperObj_Num = num;
}
//Only Used by CsBot Rescue Platform
DLL_EXPORT void GetSuperObj(int *X, int *Y, int *num)
{
    *X = SuperObj_X;
    *Y = SuperObj_Y;
    *num = SuperObj_Num;
}

#endif ////CSBOT_REAL

DLL_EXPORT void SetDataAI(volatile int* packet, volatile int *AI_IN)
{

    int sum = 0;

    US_Front = AI_IN[0]; packet[0] = US_Front; sum += US_Front;
    US_Left = AI_IN[1]; packet[1] = US_Left; sum += US_Left;
    US_Right = AI_IN[2]; packet[2] = US_Right; sum += US_Right;
    CSLeft_R = AI_IN[3]; packet[3] = CSLeft_R; sum += CSLeft_R;
    CSLeft_G = AI_IN[4]; packet[4] = CSLeft_G; sum += CSLeft_G;
    CSLeft_B = AI_IN[5]; packet[5] = CSLeft_B; sum += CSLeft_B;
    CSRight_R = AI_IN[6]; packet[6] = CSRight_R; sum += CSRight_R;
    CSRight_G = AI_IN[7]; packet[7] = CSRight_G; sum += CSRight_G;
    CSRight_B = AI_IN[8]; packet[8] = CSRight_B; sum += CSRight_B;
    PositionX = AI_IN[9]; packet[9] = PositionX; sum += PositionX;
    PositionY = AI_IN[10]; packet[10] = PositionY; sum += PositionY;
    Compass = AI_IN[11]; packet[11] = Compass; sum += Compass;
    Time = AI_IN[12]; packet[12] = Time; sum += Time;
    packet[13] = sum;

}
DLL_EXPORT void GetCommand(int *AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED_1;
}
void TurnTo(int curRot, int targetRot)
{
    int p0 = targetRot;
    int p3 = (targetRot + 3) % 360;
    int p15 = (targetRot + 15) % 360;
    int n3 = (targetRot - 3 + 360) % 360;
    int n15 = (targetRot - 15 + 360) % 360;
    int p180 = (targetRot + 180) % 360;
    int l = 0, r = 0;
    Duration = 6;
    //Within(-3,+3)deg, stop turing.
    l = n3; r = p3;
    if ((l < r && curRot > l && curRot < r) ||
    (l > r && (curRot > l || curRot < r)))
    {
        WheelLeft = 0;
        WheelRight = 0;
        Duration = 0;
        return;
    }
    //Within[3,15]deg,Turn Slowly
    l = p3; r = p15;
    if ((l < r && curRot >= l && curRot <= r) ||
        (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = 10;
        WheelRight = -10;
        return;
    }
    //Within[15,180]deg,Turn Faast
    l = p15; r = p180;
    if ((l < r && curRot >= l && curRot <= r) ||
       (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = 30;
        WheelRight = -30;
        return;
    }
    //Within[-15,-3]deg,Turn Slowly
    l = n15; r = n3;
    if ((l < r && curRot >= l && curRot <= r) ||
    (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = -10;
        WheelRight = 10;
        return;
    }
    //Within[-180,-15]deg,Turn Fast
    l = p180; r = n15;
    if ((l < r && curRot >= l && curRot <= r) ||
    (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = -30;
        WheelRight = 30;
        return;
    }
}









int black = 0;
int red = 0;
int cyan = 0;
int super = 0;
int log_super_x = -1;
int log_super_y = -1;
int pos_x,pos_y;
int process = 0;
int T2 = 0;

void motor(int left, int right)
{
    WheelLeft = left;
    WheelRight = right;
}

void get_red_object()
{
    motor(0, 0);
    LED_1 = 1;
    T2 = Time;
    if (SuperDuration == 50)
    {
        LoadedObjects++;
        red++;
    }
    if (SuperDuration == 2)
    {
        motor(60,60);
    }
    if (SuperDuration == 0)
    {
        LED_1 = 0;
        CurAction = 0;
    }
}

void get_cyan_object()
{
    motor(0, 0);
    LED_1 = 1;
    T2 = Time;
    if (SuperDuration == 50)
    {
        LoadedObjects++;
        cyan++;
    }
    if (SuperDuration == 2)
    {
        motor(60,60);
    }
    if (SuperDuration == 0)
    {
        LED_1 = 0;
        CurAction = 0;
    }
}

void get_black_object()
{
    motor(0, 0);
    LED_1 = 1;
    T2 = Time;
    if (SuperDuration == 50)
    {
        LoadedObjects++;
        black++;
    }
    if (SuperDuration == 2)
    {
        motor(60,60);
    }
    if (SuperDuration == 0)
    {
        LED_1 = 0;
        CurAction = 0;
    }
}

void get_super_object()
{
    motor(0, 0);
    LED_1 = 1;
    T2 = Time;
    if (SuperDuration == 50)
    {
        LoadedObjects++;
        super++;
    }
    if (SuperDuration == 2)
    {
        motor(60,60);
    }
    if (SuperDuration == 0)
    {
        LED_1 = 0;
        log_super_x = -1;
        log_super_y = -1;
        CurAction = 0;
    }
}

void do_deposit()
{
    motor(0, 0);
    LoadedObjects = 0;
    LED_1 = 2;
    red = 0;
    black = 0;
    cyan = 0;
    super = 0;
    if (SuperDuration == 0)
    {
        LED_1 = 0;
        CurAction = 0;
    }
}

bool go_angle(int angle)
{
    angle = angle - Compass;
    angle %= 360;
    if (angle < -180)
    {
        angle += 360;
    }
    if (angle > 180)
    {
        angle -= 360;
    }

    if (abs(angle) < 10)
    {
        motor(90/*(rand() % 100)*/, 90/*(rand() % 100)*/);
        /*if(Time % 200 < 100){
            motor(100,40);
        }else{
            motor(40,100);
        }*/
        return true;
    }else if (abs(angle) < 50)
    {
        if (angle < 0)
        {
            /*if(SuperObj_Num != 0){
                if(US_Left < 30){
                    motor(75, 75);    
                }else{
                    motor(60, 40);
                }
            }else{
                motor(60, 40);
            }*/
            motor(80, 30);
        }
        else
        {
            /*if(SuperObj_Num != 0){
                if(US_Right < 30){
                    motor(75, 75);
                }else{
                    motor(40, 60);
                }
            }else{
                motor(40,60);
            }*/
            motor(30,80);
        }
    }
    else if (abs(angle) < 100)
    {
        if (angle < 0)
        {
            /*if(SuperObj_Num != 0){
                if(US_Left < 30){
                    motor(75, 75);    
                }else{
                    motor(60, 20);
                }
            }else{
                motor(60,20);
            }*/
            motor(80,-40);
        }
        else
        {
            /*if(SuperObj_Num != 0){
                if(US_Right < 30){
                    motor(75, 75);
                }else{
                    motor(20, 60);
                }
            }else{
                motor(20,60);
            }*/
            motor(-40,80);
        }
    }
    else
    {
        if (angle < 0)
        {
            /*if(SuperObj_Num != 0){
                if(US_Left < 30){
                    motor(75, 75);    
                }else{
                    motor(80, -80);
                }
            }else{
                motor(80,-80);
            }*/
            
                motor(100,-40);
            
            /*motor(80,-80);*/
        }
        else
        {
            /*if(SuperObj_Num != 0){
                if(US_Right < 30){
                    motor(75, 75);    
                }else{
                    motor(-80, 80);
                }
            }else{
                motor(-80,80);
            }*/
            
                motor(-40,100);
            
            /*motor(-80,80);*/
        }
    }
    return false;
}

bool go_position(int x, int y, int wide_x, int wide_y, int wide_judge_arrived)
{
    static int absolute_x = -1, absolute_y;
    if (x - wide_judge_arrived < pos_x && pos_x < x + wide_judge_arrived && y - wide_judge_arrived < pos_y && pos_y < y + wide_judge_arrived)
    {
        motor(-50+(rand() % 100), 50-(rand() % 100));
        return true;
        
    }

    if (!(x - wide_x < absolute_x &&
          absolute_x < x + wide_x &&
          y - wide_y < absolute_y &&
          absolute_y < y + wide_y))
    {
        absolute_x = -1;
    }

    if (absolute_x == -1)
    {

        absolute_x = x - wide_x + rand() % (wide_x * 2);
        absolute_y = y - wide_y + rand() % (wide_y * 2);
    }

    x = absolute_x - pos_x;
    y = absolute_y - pos_y;
    // There is 90 degrees difference between math degree and cospace angle
    // 数学上での角と、Cospaceの角度には90度の差がある
    int angle = ((atan2(y, x) / M_PI * 180.0)) - 90;
    go_angle(angle);
    return false;
}

void go_superobj()
{
    static int loop_num = 0;
    static int prev_time = 0;

    // if there is a time difference
    if (prev_time + 5 <= Time)
    {
        prev_time = 0;
        loop_num = 0;
    }

    // 30s * 100 / 60 = ms
    if (loop_num >= (30 * 100 / 60))
    {
        log_super_x = -1;
        log_super_y = -1;
    }

    go_position(log_super_x, log_super_y, 10, 10, 10);
    ++loop_num;
}
void Game1()
{

    if (PositionX != 0 || PositionY != 0)
    {
        pos_x = PositionX;
        pos_y = PositionY;
    }

    if(SuperObj_Num != 0){
        log_super_x = SuperObj_X;
        log_super_y = SuperObj_Y;
    }

    //デポジットエリアにいくやつ
    if(Time < 24000){
        if((red == 2 && black == 2 && cyan == 2) || (super != 0 && Time- T2 > 1000)|| LoadedObjects == 6 || (Time - T2 > 3000 && LoadedObjects>3)){
            if(pos_x*2 +pos_y*5 < 1100){
                process = 14;
            }else{
                process = 15;
            }
        }    
    }else{
        if((red != 0 && black != 0 && cyan != 0)|| super != 0 || LoadedObjects == 6 || (Time - T2 > 3000 && LoadedObjects>3)){
            if(pos_x*2 +pos_y*5 < 1100){
                process = 14;
            }else{
                process = 15;
            }
        }
    }

    if (SuperDuration > 0)
    {
        SuperDuration--;
    }
    else if (CSLeft_R>=250 && CSLeft_G>=155 && CSLeft_G<=165 && CSLeft_B<=10 && CSRight_R>=250 && CSRight_G>=155 && CSRight_G<=165 && CSRight_B<=10 && ((Time >= 24000 && ((red != 0 && black != 0 && cyan != 0)||(Time - T2 > 3000 && LoadedObjects > 3))) || (Time < 24000 && ((red == 2 && black == 2 && cyan == 2)||(Time - T2 > 3000 && LoadedObjects > 3)))|| super != 0 ))
    {
        SuperDuration = 60;
        CurAction = 1;
    }
    else if (CSLeft_R >= 250 && CSLeft_G <= 10 && CSLeft_B >= 240&& LoadedObjects != 6)
    {
        SuperDuration = 60;
        CurAction = 5;
    }
    else if (CSRight_R >= 250 && CSRight_G <= 10 && CSRight_B >= 240&& LoadedObjects != 6)
    {
        SuperDuration = 60;
        CurAction = 5;
    }
    else if (CSRight_R>=250 && CSRight_G<=10 && CSRight_B<=10 && LoadedObjects != 6 && red < 2 && log_super_x == -1)
    {
        SuperDuration = 60;
        CurAction = 4;
    }
    else if (CSLeft_R>=250 && CSLeft_G<=10 && CSLeft_B<=10 && LoadedObjects != 6 && red < 2 && log_super_x == -1)
    {
        SuperDuration = 60;
        CurAction = 4;
    }
    else if (CSRight_R<=10 && CSRight_G>=250 && CSRight_B>=250 && LoadedObjects != 6 && cyan < 2 && log_super_x == -1)
    {
        SuperDuration = 60;
        CurAction = 3;
    }
    else if (CSLeft_R<=10 && CSLeft_G>=250 && CSLeft_B>=250 && LoadedObjects != 6 && cyan < 2 && log_super_x == -1)
    {
        SuperDuration = 60;
        CurAction = 3;
    }
    else if (CSRight_R<=10 && CSRight_G<=10 && CSRight_B<=10 && LoadedObjects != 6 && black < 2 && log_super_x == -1)
    {
        SuperDuration = 60;
        CurAction = 2;
    }
    else if (CSLeft_R<=10 && CSLeft_G<=10 && CSLeft_B<=10 && LoadedObjects != 6 && black < 2 && log_super_x == -1)
    {
        SuperDuration = 60;
        CurAction = 2;
    }
    else if (CSRight_R>=250 && CSRight_G>=155 && CSRight_G<=165 && CSRight_B<=10 && ((red != 0 && black != 0 && cyan != 0)|| super != 0))
    {
        motor(70, 0);
    }
    else if (CSLeft_R>=250 && CSLeft_G>=155 && CSLeft_G<=165 && CSLeft_B<=10 && ((red != 0 && black != 0 && cyan != 0)|| super != 0))
    {
        motor(0, 70);
    }
    /*else if (either_color_detected(Color_const::TRAP_BLUE) && ((pos_x == 180 && pos_y == 50) || (pos_x == 160 && pos_y == 170) || (pos_x == 305 && pos_y == 115)) )
    {
        LoadedObjects = 0;
        red = 0;
        black = 0;
        cyan = 0;
        super = 0;
    }*/
    else if(Duration>0)
    {
        Duration--;
    }
    else if (CSRight_R>=250 && CSRight_G>=250 && CSRight_B<=10 && LoadedObjects > 0)
    {
        Duration = 4;
        motor(-40, 40);       
    }
    else if (CSLeft_R>=250 && CSLeft_G>=250 && CSLeft_B<=10 && LoadedObjects > 0)
    {
        Duration = 4;
        motor(40, -40);
    }
    else if (US_Front >= 30 && 10 >= US_Right && 10 >= US_Left)
    {
        motor(60, 60);
    }
    else if (US_Left < 13)
    {
        motor(60, -40);
    }
    else if (US_Right < 13)
    {
        motor(-40, 60);
    }
    else if (US_Front <= 8)
    {
        if(US_Left>US_Right)
        {
            motor(-50,50);
        }else
        {
            motor(50,-50);
        }
    }
    else if (log_super_x != -1)
    {
        /*if(log_super_y > 65 && log_super_y < 210 && log_super_x > 115 && log_super_x < 235){
            go_superobj();
        }else if(log_super_x < 95 || log_super_x > 260 || log_super_y < 40 || log_super_y > 235){
            if(PositionY > 65 && PositionY < 210 && PositionX > 115 && PositionX < 235){
                if (US_Right <= 20 || US_Left <= 20) 
                {
                    motor(75, 75);
                }else if(US_Left == 255){
                    Duration = 3;
                    motor(8,80);
                }else if(US_Right == 255){
                    Duration = 3;
                    motor(80,8);
                }
            }else{
                go_superobj();
            }
        }*/
        go_superobj();
    }
    /*else if(pos_x < 8 || pos_x > 340 || pos_y < 8 || pos_y > 250)
    {
        motor(-70,-70);
    }*/
    else if(PositionX == 0 && PositionY == 0){
        motor(70, 70);
    }
    else if(process == 0){
            if(go_position(40,140,10,10,15)){
                process = 1;
            }
        }else if(process == 1){
            if(go_position(90,200,10,10,15)){
                process = 2;
            }           
        }else if(process == 2){
            if(go_position(90,260,10,10,15)){
                process = 3;
            }           
        }else if(process == 3){
            if(go_position(10,260,10,10,15)){
                process = 4;
            }           
        }else if(process == 4){
            if(go_position(23,20,10,10,15)){
                process = 5;
            }           
        }else if(process == 5){
            if(go_position(225,15,10,10,15)){
                /*if(red != 0 && cyan != 0 && black != 0){
                    process = 18;
                }else{*/
                    process = 6;
                //}
            }        
        }else if(process == 6){
            if(go_position(225,70,10,10,15)){
                process = 7;
            }
        }else if(process == 7){
            if(go_position(345,75,10,10,15)){
                process = 8;
            }
        }else if(process == 8){
            if(go_position(225,130,10,10,15)){
                process = 9;
            }           
        }else if(process == 9){
            if(go_position(340,165,10,10,15)){
                process = 10;
            }           
        }else if(process == 10){
            if(go_position(270,185,10,10,15)){
                process = 11;
            }
        }else if(process == 11){
            if(go_position(270,260,10,10,15)){
                process = 12;
            }
        }else if(process == 12){
            if(go_position(140,260,10,10,15)){
                    process = 13;
            }           
        }else if(process == 13){
            if(go_position(165,150,10,10,15)){
                process = 0;
            }           
        }/*else if(process == 14){
            if(go_position(350,110,10,10,20)){
                process = 15;
            }           
        }else if(process == 15){
            if(go_position(345,50,10,10,20)){
                process = 16;
            }           
        }else if(process == 16){
            if(go_position(250,40,10,10,20)){
                process = 17;
            }           
        }else if(process == 17){
            if(go_position(335,20,10,10,20)){
                process = 0;
            }           
        }else if(process == 15){
            if(go_position(260,50,10,10,10)){
                process = 16;
            }           
        }else if(process == 16){
            if(go_position(325,25,10,10,10)){
                    process = 0;
            }           
        }else if(process == 17){
            if(go_position(50,15,10,10,10)){
                process = 18;
            }           
        }else if(process == 18){
            if(go_position(200,40,10,10,10)){
                process = 0;
            }           
        }*/

        //デポジットエリア
        else if(process == 14){
            if(go_position(150,90,10,10,10)){
                process = 4;
            }           
        }else if(process == 15){
            if(go_position(200,210,10,10,10)){
                process = 12;
            }           
        }
    else
    {
        motor(80, 80 + (rand() % 40 - 20));
    }


    switch (CurAction)
    {
    case 0:
        break;
    case 1:
        do_deposit();
        break;
    case 2:
        get_black_object();
        break;
    case 3:
        get_cyan_object();
        break;
    case 4:
        get_red_object();
        break;
    case 5:
        get_super_object();
        break;
    default:
        break;
    }

}


DLL_EXPORT void OnTimer()
{
    switch (CurGame)
    {
        case 9:
            break;
        case 10:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            break;
        case 1:
            Game1();
            break;
        default:
            break;
    }
}

