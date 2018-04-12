#pragma once
#include "StdAfx.h"
#include "WinSCard.h"
namespace eBoard
{
	using namespace System;
	ref class smartcard
	{
	public:
		smartcard(void);
		int GetReaders();
		int ConnectReader(int ReaderID);
		int DisconnectReader(); 
		int TransmitReader(String^sendstr,String^ &recstr);
		int ReleaseContext(); 
		~smartcard();
		SCARDCONTEXT hContext;
		array<String^>^ReaderName;
		Byte ReaderCnt;
		SCARDHANDLE hCardHandle;
	};
	

}

