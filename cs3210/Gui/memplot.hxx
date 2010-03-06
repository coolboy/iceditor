/*
 * Copyright (c) 2010, Sysu
 * 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：cpuplot.hxx
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
#include <qwt_plot.h>

#include "memstat.h"

static const int HISTORY = 60; // seconds

class QwtPlotCurve;

class MemPlot : public QwtPlot 
{
    Q_OBJECT
public:
    enum CpuData
    {
        User,
        System,
        Total,
        Idle,

        NCpuData
    };

    MemPlot(QWidget * = 0);
    const QwtPlotCurve *cpuCurve(int id) const
        { return data[id].curve; }

protected:
    void timerEvent(QTimerEvent *e);

private Q_SLOTS:
    void showCurve(QwtPlotItem *, bool on);

private:
    struct
    {
        QwtPlotCurve *curve;
        double data[HISTORY];
    } data[NCpuData];
    double timeData[HISTORY];

    int dataCount;
    MemStat cpuStat;
};
