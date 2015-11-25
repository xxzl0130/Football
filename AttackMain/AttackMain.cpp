#include "AttackMain.h"
#include <arduino.h>

LSM303          compass;
IR_Eye          eye(A0,10,360);

DC_MotorVerticalSquare<DC_Motor_EN> motor(DC_MotorPair<DC_Motor_EN>(DC_Motor_EN(4,5,21),DC_Motor_EN(6,7,22)),
        DC_MotorPair<DC_Motor_EN>(DC_Motor_EN(8,9,23),DC_Motor_EN(10,11,24)));
DC_Motor_EN_1   ballMotor(50,51,12);

US_Distance     xAxisUS1(30,31);
US_Distance     xAxisUS2(28,29);
US_Distance     yAxisUS1(32,33);
US_Distance     yAxisUS2(26,27);

AnalogGray_Color      xAxisGray1(A11,4);
AnalogGray_Color      xAxisGray2(A12,4);
AnalogGray_Color      yAxisGray1(A10,4);
AnalogGray_Color      yAxisGray2(A13,4);

float xAxisMagDir = 0.0;
//比赛场地x轴方向的磁角度，用于确定方向
const Position<float>    gatePosition(41.75,183.0);
const Position<float>    homePosition(41.75,0);

void presetColor()
{
    uint color[4][4];

    for(uchr i = 0; i < 10; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //初始化平均值

    color[0][0] = xAxisGray1.smoothRead();
    color[1][0] = xAxisGray2.smoothRead();
    color[2][0] = yAxisGray1.smoothRead();
    color[3][0] = yAxisGray2.smoothRead();
    //保存第一组数据，需要保证放置机器在第一个（最左侧或最右侧）色块

    do
    {
        motor.xAxis.run(FORWORD,80);
        delay(40);
    }
    while(abs(color[0][0] - xAxisGray1.smoothRead()) <= ColorThreshold ||
            abs(color[1][0] - xAxisGray2.smoothRead()) <= ColorThreshold ||
            abs(color[2][0] - yAxisGray1.smoothRead()) <= ColorThreshold ||
            abs(color[3][0] - yAxisGray2.smoothRead()) <= ColorThreshold);
    delay(40);
    motor.stop();
    //向一侧缓慢移动并同时检测颜色

    for(uchr i = 0; i < 8; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //平滑处理

    color[0][1] = xAxisGray1.smoothRead();
    color[1][1] = xAxisGray2.smoothRead();
    color[2][1] = yAxisGray1.smoothRead();
    color[3][1] = yAxisGray2.smoothRead();
    //存入第二组颜色

    do
    {
        motor.xAxis.run(FORWORD,80);
        delay(40);
    }
    while(abs(color[0][1] - xAxisGray1.smoothRead()) <= ColorThreshold ||
            abs(color[1][1] - xAxisGray2.smoothRead()) <= ColorThreshold ||
            abs(color[2][1] - yAxisGray1.smoothRead()) <= ColorThreshold ||
            abs(color[3][1] - yAxisGray2.smoothRead()) <= ColorThreshold);
    delay(40);
    motor.stop();
    //继续移动寻找第三个色块

    for(uchr i = 0; i < 8; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //平滑处理

    color[0][2] = xAxisGray1.smoothRead();
    color[1][2] = xAxisGray2.smoothRead();
    color[2][2] = yAxisGray1.smoothRead();
    color[3][2] = yAxisGray2.smoothRead();
    //存入第三个颜色

    do
    {
        motor.xAxis.run(BACKWORD,80);
        delay(40);
    }
    while(abs(color[0][1] - xAxisGray1.smoothRead()) > ColorThreshold ||
            abs(color[1][1] - xAxisGray2.smoothRead()) > ColorThreshold ||
            abs(color[2][1] - yAxisGray1.smoothRead()) > ColorThreshold ||
            abs(color[3][1] - yAxisGray2.smoothRead()) > ColorThreshold);
    delay(40);
    motor.stop();
    //退回到第二个色块
    do
    {
        motor.yAxis.run(FORWORD,80);
        delay(40);
    }
    while(abs(color[0][1] - xAxisGray1.smoothRead()) <= ColorThreshold ||
            abs(color[1][1] - xAxisGray2.smoothRead()) <= ColorThreshold ||
            abs(color[2][1] - yAxisGray1.smoothRead()) <= ColorThreshold ||
            abs(color[3][1] - yAxisGray2.smoothRead()) <= ColorThreshold);
    delay(40);
    motor.stop();
    //竖直移动到禁区黑色块

    for(uchr i = 0; i < 8; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //平滑处理

    color[0][2] = xAxisGray1.smoothRead();
    color[1][2] = xAxisGray2.smoothRead();
    color[2][2] = yAxisGray1.smoothRead();
    color[3][2] = yAxisGray2.smoothRead();
    //存入第四个颜色

    uint addr = ColorStorageAddr;
    byte high,low;
    for(uchr i = 0; i < 4; ++i)
    {
        for(uchr j = 0; j < 4; ++j)
        {
            high = (byte)(color[i][j] >> 8);
            low  = (byte)(color[i][j] & 0xff);
            EEPROM.write(addr++,high);
            EEPROM.write(addr++,low);
        }
    }
    //将数据存入EEPROM
}

void loadPresetColor()
{
    uint addr = ColorStorageAddr,val;
#ifdef DEBUG
    debugSerial.print("Preset Color:");
#endif // DEBUG
    for(uchr i = 0; i < 4; ++i)
    {
        val = EEPROM_readInt(addr);
        addr += 2;
        xAxisGray1.setColor(i,val);
#ifdef DEBUG
        debugSerial.print(val);
        debugSerial.print(" | ");
#endif // DEBUG
        val = EEPROM_readInt(addr);
        addr += 2;
        xAxisGray2.setColor(i,val);
#ifdef DEBUG
        debugSerial.print(val);
        debugSerial.print(" | ");
#endif // DEBUG
        val = EEPROM_readInt(addr);
        addr += 2;
        yAxisGray1.setColor(i,val);
#ifdef DEBUG
        debugSerial.print(val);
        debugSerial.print(" | ");
#endif // DEBUG
        val = EEPROM_readInt(addr);
        addr += 2;
        yAxisGray1.setColor(i,val);
#ifdef DEBUG
        debugSerial.print(val);
        debugSerial.print("\n");
#endif // DEBUG
    }
}

void setCompassOffset()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();

    LSM303::vector<int> Max;
    LSM303::vector<int> Min;
    LSM303::vector<int> Offset;
    Max.x = Max.y = Max.z = -32768;
    Min.x = Min.y = Min.z = 32767;

    motor.rotateRun(FORWORD, 64);
    delay(200);

    for(int i = 0; i < 500; ++i)
    {
        compass.read();

        Max.x = max(Max.x,compass.m.x);
        Max.y = max(Max.y,compass.m.y);
        Max.z = max(Max.z,compass.m.z);

        Min.x = min(Min.x,compass.m.x);
        Min.y = min(Min.y,compass.m.y);
        Min.z = min(Min.z,compass.m.z);

        delay(20);
    }

    Offset.x = (Max.x + Min.x) / 2;
    Offset.y = (Max.y + Min.y) / 2;
    Offset.z = (Max.z + Min.z) / 2;

    uint addr = CompassStorageAddr;

    EEPROM_writeInt(addr,Offset.x);
    addr += 2;
    EEPROM_writeInt(addr,Offset.y);
    addr += 2;
    EEPROM_writeInt(addr,Offset.z);
    addr += 2;

    compass.setOffset(Offset.x,Offset.y,Offset.z);

    motor.stop();

#ifdef DEBUG
    debugSerial.print("Compass Offset:");
    debugSerial.print(Offset.x);
    debugSerial.print(" | ");
    debugSerial.print(Offset.y);
    debugSerial.print(" | ");
    debugSerial.print(Offset.z);
    debugSerial.print("\n");
#endif // DEBUG
}

void loadCompassOffset()
{
    byte high,low;
    uint addr = CompassStorageAddr;
    LSM303::vector<int> Offset;

    Wire.begin();
    compass.init();
    compass.enableDefault();

    Offset.x = EEPROM_readInt(addr);
    addr += 2;
    Offset.y = EEPROM_readInt(addr);
    addr += 2;
    Offset.z = EEPROM_readInt(addr);

    compass.setOffset(Offset.x,Offset.y,Offset.z);

#ifdef DEBUG
    debugSerial.print("Compass Offset:");
    debugSerial.print(Offset.x);
    debugSerial.print(" | ");
    debugSerial.print(Offset.y);
    debugSerial.print(" | ");
    debugSerial.print(Offset.z);
    debugSerial.print("\n");
#endif // DEBUG
}

//设置比赛场地x轴方向的磁角度，需要在此之前初始化罗盘。
void setXAxisMagDir(void)
{
    CircleQueue_Avg<float> heading;

    for(int i = 0; i < 10; ++i)
    {
        compass.read();
        heading.push(compass.heading());
        delay(100);
    }

    xAxisMagDir = heading.avg();

#ifdef DEBUG
    debugSerial.print("xAxis Mag Dir:");
    debugSerial.println(xAxisMagDir);
#endif // DEBUG
}

void flashLED(uint time)
{
    pinMode(13,OUTPUT);

    for(int i = 0; i < 3; ++i)
    {
        digitalWrite(13,HIGH);
        delay(time);
        digitalWrite(13,LOW);
        delay(time);
    }
}

float checkDistance(float dis)
{
    if(dis > MaxDistance || dis < MinDistance)
        return 0.0;
    else
        return dis;
}

float getAngle2Ball(void)
{
    return eye.getMinDir() + 45.0;
}

float getAngle2Ball(uint *arr)
{
    return eye.getMinDir(arr) + 45.0;
}

float getAngle2xAxis(void)
{
    return compass.heading() - xAxisMagDir;
}

float getAngle2Gate(void)
{
    Position<float> pos;
    float angle;
    pos = getCurPos();
    angle = atan2f(gatePosition.y - pos.y,gatePosition.x - pos.x);
    return radian2degree(angle);
}

float getAngle2Home(void)
{
    Position<float> pos;
    float angle;
    pos = getCurPos();
    angle = atan2f(homePosition.y - pos.y,homePosition.x - pos.x);
    return radian2degree(angle);
}

bool keyPressed(uchr pin,uchr mode)
{
    if(digitalRead(pin) == mode)
    {
        delay10ms();
        if(digitalRead(pin) == mode)
        {
            return true;
        }
    }
    return false;
}

/*uchr judgeArea(void)
{
    uchr area[4] = {0},t,i,j;

    for(uchr i = 0;i < 10;++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray2.smoothRead();
        //delay(1);
    }
    if((t = xAxisGray1.color(xAxisGray1.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    else
    {
        return 0xff;
    }
    if((t = xAxisGray2.color(xAxisGray2.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    else
    {
        return 0xff;
    }
    if((t = yAxisGray1.color(yAxisGray1.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    else
    {
        return 0xff;
    }
    if((t = yAxisGray2.color(yAxisGray2.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    else
    {
        return 0xff;
    }

    for(t = i = 0; i < 4; ++i)
    {
        if(area[i] > t)
        {
            t = area[i];
            j = i;
        }
    }
    return j;
}*/

uchr judgeArea(void)
{
    uchr area[4] = {0},t,i,j;

    if((t = xAxisGray1.color()) != 0xff)
    {
        ++area[t];
    }
    if((t = xAxisGray2.color()) != 0xff)
    {
        ++area[t];
    }
    if((t = yAxisGray1.color()) != 0xff)
    {
        ++area[t];
    }
    if((t = yAxisGray2.color()) != 0xff)
    {
        ++area[t];
    }

    for(t = i = 0; i < 4; ++i)
    {
        if(area[i] > t)
        {
            t = area[i];
            j = i;
        }
    }
    return t ? j : 0xff;
}

bool inArea(uchr no)
{
    uchr area[4] = {0},t;

    if((t = xAxisGray1.color(xAxisGray1.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    if((t = xAxisGray2.color(xAxisGray2.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    if((t = yAxisGray1.color(yAxisGray1.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    if((t = yAxisGray2.color(yAxisGray2.smoothRead())) != 0xff)
    {
        ++area[t];
    }
    return area[no] != 0;
}

void preset(void)
{
#ifdef DEBUG
    debugSerial.println("Start preset.");
#endif // DEBUG
    for(uchr i = 0; i < KeyPinCnt; ++i)
    {
        pinMode(KeyPinSt + i,INPUT);
    }
    while(1)
    {
        if(keyPressed(KeyPinSt))
        {
            adjustColor(0);
            flashLED();
        }
        else if(keyPressed(KeyPinSt + 1))
        {
            adjustColor(1);
            flashLED();
        }
        else if(keyPressed(KeyPinSt + 2))
        {
            adjustColor(2);
            flashLED();
        }
        else if(keyPressed(KeyPinSt + 3))
        {
            adjustColor(3);
            flashLED();
        }
        else if(keyPressed(KeyPinSt + 4))
        {
            setCompassOffset();
            flashLED();
        }
        else if(keyPressed(KeyPinSt + 5))
        {
            setXAxisMagDir();
            flashLED();
        }
        else if(keyPressed(KeyPinSt + 6))
        {
#ifdef DEBUG
            debugSerial.println("End preset.");
#endif // DEBUG
            flashLED();
            break;
        }
    }
}

void adjustColor(uchr no)
{
    uint color[4];
    static Queue_Avg<uint> avg[4][4];
    static Queue_Avg<uint> IR;
    //用于在场地多个位置多次测量求平均值

    IR.push(Avg(eye.getAllValue(),eye.getCntEye()));
    //顺带同时测环境光
    for(uchr i = 0; i < 10; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray2.smoothRead();
        delay(100);
    }
    //初始化平均值

    color[0] = xAxisGray1.smoothRead();
    color[1] = xAxisGray2.smoothRead();
    color[2] = yAxisGray1.smoothRead();
    color[3] = yAxisGray2.smoothRead();

    for(uchr i = 0; i < 4; ++i)
    {
        avg[i][no].push(color[i]);
    }

    xAxisGray1.setColor(no,avg[0][no].avg());
    xAxisGray2.setColor(no,avg[1][no].avg());
    yAxisGray1.setColor(no,avg[2][no].avg());
    yAxisGray2.setColor(no,avg[3][no].avg());

    IR.push(Avg(eye.getAllValue(),eye.getCntEye()));
    //顺带同时测环境光
    eye.setEnvironIR(IR.avg());
#ifdef DEBUG
    debugSerial.print("Adjust Color No.");
    debugSerial.print(no);
    debugSerial.print(":");
    for(char i = 0; i < 4; ++i)
    {
        debugSerial.print(avg[i][no].avg());
        if(i < 3)
        {
            debugSerial.print(" | ");
        }
    }
    debugSerial.print("\n");
#endif // DEBUG

    uint addr = ColorStorageAddr + no * 4 * 2;
    for(uchr i = 0; i < 4; ++i)
    {
        EEPROM_writeInt(addr,avg[i][no].avg());
        addr += 2;
    }
}

Position<float> getCurPos(void)
{
    Position<float> pos;
    static uchr area[4],t,i,j,k = 0;
    if(!k)
        pos.angle = compass.heading();
    if((t = xAxisGray1.color()) != 0xff)
    {
        ++area[t];
    }
    if((t = xAxisGray2.color()) != 0xff)
    {
        ++area[t];
    }
    if((t = yAxisGray1.color()) != 0xff)
    {
        ++area[t];
    }
    if((t = yAxisGray2.color()) != 0xff)
    {
        ++area[t];
    }
    for(t = i = 0; i < 4; ++i)
    {
        if(area[i] > t)
        {
            t = area[i];
            j = i;
        }
    }
    if(t == 0)
    {
        pos.x = pos.y = 0;
    }
    else
    {
        switch(j)
        {
        case 0:
            pos.x = 19.25 + (float)(area[1] + area[3]) / 4.0 * SelfDiameter;
            if(fabsf(pos.angle - xAxisMagDir) <= 40.0)
            {
                pos.y = yAxisUS1.getDistance() + BlankWidth;
            }
            else if(fabsf(fabsf(pos.angle - xAxisMagDir) - 180) < 40.0)
            {
                pos.y = yAxisUS2.getDistance() + BlankWidth;
            }
            else
            {
                pos.y = 0;
            }
            break;
        case 1:
            pos.x = 61.0 + (float)(-area[0] + area[2]) / 4.0 * SelfDiameter;
            if(fabsf(pos.angle - xAxisMagDir) <= 40.0)
            {
                pos.y = yAxisUS1.getDistance() + GateDepth;
            }
            else if(fabsf(fabsf(pos.angle - xAxisMagDir) - 180) < 40.0)
            {
                pos.y = yAxisUS2.getDistance() + GateDepth;
            }
            else
            {
                pos.y = 0;
            }
            break;
        case 2:
            pos.x = 102.75 - (float)(area[1] + area[3]) / 4.0 * SelfDiameter;
            if(fabsf(pos.angle - xAxisMagDir) <= 40.0)
            {
                pos.y = yAxisUS1.getDistance() + BlankWidth;
            }
            else if(fabsf(fabsf(pos.angle - xAxisMagDir) - 180) < 40.0)
            {
                pos.y = yAxisUS2.getDistance() + BlankWidth;
            }
            else
            {
                pos.y = 0;
            }
            break;
        case 3:
            pos.x = 61.0 + (float)(-area[0] + area[2]) / 4.0 * SelfDiameter;
            if(fabsf(pos.angle - xAxisMagDir) <= 40.0)
            {
                pos.y = yAxisUS1.getDistance() + GateDepth;
            }
            else if(fabsf(fabsf(pos.angle - xAxisMagDir) - 180) < 40.0)
            {
                pos.y = yAxisUS2.getDistance() + GateDepth;
            }
            else
            {
                pos.y = 0;
            }
            break;
        default:
            pos.x = pos.y = 0.0;
            break;
        }
    }
    k = (k + 1) % 10;
    return pos;
}

bool face2Enemy(float angle)
{
    return fabsf(angle - xAxisMagDir) <= 180.0 ||
           fabsf(angle - xAxisMagDir + 360.0) <= 180.0 ||
           fabsf(angle - xAxisMagDir - 360.0) <= 180.0;
}

bool face2Enemy(void)
{
    return face2Enemy(compass.heading());
}

bool ballFace2Enemy(void)
{
    return face2Enemy(getAngle2xAxis() + getAngle2Ball());
}

bool face2Ball(uint* arr)
{
    uchr t = eye.getMinNo(arr);
    return t == 0 || t == 1;
}
bool face2Ball(void)
{
    uchr t = eye.getMinNo();
    return t == 0 || t == 1;
}

template <typename T>
T Sum(T *arr,uint n)
{
    T s = arr[0];
    for(uint i = 1; i < n; ++i)
        s += arr[i];
    return s;
}

template <typename T>
T Avg(T *arr,uint n)
{
    return T(Sum(arr,n) / T(n));
}

template <typename T>
T Max(T *arr,uint n)
{
    T t = arr[0];
    for(uchr i = 1; i < n; ++i)
    {
        if(arr[i] > t)
        {
            t = arr[i];
        }
    }
    return t;
}

void EEPROM_writeInt(uint addr,uint data)
{
    byte high,low;

    high = (byte)(data >> 8);
    low  = (byte)(data & 0xff);
    EEPROM.write(addr,high);
    EEPROM.write(addr + 1,low);
}

uint EEPROM_readInt(uint addr)
{
    byte high,low;
    high = EEPROM.read(addr++);
    low  = EEPROM.read(addr++);
    return (int)high << 8 | low;
}

void delay10ms()
{
    unsigned int i = 26400;
    while(--i)
        NOP;
}
