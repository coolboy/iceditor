#pragma once

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>

class ICCard
{
public:
	typedef QHash<int, ICCard> ICCards;

	enum Pattern{
		Undef,
		QuietState,//quietstate
		MyselfNone,//myselfnone
		MyselfWith,//myselfwith
		OthersNone,//othersnone
		OthersWith,//otherswith
		MixedState//mixedstate
	};

public:
	static ICCards Load(QStringList icdb, QStringList icci);

public:
	ICCard(void);
	~ICCard(void);

	QString getSimpleStr() const;

//private:
	int Id;
	int parentId;
	QString	Name;
	QString	Description;
	Pattern IntPattern;
	QString Task;
	QString TimeCriticalCondition;
	int NumberCurrent;
	int NumberTotal;
	//ic other
	QString	OtherName;
	int otherId;
	QString OtherMessage;
	QString OtherTask;
	//ic group
	QString indexCellName;
	QString groupName;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(Id);
		ar & BOOST_SERIALIZATION_NVP(parentId);
		ar & BOOST_SERIALIZATION_NVP(Name);
		ar & BOOST_SERIALIZATION_NVP(Description);
		ar & BOOST_SERIALIZATION_NVP(IntPattern);
		ar & BOOST_SERIALIZATION_NVP(Task);
		ar & BOOST_SERIALIZATION_NVP(TimeCriticalCondition);
		ar & BOOST_SERIALIZATION_NVP(NumberCurrent);
		ar & BOOST_SERIALIZATION_NVP(NumberTotal);
		ar & BOOST_SERIALIZATION_NVP(OtherName);
		ar & BOOST_SERIALIZATION_NVP(otherId);
		ar & BOOST_SERIALIZATION_NVP(OtherMessage);
		ar & BOOST_SERIALIZATION_NVP(OtherTask);
		ar & BOOST_SERIALIZATION_NVP(indexCellName);
		ar & BOOST_SERIALIZATION_NVP(groupName);
	}
};