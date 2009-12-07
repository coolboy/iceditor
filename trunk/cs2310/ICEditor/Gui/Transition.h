#pragma once

class Transition
{
public:
	Transition(void);
	~Transition(void);

	typedef std::vector<Transition> Transitions;
	static public Transitions load(QStringList strLs);

	//data
	int Id;
	QString Type;
	int srcId;
	int targetId;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(Id);
		ar & BOOST_SERIALIZATION_NVP(Type);
		ar & BOOST_SERIALIZATION_NVP(srcId);
		ar & BOOST_SERIALIZATION_NVP(targetId);
	}
};
