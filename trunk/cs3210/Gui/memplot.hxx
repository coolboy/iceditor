/*
 * Copyright (c) 2010, Sysu
 * ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�cpuplot.hxx
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
