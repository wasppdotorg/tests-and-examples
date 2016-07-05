/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <cstdint>

#include <iostream>
#include <map>

#include <msgpack.hpp>

class master_info
{
public:
	virtual ~master_info() {}

private:

};

class info
	: public master_info
{
public:
	MSGPACK_DEFINE(id, str_vec);
	
private:
	int id;
	std::vector<std::string> str_vec;

};

class master_table
{
public:
	virtual ~master_table() {}

private:

};

class table
	: public master_table
{
public:
	MSGPACK_DEFINE(map_);
	
private:
	std::map<int, info> map_;
	
};

int main()
{
	table table_1;
	
	// ---- serialize it into buffer ----
	msgpack::sbuffer sbuffer_;
	msgpack::pack(sbuffer_, table_1);

	std::cout << sbuffer_.data() << std::endl;

	// ---- deserialize it ----
	msgpack::unpacked unpacked_;
	msgpack::unpack(&unpacked_, sbuffer_.data(), sbuffer_.size());

	msgpack::object object_ = unpacked_.get();

	// ---- convert msgpack objects into statically typed object ----
	table table_2;
	object_.convert(&table_2);

	return 0;
}
