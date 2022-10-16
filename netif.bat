@echo off
set NETIF="Loopback Pseudo-Interface 1"
netsh interface ipv4 add address %NETIF% 192.168.139.11 255.255.255.0 store=active
netsh interface ipv4 add address %NETIF% 192.168.139.12 255.255.255.0 store=active
netsh interface ipv4 add address %NETIF% 192.168.139.13 255.255.255.0 store=active
netsh interface ipv4 add address %NETIF% 192.168.139.14 255.255.255.0 store=active
exit