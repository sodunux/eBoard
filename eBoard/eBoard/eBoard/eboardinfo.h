namespace eBoard
{
	using namespace System;
	public ref class eboardinfo
	{
	public:
		String^ eboardinfo_version_string;

		eboardinfo()
		{
			eboardinfo_version_string += "[20180328] 增加Version信息，添加eboardinfo头文件，用于存放一些常量信息,包括版本信息的修改。";
			eboardinfo_version_string += "[20180329] 确定基本界面，增加串口和CCID接口，增加Hex2Str和Str2Hex功能";		
		}

		
		
		
	};


}