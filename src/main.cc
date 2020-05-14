#include <iostream>
#include <sstream>

#include "JsonDeserializer.h"

using namespace jsoncpp;
using std::string;
using std::cout;
using std::cin;
using std::istringstream;

void PrintJsonTree(const JValue& root, string prefix);

int main(int argc, char *argv[])
{
	string input_file = "";	
	std::getline(cin, input_file);
	
	cout << input_file << "\n";

	istringstream input_stream(input_file);

	JsonDeserializer parser(true);
	auto parsed_json = parser.ParseJsonString(input_stream);

	if (parsed_json.get() != nullptr)
		PrintJsonTree(*parsed_json, "");
	else
		cout << "No root object in JSON\n";
	

	return 0;
}

void PrintJsonTree(const JValue& root, string prefix) 
{
	int i = 0;
	if (prefix.length() > 0)
		prefix += " / ";

	if (root.GetNumberOfChildren() > 0)
	{
		for (auto& value : root) 
		{
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
			
			if (value->GetNumberOfChildren() > 0)
				PrintJsonTree(*value, value->GetName());

			i++;
		}
	}
}