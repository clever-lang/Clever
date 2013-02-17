#include "modules/db/mysql/cmysql.h"
#include "modules/std/core/array.h"

#include <mysql/mysql.h>

namespace clever { 

void CMysql::init()
{
	m_connection = mysql_init(m_connection);
}

bool CMysql::connect()
{

	MYSQL *m;

	m = mysql_real_connect(m_connection, m_host.c_str(), m_user.c_str(),
		m_passwd.c_str(), m_db.c_str(), m_port, NULL, 0);

	// Clean the password as we do not want to keep this in memory
	m_passwd = "";

	return m != NULL;
}

int CMysql::query(const char *stmt)
{
	return mysql_query(m_connection, stmt);
}

ArrayObject* CMysql::fetchArray()
{
	return m_resultSet;
}


std::string CMysql::dump()
{
	std::stringstream strs;

	strs <<  "Host: '" << m_host << "'; "
		 <<  "User: '" << m_user << "'; "
		 <<  "DB: '"   << m_db   << "'; "
		 <<  "Port: '" << m_port << "'; "
		 ;

	return strs.str();
}



} // clever