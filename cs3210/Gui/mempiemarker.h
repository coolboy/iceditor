/*
 * Copyright (c) 2010, Sysu
 * 保留所有权利。( All rights reserved. )
 * 
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
