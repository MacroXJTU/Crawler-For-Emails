#pragma once



#include "stdio.h"

#include <string.h>
#include <string>
#include <iterator>
#include "sql.h"
#include <cstring>

using namespace std;
// parse
// 对string串进行解析，得到新的链接地址以及页面内的邮箱地址

class parse
{


public:
	parse();
	virtual ~parse();
	string cur_url; //当前页面的网址，这是为了做使用相对URL而使用的
	string cur_folder;//当前页面的目录，在最后处理的时候，需要使用replace将可能存在的 // 替换成/
	long int cur_point; //当前分析到文件的位置了
	long int length;  
	string file_to_parse;

	void set_file(string file)
	{
		try
		{
			file_to_parse=file;
			length=file_to_parse.length();
		}
		catch(CException* e)
		{
			file_to_parse=string("");
			length=0;
		}
	} //更新需要分析的文件

	void set_url(string url)
	{
		try
		{
			cur_url=url;
			get_folder();
		}
		catch(CException* e)
		{
			cur_url=string("");
			cur_folder=string("");
		}
	}
	void get_folder()
	{
		cur_folder=cur_url;
		string::iterator p=cur_folder.end();
		try
		{
			while(*--p!='/')
			{
				cur_folder.erase(p);//删除该元素
			}
		}
		catch(CException* e)
		{
			cur_folder=string("");
		}
	}
	void reset_point(){cur_point=0;}
	int parse_email();//开始对email进行解析	
	void parse_url();//开始对email进行解析	
	void make_clear();//删除空格换行等

};




