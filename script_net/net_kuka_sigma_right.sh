#!/bin/sh

# Laurent LEQUIEVRE
# laurent.lequievre@univ-bpclermont.fr
# UMR 6602 - Institut Pascal

# NETWORK INFORMATIONS
# ====================
# sudo lshw -C network
# eth0 -> pci=0000:04:00.0, hw=68:05:ca:3e:3d:35, card=82574L Gigabit Network Connection, driver=e1000e
# eth1 -> pci=0000:06:00.0, hw=f0:4d:a2:32:39:d3, card=NetXtreme BCM5761 Gigabit Ethernet PCIe, driver=tg3
# eth2 -> pci=0000:22:00.0, hw=00:1b:21:b3:ae:27, card=82574L Gigabit Network Connection, driver=e1000e
# eth3 -> pci=0000:23:00.0, hw=68:05:ca:3e:3b:52, card=82574L Gigabit Network Connection, driver=e1000e
# eth4 -> pci=0000:24:00.0, hw=68:05:ca:3e:3d:36, card=82574L Gigabit Network Connection, driver=e1000e


# eth2 -> kuka left
# eth0 -> kuka right
# eth3 -> switch
ethX_associate_kuka_left="eth2"
ethX_associate_kuka_right="eth0"
ethX_associate_switch="eth3"

# PCI address of each ethX associated
pci_address_associate_kuka_left="0000:22:00.0"
pci_address_associate_kuka_right="0000:04:00.0"

# Define IP address for RT eth0 and eth1
ip_address_associate_kuka_left="192.168.100.102"
ip_address_associate_kuka_right="192.168.100.120"
ip_address_associate_switch="192.168.100.123"
ip_address_associate_sub_net_switch="192.168.100.0/24"

# Define Netmask for RT eth0 and eth1
netmask_associate_kuka_left="255.255.255.0"
netmask_associate_kuka_right="255.255.255.0"
netmask_associate_switch="255.255.255.0"

# Define hw of each ethX associated
hw_ethX_associate_kuka_left="00:1b:21:b3:ae:27"
hw_ethX_associate_kuka_right="68:05:ca:3e:3d:35"
hw_ethX_associate_switch="68:05:ca:3e:3b:52"

# Define IP address of kuka arms
ip_address_kuka_right="192.168.100.253"
ip_address_kuka_left="192.168.100.254"

# Define IP address for local loopback
ip_address_loopback="127.0.0.1"

do_start()
{
	echo "Start net script (RT and non RT) of kuka right arm and also switch !"
	
	echo "ifconfig down (non RT) ethX associate to kuka right !"
	sudo ifconfig $ethX_associate_kuka_right down
	
	echo "ifconfig down (non RT) ethX associated to switch !"
	sudo ifconfig $ethX_associate_switch down
	
	
	echo "rtifconfig up rteth1 for kuka right !"
	sudo ifconfig $ethX_associate_kuka_right up $ip_address_associate_kuka_right netmask $netmask_associate_kuka_right hw ether $hw_ethX_associate_kuka_right
	
	echo "Add (RT) route to kuka right arm"
	sleep 5
	sudo route add -host $ip_address_kuka_right dev $ethX_associate_kuka_right

	sudo iptables -A INPUT -p udp -i eth0 --dport 49938 -j ACCEPT
	sudo iptables -A OUTPUT -p udp --dport 49938 -j ACCEPT

	sudo iptables -A INPUT -p tcp -i eth0 --dport 49938 -j ACCEPT
	sudo iptables -A OUTPUT -p tcp --dport 49938 -j ACCEPT
	
	#echo "Start ethX (non RT) associate to switch !"
	#sudo ifconfig $ethX_associate_switch up $ip_address_associate_switch netmask $netmask_associate_switch hw ether $hw_ethX_associate_switch $ip_address_associate_switch
      
    	#echo "Delete all routes of ethX (non RT) associate to switch !"
	#sleep 5
	#sudo route del -net $ip_address_associate_sub_net_switch dev $ethX_associate_switch
        
	#echo "Add sub network (non RT) for switch !"
	#sleep 5
	#sudo route add -net $ip_address_associate_sub_net_switch dev $ethX_associate_switch
}

do_stop()
{
	echo "Stop net script (RT) !"
	
	sleep 1    
    	echo "Stop ethX (RT) associate to kuka right !"
    	sudo ifconfig $ethX_associate_kuka_right down
    
    	echo "ifconfig up ethX associate to kuka right arm !"
  
    	sudo ifconfig $ethX_associate_kuka_right up $ip_address_associate_kuka_right netmask $netmask_associate_kuka_right hw ether $hw_ethX_associate_kuka_right
	
    	echo "Add route (non RT) to kuka right arm !"
    	sleep 5
    	sudo route add -host $ip_address_kuka_right dev $ethX_associate_kuka_right
}

case "$1" in
   start)
      do_start
      ;;
   stop)
      do_stop
      ;;
   *)
      echo "--> Usage: $0 {start|stop}"
      exit 1
esac

exit 0
