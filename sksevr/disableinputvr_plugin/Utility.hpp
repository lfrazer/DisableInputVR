#ifndef UTILITY_HPP
#define UTILITY_HPP

#pragma once

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

// trim from start (in place)
static inline void ltrim(std::string &s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) 
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) 
{
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) 
{
	rtrim(s);
	return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) 
{
	trim(s);
	return s;
}

static inline std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

static inline void skipComments(std::string& str)
{
	auto pos = str.find("#");
	if (pos != std::string::npos)
	{
		str.erase(pos);
	}
}

static inline std::string GetConfigSettingsValue(std::string line, std::string &variable)
{
	std::string valuestr = "";
	std::vector<std::string> splittedLine = split(line, '=');
	variable = "";
	if (splittedLine.size() > 1)
	{
		variable = splittedLine[0];
		trim(variable);

		valuestr = splittedLine[1];
		trim(valuestr);
	}
	else
	{
		variable = line;
		trim(variable);
		valuestr = "0";
	}

	return valuestr;
}

static inline bool Contains(std::string str, std::string ministr)
{
	if (str.find(ministr) != std::string::npos) {
		return true;
	}
	else
		return false;
}

template <typename I> static inline std::string num2hex(I w, size_t hex_len = sizeof(I) << 1) {
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i<hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}

static inline std::string removeWhiteSpaces(std::string s)
{
	std::string s2;
	s2.reserve(s.size());
	std::remove_copy_if(begin(s), end(s), std::back_inserter(s2), [l = std::locale{}](auto ch) { return std::isspace(ch, l); });
	return s2;
}

static inline bool has_suffix(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static inline uint64_t GetButtonMaskFromId(int id)
{ 
	return 1ull << id; 
}

#endif