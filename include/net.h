#ifndef __NET_H__
#define __NET_H__

#include <sys/socket.h>
#include <arpa/inet.h>

#include "sheepdog_proto.h"

enum conn_state {
	C_IO_HEADER = 0,
	C_IO_DATA_INIT,
	C_IO_DATA,
	C_IO_END,
	C_IO_CLOSED,
};

struct connection {
	int fd;
	unsigned int events;

	uint16_t port;
	char ipstr[INET6_ADDRSTRLEN];

	enum conn_state c_rx_state;
	int rx_length;
	void *rx_buf;
	struct sd_req rx_hdr;

	enum conn_state c_tx_state;
	int tx_length;
	void *tx_buf;
	struct sd_rsp tx_hdr;
};

int conn_tx_off(struct connection *conn);
int conn_tx_on(struct connection *conn);
int conn_rx_off(struct connection *conn);
int conn_rx_on(struct connection *conn);
bool is_conn_dead(const struct connection *conn);
int do_read(int sockfd, void *buf, int len);
int rx(struct connection *conn, enum conn_state next_state);
int tx(struct connection *conn, enum conn_state next_state, int flags);
int connect_to(const char *name, int port);
int send_req(int sockfd, struct sd_req *hdr, void *data, unsigned int wlen);
int exec_req(int sockfd, struct sd_req *hdr, void *data);
int create_listen_ports(const char *bindaddr, int port,
			int (*callback)(int fd, void *), void *data);
int create_unix_domain_socket(const char *unix_path,
			      int (*callback)(int, void *), void *data);

char *addr_to_str(char *str, int size, const uint8_t *addr, uint16_t port);
uint8_t *str_to_addr(const char *ipstr, uint8_t *addr);
int set_nonblocking(int fd);
int set_nodelay(int fd);
int set_keepalive(int fd);
int set_snd_timeout(int fd);
int set_timeout(int fd);
int get_local_addr(uint8_t *bytes);
bool inetaddr_is_valid(char *addr);

#endif
