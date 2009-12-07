#pragma once

#include <boost/serialization/nvp.hpp>

class IndexSystem
{
public:
	IndexSystem():Id(-1){}
	~IndexSystem(){}

	typedef QVector<IndexSystem> IndexSystems;
	static IndexSystems Load(QStringList strLs);

//data
	int Id;
	QString Name;
	QString Description;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(Id);
		ar & BOOST_SERIALIZATION_NVP(Name);
		ar & BOOST_SERIALIZATION_NVP(Description);
	}
};
