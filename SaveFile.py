#!/usr/bin/env python
# -*- coding: utf-8 -*-


import MySQLdb
conn =   MySQLdb.Connection('127.0.0.1', 'root', '123456', 'mail')
cursor = conn.cursor()
# 執行 SQL 語句
cursor.execute("SELECT * FROM mail")
result = cursor.fetchall()
output = open('mail.txt', 'w+')

import string
if __name__ == '__main__':
    # 輸出結果
    
    for record in result:        
        output.write(string.replace(record[1][7:]+'\n','www.','',1))
