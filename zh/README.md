# sshfortress

[中文](https://github.com/sshfortress/sshfortress/blob/master/zh/README.md)  [English](https://github.com/sshfortress/sshfortress/blob/master/README.md)

尝试所有SSHfortress功能-免费30天

这个说明是介绍如何安装sshfortress系统，它有一个Web管理界面greatfortress是用java实现的用来可示化的增加账号、资产等功能；后端sshfortress是在openssh的基础上开发的功能，好了往下面看如何安装和配置吧。  

目录
================
* [依赖](#依赖)
* [功能](#功能)
* [安装](#安装)
* [配置](#配置)
* [使用](#使用)

## 依赖

1. mysql 5.7+  
2. jdk 1.8  
3. tomcat 8 

## 功能

## 安装
进入资源页按需下载即可，https://sshfortress.com/downloads/  或者  https://github.com/sshfortress/sshfortress/releases  进行下载
```
apache-tomcat-8.5.31.tar.gz                        09-Jan-2020 11:21      9M
audit_sec.sql                                      09-Jan-2020 16:07     10M
greatfortress.tar.gz                               10-Jan-2020 09:52     65M
jdk-8u172-linux-x64.tar.gz                         09-Jan-2020 11:22    182M
sshfortress-1.6.1.sha512                           09-Jan-2020 16:14     155
sshfortress-1.6.1.tar.gz                           09-Jan-2020 15:58     10M
```
### jdk

```
# tar zxvf jdk-8u172-linux-x64.tar.gz -C /usr/local/
# ln -s /usr/local/jdk1.8.0_172/ /usr/local/jdk
# echo "export JAVA_HOME=/usr/local/jdk" >> /etc/profile
# echo "export JRE_HOME=/usr/local/jdk/jre" >> /etc/profile
# echo "export CLASSPATH=.:\${JAVA_HOME}/lib:\${JRE_HOME}/lib:\${CLASSPATH}" >> /etc/profile
# echo "export PATH=\${JAVA_HOME}/bin:\${JRE_HOME}/bin:/usr/local/mysql/bin:\${PATH}" >> /etc/profile
# source /etc/profile
```

### mysql 
```
# yum -y install make gcc-c++ cmake bison-devel bison  ncurses-devel libgnomeui-devel perl-Module-Install  bzip2 libtirpc-devel

# wget https://github.com/sshfortress/sshfortress/releases/download/1.6.1/mysql-5.7.16.tar.gz
# wget https://github.com/sshfortress/sshfortress/releases/download/1.6.1/boost_1_59_0.tar.bz2
# tar jxvf boost_1_59_0.tar.bz2 -C /tmp
# tar zxvf mysql-5.7.16.tar.gz
# groupadd mysql；useradd -r -g mysql -s /bin/false -M mysql
# cd mysql-5.7.16/

# cmake . -DCMAKE_INSTALL_PREFIX=/usr/local/mysql \
    -DINSTALL_DATADIR=/usr/local/mysql/data \
    -DSYSCONFDIR=/usr/local/mysql/etc \
    -DDEFAULT_CHARSET=utf8mb4 \
    -DDEFAULT_COLLATION=utf8mb4_general_ci \
    -DEXTRA_CHARSETS=all \
    -USER=mysql \
    -DWITH_MYISAM_STORAGE_ENGINE=1 \
    -DWITH_INNOBASE_STORAGE_ENGINE=1 \
    -DWITH_ARCHIVE_STORAGE_ENGINE=1  \
    -DWITH_BLACKHOLE_STORAGE_ENGINE=1 \
    -DWITH_MEMORY_STORAGE_ENGINE=1 \
    -DWITH_BOOST=/tmp/boost_1_59_0
    
# make -j4; make install
# chown -R mysql.mysql /usr/local/mysql
# cp -f support-files/mysql.server /etc/init.d/mysqld
# mysqld --initialize --user=mysql  --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data
# /etc/init.d/mysqld start
# 初始密码，初始化的时候可以看到 <temporary password 'NiBMcstse9!i'>
# mysql -uroot -p'NiBMcstse9!i' --connect-expired-password -e "alter user 'root'@'localhost' identified by 'SSHfortress123@';CREATE USER 'sshfortress'@'%' IDENTIFIED BY 'SSHfortress123@';GRANT ALL PRIVILEGES ON *.* TO 'sshfortress'@'%' IDENTIFIED BY 'SSHfortress123@' WITH GRANT OPTION MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0 MAX_USER_CONNECTIONS 0;flush privileges;"

# mysql -uroot -p'SSHfortress123@' -e "create database audit_sec; use mysql;create user 'audit'@'127.0.0.1' identified by 'audit'; grant all privileges on audit_sec.* to 'audit'@'127.0.0.1';"

######## 5.7需要设置log_bin_trust_function_creators、explicit_defaults_for_timestamp和sql_mode  
######## 8.0只需设置log_bin_trust_function_creators=1
# mysql -uroot -p'SSHfortress123@' -e "set global log_bin_trust_function_creators=1; set global explicit_defaults_for_timestamp=1;set global sql_mode='STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION';"

# mysql -uroot -p'SSHfortress123@' audit_sec  <audit_sec.sql

为了防止每次重启失效，所以把它写入到配置文件中如下：
#[root@centos8 local]# cat /etc/my.cnf 
[mysqld]
log_bin_trust_function_creators=on
explicit_defaults_for_timestamp=on
sql_mode=NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES
# /etc/init.d/mysqld restart 


```

### tomcat

```
# tar zxvf apache-tomcat-8.5.31.tar.gz -C /usr/local/
# ln -s /usr/local/apache-tomcat-8.5.31/ /usr/local/tomcat
# rm -rf /usr/local/tomcat/webapps/*
# tar zxvf greatfortress.tar.gz -C /usr/local/tomcat/webapps
# sed -i 's/192.168.7.3/x.x.x.x/g' /usr/local/tomcat/webapps/greatfortress/common/layui/layui.js
# /usr/local/tomcat/bin/catalina.sh run

x.x.x.x 是你的监听地址
http://x.x.x.x:8080/greatfortress  
帐号：fortress  密码 hilookhere
启动tomcat后，首次登录有点慢
```
## 配置




## 使用

