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

class IR_Eye{
private:
    uchr sPin;
    /**��ʼͨ���Ľӿڣ�����˳��**/
    uchr cntEye;
    /**������Ŀ**/
    uchr degreePerEye;
    /**ÿֻ�۶�Ӧ�ĽǶ�**/
    uint environIR;
    /**���������**/
    uint *value;

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

        value = new uint[cnt];
        memset(value,0,sizeof(uint) * cnt);
    }

/******************************
�������ƣ�printAll2Ser
�������ܣ�������ͨ����ֵ��ӡ������
�����������
����ֵ����
******************************/
#ifdef DEBUG
    void printAll2Ser();
#endif

/******************************
�������ƣ�getMinNo
�������ܣ���ȡ����ͨ��������ֵ��С��ͨ�����
�����������
����ֵ������ֵ��С��ͨ�����
******************************/
    uchr getMinNo(void);
    uchr getMinNo(uint *arr);

/******************************
�������ƣ�getMinDir
�������ܣ���ȡ����ͨ��������ֵ��С��ͨ������Ӧ�ĽǶ�
�����������
����ֵ��ֵ��С��ͨ������Ӧ�ĽǶ�
******************************/
    uint getMinDir(void);
    uchr getMinDir(uint *arr);

/******************************
�������ƣ�getAllValue
�������ܣ���ȡ����ͨ����ֵ
������������ڴ�����ݵ�����ָ��*arr
����ֵ����
******************************/
    uint* getAllValue(uint *arr);
    inline uint* getAllValue(void);

/******************************
�������ƣ�getMinValue
�������ܣ���ȡ����ͨ��������ֵ��С��ֵ
�����������
����ֵ����
******************************/
    uint getMinValue(void);

    inline uint degreesPerEye(void) const;
    inline uint getEnvironIR(void) const;
    inline uint getCntEye(void) const;
    inline void setEnvironIR(uint IR);
};

#endif // __IR_EYE_H__
