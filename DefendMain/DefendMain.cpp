#include "DefendMain.h"
#include <arduino.h>
#include "limits.h"

#ifndef DEBUG
#define DEBUG
#define debugSerial Serial1
#endif

HMC5883L            compass;
IR_Eye              eye(A0,6,180);

DC_MotorPair<DC_Motor>  motor(DC_Motor(4,5),DC_Motor(6,7));

US_Distance         frontUS(8,9);

AnalogGray_Color    gray(A6,4);

float xAxisMagDir = 0.0;

void setCompassOffset()
{
#ifdef DEBUG
    debugSerial.println("setCompassOffset");
#endif
    Wire.begin();
    compass.setRange(HMC5883L_RANGE_1_3GA);
    compass.setMeasurementMode(HMC5883L_CONTINOUS);
    compass.setDataRate(HMC5883L_DATARATE_30HZ);
    compass.setSamples(HMC5883L_SAMPLES_8);

    int minX = INT_MAX,maxX = INT_MIN,minY = INT_MAX,maxY = INT_MIN;
    int offsetX,offsetY;

    motor.rotateRun(FORWORD, 128);
    delay(200);
#ifdef DEBUG
    debugSerial.println("setCompassOffset2");
#endif
    for(int i = 0; i < 500; ++i)
    {
        Vector mag = compass.readRaw();

        maxX = max(maxX,mag.XAxis);
        maxY = max(maxY,mag.XAxis);

        minX = min(minX,mag.XAxis);
        minY = min(minY,mag.XAxis);

        delay(20);
#ifdef DEBUG
        debugSerial.println(i);
#endif // DEBUG
    }

    motor.stop();

    offsetX = (maxX + minX) / 2;
    offsetY = (maxY + minY) / 2;

    uint addr = CompassStorageAddr;

    EEPROM_writeInt(addr,offsetX);
    addr += 2;
    EEPROM_writeInt(addr,offsetY);
    addr += 2;

    compass.setOffset(offsetX,offsetY);

#ifdef DEBUG
    debugSerial.print("Compass Offset:");
    debugSerial.print(offsetX);
    debugSerial.print(" | ");
    debugSerial.print(offsetY);
    debugSerial.print("\n");
#endif // DEBUG
}

void loadCompassOffset()
{
    byte high,low;
    uint addr = CompassStorageAddr;
    int offsetX,offsetY;

    Wire.begin();
    compass.setRange(HMC5883L_RANGE_1_3GA);
    compass.setMeasurementMode(HMC5883L_CONTINOUS);
    compass.setDataRate(HMC5883L_DATARATE_30HZ);
    compass.setSamples(HMC5883L_SAMPLES_8);

    offsetX = EEPROM_readInt(addr);
    addr += 2;
    offsetY = EEPROM_readInt(addr);

    compass.setOffset(offsetX,offsetY);

#ifdef DEBUG
    debugSerial.print("Compass Offset:");
    debugSerial.print(offsetX);
    debugSerial.print(" | ");
    debugSerial.print(offsetY);
    debugSerial.print("\n");
#endif // DEBUG
}

void setXAxisMagDir(void)
{
#ifdef DEBUG
    debugSerial.println("setXAxisMagDir");
#endif
    CircleQueue_Avg<float> heading;

    for(int i = 0; i < 10; ++i)
    {
        heading.push(compass.heading());
        delay(100);
    }

    xAxisMagDir = heading.avg();

#ifdef DEBUG
    debugSerial.print("xAxis Mag Dir:");
    debugSerial.println(xAxisMagDir);
#endif // DEBUG
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

template <typename T>
T Sum(T *arr,uint n)
{
    T s = T(0);
    for(uint i = 0; i < n; ++i)
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
    T t = 0;
    for(uchr i = 0; i < n; ++i)
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

float getAngle2xAxis(void)
{
    return xAxisMagDir - compass.heading();
}

float getAngle2xAxis(float angle)
{
    return xAxisMagDir - angle;
}
