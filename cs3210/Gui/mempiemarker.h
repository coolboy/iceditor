/*
 * Copyright (c) 2010, Sysu
 * ��������Ȩ����( All rights reserved. )
 * 
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
//-----------------------------------------------------------------
// This class shows how to extend QwtPlotItems. It displays a
// pie chart of user/total/idle cpu usage in percent.
//-----------------------------------------------------------------

#include <qwt_plot_item.h>

class MemPieMarker: public QwtPlotItem
{
public:
    MemPieMarker();

    virtual int rtti() const;

    virtual void draw(QPainter *p,
        const QwtScaleMap &, const QwtScaleMap &,
        const QRectF &rect) const;
};
