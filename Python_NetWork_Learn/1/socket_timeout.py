#Based on python 2.7
#2015-1-26
#Sliencer

import socket

def test_socket_timeout():
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	print "Defualt socket timeout: %s" %s.gettimeout()
	s.settimeout(100)
	print "Current socket timeout: %s" %s.gettimeout()

if __name__ == '__main__':
	test_socket_timeout()