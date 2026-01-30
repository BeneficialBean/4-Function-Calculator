#include "stackCalc.h"

#include <cmath>

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <sstream>
using namespace std;

int main()
{
    double prevAns = 0;
    while(program(prevAns) != -1)
    {
    }
}

int program(double& prevAns)
{
    queue<string> output;
    stack<string> opers;

    istringstream inSS;
    string input;
    cout << "Input an operation (ex 3 + 2 or 3+2): ";

    getline(cin, input);
    char tmpChar;
    double tmpNum;
    string tmpStr;

    //parse from infix notation to reverse polish using the shunting yard alg
    inSS.str(input);
    double firstNum = 0;
    
    // Parse first number
    if (!(inSS >> firstNum))
    {
        inSS.clear();
        if (inSS >> tmpStr)
        {
            if (tmpStr == "cls")
            {
                cout << "\033[2J\033[1;1H";
                return 0;
            }
            else if (tmpStr == "ans")
            {
                firstNum = prevAns;
            }
            else if (tmpStr == "q")
            {
                cout << "   Quitting program. ";
                return -1;
            }
            else
            {
                cout << "   Invalid input - expected a number" << endl;
                return 1;
            }
        }
    }
    output.push(to_string(firstNum));

    
    // Parse operators and following numbers
    while (inSS >> tmpChar)
    {
        if (isOper(tmpChar))
        {
            tmpStr = string(1, tmpChar);
            while (!opers.empty() && hasPrecedence(opers.top(), tmpStr))
            {
                output.push(opers.top());
                opers.pop();
            }
            opers.push(tmpStr);
        }
        else
        {
            cout << "   Invalid input - expected an operator" << endl;
            return 1;  
        }

        if (!(inSS >> tmpNum))
        {
            inSS.clear();
            inSS >> tmpStr;
    
            if (tmpStr == "ans")
            {
                output.push(to_string(prevAns));
            }
            else
            {
                cout << "   Invalid input - expected a number" << endl;
                return 1;
            }
        }
        else
        {
            output.push(to_string(tmpNum));
        }
    }
    while (!opers.empty())
    {
        output.push(opers.top());
        opers.pop();
    }

    //parse through output, reusing tmpStr and tmpNum
    stack<double> numStack;
    tmpNum = 0;
    tmpStr = "";

    while (!output.empty())
    {  
        tmpStr = output.front();
        if (strIsDig(tmpStr))
        {
            numStack.push(stod(tmpStr));
        }
        else
        {
            // Check if there are enough operands on the stack
            if (numStack.size() < 2)
            {
                cout << "   Error: Invalid expression (not enough operands for operator '" << tmpStr << "')" << endl;
                return 1;
            }
            // Pop two operands (second operand first, then first operand)
            double secondOperand = numStack.top();
            numStack.pop();
            double firstOperand = numStack.top();
            numStack.pop();
            
            switch(tmpStr[0])
            {
                case '%':
                    tmpNum = fmod(firstOperand, secondOperand);
                    break;
                case '*':
                    tmpNum = firstOperand * secondOperand;
                    break;
                case '/':
                    tmpNum = firstOperand / secondOperand;
                    break;
                case '+':
                    tmpNum = firstOperand + secondOperand;
                    break;
                default: //assume case '-'
                    tmpNum = firstOperand - secondOperand;
                    break;
            }
            // Push result back onto stack
            numStack.push(tmpNum);
        }
        output.pop();
    }
    prevAns = numStack.top();
    cout << "   Answer: " << prevAns << endl;
    return 0;
}

bool hasPrecedence(string x, string y) //if x has precedence over y
{
    int xVal = 0;
    int yVal = 0;
    switch (x[0])
    {
        case '*':
	    case '/':
        case '%':
            xVal = 1;
            break;
        default:
            xVal = 0;
    }

    switch (y[0])
    {
        case '*':
	    case '/':
        case '%':
            yVal = 1;
            break;
        default:
            yVal = 0;
    }
    
    return (xVal >= yVal);
}

bool isOper(char oper)
{
	switch (oper)
	{
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return true;
        default:
            return false;
	}
}

bool strIsDig(string& s)
{
    if (s.empty()) 
    {
        return false;
    }
    if (s.size() == 1)
    {
        return isdigit(s[0]);
    }

    char c;
    for (size_t i = 0; i < s.length(); i++)
    {
        c = s[i];

        if ((i == 0 && c == '-') || c == '.') { continue; }
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

void printStack(stack<double> s)
{
    cout << "stack: ";
    cout << "top ->";
    while (!s.empty())
    {
        cout << s.top() << ", ";
        s.pop();
    }
    cout << endl;
}

void printQueue(queue<string> q)
{
    cout << "queue: front ->";
    while (!q.empty())
    {
        cout << q.front() << ", ";
        q.pop();
    }
    cout << endl;
}
