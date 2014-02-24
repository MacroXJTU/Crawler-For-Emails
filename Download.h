#pragma once


// Download

#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include <afxinet.h>
#define MAXBLOCKSIZE 102400
#pragma comment( lib, "wininet.lib" )


class Download 
{
public:
	Download();
	virtual ~Download();
	void SetUrl(const char *); //设置网址
	CString Action();
	CString GetSourceHtml();	
	CString theUrl; //最大长度为256字节的网址	
};


