
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
		Aws::Client::ClientConfiguration clientConfiguration;
		Aws::DynamoDB::DynamoDBClient dynamoDbClient(clientConfiguration);

		Aws::DynamoDB::Model::PutItemRequest putItemRequest;
		{
			putItemRequest.WithTableName("users");
		}

		Aws::DynamoDB::Model::AttributeValue attrHashKey;
		{
			attrHashKey.SetS("user_id");
			putItemRequest.AddItem("HashKey", attrHashKey);
		}

		Aws::DynamoDB::Model::AttributeValue attrRangeKey;
		{
			attrRangeKey.SetN("0");
			putItemRequest.AddItem("platform_type", attrRangeKey);
		}

		std::cout << 1 << std::endl;
		auto putItemOutcome = dynamoDbClient.PutItem(putItemRequest);
		std::cout << 2 << std::endl;

		if(putItemOutcome.IsSuccess())
		{
			std::cout << "PutItem Sucess" << std::endl;
			//std::cout << "PutItem Success Using IOPS " << putItemOutcome.GetResult().GetConsumedCapacity();
		}
		else
		{
			std::cout << "PutItem failed with error " << putItemOutcome.GetError().GetMessage();
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	Aws::ShutdownAPI(options);
	return 0;
}

