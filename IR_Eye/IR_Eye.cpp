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
�������ƣ�getMinNo
�������ܣ���ȡ����ͨ��������ֵ��С��ͨ�����
�����������
����ֵ������ֵ��С��ͨ�����
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
�������ƣ�getMinDir
�������ܣ���ȡ����ͨ��������ֵ��С��ͨ������Ӧ�ĽǶ�
�����������
����ֵ��ֵ��С��ͨ������Ӧ�ĽǶ�
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
�������ƣ�getAllValue
�������ܣ���ȡ����ͨ����ֵ
������������ڴ�����ݵ�����ָ��*arr
����ֵ����
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
