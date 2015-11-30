#include "AttackMain.h"
#include <arduino.h>

LSM303          compass;
IR_Eye          eye(A0,10,360);

DC_MotorVerticalSquare<DC_Motor_EN> motor(DC_MotorPair<DC_Motor_EN>(DC_Motor_EN(4,5,21),DC_Motor_EN(6,7,22)),
        DC_MotorPair<DC_Motor_EN>(DC_Motor_EN(8,9,23),DC_Motor_EN(11,12,24)));
DC_Motor_EN_1   ballMotor(50,51,3);

US_Distance     xAxisUS1(30,31);
US_Distance     xAxisUS2(28,29);
US_Distance     yAxisUS1(32,33);
US_Distance     yAxisUS2(26,27);

AnalogGray_Color      xAxisGray1(A11,4);
AnalogGray_Color      xAxisGray2(A12,4);
AnalogGray_Color      yAxisGray1(A10,4);
AnalogGray_Color      yAxisGray2(A13,4);

float xAxisMagDir = 0.0;
//��������x�᷽��ĴŽǶȣ�����ȷ������
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
    //��ʼ��ƽ��ֵ

    color[0][0] = xAxisGray1.smoothRead();
    color[1][0] = xAxisGray2.smoothRead();
    color[2][0] = yAxisGray1.smoothRead();
    color[3][0] = yAxisGray2.smoothRead();
    //�����һ�����ݣ���Ҫ��֤���û����ڵ�һ�������������Ҳࣩɫ��

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
    //��һ�໺���ƶ���ͬʱ�����ɫ

    for(uchr i = 0; i < 8; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //ƽ������

    color[0][1] = xAxisGray1.smoothRead();
    color[1][1] = xAxisGray2.smoothRead();
    color[2][1] = yAxisGray1.smoothRead();
    color[3][1] = yAxisGray2.smoothRead();
    //����ڶ�����ɫ

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
    //�����ƶ�Ѱ�ҵ�����ɫ��

    for(uchr i = 0; i < 8; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //ƽ������

    color[0][2] = xAxisGray1.smoothRead();
    color[1][2] = xAxisGray2.smoothRead();
    color[2][2] = yAxisGray1.smoothRead();
    color[3][2] = yAxisGray2.smoothRead();
    //�����������ɫ

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
    //�˻ص��ڶ���ɫ��
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
    //��ֱ�ƶ���������ɫ��

    for(uchr i = 0; i < 8; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //ƽ������

    color[0][2] = xAxisGray1.smoothRead();
    color[1][2] = xAxisGray2.smoothRead();
    color[2][2] = yAxisGray1.smoothRead();
    color[3][2] = yAxisGray2.smoothRead();
    //������ĸ���ɫ

    uint addr = ColorStorageAddr;
    for(uchr i = 0; i < 4; ++i)
    {
        for(uchr j = 0; j < 4; ++j)
        {
            EEPROM_writeInt(addr,color[i][j]);
            addr += 2;
        }
    }
    //�����ݴ���EEPROM
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

//���ñ�������x�᷽��ĴŽǶȣ���Ҫ�ڴ�֮ǰ��ʼ�����̡�
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

    EEPROM_writeInt(xAxisStorageAddr,(uint)xAxisMagDir);
#ifdef DEBUG
    debugSerial.print("xAxis Mag Dir:");
    debugSerial.println(xAxisMagDir);
#endif // DEBUG
}

void loadxAxisMagDir(void)
{
    xAxisMagDir = (float)EEPROM_readInt(xAxisStorageAddr);
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
    return xAxisMagDir - compass.heading();
}

float getAngle2xAxis(float angle)
{
    return xAxisMagDir - angle;
}

//�������ĵ������������߽Ƕ�
float getAngle2Gate(void)
{
    Position<float> pos;
    pos = getCurPos();
    return getAngle2Gate(pos);
}

float getAngle2Gate(Position<float> &pos)
{
    float angle;
    angle = atan2f(gatePosition.y - pos.y,gatePosition.x - pos.x);
    return radian2degree(angle);
}

float getFaceAngle2Gate(void)
{
    return getAngle2xAxis() - getAngle2Gate();
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
    //�����ڳ��ض��λ�ö�β�����ƽ��ֵ

    IR.push(Avg(eye.getAllValue(),eye.getCntEye()));
    //˳��ͬʱ�⻷����
    for(uchr i = 0; i < 10; ++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray2.smoothRead();
        delay(100);
    }
    //��ʼ��ƽ��ֵ

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
    //˳��ͬʱ�⻷����
    eye.setEnvironIR(IR.avg());
#ifdef DEBUG
    debugSerial.print("Adjust Color No.");
    debugSerial.print(no);
    debugSerial.print(":");
    for(uchr i = 0; i < 4; ++i)
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
    static Position<float> pos,lastPos;
    static uchr area[4],t,i,j;
    static float angle;

    memset(area,0,sizeof(area));

    lastPos = pos;

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
    for(t = i = j = 0; i < 4; ++i)
    {
        if(area[i] > t)
        {
            t = area[i];
            j = i;
        }
    }

    pos.area = j;

    if(t == 0)
    {
        pos.x = xAxisUS1.getDistance() - xBlankWidth + halfSelfDiameter;
        pos.y = yAxisUS1.getDistance() - yBlankWidth + halfSelfDiameter;
        pos.area = 0xff;
    }
    else
    {
        angle = getAngle2xAxis(pos.angle);
        if(fabsf(sinf(degree2radian(angle))) < 0.5)
        {
            //�ǶȽ�С������ֱ���ó�������λ
            pos.x = xAxisUS1.getDistance() - xBlankWidth + halfSelfDiameter;
            switch(pos.area)
            {
            case 0:
            case 2:
                pos.y = yAxisUS1.getDistance() - yBlankWidth + halfSelfDiameter;
                break;
            case 1:
            case 3:
                pos.y = yAxisUS1.getDistance() - GateDepth + halfSelfDiameter;
                break;
            default:
                pos.y = yAxisUS1.getDistance() - yBlankWidth + halfSelfDiameter;
                break;
            }
            if(!face2Enemy(pos.angle))
            {
                pos.x = MapWidth - pos.x;
                pos.y = MapLength - pos.y;
            }
        }
        else
        {
            //�ǶȽϴ�x��ͨ��������ɫ��λ���ƣ�y��ͨ����һ�ε�λ�ù���
            switch(pos.area)
            {
            case 0:
                pos.x = 19.25 + (float)(area[1] + area[3]) / 4.0 * SelfDiameter;
                break;
            case 1:
                pos.x = 61.0 + (float)(-area[0] + area[2]) / 4.0 * SelfDiameter;
                break;
            case 2:
                pos.x = 102.75 - (float)(area[1] + area[3]) / 4.0 * SelfDiameter;
                break;
            case 3:
                pos.x = 61.0 + (float)(-area[0] + area[2]) / 4.0 * SelfDiameter;
                break;
            default:
                pos.x = pos.y = 0.0;
                break;
            }
            pos.y = lastPos.y + sinf(degree2radian(motor.getCurrentAngle())) * 20.0;
        }
    }
    return pos;
}

bool face2Enemy(float angle)
{
    return cosf(degree2radian(getAngle2xAxis(angle))) >= 0.0;
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
