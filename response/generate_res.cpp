#include "../webserv.hpp"
#include "../config.hpp"

std::string generate_http_response(HttpResponse &res)
{
	std::stringstream res_str;
	res_str << res.version << " " << res.code<< " " << res.reason_phrase << HTTP_DEL;
	for (std::map<std::string, std::string>::iterator it = res.headers.begin(); it != res.headers.end(); it++)
		res_str << it->first << ": " << it->second << HTTP_DEL;
	res_str << HTTP_DEL;
	return res_str.str();
}

void init_response(HttpResponse& response, HttpRequest& request, int fd)
{
	response.fd = fd;
	response.byte_reading = 0;
	response.request = request;
	response.get_length = false;
	response.finish_reading = false;
	response.url_changed = false;
	response.pid = -1;
	response.server_it = server(response.request);
	std::cout << YELLOW<< "***********>reser  " << response.server_it->ip<< " || "<< response.server_it->port << END << std::endl; 
	response.location_it = location(response.request, response.server_it);
	std::cout << YELLOW<< "***********> location = " << response.location_it->target  << END << std::endl; 
}

std::string get_content_type(std::string path)
{
	std::map<std::string, std::string> content_type;
	std::string type = path.substr(path.rfind(".") + 1,path.length());

	content_type["html"] = "text/html";
	content_type["htm"] = "text/html";
	content_type["shtml"] = "text/html";
	content_type["css"] = "text/css";
	content_type["js"] = "text/javascript";
	content_type["png"] = "image/png";
	content_type["json"] = "application/json";
	content_type["xml"] = "application/xml";
	content_type["pdf"] = "application/pdf";
	content_type["jpeg"] = "image/jpeg";
	content_type["jpg"] = "image/jpeg";
	content_type["gif"] = "image/gif";
	content_type["txt"] = "text/plain";
	content_type["mp4"] = "video/mp4";
	content_type["c"] = "text/x-csrc";
	content_type["cpp"] = "text/x-c++src";

	if (content_type.find(type) != content_type.end())
		return (content_type[type]);
	return("application/octet-stream");
}

std::string	get_reason_phase(int status_code)
{
	std::map<int, std::string> reason_phase;

	reason_phase[200] = "ok";
	reason_phase[201] = "created";
	reason_phase[204] = "No Content";
	reason_phase[301] = "Moved Permanently"; 
	reason_phase[302] = "Found"; 
	reason_phase[400] = "Bad Request";
	reason_phase[403] = "Forbidden";
	reason_phase[404] = "Not Found";
	reason_phase[405] = "Method Not Allowed";
	reason_phase[409] = "Conflict";
	reason_phase[413] = "Request Entity Too Large";
	reason_phase[414] = "Request-URI Too Long";
	reason_phase[500] = "Internal Server Error";
	reason_phase[501] = "not implemented";
	reason_phase[504] = "Gateway Timeout";

	return(reason_phase[status_code]);
}

void fill_response(int status_code, HttpResponse& response)
{
	response.version = response.request.version;
	response.code = status_code;
	response.reason_phrase = get_reason_phase(status_code);
	response.headers["Connection"] = "Close";
	response.headers["Content-type"] = get_content_type(response.path_file);
}

int check_connexion(HttpResponse &response)
{
	char    line[2];
    int byte_read = 0;

    byte_read = recv(response.fd, line, 0, MSG_PEEK);
	if (byte_read < 0)
	{
		std::cerr << "Resource temporarily unavailable" << std::endl;
		return (-1);
	}
	return (0);
}