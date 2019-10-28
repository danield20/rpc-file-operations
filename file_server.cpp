#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "file_server.h"

using namespace std;

static map<string, int> wordMap;
static map<int, int> lengthMap;

void populateMaps() {
	fstream file;
	string current_line;

	file.open(FILE, std::fstream::in);

	if (file.fail()) {
		perror("Error opening file: ");
		exit(1);
	}

	while(getline(file, current_line)) {
		char *token = strtok((char *)current_line.c_str(), delimiters);

		while (token != NULL) {
			string current_word = string(token);
			wordMap[current_word]++;
			lengthMap[current_word.size()]++;
			token = strtok(NULL, delimiters);
		}
	}

	file.close();
}

bool validateCommand(string cmd) {
	return (cmd.compare(SEARCH) == 0) || (cmd.compare(APPEND) == 0);
}

response confirmCommand(string current_command) {
	if (validateCommand(current_command)) {
		return YES_RESPONSE;
	} else {
		return NO_RESPONSE;
	}
}

response execAppend(string current_word) {
	response result;
	fstream file;

	file.open(FILE, fstream::app);

	if (file.fail()) {
		result.type = -1;
		return result;
	}

	file << " " + current_word;
	wordMap[current_word]++;
	lengthMap[current_word.length()]++;

	file.close();

	result.type = 2;
	string succ_msg = "SUCCESS " + to_string(wordMap[current_word]);
	strcpy(result.response_message, succ_msg.c_str());

	return result;
}

response getSearch(string current_word) {
	response result;

	try
	{
		int length = stoi(current_word);
		result.type = 2;
		int number_of_words = lengthMap[length];
		string number_of_words_string = to_string(number_of_words);
		strcpy(result.response_message, number_of_words_string.c_str());
	}
	catch(const std::exception& e)
	{
		int number_of_apps = wordMap[current_word];
		result.type = 2;
		string word_and_apps = current_word + " " + to_string(number_of_apps);
		strcpy(result.response_message, word_and_apps.c_str());
	}

	return result;
}

response getResult(string current_word, string current_command) {
	if (current_command.compare(APPEND) == 0) {
		return execAppend(current_word);
	} else if (current_command.compare(SEARCH) == 0){
		return getSearch(current_word);
	}
}

response* oprequest_1_svc(request *req, struct svc_req *rqstp) {
	static response result;
	static string in_progress_command = "none";

	int current_type = req->type;
	string current_command = string(req->request_message);

	if (current_type == 1) {
		result = confirmCommand(current_command);

		if (strcmp(result.response_message, "YES") == 0) {
			in_progress_command = current_command;
			strcat(result.response_message, (" " + current_command).c_str());
		}

		if (wordMap.empty()) {
			populateMaps();
		}

	} else if ((current_type == 2) && (in_progress_command.compare("none") != 0)) {
		result = getResult(current_command, in_progress_command);
	}

	return &result;
}
