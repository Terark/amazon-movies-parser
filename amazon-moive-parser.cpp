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
	if (end == -1)
	{
		return std::map<std::string, std::string>::value_type(std::string(), src);
	}
	return make_pair(src.substr(start, end), src.substr(end + delim.size()));
}

bool replace(std::string& src, const std::string& target, const std::string& dst)
{
	std::string::size_type start = src.find(target);
	if (start == -1)
	{
		return false;
	}

	src.replace(start, target.size(), dst);
	return true;
}

bool find(const std::vector<std::string>& vec, const std::string& target)
{
	for (std::vector<std::string>::const_iterator it = vec.begin();
		 it != vec.end();
		 it++)
	{
		if (*it == target)
		{
			return true;
		}
	}
	return false;
}

void print(const std::map<std::string, std::string>& value_buffer,
		   const std::vector<std::map<std::string, std::string>::value_type> NAMES,
		   std::ostream& os)
{
	for (std::vector<std::map<std::string, std::string>::value_type>::const_iterator it = NAMES.begin();
			 it != NAMES.end();
			 it++)
	{
		std::map<std::string, std::string>::const_iterator ivalue = value_buffer.find(it->first);
		if (ivalue == value_buffer.end())
		{
			continue;
		}
		os << ivalue->second << "\t";
	}
	os << std::endl;
}

/**
 * ./movie infilename outfilename
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
	const vector<map<string, string>::value_type> NAMES = {
			map<string, string>::value_type("product/productId", string()),
			map<string, string>::value_type("review/userId", string()),
			map<string, string>::value_type("review/profileName", string()),
			map<string, string>::value_type("review/helpfulness", string()),
			map<string, string>::value_type("review/score", string()),
			map<string, string>::value_type("review/time", string()),
			map<string, string>::value_type("review/summary", string()),
			map<string, string>::value_type("review/text", string())
	};
	map<string, string> value_buffer(NAMES.begin(), NAMES.end());
	unsigned int line_no = 0;
	while(!fin.eof())
	{
		getline(fin, one_line); line_no++;
		const map<string, string>::value_type name_value = split(one_line, ": ");
		name = name_value.first;
		value = name_value.second;
		replace(value, "\t", "\\t");
		replace(value, "\r", "\\r");

		if (name == NAMES[0].first)
		{
			print(value_buffer, NAMES, fout);
		}

		if (name == string())
		{// split by '\n' incorrectly
			value_buffer[last_name] += "\\n" + value;
			cout << line_no << ": ";print(value_buffer, NAMES, cout);
		}
		else
		{
			value_buffer[name] = value;
			last_name = name;
		}
	}

	fin.close();
	return 0;
}
