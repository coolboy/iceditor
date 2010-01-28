/*
 * Copyright (c) 2010, Sysu
 * ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�cpustat.h
 * �ļ���ʶ��
 * ժ    Ҫ��
 * 
 * ��ǰ�汾��1.0
 * ��    �ߣ�Coolboy
 * ������ڣ�2010��1��27��
 *
 * ȡ���汾��
 * ԭ����  ��
 * ������ڣ�
 */
#pragma once

#include <qdatetime.h>

class CpuStat 
{
public:
    CpuStat();
    void statistic(double &user, double &system);
    QTime upTime() const;

    enum Value
    {
        User,
        Nice,
        System,
        Idle,

        NValues
    };

private:
    void lookUp(double[NValues]) const;
    double procValues[NValues];
};
