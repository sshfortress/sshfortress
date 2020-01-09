# sshfortress

[中文](https://github.com/sshfortress/sshfortress/blob/master/zh/README.md)  [English](https://github.com/sshfortress/sshfortress/blob/master/README.md)

尝试所有SSHfortress功能-免费30天

这个说明是介绍如何安装sshfortress系统，它有一个Web管理界面是用java实现的用来可示化的增加账号、资产等功能；后端是在openssh的基础上开发的功能，好了往下面看如何安装和配置吧。  

目录
================
* [依赖](#依赖)
* [功能](#功能)
* [安装](#安装)
* [配置](#配置)
* [使用](#使用)

## 依赖

1. mysql 5.6+ 
2. jdk 1.8  
3. tomcat 8 

## 功能

## 安装
进入资源页按需下载即可，https://sshfortress.com/downloads/ 如果官网网络慢可到 https://github.com/sshfortress/sshfortress/releases进行下载
apache-tomcat-8.5.31.tar.gz                        09-Jan-2020 11:21      9M
audit_sec.sql                                      09-Jan-2020 16:07     10M
greatfortress.tar.gz                               09-Jan-2020 13:33     15M
greatfortressframework.war                         09-Jan-2020 11:21     50M
jdk-8u172-linux-x64.tar.gz                         09-Jan-2020 11:22    182M
sshfortress-1.6.1.sha512                           09-Jan-2020 16:14     155
sshfortress-1.6.1.tar.gz                           09-Jan-2020 15:58     10M

```
# tar zxvf jdk-8u172-linux-x64.tar.gz -C /usr/local/
# ln -s /usr/local/jdk1.8.0_172/ /usr/local/jdk
# echo "export JAVA_HOME=/usr/local/jdk" >> /etc/profile
# echo "export JRE_HOME=/usr/local/jdk/jre" >> /etc/profile
# echo "export CLASSPATH=.:\${JAVA_HOME}/lib:\${JRE_HOME}/lib:\${CLASSPATH}" >> /etc/profile
# echo "export PATH=\${JAVA_HOME}/bin:\${JRE_HOME}/bin:/usr/local/mysql/bin:\${PATH}" >> /etc/profile
# source /etc/profile

## 配置




## 使用

