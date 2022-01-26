
#sudo ip link set br0 down
#sudo brctl delbr br0
# sudo ip link delete veth1
# sudo route del -net 192.168.0.0 netmask 255.255.255.0
# sudo iptables -t nat -D POSTROUTING 1

sudo brctl addbr br0 # 添加 br0 网卡
sudo ip addr add 192.168.0.1/24 dev br0  # 设置 br0 网卡IP地址 192.168.0.1
sudo ip link set dev br0 up # 开启网卡

sudo ip link add veth1 type veth peer name veth2

sudo ip addr add 192.168.0.2/24 dev veth1
sudo ip addr add 192.168.0.3/24 dev veth2

sudo ip link set veth1 up
sudo ip link set veth2 up


# 创建一个名为 net1 的 net namespace
sudo ip netns add net1
# 新建 vetch 对 veth1-veth2
sudo ip link add veth1 type veth peer name veth2
# 把其中的一头 veth1 放到这个新的名为 net1 的 net namespace。
sudo ip link set veth1 netns net1

# 给 veth1 配置 ip 并启用
sudo ip netns exec net1 ip addr add 192.168.0.101/24 dev veth1
sudo ip netns exec net1 ip link set veth1 up

#查看 veth1 是否已经正常
sudo ip netns exec net1 ifconfig


sudo ip netns add net2
# 新建 vetch 对 veth1-veth2
sudo ip link add veth3 type veth peer name veth4
# 把其中的一头 veth1 放到这个新的名为 net1 的 net namespace。
sudo ip link set veth3 netns net2

# 给 veth3 配置 ip 并启用
sudo ip netns exec net2 ip addr add 192.168.0.102/24 dev veth3
sudo ip netns exec net2 ip link set veth3 up

# 把两个网络连接到一起
sudo brctl addbr br0
sudo ip link set dev veth2 master br0
sudo ip link set dev veth4 master br0
sudo ip addr add 192.168.0.1/24 dev br0
sudo ip link set veth2 up
sudo ip link set veth4 up
sudo ip link set br0 up

sudo ip netns exec net1 ping 192.168.0.102 -I veth1