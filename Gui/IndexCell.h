#pragma once

#include <boost/serialization/nvp.hpp>

#include "Transition.h"

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

	typedef QMap<int, IndexCell> IndexCells;
	static IndexCells Load(QStringList strLs, Transition::Transitions trans);

	//data
	int Id;
	int parentId;
	QString Name;
	QString MaxLifeTime;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(Id);
		ar & BOOST_SERIALIZATION_NVP(parentId);
		ar & BOOST_SERIALIZATION_NVP(Name);
		ar & BOOST_SERIALIZATION_NVP(MaxLifeTime);
	}
};
