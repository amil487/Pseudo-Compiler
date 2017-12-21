#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void removeDuplicates(vector<string>& vec);		//method prototype

int main()
{
	vector<string> keyword;				//All the containers
	vector<string> identifier;
	vector<string> constant;
	vector<string> operators;
	vector<string> delimeter;
	vector<string> error;
	vector<string> stack;

	string fileName;		//input file variable

	int current = 0;		//counters used for depth
	int max = 0;

	ifstream myfile;		//file stream

	do						//loop for file input validation 
	{
		cout << "Please enter the name of the input file:\n";
		cin >> fileName;
		myfile.open(fileName);
		if (!myfile)
		{
		cout << "File does not exist! Try again.\n";
		}
		
	} while (!myfile);

	cout << endl;

	string word;						//string for taking in words
	string real = "";					//variable used for adding to containers
	string single = "";
	bool isFor = false;					//bools for checking keywords
	bool isBegin = false;
	bool isEnd = false;

	while (myfile >> word)			//Input loop
	{
		if (isupper(word[0]))		//if the word is a possible keyword
		{
			if (word == "FOR")		//if the word is FOR 
			{
				stack.push_back(word);			//pushed to stack
				isFor = true;
			}

			if (word == "BEGIN")		//If the word is BEGIN
			{
				stack.push_back(word);		//BEGIN is added to stack
				isBegin = true;
				if (stack[stack.size() - 2] == "FOR")			//Checks are made to make sure it's valid
					current++;
				if (current > max)						//Depth checks and balances
					max = current;
			}

			if (word == "END" && stack.size() > 1)			//If word is END and valid
			{
				stack.pop_back();				//Pop two from the stack
				stack.pop_back();
				isEnd = true;
				if (current > 0)			//decrement loop depth
					current--;
			}

			if (word == "FOR" || word == "BEGIN" || word == "END")		//add keyword to container
				keyword.push_back(word);
			else
				error.push_back(word);				//if the word isn't a keyword(misspelled)
		}
		else if (islower(word[0]) || word[0] == '(')		//if the word is lowercase
		{
			int i = 1;

			if (word[0] == '(' && word[1] == '(')			//check for parentheses error
				error.push_back("(");

			if (word[0] == '(')			//if the identifier is first in parameter
			{
				for (; i < word.length(); i++)
				{
					if (word[i] != ';' && word[i] != ',' && word[i] != ')' && word[i] != '=')  //exclude non letter characters
					{
						real += word[i];
					}
				}

				identifier.push_back(real);		//identifier is added to container and we resest the variable
				real = "";

				if (word[i] == ';' || word[i] == ',')		//check to see if the next character is a delimeter
				{
					single = word[i];
					delimeter.push_back(single);		//it's added to it's container and the variable is reset
					single = "";
				}

				if (word[i] == '+' || word[i] == '-' || word[i] == '*' || word[i] == '/' || word[i] == '=')				// if the next character is an operator
				{
					single = word[i];
					operators.push_back(single);
					single = "";
				}
			}
			else
				for (int j = 0; j < word.length(); j++)			//if the identifer is in a block 
				{
					if (word[j] == ';' || word[j] == ',')		//make sure the delimeter isn't added
					{
						single = word[j];
						delimeter.push_back(single);			//delimeter is added to it's container
						single = "";
						break;
					}
					else if (word[j] == '+' || word[j] == '-' || word[j] == '*' || word[j] == '/' || word[j] == '=')  //make sure operator isn't added
					{
						single = word[j];
						operators.push_back(single);					//operator is added to it's container
						single = "";
						break;
					}
					else
						real += word[j];
				}
			identifier.push_back(real);		//the final identifier is added
			real = "";
		}	
		else if (word[0] == '0' || word[0] == '1' || word[0] == '2' || word[0] == '3' || word[0] == '4' || word[0] == '5' || word[0] == '6' || word[0] == '7' || word[0] == '8' || word[0] == '9')		//if input is a constant
		{
			for (int i = 0; i < word.length(); i++)		//constant is added until a delimer or operator is hit
			{
				if (word[i] == '0' || word[i] == '1' || word[i] == '2' || word[i] == '3' || word[i] == '4' || word[i] == '5' || word[i] == '6' || word[i] == '7' || word[i] == '8' || word[i] == '9')
					real += word[i];
				else
				{
					single = word[i];
					if (word[i] == ';' || word[i] == ',')
					{
						single = word[i];
						delimeter.push_back(single);
						single = "";
						break;
					}
					else if (word[i] == '+' || word[i] == '-' || word[i] == '*' || word[i] == '/')
					{
						single = word[i];
						operators.push_back(single);
						single = "";
						break;
					}
				}
			}
			constant.push_back(real);		//final constant added
			real = "";
		}
		else if (word[0] == '+' || word[0] == '-' || word[0] == '*' || word[0] == '/')			//if the input is an opperator
		{
			if (word[word.size() - 1] == ')' && word[word.size() - 2] == ')')			//check for parentheses mistakes
				error.push_back(")");

			for (int i = 0; i < word.length(); i++)		//while operators are hit
			{
				if (word[i] == '+' || word[i] == '-' || word[i] == '*' || word[i] == '/')
					real += word[i];
				else
					break;
			}
			operators.push_back(real);		//final operator added
			real = "";
		}
		else if (word[0] == ',' || word[0] == ';')		//if a standalone delimer is found
		{
			delimeter.push_back(word);

		}
		else
			error.push_back(word);				//if nothing else, then it's an error
	}

	myfile.close();			//file closes

	removeDuplicates(keyword);				//all duplicates in containers removed
	removeDuplicates(identifier);
	removeDuplicates(constant);
	removeDuplicates(operators);
	removeDuplicates(delimeter);
	removeDuplicates(error);

	if (current > 0)
		max = max - current;			//check for depth updates

	if (isFor == false)					//checks for missing keywords
		error.push_back("FALSE");

	if (isBegin == false)
		error.push_back("BEGIN");

	if (isEnd == false)
		error.push_back("END");


	cout << "The depth of the nested loop(s) is " << max << endl << endl;		//all the outputs with container iterators

	cout << "Keywords: ";
	for (int i = 0; i < keyword.size(); i++)
		cout << keyword[i] << " ";
	cout << endl;

	cout << "Identifiers: ";
	for (int i = 0; i < identifier.size(); i++)
		cout << identifier[i] << " ";
	cout << endl;


	cout << "Constants: ";
	for (int i = 0; i < constant.size(); i++)
		cout << constant[i] << " ";
	cout << endl;


	cout << "Operators: ";
	for (int i = 0; i < operators.size(); i++)
		cout << operators[i] << " ";
	cout << endl;


	cout << "Delimeters: ";
	for (int i = 0; i < delimeter.size(); i++)
		cout << delimeter[i] << " ";
	cout << endl << endl;

	cout << "Syntax Error(s): ";
	if (error.size() == 0)
		cout << "NA" << endl;
	else
		for (int i = 0; i < error.size(); i++)
			cout << error[i] << " ";
	cout << endl << endl;
}

void removeDuplicates(vector<string>& vec)			//A method that removes duplicates from a vector 
{
	sort(vec.begin(), vec.end());
	vec.erase(unique(vec.begin(), vec.end()), vec.end());
}