#include <iostream>
#include "JsonDeserializer.h"

using namespace jsoncpp;
using std::string;
using std::cout;
using std::cin;

int main(int argc, char *argv[])
{
	string input_file = "";	
	std::getline(cin, input_file);
	
	cout << input_file << "\n";

	JsonDeserializer parser;
	auto parsed_json = parser.ParseJsonString(input_file);

	cout << parsed_json << "\n";

	return 0;
}