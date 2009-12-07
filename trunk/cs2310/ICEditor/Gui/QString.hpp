#pragma once

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>

namespace boost {
	namespace serialization {

		template<class Archive>
		void save(
			Archive & ar, 
			const QString& t, 
			const unsigned int /* version */
			){
				std::wstring tmp = (wchar_t*)t.utf16();
				ar << boost::serialization::make_nvp("str", tmp);
		}

		template<class Archive>
		void load(
			Archive & ar, 
			QString& t, 
			const unsigned int /* version */
			){
				std::wstring tmp;
				ar >> boost::serialization::make_nvp("str", tmp);
				t = QString::fromUtf16((const ushort*)tmp.c_str());
		}

		template<class Archive>
		void serialize(
			Archive& ar, 
			QString& t, 
			const unsigned int version
			){
				boost::serialization::split_free(ar, t, version);
		}

	} // namespace serialization
} // namespace boost