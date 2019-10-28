struct request {
    int type;
    char request_message[150];
};

struct response {
    int type;
    char response_message[150];
};

program FILEPROG {
	version FILEVERS {
			response OPREQUEST(request) = 1;
	} = 1;
} = 0x20000044;
