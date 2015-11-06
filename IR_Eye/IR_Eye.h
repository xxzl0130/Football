#ifndef __IR_EYE_H__
#define __IR_EYE_H__

#include "arduino.h"
#include "limits.h"

#ifndef uchr
#define uchr unsigned char
#endif // uchr

#ifndef uint
#define uint unsigned int
#endif // uint

class IR_Eye{
private:
    uchr sPin;
    /**��ʼͨ���Ľӿڣ�����˳��**/
    uchr cntEye;
    /**������Ŀ**/
    uint degreePerEye;
    /**ÿֻ�۶�Ӧ�ĽǶ�**/

public:
    IR_Eye(uchr Pin = A0,uchr Cnt = 6,uint Angel = 180)
    {
        sPin = Pin;
        cntEye = Cnt;

        for(uchr i = 0;i < cntEye;++i)
        {
            pinMode(sPin + i, INPUT);
        }

        degreePerEye = Angel / (cntEye - 1);
    }

/******************************
�������ƣ�getMinNo
�������ܣ���ȡ����ͨ��������ֵ��С��ͨ�����
�����������
����ֵ������ֵ��С��ͨ�����
******************************/
    uchr getMinNo()
    {
        uint Min = INT_MAX, tmp;
        uchr no = 0;

        for(uchr i = 0;i < cntEye;++i)
        {
            tmp = analogRead(sPin + i);
            if(tmp < Min)
            {
                Min = tmp;
                no = i;
            }
        }

        return no;
    }

    uchr getMinNo(uint *arr)
    {
        uint Min = INT_MAX;
        uchr no = 0;

        for(uchr i = 0;i < cntEye;++i)
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
    uchr getMinDir()
    {
        return degreePerEye * getMinNo();
    }

    uchr getMinDir(uint *arr)
    {
        return degreePerEye * getMinNo(arr);
    }

/******************************
�������ƣ�getAllValue
�������ܣ���ȡ����ͨ����ֵ
������������ڴ�����ݵ�����ָ��*arr
����ֵ����
******************************/
    void getAllValue(uint *arr)
    {
        for(uchr i = 0;i < cntEye;++i)
        {
            arr[i] = analogRead(sPin + i);
        }
    }

/******************************
�������ƣ�printAll2Ser
�������ܣ�������ͨ����ֵ��ӡ������
�����������
����ֵ����
******************************/
    void printAll2Ser()
    {
#ifdef debugSerial
        for(uchr i = 0;i < cntEye;++i)
        {
            debugSerial.print(analogRead(sPin + i));
            debugSerial.print(" | ");
        }
        debugSerial.print("\n");
#endif // debugSerial
    }

    inline uint degreesPerEye(void) const
    {
        return this -> degreePerEye;
    }
};

#endif // __IR_EYE_H__
