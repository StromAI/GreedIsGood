#Based on python 2.7
#2015-1-29
#Sliencer

import ntplib
from time import ctime

def print_time():
	ntp_client = ntplib.NTPClient()
	response = ntp_client.request('pool.ntp.org')
	print ctime(response.tx_time)

if __name__ == '__main__':
	print_time();