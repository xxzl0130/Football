#include "AttackMain.h"
#include <arduino.h>

LSM303          compass;
IR_Eye          eye(A0,10,360);

DC_MotorVerticalSquare<DC_Motor_EN> motor(DC_MotorPair<DC_Motor_EN>(DC_Motor_EN(4,5,21),DC_Motor_EN(6,7,22)),
        DC_MotorPair<DC_Motor_EN>(DC_Motor_EN(8,9,23),DC_Motor_EN(10,11,24)));

US_Distance     xAxisUS1(26,27);
US_Distance     xAxisUS2(28,29);
US_Distance     yAxisUS1(30,31);
US_Distance     yAxisUS2(32,33);

AnalogGray_Color      xAxisGray1(A10,4);
AnalogGray_Color      xAxisGray2(A11,4);
AnalogGray_Color      yAxisGray1(A12,4);
AnalogGray_Color      yAxisGray2(A13,4);

float xAxisMagDir = 0.0;
//比赛场地x轴方向的磁角度，用于确定方向

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
        motor.xAxisrun(BACKWORD,80);
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
        motor.yAxisRun(FORWORD,80);
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
    byte high,low;
    uint addr = ColorStorageAddr;

    for(uchr i = 0; i < 4; ++i)
    {
        high = EEPROM.read(addr++);
        low  = EEPROM.read(addr++);
        xAxisGray1.setColor(i,(uint)high << 8 | low);
    }

    for(uchr i = 0; i < 4; ++i)
    {
        high = EEPROM.read(addr++);
        low  = EEPROM.read(addr++);
        xAxisGray2.setColor(i,(uint)high << 8 | low);
    }

    for(uchr i = 0; i < 4; ++i)
    {
        high = EEPROM.read(addr++);
        low  = EEPROM.read(addr++);
        yAxisGray1.setColor(i,(uint)high << 8 | low);
    }

    for(uchr i = 0; i < 4; ++i)
    {
        high = EEPROM.read(addr++);
        low  = EEPROM.read(addr++);
        yAxisGray2.setColor(i,(uint)high << 8 | low);
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

    byte high,low;
    uint addr = CompassStorageAddr;

    high = (byte)(Offset.x >> 8);
    low  = (byte)(Offset.x & 0xff);
    EEPROM.write(addr++,high);
    EEPROM.write(addr++,low);
    high = (byte)(Offset.y >> 8);
    low  = (byte)(Offset.y & 0xff);
    EEPROM.write(addr++,high);
    EEPROM.write(addr++,low);
    high = (byte)(Offset.z >> 8);
    low  = (byte)(Offset.z & 0xff);
    EEPROM.write(addr++,high);
    EEPROM.write(addr++,low);

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

    high = EEPROM.read(addr++);
    low  = EEPROM.read(addr++);
    Offset.x = (int)high << 8 | low;
    high = EEPROM.read(addr++);
    low  = EEPROM.read(addr++);
    Offset.y = (int)high << 8 | low;
    high = EEPROM.read(addr++);
    low  = EEPROM.read(addr++);
    Offset.z = (int)high << 8 | low;

    compass.setOffset(Offset.x,Offset.y,Offset.z);
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

    flashLED();

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

inline float checkDistance(float dis)
{
    if(dis > MaxDistance || dis < MinDistance)
        return 0.0;
    else
        return dis;
}

Position<float> getCurPosInfo(void)
{
    float xAxis1,xAxis2,yAxis1,yAxis2,x,y;
    float angle;
    Position<float> pos;

    /*xAxis1 = checkDistance(xAxisUS1.getDistance());
    xAxis2 = checkDistance(xAxisUS2.getDistance());
    yAxis1 = checkDistance(yAxisUS1.getDistance());
    yAxis2 = checkDistance(yAxisUS2.getDistance());*/

    xAxis1 = xAxisUS1.getDistance();
    xAxis2 = xAxisUS2.getDistance();
    yAxis1 = yAxisUS1.getDistance();
    yAxis2 = yAxisUS2.getDistance();
    angle = getAngle2xAxis();

    x = xAxis1 + xAxis2 + SelfDiameter;
    y = yAxis1 + yAxis2 + SelfDiameter;

    x *= fabs(sin(degree2radian(angle)));
    y *= fabs(cos(degree2radian(angle)));

    //if(angle )
}

float getAngle2Ball(void)
{
    return eye.getMinDir() + eye.degreesPerEye();
}

float getAngle2xAxis(void)
{
    compass.read();
    return compass.heading() - xAxisMagDir;
}

inline bool keyPressed(uint8_t pin,uint8_t mode)
{
    if(digitalRead(pin) == mode)
    {
        delay(10);
        if(digitalRead(pin) == mode)
        {
            return true;
        }
    }
    return false;
}

void keyInterrupt(void)
{
    if(keyPressed(IntKey))
    {
        preset();
    }
}

void preset(void)
{
#ifdef DEBUG
    debugSerial.begin(9600);
    debugSerial.println("Start preset.");
    Serial.println("Start preset.");
#endif // DEBUG
    for(uchr i = 0; i < KeyPinCnt; ++i)
    {
        pinMode(KeyPinSt + i,INPUT);
    }
    while(1)
    {
        if(digitalRead(KeyPinSt) == HIGH)
        {
            delay(10);
            if(digitalRead(KeyPinSt) == HIGH)
            {
                adjustColor(0);
                flashLED();
            }
        }
        else if(digitalRead(KeyPinSt + 1) == HIGH)
        {
            delay(10);
            if(digitalRead(KeyPinSt + 1) == HIGH)
            {
                adjustColor(1);
                flashLED();
            }
        }
        else if(digitalRead(KeyPinSt + 2) == HIGH)
        {
            delay(10);
            if(digitalRead(KeyPinSt + 2) == HIGH)
            {
                adjustColor(2);
                flashLED();
            }
        }
        else if(digitalRead(KeyPinSt + 3) == HIGH)
        {
            delay(10);
            if(digitalRead(KeyPinSt + 3) == HIGH)
            {
                adjustColor(3);
                flashLED();
            }
        }
        else if(digitalRead(KeyPinSt + 4) == HIGH)
        {
            //1键对应校准磁场
            delay(10);
            if(digitalRead(KeyPinSt + 4) == HIGH)
            {
                setCompassOffset();
                flashLED();
            }
        }
        else if(digitalRead(KeyPinSt + 5) == HIGH)
        {
            delay(10);
            if(digitalRead(KeyPinSt + 5) == HIGH)
            {
                setXAxisMagDir();
                flashLED();
            }
        }
        else if(digitalRead(KeyPinSt + 6) == HIGH)
        {
            //结束键
            delay(10);
            if(digitalRead(KeyPinSt + 6) == HIGH)
            {
                Serial.println("End preset.");
                break;
            }
        }
    }
}

void adjustColor(uchr no)
{
    uint color[4];

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

    xAxisGray1.setColor(no,color[0]);
    xAxisGray2.setColor(no,color[1]);
    yAxisGray1.setColor(no,color[2]);
    yAxisGray2.setColor(no,color[3]);

#ifdef DEBUG
    debugSerial.print("Adjust Color No.");
    debugSerial.print(no);
    debugSerial.print(":");
    for(char i = 0;i < 4;++i)
    {
        debugSerial.print(color[i]);
        if(i < 3)
        {
            debugSerial.print(" | ");
        }
    }
    debugSerial.print("\n");
#endif // DEBUG
}
