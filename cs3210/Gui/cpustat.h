/*
 * Copyright (c) 2010, Sysu
 * 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：cpustat.h
 * 文件标识：
 * 摘    要：
 * 
 * 当前版本：1.0
 * 作    者：Coolboy
 * 完成日期：2010年1月27日
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
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
