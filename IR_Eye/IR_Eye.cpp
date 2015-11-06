#include "IR_Eye.h"

#ifdef DEBUG
void IR_Eye::printAll2Ser()
{
    for(uchr i = 0; i < cntEye; ++i)
    {
        debugSerial.print(analogRead(sPin + i));
        debugSerial.print(" | ");
    }
    debugSerial.print("\n");
}
#endif
/******************************
函数名称：getMinNo
函数功能：读取所有通道并返回值最小的通道编号
传入参数：无
返回值：返回值最小的通道编号
******************************/
uchr IR_Eye::getMinNo(void)
{
    getAllValue(value);
    return getMinNo(value);
}

uchr IR_Eye::getMinNo(uint *arr)
{
    uint Min = INT_MAX;
    uchr no = 0;

    for(uchr i = 0; i < cntEye; ++i)
    {
        if(arr[i] < Min)
        {
            Min = arr[i];
            no = i;
        }
    }

    return no;
}

/******************************
函数名称：getMinDir
函数功能：读取所有通道并计算值最小的通道所对应的角度
传入参数：无
返回值：值最小的通道所对应的角度
******************************/
uint IR_Eye::getMinDir(void)
{
    return (uint)degreePerEye * getMinNo();
}

uchr IR_Eye::getMinDir(uint *arr)
{
    return degreePerEye * getMinNo(arr);
}

/******************************
函数名称：getAllValue
函数功能：读取所有通道的值
传入参数：用于存放数据的数组指针*arr
返回值：无
******************************/
uint* IR_Eye::getAllValue(uint *arr)
{
    for(uchr i = 0; i < cntEye; ++i)
    {
        arr[i] = analogRead(sPin + i);
    }
    return arr;
}
inline uint* IR_Eye::getAllValue(void)
{
    return getAllValue(value);
}

inline uint IR_Eye::degreesPerEye(void) const
{
    return this -> degreePerEye;
}

uint IR_Eye::getMinValue(void)
{
    getAllValue(value);
    return value[getMinNo(value)];
}

inline uint IR_Eye::getEnvironIR(void) const
{
    return environIR;
}

inline void IR_Eye::setEnvironIR(uint IR)
{
    environIR = IR;
}

inline uint IR_Eye::getCntEye(void) const
{
    return cntEye;
}
