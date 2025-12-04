#pragma once

#include <string>
#include <stack>
#include <queue>

using namespace std;



int program(double&);

bool hasPrecedence(string, string);

bool isOper(char);

bool strIsDig(string&);

void printStack(stack<string>);

void printStack(stack<double>);

void printQueue(queue<string>);