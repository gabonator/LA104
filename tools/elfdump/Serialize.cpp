#include "Serialize.h"
	
	CSerialize& CSerialize::operator <<( CStream& stream )
	{
		_ASSERT(0);
		//stream << CStream(this, sizeof(*this));
		return *this;
	}

	CSerialize& CSerialize::operator >>( CStream& stream )
	{
		_ASSERT(0);
		//stream >> CStream(this, sizeof(*this));
		return *this;
	}
	