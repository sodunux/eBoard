#include "StdAfx.h"
#include <WinSCard.h>
#include "smartcard.h"
using namespace std;
using namespace eBoard;

smartcard::smartcard(void)
{
	
	hContext=NULL;
	ReaderName=gcnew array<String^>(10);	
}

int smartcard::GetReaders()
{
	int returnvalue;
	TCHAR readerbuff[1024];
	DWORD dwlen=1024;
	DWORD i,j;
	SCARDCONTEXT hSC;
	LONG lreturn;
	//String ^ temp;
	try
	{
		ReaderCnt=0;
		lreturn=SCardEstablishContext(SCARD_SCOPE_SYSTEM,NULL,NULL,&hSC);
		if(lreturn!=SCARD_S_SUCCESS) throw lreturn;
		hContext=hSC;

		lreturn=SCardListReaders(hContext,NULL,(LPTSTR)readerbuff,&dwlen);
		if(lreturn!=SCARD_S_SUCCESS) throw lreturn;
		
		for(i=0,j=0;i<(dwlen-1);i++)
		{
			if(readerbuff[i]=='\0')
			{
				j++;
				ReaderName[j]="";				
			}				
			else
				ReaderName[j]+=readerbuff[i];
		}
		ReaderCnt=j;
		returnvalue=0;
	}
	catch(...)
	{
		returnvalue=1;
	}
	return returnvalue;
}

int smartcard::ConnectReader(int ReaderID)
{
	int returnvalue;
	DWORD dwAP;
	DWORD ret,i;
	TCHAR reader[1000];
	
	SCARDHANDLE handletemp;
	try
	{
		for(i=0;i<ReaderName[ReaderID]->Length;i++)
		{
			reader[i]=ReaderName[ReaderID][i];
		}
		reader[i]='\0';
		reader[i+1]='\0';

		ret=SCardConnect(hContext,(LPTSTR)reader,SCARD_SHARE_SHARED,SCARD_PROTOCOL_T0|SCARD_PROTOCOL_T1,&handletemp,&dwAP);
		if(ret!=SCARD_S_SUCCESS)throw ret;

		hCardHandle=handletemp;
		returnvalue=0;

	}
	catch(...)
	{
		returnvalue=1;
	}
	return returnvalue;


}

int smartcard::DisconnectReader()
{
	int returnvalue;
	DWORD ret;
	try
	{
		ret=SCardDisconnect(hCardHandle,SCARD_LEAVE_CARD);
		if(ret!=SCARD_S_SUCCESS)throw ret;
		returnvalue=0;
	}
	catch(...)
	{
		returnvalue=1;
	}
	return returnvalue;
}

int smartcard::ReleaseContext()
{
	int returnvalue;
	DWORD ret;
	try
	{
		ret=SCardReleaseContext(hContext);
		if(ret!=SCARD_S_SUCCESS)throw ret;
		returnvalue=0;
	}
	catch(...)
	{
		returnvalue=1;
	}
	return returnvalue;
}

int smartcard::TransmitReader(String^sendstr,String^ &recstr)
{
	int returnvalue;
	DWORD ret,i;
	String ^temp;
	String ^sendstr_temp;
	Byte sendbuff[1024],recebuff[1024];
	DWORD sendlen=1024,recelen=1024;
	SCARD_IO_REQUEST g_rgSCardT1Pci;
	g_rgSCardT1Pci.dwProtocol=SCARD_PROTOCOL_T1;
	g_rgSCardT1Pci.cbPciLength=sizeof(SCARD_IO_REQUEST);

	try
	{
		sendstr_temp=sendstr->Replace(" ","");
		if(sendstr_temp->Length%2) throw ret;

		for(i=0;i<(sendstr_temp->Length/2);i++)
		{
			sendbuff[i]=Convert::ToByte(sendstr_temp->Substring(i*2,2),16);
		}
		sendlen=sendstr_temp->Length/2;
		

		ret=SCardBeginTransaction(hCardHandle); 
		if(ret!=SCARD_S_SUCCESS)throw ret;
		ret=SCardTransmit(hCardHandle,&g_rgSCardT1Pci,sendbuff,sendlen,NULL,recebuff,&recelen);
		if((ret!=SCARD_S_SUCCESS)||recelen==0)throw ret;
		ret=SCardEndTransaction(hCardHandle,SCARD_LEAVE_CARD);
		if(ret!=SCARD_S_SUCCESS)throw ret;

		recstr="";
		for(i=0;i<recelen;i++)
		{
			if(recebuff[i]<0x10)
				temp="0";
			else 
				temp="";
			temp+=Convert::ToString(recebuff[i],16);	
			recstr+=temp;
		}
		returnvalue=0;

	}
	catch(...)
	{
		returnvalue=1;
	}
	return returnvalue;
}

smartcard::~smartcard()
{
	DisconnectReader();
	ReleaseContext();
}
