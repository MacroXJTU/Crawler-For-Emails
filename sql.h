#pragma once

#include "stdio.h"

#include <string.h>
#include <string>
#include "mysql.h"
#include <cstring>
using namespace std;

#pragma comment( lib, "WS2_32.LIB")
#pragma comment( lib, "libmysql.lib")


// sql

class usesql 
{

public:
	usesql(){ConnMySQL("127.0.0.1","3306","mail","root","123456","UTF-8",szMsg);};
	~usesql(){CloseMySQLConn();};

	MYSQL mysql;
	char szMsg[256];
	int ConnMySQL(char *host,char * port,char * Db,char * user,char* passwd,char * charset,char * Msg)
	{
	  if( mysql_init(&mysql) == NULL )
       {
              Msg = "inital mysql handle error";
              return 1;
       }    
 
       if (mysql_real_connect(&mysql,host,user,passwd,Db,0,NULL,0) == NULL)
       {
              Msg = "Failed to connect to database: Error";
              return 1;
       }    
 
       if(mysql_set_character_set(&mysql,"UTF-8") != 0)
       {
              Msg = "mysql_set_character_set Error";
              return 1;
       }
       return 0;
	}
	string SelectData(char * SQL,int Cnum ,char * Msg)
	{
		MYSQL_ROW m_row;
		MYSQL_RES *m_res;
		char sql[2048];
		sprintf(sql,SQL);
        int rnum = 0;
        char rg = 0x06;//行隔开
        char cg = {0x05};//字段隔开
 
       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "select ps_info Error";
              return "";
       }
       m_res = mysql_store_result(&mysql);
 
       if(m_res==NULL)
       {
              Msg = "select username Error";
              return "";
       }
       string str("");
       while(m_row = mysql_fetch_row(m_res))
       {
              for(int i = 0;i < Cnum;i++)
              {
                     str += m_row[i];
                     //str += rg;
              }
              //str += rg;             
              rnum++;
       }
 
       mysql_free_result(m_res); 
       return str;

	}
	int InsertData(char * SQL,char * Msg)
	{
		char sql[2048];
		sprintf(sql,SQL);
       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "Insert Data Error";
              return 1;
       }
       return 0;
	}
	int UpdateData(char * SQL,char * Msg)
	{
		char sql[2048];
		sprintf(sql,SQL);
       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "Update Data Error";
              return 1;
       }
       return 0;

	}
	int DeleteData(char * SQL,char * Msg)
	{
		char sql[2048];
		sprintf(sql,SQL);
       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "Delete Data error";
              return 1;
       }
       return 0;
	}
	void CloseMySQLConn()
	{
		 mysql_close(&mysql);
	}

	bool insert_a_mail(string mail)
	{

		string h=SelectData((char*)(string("select address from mail where address='")+mail+string("';")).data(),1,szMsg);
		if (h!="") return false;
		string query("INSERT INTO  `mail`.`mail` (`address` ,`time` ,`used`)VALUES ( '"+mail+"',  '0',  '0');");
		return InsertData((char*)query.data(),szMsg);
	}
	bool insert_a_url(string url)
	{
		string h=SelectData((char*)(string("select url from url where url='")+url+string("';")).data(),1,szMsg);
		if (h!="")
		{			
			return false;
		}
		string query("INSERT INTO  `mail`.`url` (`url` ,`scrached` ,`used`)VALUES ( '"+url+"',  '0',  '0');");
		return InsertData((char*)query.data(),szMsg);
	}
	bool update_a_url(string url)
	{
		if(url.find(".com",0)<0 && url.find(".cn",0)<0 && url.find(".net",0)<0) return false;//这样的地址需要忽略掉
		string h=SelectData((char*)(string("select url from url where url='")+url+string("';")).data(),1,szMsg);
		if (h=="")
		{			
			return false;
		}
		string query("update url set scrached=1 where url='"+url+"';");
		return InsertData((char*)query.data(),szMsg);
	}
	string get_next_url()
	{
		return SelectData("select `url` from `mail`.`url` where scrached =0 order by id asc limit 0,1;",1,szMsg);
	}
	string get_total_mail()
	{
		return SelectData("select count(*) from mail where 1;",1,szMsg);
	}
	string get_total_url()
	{
		return SelectData("select count(*) from url where 1;",1,szMsg);
	}
	string get_unscrached_url()
	{
		return SelectData("select count(*) from url where scrached=0;",1,szMsg);
	}

};


