#ifndef WEBSERV
#define WEBSERV

#include <string>
#include <map>
#include <vector>

#define PORT 8080
#define BACKLOG_SIZE 32
#define HTTP_DEL "\r\n"
/*
	execve, dup, dup2, pipe, strerror, gai_strerror,
	errno, dup, dup2, fork, htons, htonl, ntohs, ntohl,
	select, poll, epoll (epoll_create, epoll_ctl,
	epoll_wait), kqueue (kqueue, kevent), socket,
	accept, listen, send, recv, bind, connect,
	getaddrinfo, freeaddrinfo, setsockopt, getsockname,
	getprotobyname, fcntl, close, read, write,
	waitpid, kill, signal, access, opendir, readdir
	and closedir.
*/
class HttpRequest {
	public:
		std::string method;
		std::string url;
		std::string version;
		std::map<std::string, std::string> headers;
};

class HttpResponse {
	public:
		std::string version;
		int code;
		std::string content;
		std::map<std::string, std::string> headers;
};

void die(std::string msg);

// http
std::vector<std::string> split(std::string s, std::string delimiter, unsigned int max_splits = -1);
void parse_http_request(std::string req_str, HttpRequest &req);
std::string generate_http_response(HttpResponse &res);

// epoll
int init_epoll();
void epoll_add_fd(int efd, int fd, uint32_t events);
void epoll_del_fd(int efd, int fd);
int epoll_wait_fd(int efd);

#endif // WEBSERV
