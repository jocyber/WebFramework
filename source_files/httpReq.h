#ifndef HTTP
#define HTTP

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <netinet/tcp.h>
#include <unordered_map>
#include <climits>

const std::string HTTP_HEADER = "HTTP/1.1 200 OK\r\n\n";
const std::string HTTP_ERROR = "HTTP/1.1 404 Not Found\r\n\n";
const std::string HTTP_IF_MODIFIED = "HTTP/1.1 304 Not Modified\r\n\n";

#define PORT 5200
#define BUFFSIZE 512
#define REQUEST_SIZE 256

using usint = unsigned short int;
extern void errexit(const std::string message);
extern int errclose(const std::string message, int fd);

namespace web {
	class app {
		struct last_mod_date {
			usint month_c = USHRT_MAX;
			usint year = USHRT_MAX;
			usint day = USHRT_MAX;

			usint hour = USHRT_MAX;
			usint min = USHRT_MAX;
			usint sec = USHRT_MAX;
		};

		std::unordered_map<std::string, int> month_code = {
		 {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6},
		 {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
		};

		int clientfd, sockfd;
		struct sockaddr_in addr;
		int file;
		char req[REQUEST_SIZE];

		std::string req_method = "", file_name = "";

		public:
			app();
			~app();
			void run(std::string(*mainLogic)(void));
			std::string getMethodType() const {return req_method;}
			std::string getFileName() const {return file_name;}

		private:
			std::string getRequestedFile(const std::string &request) const;
			int handleGetRequest(const std::string &name);
			bool was_modified(char date[], char time[]);
	};
}

#endif
