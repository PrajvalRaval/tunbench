#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "tuntcp.h"

int main(void)
{

	int tun = openTun("tun1");
	struct tcp_conn conn;
	TCPConnection(tun, "192.0.2.2", 80, &conn);
	char buffer[1024] = {0};

	read(tun, buffer, sizeof(buffer));

	struct ipv4 *ip = buf2ip(buffer);
	struct tcp *tcp = buf2tcp(buffer, ip);
	int tcplen = ipdlen(ip);

	conn.seq = ntohl(tcp->ack);
	conn.ack = ntohl(tcp->seq) + 1;

	// Sending an ACK packet
	send_tcp_packet(&conn, TCP_ACK);
	send_tcp_packet(&conn, TCP_SYN);
	conn.state = TCP_ESTABLISHED;

	// Sending a RST packet
	// send_tcp_packet(&conn, TCP_RST);
	// conn.state = TCP_CLOSED;
	return 0;
}
