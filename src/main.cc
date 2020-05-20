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
	JsonDeserializer parser(true);
	auto parsed_json = parser.ParseJsonString(cin);

	if (parsed_json.get() != nullptr)
		PrintJsonTree(*parsed_json, "");
	else
		cout << "No root object in JSON\n";
	

	return 0;
}

void PrintJsonTree(const JValue& root, string prefix) 
{
	int i = 0;

	if (root.GetNumberOfChildren() > 0)
	{
		for (auto& value : root) 
		{
			string name(value->GetName());

			if (name.length() == 0)
				name = std::to_string(i);
			
			cout << prefix << "[" << name << "]: ";

			if (value->GetValueType() == JValueType::Object)
				cout << "[object[" << value->GetNumberOfChildren() << "]]\n";
			else if (value->GetValueType() == JValueType::Array)
				cout << "[array[" << value->GetNumberOfChildren() << "]]\n";
			else if (value->GetValueType() == JValueType::Null)
				cout << "[null]\n";
			else
				cout << value->GetStringValue().value_or("") << "\n";
			
			if (value->GetNumberOfChildren() > 0)
			{
				PrintJsonTree(*value, prefix + "[" + name + "]");
			}

			i++;
		}
	}
}