#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

enum ErrorCode
{
	SUCCESS,
	INVALID_OPERATOR,
	INVALID_INPUT,
	DIVISION_BY_ZERO
};

bool runProgram();
ErrorCode sortInput(std::string input);
ErrorCode pemdas();
bool isOperator(char oper);

//vectors used to store user input data
std::vector<double> nums{}; 
std::vector<char> opers{};

/*
Loops the program
*/
int main()
{
	while (true)
	{
		runProgram();
	}
	return 0;
}

/*
Handles the I/O and input errors
*/
bool runProgram()
{
	std::string input{};
	nums.clear();
	opers.clear();
	std::cout << "Input an operation (ex 3 + 2 or 3+2): ";
	std::getline(std::cin, input);

	if (input.empty())
	{
		std::cout << "	Error: Input is empty\n";
		return false;
	}

	for (int i = 0; i < input.length(); i++) 
	{
		if (input[i] == ' ')
		{
			input = input.substr(0, i - 0) + input.substr(i + 1, input.length() - (i + 1));
		}
	}

	ErrorCode errorCheck = sortInput(input);
	if (errorCheck == INVALID_OPERATOR)
	{
		std::cout << "	Error: Invalid Operator\n";
		return false;
	}
	if (errorCheck == INVALID_INPUT)
	{
		std::cout << "	Error: Invalid Input\n";
		return false;
	}

	if (pemdas() == DIVISION_BY_ZERO)
	{
		std::cout << "	Error: Division by Zero\n";
		return false;
	}
	std::cout << "	Answer: " << nums[0] << "\n";
	return true;
}

/*
	Checks validity of input and sorts/adds ints and chars into vectors nums and opers, respectively
*/
ErrorCode sortInput(std::string input)
{
	int point{ 0 };
	int charCount{ 0 };
	int numCount{ 0 };
	for (int i = 0; i < input.length(); i++)
	{
		if (!isdigit(input[i]))
		{
			if (input[i] == '.' && isdigit(input[i + 1]))
			{
				numCount++;
			}
			else if (!isOperator(input[i]))
			{
				return INVALID_OPERATOR; //invalid operator
			}
			else
			{
				charCount++;
			}
		}
		else
		{
			numCount++;
		}
	}
	if (numCount == 0)
	{
		return INVALID_INPUT; //no numbers inputted
	}
	if (charCount == 0)
	{
		nums.push_back(stod(input));
		return SUCCESS;
	}
	for (int i = 1; i < input.length(); i++)
	{
		if (i == 1 && !isdigit(input[i - 1]))
		{
			if (input[i - 1] == '/' || input[i - 1] == '*')
			{
				return INVALID_INPUT; //input started with invalid character
			}
		}
		if (!isdigit(input[i - 1]) && !isdigit(input[i]))
		{
			if (input[i] != '-' && input[i] != '+')
			{
				return INVALID_INPUT; //inputted operator twice (2//3)
			}
		}
		if (isdigit(input[i - 1]) && !isdigit(input[i]))
		{
			if (!(input[i] == '.'))
			{
				nums.push_back(stod(input.substr(point, i - point)));
				point = i + 1;
				opers.push_back(input[i]);
			}
		}
		if (i == input.length() - 1)
		{
			if (!isdigit(input[i]))
			{
				return INVALID_INPUT; //last digit of input isnt number
			}
			else
			{
				nums.push_back(stod(input.substr(point, input.length() - point)));
			}
		}
	}
	return SUCCESS;
}

/*
	iterates through opers and executes math operations on the respective ints stored in nums, following pemdas order
*/

ErrorCode pemdas()
{
	while (opers.size() != 0)
	{
		for (int j = 0; j < opers.size(); j++)
		{
			if (std::find(opers.begin(), opers.end(), '*') != opers.end() || std::find(opers.begin(), opers.end(), '/') != opers.end())
			{
				if (opers[j] == '*')
				{
					nums[j] = nums[j] * nums[j + 1];
					opers.erase(opers.begin() + j);
					nums.erase(nums.begin() + j + 1);
				}
				else if (opers[j] == '/')
				{
					if (nums[j + 1] == 0)
					{
						return DIVISION_BY_ZERO; //division by zero error
					}
					else
					{
						nums[j] = nums[j] / nums[j + 1];
						opers.erase(opers.begin() + j);
						nums.erase(nums.begin() + j + 1);
					}
				}
			}
			else
			{
				if (opers[j] == '+')
				{
					nums[j] = nums[j] + nums[j + 1];
					opers.erase(opers.begin() + j);
					nums.erase(nums.begin() + j + 1);
				}
				else
				{
					nums[j] = nums[j] - nums[j + 1];
					opers.erase(opers.begin() + j);
					nums.erase(nums.begin() + j + 1);
				}
			}
		}
	}
	return SUCCESS;
}

/*
switch-case to check if operator is valid
*/
bool isOperator(char oper)
{
	switch (oper)
	{
	case '+':
	case '-':
	case '*':
	case '/':
		return true;
	default:
		return false;
	}
}