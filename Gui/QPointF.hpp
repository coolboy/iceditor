#pragma once

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>

namespace boost {
	namespace serialization {

		template<class Archive>
		void save(
			Archive & ar, 
			const QPointF& t, 
			const unsigned int /* version */
			){
				qreal x = t.x(), y = t.y();
				ar << boost::serialization::make_nvp("x", x);
				ar << boost::serialization::make_nvp("y", y);
		}

		template<class Archive>
		void load(
			Archive & ar, 
			QPointF& t, 
			const unsigned int /* version */
			){
				qreal x, y;
				ar >> boost::serialization::make_nvp("x", x);
				ar >> boost::serialization::make_nvp("y", y);
				t.setX(x);
				t.setY(y);
		}

		template<class Archive>
		void serialize(
			Archive& ar, 
			QPointF& t, 
			const unsigned int version
			){
				boost::serialization::split_free(ar, t, version);
		}

	} // namespace serialization
} // namespace boost