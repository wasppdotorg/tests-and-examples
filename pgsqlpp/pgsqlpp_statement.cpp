/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "pgsqlpp.hpp"

namespace pgsqlpp
{

	statement::statement(st_pgsql* pgsql, const std::string& query)
	{
		try
		{
			stmt = pgsql_stmt_init(pgsql);
			if (!stmt)
			{
				throw exception(__FILE__, __LINE__, "stmt_init failed");
			}

			if (pgsql_stmt_prepare(stmt, query.c_str(), query.size()) != 0)
			{
				throw exception(__FILE__, __LINE__, pgsql_stmt_error(stmt));
			}

			param_count = pgsql_stmt_param_count(stmt);

			binds.resize(0);
			binds.resize(param_count, st_pgsql_bind());

			lengths.resize(0);
			lengths.resize(param_count, 0);

			bind_index = 0;
		}
		catch (...)
		{
			if (stmt)
			{
				pgsql_stmt_close(stmt);
			}

			throw;
		}
	}

	statement::~statement()
	{
		pgsql_stmt_close(stmt);
	}

	void statement::param(const unsigned char& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_TINY;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const short int& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_SHORT;
		b.buffer = (void*)&value;
		b.is_unsigned = 0;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const unsigned short int& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_SHORT;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const int& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 0;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const unsigned int& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const long long int& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONGLONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 0;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const unsigned long long int& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONGLONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const float& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_FLOAT;
		b.buffer = (void*)&value;
		b.is_null = 0;

		lengths[bind_index - 1] = sizeof(value);
		b.length = &lengths[bind_index - 1];
	}

	void statement::param(const double& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_DOUBLE;
		b.buffer = (void*)&value;
		b.is_null = 0;

		lengths[bind_index - 1] = sizeof(value);
		b.length = &lengths[bind_index - 1];
	}

	void statement::param(const std::string& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_STRING;
		b.buffer = (void*)value.c_str();
		b.is_null = 0;

		lengths[bind_index - 1] = value.size();
		b.buffer_length = lengths[bind_index - 1];
		b.length = &lengths[bind_index - 1];
	}

	void statement::param_blob(const std::string& value)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_BLOB;
		b.buffer = (void*)value.c_str();
		b.is_null = 0;

		lengths[bind_index - 1] = value.size();
		b.buffer_length = lengths[bind_index - 1];
		b.length = &lengths[bind_index - 1];
	}

	void statement::param_null(char is_null)
	{
		st_pgsql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_NULL;
		b.is_null = &is_null;
	}

	unsigned long long int statement::execute()
	{
		unsigned long long int affected_rows = 0;

		try
		{
			if (param_count > 0 && !binds.empty())
			{
				if (pgsql_stmt_bind_param(stmt, &binds.front()) != 0)
				{
					throw exception(__FILE__, __LINE__, pgsql_stmt_error(stmt));
				}
			}

			if (pgsql_stmt_execute(stmt) != 0)
			{
				throw exception(__FILE__, __LINE__, pgsql_stmt_error(stmt));
			}

			affected_rows = pgsql_stmt_affected_rows(stmt);
		}
		catch (...)
		{
			throw;
		}

		return affected_rows;
	}

	result* statement::query()
	{
		try
		{
			if (param_count > 0 && !binds.empty())
			{
				if (pgsql_stmt_bind_param(stmt, &binds.front()) != 0)
				{
					throw exception(__FILE__, __LINE__, pgsql_stmt_error(stmt));
				}
			}

			if (pgsql_stmt_execute(stmt) != 0)
			{
				throw exception(__FILE__, __LINE__, pgsql_stmt_error(stmt));
			}
		}
		catch (...)
		{
			throw;
		}

		return new result(stmt);
	}

	st_pgsql_bind& statement::get_bind()
	{
		if (bind_index < 0 || bind_index >= param_count)
		{
			throw exception(__FILE__, __LINE__, "invalid bind_index");
		}

		return binds[bind_index++];
	}

} // namespace pgsqlpp
