#include <iostream>

#include "MyDelegate.h"

class mainclass
{
public:
	void parentfunc(int _ival, float _fval)
	{
		printf("Parent func : %d | %f\n", _ival, _fval);
	}

	void parentfuncdynamic(int _ival, float _fval)
	{
		printf("Parent func Dynamic : %d | %f\n", _ival, _fval);
	}
};

class childclass : public mainclass
{
public:
	void childfunc(int _ival, float _fval)
	{
		printf("Child func : %d | %f\n", _ival, _fval);
	}
	void childfuncDynamic(int _ival, float _fval)
	{
		printf("Child func Dynamic : %d | %f\n", _ival, _fval);
	}
};

void function(int _ival, float _fval)
{
	printf("function : %d | %f\n", _ival, _fval);
}

void func()
{

}

int main()
{

	return 0;
}