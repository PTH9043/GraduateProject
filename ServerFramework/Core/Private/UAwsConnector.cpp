#include <aws/ec2/EC2Client.h>
#include <aws/ec2/model/DescribeInstancesRequest.h>
#include <iomanip>
#include "CoreDefines.h"
#include "UAwsConnector.h"

namespace Core
{
	UAwsConnector::UAwsConnector(OBJCON_CONSTRUCTOR) :
		UObject(OBJCON_CONDATA), m_Options{}
	{
		// √ ±‚»≠
		MemoryInitialization(&m_Options, sizeof(Aws::SDKOptions));
	}

	_bool UAwsConnector::Start()
	{
		Aws::InitAPI(m_Options);
		return _bool();
	}

	void UAwsConnector::Free()
	{
		Aws::ShutdownAPI(m_Options);
	}
}