#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <algorithm>

class Utils {
public:
	//unsigned int split: split a string <txt> everywhere a certain character <ch> is found
	//store the resulting substrings in a vector <strs>
	//returns the size of the vector
	//string of list of int I.E: 25,46,3,64 If we were to pass in a character (,) and say split it
	//It would look for every (,) split up the numbers in seperate strings and store it in this vector
	//static so we dont have to create an instance of util, just call using class name and function
	static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch) {
		int pos = txt.find(ch);
		int initialPos = 0;
		strs.clear();
		while (pos != std::string::npos) { //if it can no longer find what its looking for, it will set pos to npos (large number)
			strs.push_back(txt.substr(initialPos, pos - initialPos + 1)); //how far into the string to go from whever initial pos was
			initialPos = pos + 1; //found one so increase, making next time starting + 1

			pos = txt.find(ch, initialPos);
		}
		//add last one
		strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1)))); //gets right pos to go up until gets last one and adds to vector

		return strs.size(); //return size of array
	}
};

#endif