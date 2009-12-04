#pragma once

#include <boost/serialization/nvp.hpp>

class IndexCell
{
	//class State{
	//public:
	//	int	Id;
	//	QString Type;
	//};

public:
	IndexCell(void);
	~IndexCell(void);

	typedef QVector<IndexCell> IndexCells;
	static IndexCells Load(QStringList strLs);

	//data
	int Id;
	QString Name;
	QString MaxLifeTime;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(Id);
		ar & BOOST_SERIALIZATION_NVP(Name);
		ar & BOOST_SERIALIZATION_NVP(MaxLifeTime);
	}
};
