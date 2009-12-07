#pragma once

#include <boost/serialization/nvp.hpp>

#include "Transition.h"

class IndexCell
{
public:
	class State{
	public:
		typedef std::vector<State> States;

	public:
		State():Id(-1){}
		//data
		int	Id;
		QString Type;
		QString Name;

		static States Load(QStringList strLs);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(Id);
			ar & BOOST_SERIALIZATION_NVP(Type);
			ar & BOOST_SERIALIZATION_NVP(Name);
		}
	};

public:
	IndexCell(void);
	~IndexCell(void);

	//static
	typedef QMap<int, IndexCell> IndexCells;
	//for all
	static IndexCells Load(QIODevice& src);
	//for fine
	static IndexCells Load(QStringList strLs, Transition::Transitions trans);

	//data
	int Id;
	int parentId;
	QString Name;
	QString MaxLifeTime;
	State::States states;
	Transition::Transitions strans;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(Id);
		ar & BOOST_SERIALIZATION_NVP(parentId);
		ar & BOOST_SERIALIZATION_NVP(Name);
		ar & BOOST_SERIALIZATION_NVP(MaxLifeTime);
		ar & BOOST_SERIALIZATION_NVP(states);
		ar & BOOST_SERIALIZATION_NVP(strans);
	}
};
