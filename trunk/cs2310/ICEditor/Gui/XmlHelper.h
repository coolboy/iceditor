#pragma once

#include <QtXmlPatterns/QtXmlPatterns>

#ifdef _DEBUG
class MsgHandler : public QAbstractMessageHandler
{
	virtual void handleMessage(QtMsgType type,
		const QString &description,
		const QUrl &identifier,
		const QSourceLocation &sourceLocation)
	{
		qDebug()<<identifier<<" "<<description<<" "<<sourceLocation;
	}
};
#endif // _DEBUG

template<typename XQueryType>
QString XQuery (QIODevice& sourceDocument, XQueryType xquery)
{
	if (sourceDocument.open(QIODevice::ReadOnly) == false)
		return QString();

	QByteArray outArray;
	QBuffer sinkDocument(&outArray);
	sinkDocument.open(QIODevice::WriteOnly);

	QXmlQuery query;
	//query.setMessageHandler(new MsgHandler);
	query.bindVariable("inputDocument", &sourceDocument);
	query.setQuery(xquery);
	if (!query.isValid())
		return QString();

	QXmlFormatter formatter(query, &sinkDocument);
	if (!query.evaluateTo(&formatter))
		return QString();

	sinkDocument.close();
	sourceDocument.close();
	return QString::fromUtf8(outArray.constData());
}

template<typename XQueryType>
QStringList XQuery2 (QIODevice& sourceDocument, XQueryType xquery)
{
	sourceDocument.open(QIODevice::ReadOnly);
	QStringList strList;

	QXmlQuery query;
#ifdef _DEBUG
	query.setMessageHandler(new MsgHandler);
#endif // _DEBUG
	query.bindVariable("inputDocument", &sourceDocument);
	query.setQuery(xquery);
	query.evaluateTo(&strList);

	return strList;
}