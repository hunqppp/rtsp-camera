#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "Utils.h"

bool writeRawFile(const string data, const string& filename) {
    FILE* file = fopen(filename.data(), "w");
    if (file == NULL) {
        return false;
    }
    fwrite(data.data(), 1, data.length(), file);
	fsync(fileno(file));
    fclose(file);
    return true;
}

bool readRawFile(std::string &data, const string &filename) {
    struct stat file_info;
	int file_obj = -1;
	char *buffer;

	file_obj = open(filename.data(), O_RDONLY);
	if (file_obj < 0) {
		return false;
	}
	fstat(file_obj, &file_info);
	buffer = (char*)malloc(file_info.st_size);
	if (buffer == NULL) {
        close(file_obj);
		return false;
	}
	ssize_t n = pread(file_obj, buffer, file_info.st_size, 0);
    (void)n;
	close(file_obj);

    data.assign((const char*)buffer, file_info.st_size);    
	free(buffer);

    return true;
}

bool readJSONFile(json &data, const std::string &filename) {
	std::string content;
	if (readRawFile(content, filename)) {
		try {
			data = json::parse(content);
			return true;
		}
		catch (const exception &e) {
			std::cout << e.what() << std::endl;
		}
	}
	return false;
}