import sys

from pythonosc import udp_client


if __name__ == "__main__":
	client = udp_client.SimpleUDPClient('127.0.0.1', 9001)
	url = sys.argv[1]
	args = sys.argv[2:]
	args = list(map(int, args))
	client.send_message(url, args)

	