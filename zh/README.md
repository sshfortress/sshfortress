# sshfortress

[中文](https://github.com/sshfortress/sshfortress/blob/master/zh/README.md)  [English](https://github.com/sshfortress/sshfortress/blob/master/README.md)

尝试所有SSHfortress功能-免费30天

这个说明是介绍如何安装sshfortress系统，它有一个Web管理界面是用java实现的用来可示化的增加账号、资产等功能；后端是在openssh的基础上开发的功能，好了往下面看如何安装和配置吧。  

content
================
* [依赖](#依赖)
* [功能](#功能)
* [安装](#安装)
* [配置](#配置)
* 使用](#使用)

## 依赖

1. mysql 5.6+ 
2. jdk 1.8  <web>
3. tomcat 8 <web>

## 功能

RedisPushIptables is used to update firewall rules to reject the IP addresses for a specified amount of time or forever reject. however fail2ban relies on regular expressions. Once the application's log format has changed (the reason for the change may be due to version iteration), the filter needs to be reconfigured. RedisPushIptables does not have these concerns,it's used in the form of an API.

## 配置

In order to test the module you are developing, you can load the module using the following redis.conf configuration directive:

```
loadmodule /etc/redis/modules/iptablespush.so
```

It is also possible to load a module at runtime using the following command:

```
MODULE LOAD /etc/redis/modules/iptablespush.so
```

In order to list all loaded modules, use:

```
MODULE LIST
```
Running ttl_iptables daemon with root user

```
root@debian:~/RedisPushIptables# /etc/init.d/ttl_iptables start
``` 

Logs are viewed in /var/log/ttl_iptables.log
```
root@debian:~# redis-cli TTL_DROP_INSERT 192.168.18.5 60  
(integer) 12
root@debian:~# date
Fri Mar 15 09:38:49 CST 2019    
root@debian:~# iptables -L -n
Chain INPUT (policy ACCEPT)
target     prot opt source               destination         
DROP       all  --  192.168.18.5        0.0.0.0/0 
root@debian:~/RedisPushIptables# tail -f /var/log/ttl_iptables.log 
pid=5670 03/15-09:39:48 iptables -D INPUT -s 192.168.18.5 -j DROP
root@debian:~# iptables -L INPUT -n
Chain INPUT (policy ACCEPT)
target     prot opt source               destination
```

## 安装
#### Installing Packages on Linux

```
  #1: Compile hiredis
    cd redis-4.0**version**/deps/hiredis
    make 
    make install 
    echo /usr/local/lib >> /etc/ld.so.conf
    ldconfig 
    
  #2: git clone  https://github.com/limithit/RedisPushIptables.git
    cd RedisPushIptables
    make                      #OR make CFLAGS=-DWITH_IPSET    #OR make CFLAGS=-DWITH_NFTABLES
    make install
   ```
* If you need to enable ipset, you must configure the following settings
```
#ipset create block_ip hash:ip timeout 60 hashsize 4096 maxelem 10000000
#iptables -I INPUT -m set --match-set block_ip src -j DROP
#ipset create allow_ip hash:ip hashsize 4096 maxelem 10000000
#iptables -I INPUT -m set --match-set allow_ip src -j ACCEPT
```
The `timeout` parameter and  `ttl_drop_insert` parameter has the same effect. If the `timeout` parameter is configured, ipset is used to implement periodic deletion. If the `timeout` parameter is not configured, it is periodic deletion used `ttl_drop_insert`.

First edit the /etc/pf.conf file and add the code as follows:
```
table <block_ip> persist file "/etc/pf.block_ip.conf"
table <allow_ip> persist file "/etc/pf.allow_ip.conf"
block in log proto {tcp,udp,sctp,icmp} from <block_ip> to any
pass in proto {tcp,udp,sctp,icmp} from <allow_ip> to any
```
then 
```
touch /etc/pf.block_ip.conf
touch /etc/pf.allow_ip.conf
pfctl -F all -f /etc/pf.conf 
pfctl -e
```
BSD system does not provide a startup script

## HOWTOs
In theory, except for the C language native support API call, the corresponding library before the other language API calls must be re-encapsulated because the third-party modules are not supported by other languages. Only C, Python, Bash, Lua are shown here, and the principles of other languages are the same.
