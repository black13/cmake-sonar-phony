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

#pragma once

#include "sonarConnection.h"

#include <QObject>


class daemon_t : public QObject
{
	Q_OBJECT
public:
	~daemon_t ();
	daemon_t ();

	/// \brief Initialize the daemon
	void initialize ();

private slots:
	/// \brief Handle a ping message
	/// \param ping_ Ping message to handle
	void handlePing (sonarphony::pingMsg_t const &ping_);

private:
	/// \brief Connection to the sonar unit
	sonarphony::sonarConnection_t m_connection;
};
