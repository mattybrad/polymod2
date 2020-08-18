import sys
import time

from pythonosc import udp_client
import smbus

bus = smbus.SMBus(1)    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)
modules = [ 4 ];

if __name__ == "__main__":
	client = udp_client.SimpleUDPClient('127.0.0.1', 9001)
	#url = sys.argv[1]
	#args = sys.argv[2:]
	#args = list(map(int, args))
	#client.send_message(url, args)
	while True:
		for tickNum in range(0, 32):
			bus.write_byte_data(0, 0, tickNum)	# Broadcast, offset=0 means tick message
		time.sleep(1e-3)
		for addr in modules:
			block = bus.read_i2c_block_data(addr, 1, 18) # Offset=1 means connections polling
			nb_new_connections = block[0]
			nb_new_disconnections = block[1]
			ptr = 2
			for i in range(0, nb_new_connections):
				fromModule = block[ptr]
				fromPort = block[ptr+1]
				toModule = block[ptr+2]
				toPort = block[ptr+3]
				ptr += 4
				print("connected: m%dp%d -> m%dp%d" % (fromModule, fromPort, toModule, toPort))
			for i in range(0, nb_new_disconnections):
				fromModule = block[ptr]
				fromPort = block[ptr+1]
				toModule = block[ptr+2]
				toPort = block[ptr+3]
				ptr += 4
				print("disconnected: m%dp%d -> m%dp%d" % (fromModule, fromPort, toModule, toPort))
		time.sleep(1e-3)

	