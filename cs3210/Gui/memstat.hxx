#pragma once

class MemStat : public QObject
{
	Q_OBJECT
public:
	MemStat(QObject* parent);

	QVariant getData(const QString &key);

private:
	QSettings test_out;
};
