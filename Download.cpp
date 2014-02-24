// Download.cpp : 实现文件
//

#include "stdafx.h"
#include "Mymail.h"
#include "Download.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


// Download

Download::Download()
{
	
}

Download::~Download()
{
	GetSourceHtml();
}

void Download::SetUrl(const char * Url)
{
	try
	{
		if(strlen(Url)<6)
		{
			theUrl=CString("");
		}
	}
	catch(CException* e)
	{
		theUrl=CString("");
	}
	theUrl=CString(Url);
}

CString Download::Action()
{
	try
	{
		if(theUrl.GetLength()<6 || theUrl.GetLength()>45 ) 
			return CString("");
	}
	catch(CException* e)
	{
		return CString("");
	}

	return GetSourceHtml();	
}
CString Download::GetSourceHtml()
{
    //创建并且初始化Internet会话
    CInternetSession session;
	//http协议控制对象
	CHttpConnection* pServer=NULL;
	try
	{
		session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5秒的连接超时
		session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1秒的发送超时
		session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 7000);        // 7秒的接收超时
		session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1秒的发送超时
		session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7秒的接收超时
		session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);          // 1次重试		
	}
	catch(CException* e)
	{
		return CString("");
	}

    //http协议下的文件操作对象
    CHttpFile* pFile=NULL;

    //url地址
	CString m_adress;
	try
	{
		m_adress=theUrl; 
	}
	catch(CException* e)
	{
		return CString("");
	}

    CString strServer;
    CString strObject;
    INTERNET_PORT nPort;
    DWORD dwServiceType;

    //通过url地址解析出url对象的端口
	try{
    AfxParseURL(m_adress,dwServiceType,strServer,strObject,nPort);  //对网址进行解析
    pServer=session.GetHttpConnection(strServer,nPort);//主要是用来返回一个CHttpConnection的句柄
    pFile=pServer->OpenRequest(1,strObject,NULL,1,NULL,NULL,INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_AUTO_REDIRECT);
	    	
    pFile->SendRequest();   //把一个请求发送到http服务器ITPUB个人空间
	}
	catch(CException *e)
	{
		return CString("");
	}
    CString line("");
    CString strConnect("");

	try
	{
		while(pFile->ReadString(line))
		{   
			strConnect+=line;			
		}
	}
	catch(CException* e)
	{
		return CString("");
	}
	return strConnect;
}
