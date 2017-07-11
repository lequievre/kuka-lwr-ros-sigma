FRI :

====

-> ATTENTION : j'ai du modifie des limits systemes comme indique par les italiens

1- sudo nano /etc/security/limits.conf and add these lines:

- "soft" for enforcing the soft limits

- "hard" for enforcing hard limits

( 	- rtprio - max realtime priority

	- memlock - max locked-in-memory address space (KB)

)

YOUR_USERNAME hard rtprio 95

YOUR_USERNAME soft rtprio 95

YOUR_USERNAME hard memlock unlimited

YOUR_USERNAME soft memlock unlimited

2- sudo nano /etc/pam.d/common-session and add session required pam_limits.so

Reboot, open a terminal, and check that ulimit -r -l gives you the values set above.

-> Pour connaitre la liste des programmes utilisant un port UDP

netstat -ap -u

pour un port particulier faire : netstat -ap -u | grep 49938



sudo iptables -A INPUT -p udp -i eth0 --dport 49938 -j ACCEPT
sudo iptables -A OUTPUT -p udp --dport 49938 -j ACCEPT


emergency stop :

===============

rostopic pub -r 10 /kuka_lwr/emergency_stop std_msgs/Bool 'data: true'



How do I fix "fatal error: 'sys/cdefs.h' file not found # include <sys/cdefs.h>"?

This is probably caused by trying to build a 32 bit binary on a 64 bit system (with -m32 for instance). You need to install the 32 bit versions of the common libraries. Use:

    sudo apt-get install gcc-multilib g++-multilib



