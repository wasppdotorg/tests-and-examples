/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <iostream>
#include <sstream>
#include <exception>

#include <aws/core/Aws.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/utils/Outcome.h>

#include <aws/config/ConfigServiceClient.h>

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemResult.h>
#include <aws/dynamodb/model/PutItemResult.h>
#include <aws/dynamodb/model/ConsumedCapacity.h>

int main()
{
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	try
	{
		Aws::Client::ClientConfiguration client_configuration;
		Aws::DynamoDB::DynamoDBClient dynamodb_client(client_configuration);

		Aws::DynamoDB::Model::PutItemRequest put_item_request;
		{
			put_item_request.WithTableName("users");
		}

		Aws::DynamoDB::Model::AttributeValue hash_key;
		{
			hash_key.SetS("user_id_1");
			put_item_request.AddItem("user_id", hash_key);
		}

		Aws::DynamoDB::Model::AttributeValue range_key;
		{
			range_key.SetN("0");
			put_item_request.AddItem("platform_type", range_key);
		}

		std::cout << 1 << std::endl;
		auto put_item_outcome = dynamodb_client.PutItem(put_item_request);
		std::cout << 2 << std::endl;

		if (put_item_outcome.IsSuccess())
		{
			std::cout << "put_item Sucess" << std::endl;
			//std::cout << "PutItem Success Using IOPS " << putItemOutcome.GetResult().GetConsumedCapacity();
		}
		else
		{
			std::cout << "put_item failed with error " << put_item_outcome.GetError().GetMessage();
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	Aws::ShutdownAPI(options);
	return 0;
}

