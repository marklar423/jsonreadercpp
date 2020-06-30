#include <iostream>
#include <sstream>

#include "jsonreadercpp/JsonReader.h"

using namespace jsonreadercpp;
using std::string;
using std::cout;
using std::cin;
using std::istringstream;

void PrintJValue(const string& name, const JValue& value, string prefix = "");
void PrintJsonTree(const JValue& root, string prefix = "");

int main(int argc, char *argv[])
{
	//This example uses the >> operator, 
	//but alternatively a singleton JsonReader can be created and reused by calling JsonReader::ParseJsonString(input_stream)
	//This skips creating the internal state machine graph over and over again every time JSON is parsed		
	JValue parsed_value;
	
	if (cin >> parsed_value) //alternatively if (parsed_value.GetValueType() != JValueType::Null)
		PrintJsonTree(parsed_value);
	else
		cout << "No root object in JSON\n";
	
	return 0;
}

void PrintJsonTree(const JValue& root, string prefix) 
{
	if (root.GetNumberOfChildren() > 0)
	{
		if (root.GetValueType() == JValueType::Array)
		{
			int i = 0;
			for (const auto& value : root.AsArrayConst()) 
			{
				PrintJValue(std::to_string(i), value, prefix);
				i++;
			}
		}
		else if (root.GetValueType() == JValueType::Object)
		{
			for (const auto& kvp : root.AsObjectConst()) 
			{				
				PrintJValue(kvp.first, *kvp.second, prefix);
			}
		}
	}
}

void PrintJValue(const string& name, const JValue& value, string prefix)
{
	cout << prefix << "[" << name << "]: ";

	if (value.GetValueType() == JValueType::Object)
		cout << "[object[" << value.GetNumberOfChildren() << "]]\n";
	else if (value.GetValueType() == JValueType::Array)
		cout << "[array[" << value.GetNumberOfChildren() << "]]\n";
	else if (value.GetValueType() == JValueType::Null)
		cout << "[null]\n";
	else
		cout << value.ConvertToString() << "\n";
	
	if (value.GetNumberOfChildren() > 0)
	{
		PrintJsonTree(value, prefix + "[" + name + "]");
	}
}