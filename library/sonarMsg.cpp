// This file is part of SonarPhony.
//
// SonarPhony is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SonarPhony is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SonarPhony.  If not, see <http://www.gnu.org/licenses/>.

#include "sonarMsg.h"
#include "msgPrivate.h"
using namespace sonarphony;

#include <QtEndian>

using namespace std;


///////////////////////////////////////////////////////////////////////////
namespace
{

enum
{
	O_SYNC          =  0,   // 4 bytes
	O_SIZE          =  4,   // 2 bytes, LE
	O_STATUS        =  6,   // 4 bytes, ASCII
	O_TYPE          = 10,   // 2 bytes
};

inline bool isSync (QByteArray const &buffer_, int offset_)
{
	static unsigned char const sync[] = { 0xFF, 0x00, 0xFF, 0x00 };

	if (buffer_.size () - offset_ < 4)
		return false;

	for (int i = 0; i < 4; ++i)
		if (static_cast<unsigned char> (buffer_[i + offset_]) != sync[i])
			return false;
	return true;
}

inline int decode (QByteArray const &buffer_, int start_)
{
	if (buffer_.size () - start_ <= 4)
		return 0;
	if (! isSync (buffer_, start_))
		return -1;

	int i = 4 + start_;
	for (; i < buffer_.size () && ! isSync (buffer_, i); ++i);

	return i - start_;
}

}


///////////////////////////////////////////////////////////////////////////
sonarMsg_t::~sonarMsg_t ()
{
}

sonarMsg_t::sonarMsg_t () :
	m_buffer ()
{
}

sonarMsg_t::sonarMsg_t (QByteArray const &buffer_) :
	m_buffer (buffer_)
{
	//Q_ASSERT (decode (buffer_, 0) == buffer_.size ());
}

sonarMsg_t::sonarMsg_t (sonarMsg_t const &other_) :
	m_buffer (other_.m_buffer)
{
}

sonarMsg_t::status_t sonarMsg_t::status () const
{
	Q_ASSERT (m_buffer.size () >= 10);

	char status[] = { m_buffer[6], m_buffer[7],
	                  m_buffer[8], m_buffer[9], 0 };

	if (strcmp (status, "REDY") == 0)
		return S_READY;
	if (strcmp (status, "BUSY") == 0)
		return S_BUSY;

	return S_UNKNOWN;
}

unsigned sonarMsg_t::reportedSize () const
{
	return qFromLittleEndian<quint16> (
		reinterpret_cast<unsigned char const *> (
			m_buffer.constData () + O_SIZE));
}

sonarMsg_t::type_t sonarMsg_t::type () const
{
	switch (reportedSize ())
	{
	case 10:        return T_BUSY;
	case 32:        return T_HANDSHAKE;
	case 340:       return T_PING;
	default:        return T_UNKNOWN;
	}
}


///////////////////////////////////////////////////////////////////////////
vector<sonarMsg_t> sonarphony::splitMessages (QByteArray const &buffer_)
{
	vector<sonarMsg_t> messages;
	int start = 0;

	while (start < buffer_.size ())
	{
		int const size = decode (buffer_, start);

		if (size < 0)
			start += -size;
		else if (size == 0)
			return messages;
		else if (size > 0)
		{
			messages.push_back (sonarMsg_t (
				buffer_.mid (start, size)));
			start += size;
		}
	}

	return messages;
}

string sonarphony::toString (sonarMsg_t::type_t type_)
{
	switch (type_)
	{
	case sonarMsg_t::T_BUSY:        return "Busy";
	case sonarMsg_t::T_HANDSHAKE:   return "Handshake";
	case sonarMsg_t::T_PING:        return "Ping";
	case sonarMsg_t::T_UNKNOWN:     return "Unknown";
	}

	Q_ASSERT (false);
	return "FATAL";
}

