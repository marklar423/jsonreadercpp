#include <iostream>
#include <sstream>

#include "jsonreadercpp/JsonReader.h"

using namespace jsonreadercpp;
using std::string;
using std::cout;
using std::cin;
using std::istringstream;

void PrintJsonTree(const JValue& root, string prefix);

int main(int argc, char *argv[])
{
	JsonReader parser;
	auto parsed_json = parser.ParseJsonString(cin);

	if (parsed_json.has_value())
		PrintJsonTree(parsed_json.value(), "");
	else
		cout << "No root object in JSON\n";
	

	return 0;
}

void PrintJsonTree(const JValue& root, string prefix) 
{
	int i = 0;

	if (root.GetNumberOfChildren() > 0)
	{
		for (const auto& value : root) 
		{
			string name(value.GetName());

			if (name.length() == 0)
				name = std::to_string(i);
			
			cout << prefix << "[" << name << "]: ";

			if (value.GetValueType() == JValueType::Object)
				cout << "[object[" << value.GetNumberOfChildren() << "]]\n";
			else if (value.GetValueType() == JValueType::Array)
				cout << "[array[" << value.GetNumberOfChildren() << "]]\n";
			else if (value.GetValueType() == JValueType::Null)
				cout << "[null]\n";
			else
				cout << value.GetStringValue().value_or("") << "\n";
			
			if (value.GetNumberOfChildren() > 0)
			{
				PrintJsonTree(value, prefix + "[" + name + "]");
			}

			i++;
		}
	}
}