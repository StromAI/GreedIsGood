#Based on python 2.7
#2015-1-26
#Sliencer

import socket

def convert_integer():
	data = 1234
	#32-bit
	print "Orginal: %s => Long host byte order: %s, Network byte order: %s"\
		% (data, socket.ntohl(data), socket.htonl(data))
	#16-bit
	print "Orginal: %s => Short host byte order: %s, Network byte order: %s"\
		% (data, socket.ntohs(data), socket.htons(data))

if __name__ == '__main__':
	convert_integer()