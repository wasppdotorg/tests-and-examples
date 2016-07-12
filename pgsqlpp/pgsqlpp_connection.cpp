/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "pgsqlpp.hpp"

namespace pgsqlpp
{

	connection::connection(const std::string& host, const std::string& userid, const std::string& passwd, const std::string& database, unsigned int port, const std::string& charset, bool pooled_)
	{
		try
		{
			pgsql = pgsql_init(0);

			if (!pgsql_real_connect(pgsql, host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, 0, 0))
			{
				throw exception(__FILE__, __LINE__, pgsql_error(pgsql));
			}

			if (pgsql_set_character_set(pgsql, charset.c_str()) != 0)
			{
				throw exception(__FILE__, __LINE__, pgsql_error(pgsql));
			}
		}
		catch (...)
		{
			pgsql_close(pgsql);

			// let your own database manager or main call this function below
			//pgsql_library_end();

			throw;
		}

		auto time_ = std::time(nullptr);
		set_released(*std::localtime(&time_));
		set_pooled(pooled_);
	}

	connection::~connection()
	{
		pgsql_close(pgsql);

		// let your own database manager or main call this function below
		//pgsql_library_end();
	}

	bool connection::ping()
	{
		return pgsql_ping(pgsql) == 0;
	}

	statement* connection::prepare(const std::string& query)
	{
		return new statement(pgsql, query);
	}

	statement* connection::prepare_like(const std::string& query, bool left_percent, const std::string& keyword, bool right_percent)
	{
		auto escaped_keyword = new char[(keyword.size() * 2) + 1];

		pgsql_real_escape_string(pgsql, escaped_keyword, keyword.c_str(), keyword.size());

		std::ostringstream oss;
		oss << query << " '";

		if (left_percent)
		{
			oss << "%";
		}
		oss << escaped_keyword;
		delete[] escaped_keyword;

		if (right_percent)
		{
			oss << "%";
		}
		oss << "'";

		return new statement(pgsql, oss.str());
	}

} // namespace pgsqlpp
