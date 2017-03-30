/*
 * main.cpp
 *
 *  Created on: 2017年3月27日
 *      Author: terark
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cassert>

const std::map<std::string, std::string>::value_type split(const std::string& src, const std::string& delim)
{
	std::string::size_type start = 0, end = src.find(delim);
	if (end == std::string::npos)
	{
		return std::map<std::string, std::string>::value_type(std::string(), src);
	}
	return make_pair(src.substr(start, end), src.substr(end + delim.size()));
}

bool replace(std::string& src, const std::string& target, const std::string& dst)
{
	std::string::size_type start = src.find(target);
	if (start == std::string::npos)
	{
		return false;
	}

	src.replace(start, target.size(), dst);
	return true;
}

void print(const std::map<std::string, std::string>& value_buffer,
		   const std::vector<std::string>& NAMES,
		   std::ostream& os)
{
	for (auto& name : NAMES)
	{
		auto ivalue = value_buffer.find(name);
		if (ivalue == value_buffer.end())
		{
			os << "\t";
			continue;
		}
		os << ivalue->second << "\t";
	}
	os << std::endl;
}

/**
 * ./movie <infilename> <outfilename>
 */
int main(int argc, char** argv)
{
	using namespace std;

	if (argc != 3)
	{
		cout << "please input input file name and/or output file name" << endl;
		return 0;
	}

	string infilename(argv[1]);
	string outfilename(argv[2]);

	ifstream fin(infilename, ios::in);
	if (!fin.is_open() )
	{
		cout << "cannot open file" << infilename <<endl;
		return 0;
	}
	ofstream fout(outfilename, ios::out);
	if (!fout.is_open())
	{
		cout << "cannot open file" << outfilename <<endl;
		return 0;
	}

	string one_line, name, value, last_name;
	const vector<string> NAMES = {
			"product/productId",
			"review/userId",
			"review/profileName",
			"review/helpfulness",
			"review/score",
			"review/time",
			"review/summary",
			"review/text",
	};
	map<string, string> value_buffer;
	unsigned int line_no = 0;
	while(!fin.eof())
	{
		getline(fin, one_line); ++line_no;
		auto name_value = split(one_line, ": ");
		name = name_value.first;		value = name_value.second;
		replace(value, "\t", "\\t");	replace(value, "\r", "\\r");

		if (name == NAMES[0] && !value_buffer.empty())
		{
			print(value_buffer, NAMES, fout);
			value_buffer.clear();
		}

		if (name == string())
		{ // split by '\n' incorrectly
			value_buffer[last_name] += "\\n" + value; // append value
//			cout << line_no << ": ";	print(value_buffer, NAMES, cout); // show broken line on the console
		}
		else
		{
			value_buffer[name] = value;
			last_name = name;
		}
	}

	fin.close();
	fout.close();
	return 0;
}
