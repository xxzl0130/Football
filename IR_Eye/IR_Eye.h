#ifndef __IR_EYE_H__
#define __IR_EYE_H__

#include "arduino.h"
#include "limits.h"
#include "string.h"

#ifndef uchr
#define uchr unsigned char
#endif // uchr

#ifndef uint
#define uint unsigned int
#endif // uint

class IR_Eye
{
private:
    uchr sPin;
    /**��ʼͨ���Ľӿڣ�����˳��**/
    uchr cntEye;
    /**������Ŀ**/
    uchr degreePerEye;
    /**ÿֻ�۶�Ӧ�ĽǶ�**/
    uint *value;

public:
    uint environIR;
    /**���������**/
    IR_Eye(uchr Pin = A0,uchr Cnt = 6,uint Angel = 180)
    {
        sPin = Pin;
        cntEye = Cnt;

        for(uchr i = 0; i < cntEye; ++i)
        {
            pinMode(sPin + i, INPUT);
        }

        degreePerEye = Angel / (cntEye - 1);

        value = new uint[cntEye];
        memset(value,0,sizeof(uint) * cntEye);
    }

    /******************************
    �������ƣ�printAll2Ser
    �������ܣ�������ͨ����ֵ��ӡ������
    �����������
    ����ֵ����
    ******************************/
#ifdef DEBUG
    //void printAll2Ser();
#endif

    /******************************
    �������ƣ�getMinNo
    �������ܣ���ȡ����ͨ��������ֵ��С��ͨ�����
    �����������
    ����ֵ������ֵ��С��ͨ�����
    ******************************/
    //uchr getMinNo(void);
    //uchr getMinNo(uint *arr);

    /******************************
    �������ƣ�getMinDir
    �������ܣ���ȡ����ͨ��������ֵ��С��ͨ������Ӧ�ĽǶ�
    �����������
    ����ֵ��ֵ��С��ͨ������Ӧ�ĽǶ�
    ******************************/
    //uint getMinDir(void);
    //uchr getMinDir(uint *arr);

    /******************************
    �������ƣ�getAllValue
    �������ܣ���ȡ����ͨ����ֵ
    ������������ڴ�����ݵ�����ָ��*arr
    ����ֵ����
    ******************************/
    //uint* getAllValue(uint *arr);
    //inline uint* getAllValue(void);

    /******************************
    �������ƣ�getMinValue
    �������ܣ���ȡ����ͨ��������ֵ��С��ֵ
    �����������
    ����ֵ����
    ******************************/
    //uint getMinValue(void);
    //uint getMinValue(uint *arr);

    //inline uint degreesPerEye(void) const;
    //inline uint getEnvironIR(void) const;
    //inline uint getCntEye(void) const;
    //inline void setEnvironIR(uint IR);
    /******************************
    �������ƣ�getAllValue
    �������ܣ���ȡ����ͨ����ֵ
    ������������ڴ�����ݵ�����ָ��*arr
    ����ֵ����
    ******************************/
    uint* getAllValue(uint *arr)
    {
        for(uchr i = 0; i < cntEye; ++i)
        {
            arr[i] = analogRead(sPin + i);
        }
        return arr;
    }

    inline uint* getAllValue(void)
    {
        return getAllValue(value);
    }

#ifdef DEBUG
    void printAll2Ser()
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
    uchr getMinNo(void)
    {
        getAllValue(value);
        return getMinNo(value);
    }

    uchr getMinNo(uint *arr)
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
    uint getMinDir(void)
    {
        return (uint)degreePerEye * getMinNo();
    }

    uchr getMinDir(uint *arr)
    {
        return degreePerEye * getMinNo(arr);
    }

    inline uint degreesPerEye(void) const
    {
        return this -> degreePerEye;
    }

    uint getMinValue(void)
    {
        getAllValue(value);
        return value[getMinNo(value)];
    }

    uint getMinValue(uint *arr)
    {
        return arr[getMinNo(arr)];
    }

    inline uint getEnvironIR(void) const
    {
        return environIR;
    }

    inline void setEnvironIR(uint IR)
    {
        environIR = IR;
    }

    inline uint getCntEye(void) const
    {
        return cntEye;
    }


};


#endif // __IR_EYE_H__
