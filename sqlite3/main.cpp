/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <cstring>

#include "sqlite3.h"

#include <iostream>

int main()
{
	sqlite3* sqlite3_ = nullptr;
	sqlite3_stmt* stmt_ = nullptr;

	int result = sqlite3_open("sqlite3.db", &sqlite3_);
	if (result != SQLITE_OK)
	{
		std::cout << "failed to open database file : " << sqlite3_errcode(sqlite3_) << " : " << sqlite3_errmsg(sqlite3_) << std::endl;
		return 1;
	}

	std::string query = "drop table test";
	result = sqlite3_prepare(sqlite3_, query.c_str(), query.size(), &stmt_, 0);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt_);
	}
	sqlite3_finalize(stmt_);

	query = "create table test(test1 INT, test2 BIGINT, test3 DOUBLE, test4 VARCHAR(10), test5 VARCHAR(50), test6 VARCHAR(255), test7 TEXT, test8 DATETIME)";
	result = sqlite3_prepare(sqlite3_, query.c_str(), query.size(), &stmt_, 0);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt_);
	}
	sqlite3_finalize(stmt_);

	query = "insert into test values(?, ?, ?, ?, ?, ?, ?, ?)";
	result = sqlite3_prepare(sqlite3_, query.c_str(), query.size(), &stmt_, 0);
	if (result == SQLITE_OK)
	{
		int bind_index = 0;
		{
			sqlite3_bind_int(stmt_, ++bind_index, 1);
			sqlite3_bind_int64(stmt_, ++bind_index, 2);
			sqlite3_bind_double(stmt_, ++bind_index, 3.1f);
			sqlite3_bind_text(stmt_, ++bind_index, "4", strlen("4"), 0);
			sqlite3_bind_text(stmt_, ++bind_index, "5", strlen("5"), 0);
			sqlite3_bind_text(stmt_, ++bind_index, "6", strlen("6"), 0);
			sqlite3_bind_text(stmt_, ++bind_index, "7", strlen("7"), 0);
			sqlite3_bind_text(stmt_, ++bind_index, "1970-01-01 00:00:00", strlen("1970-01-01 00:00:00"), 0);
		}

		sqlite3_step(stmt_);
	}
	sqlite3_finalize(stmt_);

	query = "select * from test";
	result = sqlite3_prepare(sqlite3_, query.c_str(), query.size(), &stmt_, 0);
	if (result == SQLITE_OK)
	{
		int column_count = sqlite3_column_count(stmt_);

		while (sqlite3_step(stmt_) == SQLITE_ROW)
		{
			for (int i = 0; i < column_count; ++i)
			{
				std::cout << sqlite3_column_name(stmt_, i) << " = ";

				if (sqlite3_column_type(stmt_, i) == SQLITE_INTEGER)
				{
					std::cout << sqlite3_column_int64(stmt_, i) << std::endl;
				}
				else if (sqlite3_column_type(stmt_, i) == SQLITE_FLOAT)
				{
					std::cout << sqlite3_column_double(stmt_, i) << std::endl;
				}
				else if (sqlite3_column_type(stmt_, i) == SQLITE3_TEXT)
				{
					std::cout << sqlite3_column_text(stmt_, i) << std::endl;
				}
			}
		}
	}
	sqlite3_finalize(stmt_);

	sqlite3_close(sqlite3_);

	std::cin.get();
	return 0;
}

