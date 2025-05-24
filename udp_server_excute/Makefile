.PHONY:all
all:udp_server udp_client 

udp_server:main.cpp
	g++ -o $@ $^ -std=c++14
udp_client:UdpClient.cpp
	g++ -o $@ $^ -std=c++14
.PHONY:clean
clean:
	rm -fr udp_server log udp_client
