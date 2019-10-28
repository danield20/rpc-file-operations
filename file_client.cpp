#include <iostream>
#include <string>
#include "file_client.h"

using namespace std;

void appendCommand(CLIENT *clnt) {
	response  *response = {0};
	request request = {0};
	string word_to_append;

	response = oprequest_1(&APPEND_REQUEST, clnt);

	string resp = response->response_message;

	if (resp.compare("NO") == 0) {
		printf("[CLIENT]: Server refused request\n");
		return;
	}

	cout << "[CLIENT] Introduce word to append: " << flush;
	getline(cin, word_to_append);

	request.type = 2;
	strcpy(request.request_message, word_to_append.c_str());

	response = oprequest_1(&request, clnt);

	cout << "[CLIENT] " + string(response->response_message) << endl;

}

void searchCommand(CLIENT *clnt) {
	response  *response = {0};
	request request = {0};
	string current_word;

	response = oprequest_1(&SEARCH_REQUEST, clnt);

	string resp = response->response_message;

	if (resp.compare("NO") == 0) {
		printf("[CLIENT]: Server refused request\n");
		return;
	}

	cout << "[CLIENT] Introduce word to search or length: " << flush;
	getline(cin, current_word);
	request.type = 2;
	strcpy(request.request_message, current_word.c_str());

	response = oprequest_1(&request, clnt);

	cout << "[CLIENT] " + string(response->response_message) << endl;
}

void unknownCommand(CLIENT *clnt, string cmd) {
	response  *response = {0};
	request request = {0};
	request.type = 1;
	strcpy(request.request_message, cmd.c_str());

	response = oprequest_1(&request, clnt);

	string resp = response->response_message;

	if (resp.compare("NO") == 0) {
		printf("[CLIENT]: Server refused request, unknown command\n");
		return;
	}
}

void printCommands() {
	cout << "END - close client" << endl;
	cout << "APPEND - append a word to the file" << endl;
	cout << "SEARCH - search a word or number of words with a given length" << endl;
	cout << "HELP - display available commands" << endl;
}

void getCommands(CLIENT *clnt) {
	string current_cmd;
	bool client_running = true;

	while (client_running) {
		getline(cin, current_cmd);

		if (current_cmd.compare(END) == 0) {
			client_running = false;
		} else if (current_cmd.compare(SEARCH) == 0) {
			searchCommand(clnt);
		} else if (current_cmd.compare(APPEND) == 0) {
			appendCommand(clnt);
		} else if (current_cmd.compare(HELP) == 0) {
			printCommands();
		} else {
			unknownCommand(clnt, current_cmd);
		}
	}
}

void fileProgram(char *host) {
	CLIENT *clnt;

	clnt = clnt_create(host, FILEPROG, FILEVERS, "udp");

	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	printCommands();
	getCommands(clnt);

	clnt_destroy (clnt);
}


int main (int argc, char *argv[]) {
	char *host;

	if (argc < 2) {
		printf("[CLIENT] Usage: %s server_host\n", argv[0]);
		return 1;
	}

	host = argv[1];
	fileProgram(host);

	return 0;
}
