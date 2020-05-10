#include <iostream>
#include <sstream>

#include "JsonDeserializer.h"

using namespace jsoncpp;
using std::string;
using std::cout;
using std::cin;
using std::istringstream;

int main(int argc, char *argv[])
{
	string input_file = "";	
	std::getline(cin, input_file);
	
	cout << input_file << "\n";

	istringstream input_stream(input_file);

	JsonDeserializer parser;
	auto parsed_json = parser.ParseJsonString(input_stream);

	return 0;
}

void PrintJsonTree(const JValue& root, string prefix) {
	int i = 0;
	if (prefix.length() > 0)
		prefix += " / ";

	for (auto& value : root) {

		const string& name = value->GetName();

		if (name.length() > 0)
			cout << prefix << "[\"" << name << "\"]: ";
		else		
			cout << prefix << "[" << i << "]: ";
		
		if (value->GetValueType() == JValueType::Object)
			cout << "{\n";
		else if (value->GetValueType() == JValueType::Array)
			cout << "[\n";
		else
			cout << value->GetStringValue().value_or("") << "\n";
		
		if (value->GetSize() > 0)
			PrintJsonTree(*value, value->GetName());

		i++;
	}
}