// JettNeubacher_FinalProject.cpp
// author: Jett Neubacher

#include<iostream>
#include<string>
using namespace std;

#include<stdexcept>
using std::runtime_error;

#include<stack>

class SemanticError: public runtime_error
{
	public:
	SemanticError(): runtime_error("There is a semantic error!") {}
};
class SyntaxError: public runtime_error
{
	public:
	SyntaxError(): runtime_error("There is a syntax error!") {}
};

// Prints char stack for debugging purposes
void printStack(stack<char> s)
{
	cout << endl << "CURRENT STACK: " << endl;
	while(!s.empty())
	{
		cout << s.top() << endl;
		s.pop();
	}
	cout << endl;
}
// Prints int stack for debugging purposes
void printStack(stack<int> s)
{
	cout << endl << "CURRENT STACK: " << endl;
	while(!s.empty())
	{
		cout << s.top() << endl;
		s.pop();
	}
	cout << endl;
}


int PofOPS(char c) // Priority of Operations
			 // returns int that correlates to priority of operator
{
	if (c == '/' || c == '*') return 1;
	else if (c == '+' || c == '-') return 0;
	else return -1;
}

void calculator(string input)
{
	stack<char> s;	
	string postfix = "";
	bool wasOperand = false; // ensures multiple digit numbers stay together in postfix
	bool wasOperator = false; // only sets back to false when an operand is encountered
					  // this will avoid a syntax error of two operators in a row
	bool wasDiv = false; // avoids semantic error of division by 0
	for (int i = 0; i < input.length(); i++) // convert input to postfix
							     // also check for semantic errors
	{
		char c = input[i];
		if (c == ' ') wasOperand = false; // c is a space
		else if (c >= '0' && c <= '9') // c is an operand
		{
			if (wasDiv && c == '0') throw SemanticError(); // cannot divide by 0
			if (wasOperand) postfix += c;
			else
			{
				postfix += ' '; postfix += c;
			}
			wasOperand = true;
			wasOperator = false;
			wasDiv = false;
		}
		else if (c == '(') // c is an opening parenthesis
		{
			s.push('(');
			wasOperand = false;
		}
		else if (c == ')') // c is a closing parenthesis
		{
			if (s.empty() || wasOperator) throw SyntaxError(); // prevents operator from coming directly before )
									   // ensures ) has a opening parenthesis to pair with
			while (s.top() != '(')
			{
				postfix += ' '; postfix += s.top();
				s.pop();
				if (s.empty()) throw SyntaxError(); // if opening parenthesis pair is never found
			}
			s.pop(); // removes open parenthesis pair
			wasOperand = false;
		}
		else // c is an operator
		{
			if (c == '/') wasDiv = true;
			else wasDiv = false;
			if (wasOperator) throw SyntaxError();
			bool pushed = false;
			while (!pushed)
			{
				// if stack is empty or if top of stack is '('
				if (s.empty() || s.top() == '(') 
				{
					s.push(c);
					pushed = true;
				}
				// if c has a higher operator priority than top of stack
				else if (PofOPS(c) > PofOPS(s.top())) 
				{
					s.push(c);
					pushed = true;
				}
				// if c has a lower priority than operator at top of stack
				else
				{
					postfix += ' '; postfix += s.top();
					s.pop();
				}	
			}
			wasOperand = false;
			wasOperator = true;
		}
	}
	
	if (wasOperator) throw SyntaxError(); // an operator must always have a following operand

	while (!s.empty())
	{
		if (s.top() == '(') throw SyntaxError(); // checks for unpaired parenthesis
		postfix += ' '; postfix += s.top();
		s.pop();
	}

	// string postfix now holds the postfix notation of user input
	// syntax and semantics have also now been checked except for indirect division by 0

	cout << "The postfix notation: " << endl << postfix << endl;

	stack<int> s2;
	for(int i = 0; i < postfix.length(); i++)
	{
		if (postfix[i] == ' '); // space, do nothing
		else if (isdigit(postfix[i])) // operand, push full operand onto stack
		{
			int result = 0;
			while(isdigit(postfix[i]))
			{
				char a = postfix[i];
				int j = a - '0'; // converts char to int
				result = result * 10 + j;
				i++; // move through postfix until not a digit
			}
			i--; // reset by 1 so for loop does not skip an index
			s2.push(result);
		}
		else // operator
		{
			int num1 = s2.top();
			s2.pop();
			int num2 = s2.top();
			s2.pop();
			switch(postfix[i])
			{
				case '+': s2.push(num2 + num1); break;
				case '-': s2.push(num2 - num1); break;
				case '*': s2.push(num2 * num1); break;
				case '/': if (num1 == 0) throw SemanticError(); // checks the final semantics case of indirect division by 0
					  else
					  {
					   	s2.push(num2 / num1); 
				   		break;
			      	    	  }	
			}
		}
	}	
	cout << "The result: " << s2.top() << endl;
}

int main()
{
	string input;
	cout << "Please type the expression in the format of Infix Notation: " << endl;
	getline(cin, input);
	try {
		calculator(input);
	}
	catch (runtime_error e) {
		cout << e.what() << endl;
	}
	return 0;
}
