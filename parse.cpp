// parse.cpp : 实现文件
//

#include "stdafx.h"
#include "Mymail.h"
#include "parse.h"
#include "boost\xpressive\xpressive.hpp"

// parse

parse::parse()
{
	cur_url="";
	cur_folder="";
	cur_point=0;
	file_to_parse="";
	length=0;
}

parse::~parse()
{

}
int parse::parse_email()//开始对email进行解析
{	
	if(length<=100)		return 0;//没有文件
	//一个一个找email地址吧,fuck
	using namespace boost::xpressive;
	string str_all(file_to_parse); //对文件进行分析
	
	cregex reg=cregex::compile("([0-9a-zA-Z_.]+@[0-9a-zA-Z.]+)",icase);
	cmatch what;
	
	int count=0;
	int size=2;
	long int pos=0;
	usesql sql;
	while(size!=0)
	{	
		string str(str_all.begin()+pos,str_all.end());
		regex_search(str.data(),what,reg); //查找匹配的结果
		size=what.size();//找到返回非零的结果
		if(size!=0)
		{
			//保存结果
			string h(what[0]);
			//using namespace boost::xpressive;
			//去掉一些不靠谱的东西，也就是 @ 的一些常见用法
			cregex reg=cregex::compile(".com|.cn|.net|.cc",icase);
			cmatch what;	
			regex_search(h.data(),what,reg); //查找匹配的结果
			int size=what.size();//找到返回非零的结果
			if(size==0)
			{
				pos=pos+str.find(h.data(),0)+h.length();
				if(pos<0) return count;	
				continue;
			}
			sql.insert_a_mail(h);
			//更新字符串							
			pos=pos+str.find(h.data(),0)+h.length();
			if(pos<0) return count;			
		}			
	}	
}

void parse::parse_url()//开始对url进行解析
{
	if(length<=100)	return ;//没有文件
	string str_all(file_to_parse); //获得全部文本的一个拷贝
	
	//FILE* to=fopen("1.txt","wb");
	long int next_a=2;  //a标签起始
	long int next_a_end=2; //a标签结束
	long int pos=0;
	usesql sql;
	
	while(next_a>=0)
	{	
		string str(file_to_parse.begin()+pos,file_to_parse.end()); //需要考虑的临时文本
		
		next_a=str.find("<a ",0);
		if(next_a<0) return ;//找不到a标签了
		next_a_end=str.find(">",next_a); //从开始位置，立刻找a标签的结束位置
		if(next_a_end<0) return ;//找不到结束位置
		string a_content(str.begin()+next_a,str.begin()+next_a_end+1);
		//a_content 的内容大概如下，现在要提出href的值
		//<a href='http://qzlx.xjtu.edu.cn/' title='群众路线教育' target="_blank">
		int first=a_content.find("href=",0);//查找href的位置 		
		int end=a_content.find(" ",first);//查找href之后的首个空格
		if(first<0 || end<0)
		{
			//出现异常的情况
			//更新位置，直接进行下一种情况的处理
			pos=pos+next_a_end+2;
			if(file_to_parse.begin()+pos > file_to_parse.end()) return;
			if(pos<0) return;
			continue;		
		}
		
		a_content=string(a_content.begin()+first+5,a_content.begin()+end);
		//如果存在.doc .rar .jpg 之类的pass掉
		using namespace boost::xpressive;
		cregex reg=cregex::compile(".lofter|.share|.google.|.baidu.|search|so|javascript|.doc|.docx|.ppt|.pptx|.rar|.z|.jpg|.zip|.tar|.gz|.pdf|.css|.js",icase);
		cmatch what;	
		regex_search(a_content.data(),what,reg); //查找匹配的结果
		int size=what.size();//找到返回非零的结果
		if(size!=0 || a_content.length()<6 || a_content.length()>40)
		{
			pos=pos+next_a_end+2;
			if(file_to_parse.begin()+pos > file_to_parse.end()) return;
			if(pos<0) return;
			continue;
		}
		//a_content的内容大致如下
		//'http://news.xjtu.edu.cn/info/1033/39172.htm'
		//现在，需要删掉开始的'或者"
		if(a_content.data()[0]=='\'' || a_content.data()[0]=='\"')
		{
			a_content.erase(a_content.begin());
		}
		//现在，删掉结尾的'或者"
		if(a_content.at(a_content.length()-1)=='\'' || a_content.at(a_content.length()-1)=='\"')
		{
			a_content.erase(a_content.end()-1);
		}
		//ok,形式上处理完毕,接着检验是相对地址还是绝对地址
		if(strcmp("http",(const char*)(string(a_content.begin(),a_content.begin()+4)).data()))		
		//if( a_content.find("http:",0)<0 && a_content.find("https:",0)<0 )
		{
			//这是用的相对地址
			//MessageBox(0,L"",L"",0);
			if(cur_folder.at(cur_folder.length()-1)=='/' && a_content.at(0)=='/') //存在重复的东西
			{
				a_content.erase(a_content.begin());			
			}
			a_content=cur_folder+a_content;//拼凑出完整的地址
		}
		//string h(a_content);
		//fwrite((h+"\r\n").data(),h.length()+2,1,to);
		//ok,现在已经处理的差不多了，再看看这个地址是不是我们需要的
		
		
		//string h(a_content);
		//fwrite((h+"\r\n").data(),h.length()+2,1,to);
		//将URL插入到数据库中
		sql.insert_a_url(a_content);

		//更新字符串
		pos=pos+next_a_end+2;
		if(file_to_parse.begin()+pos > file_to_parse.end()) return;
		if(pos<0) return;				
					
	}	


}

//本函数不允许被调用
void parse::make_clear()
{
	return;
	
	if(length==0)		return ;//没有文件
	//去除nbsp
	CString s(file_to_parse.data());
	s.Replace(L"&nbsp;",L"");
	s.Replace(L"&nbsp",L"");
	file_to_parse=string((char*)s.GetString());
	string::iterator p=file_to_parse.begin();
	//去除空格
	while(p!=file_to_parse.end()) //去除无效字符
	{
		if(*p == '\n' || *p == '\t' || *p == ' ') file_to_parse.erase(p);	
		p++;
	}
}






