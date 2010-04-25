#include "StdAfx.h"

#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>

#include "mempiemarker.h"

#include "memplot.hxx"
#include "memstat.hxx"

#include "moc/moc_memplot.cpp"

class TimeScaleDraw: public QwtScaleDraw
{
public:
	TimeScaleDraw(const QTime &base):
			baseTime(base)
			{
			}
			virtual QwtText label(double v) const
			{
				QTime upTime = baseTime.addSecs((int)v);
				return upTime.toString();
			}
private:
	QTime baseTime;
};

class Background: public QwtPlotItem
{
public:
	Background()
	{
		setZ(0.0);
	}

	virtual int rtti() const
	{
		return QwtPlotItem::Rtti_PlotUserItem;
	}

	virtual void draw(QPainter *painter,
		const QwtScaleMap &, const QwtScaleMap &yMap,
		const QRectF &rect) const
	{
		QColor c(Qt::white);
		QRectF r = rect;

		for ( int i = 100; i > 0; i -= 10 )
		{
			r.setBottom(yMap.transform(i - 10));
			r.setTop(yMap.transform(i));
			painter->fillRect(r, c);

			c = c.dark(110);
		}
	}
};

class CpuCurve: public QwtPlotCurve
{
public:
	CpuCurve(const QString &title):
			QwtPlotCurve(title)
			{
				setRenderHint(QwtPlotItem::RenderAntialiased);
			}

			void setColor(const QColor &color)
			{
				setPen(color);
			}
};

MemPlot::MemPlot(QWidget *parent):
QwtPlot(parent),
dataCount(0)
{
	setAutoReplot(false);

	plotLayout()->setAlignCanvasToScales(true);

	QwtLegend *legend = new QwtLegend;
	legend->setItemMode(QwtLegend::CheckableItem);
	insertLegend(legend, QwtPlot::RightLegend);

	setAxisTitle(QwtPlot::xBottom, " System Uptime [h:m:s]");
	setAxisScaleDraw(QwtPlot::xBottom, 
		new TimeScaleDraw(QTime()));
	setAxisScale(QwtPlot::xBottom, 0, HISTORY);
	setAxisLabelRotation(QwtPlot::xBottom, -50.0);
	setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

	/*
	In situations, when there is a label at the most right position of the
	scale, additional space is needed to display the overlapping part
	of the label would be taken by reducing the width of scale and canvas.
	To avoid this "jumping canvas" effect, we add a permanent margin.
	We don't need to do the same for the left border, because there
	is enough space for the overlapping label below the left scale.
	*/

	QwtScaleWidget *scaleWidget = axisWidget(QwtPlot::xBottom);
	const int fmh = QFontMetrics(scaleWidget->font()).height();
	scaleWidget->setMinBorderDist(0, fmh / 2);

	setAxisTitle(QwtPlot::yLeft, "Rate [%]");
	setAxisScale(QwtPlot::yLeft, 0, 100);

	Background *bg = new Background();
	bg->attach(this);

	MemPieMarker *pie = new MemPieMarker();
	pie->attach(this);

	CpuCurve *curve = new CpuCurve("Dimm0");
	curve->setColor(Qt::red);
	curve->attach(this);
	data[Dimm0].curve = curve;

	curve = new CpuCurve("Dimm1");
	curve->setColor(Qt::blue);
	curve->setZ(curve->z() - 1);
	curve->attach(this);
	data[Dimm1].curve = curve;

	curve = new CpuCurve("Dimm2");
	curve->setColor(Qt::black);
	curve->setZ(curve->z() - 2);
	curve->attach(this);
	data[Dimm2].curve = curve;

	curve = new CpuCurve("Dimm3");
	curve->setColor(Qt::darkCyan);
	curve->setZ(curve->z() - 3);
	curve->attach(this);
	data[Dimm3].curve = curve;

	curve = new CpuCurve("SysAllocRate");
	curve->setColor(Qt::green);
	curve->setZ(curve->z() - 4);
	curve->attach(this);
	data[SysAllocRate].curve = curve;

	curve = new CpuCurve("TestRate");
	curve->setColor(Qt::yellow);
	curve->setZ(curve->z() - 5);
	curve->attach(this);
	data[TestRate].curve = curve;

	showCurve(data[Dimm0].curve, true);
	showCurve(data[Dimm1].curve, true);
	showCurve(data[Dimm2].curve, true);
	showCurve(data[Dimm3].curve, true);
	showCurve(data[SysAllocRate].curve, true);
	showCurve(data[TestRate].curve, true);

	for ( int i = 0; i < HISTORY; i++ )
		timeData[HISTORY - 1 - i] = i;

	connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
		SLOT(showCurve(QwtPlotItem *, bool)));

	memStat = 0;
}

void MemPlot::timerEvent(QTimerEvent *)
{
	for ( int i = dataCount; i > 0; i-- )
	{
		for ( int c = 0; c < NDimm; c++ )
		{
			if ( i < HISTORY )
				data[c].data[i] = data[c].data[i-1];
		}
	}

	double d0 = memStat->getData("Dimm0").toDouble();
	double d1 = memStat->getData("Dimm1").toDouble();
	double d2 = memStat->getData("Dimm2").toDouble();
	double d3 = memStat->getData("Dimm3").toDouble();
	double total = d0 + d1 + d2 + d3;

	data[Dimm0].data[0] = d0 * 100/total;
	data[Dimm1].data[0] = d1 * 100/total;
	data[Dimm2].data[0] = d2 * 100/total;
	data[Dimm3].data[0] = d3 * 100/total;

	double MaxSysAllocRate = 1000;
	double MaxTestRate = 800;

	data[SysAllocRate].data[0] = memStat->getData("SysAllocRate").toDouble() * 100/MaxSysAllocRate;
	data[TestRate].data[0] = memStat->getData("TestRate").toDouble() * 100/MaxTestRate;

	if ( dataCount < HISTORY )
		dataCount++;

	for ( int j = 0; j < HISTORY; j++ )
		timeData[j]++;

	setAxisScale(QwtPlot::xBottom, 
		timeData[HISTORY - 1], timeData[0]);

	for ( int c = 0; c < NDimm; c++ )
	{
		data[c].curve->setRawSamples(
			timeData, data[c].data, dataCount);
	}

	replot();
}

void MemPlot::showCurve(QwtPlotItem *item, bool on)
{
	item->setVisible(on);
	QWidget *w = legend()->find(item);
	if ( w && w->inherits("QwtLegendItem") )
		((QwtLegendItem *)w)->setChecked(on);

	replot();
}

void MemPlot::setDataSrc( MemStat* src )
{
	delete memStat;
	memStat = src;
	memStat->setParent(this);

	startTimer(1000); // 1 second
}

