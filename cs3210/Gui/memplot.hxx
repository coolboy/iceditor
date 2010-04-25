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

class MemStat;

static const int HISTORY = 60; // seconds

class QwtPlotCurve;

class MemPlot : public QwtPlot 
{
    Q_OBJECT
public:
    enum DimmData
    {
        Dimm0,
        Dimm1,
        Dimm2,
        Dimm3,
				SysAllocRate,
				TestRate,

        NDimm
    };

    MemPlot(QWidget * = 0);

		void setDataSrc(MemStat* src);

    const QwtPlotCurve *memCurve(int id) const
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
    } data[NDimm];
    double timeData[HISTORY];

    int dataCount;
    MemStat* memStat;
};
