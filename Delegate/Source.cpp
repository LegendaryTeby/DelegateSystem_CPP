/* marche pas trop
* 
#include <iostream>
#include <vector>

class mainclass
{
public:
	void functionParent(int ival, float fval)
	{

	}
};

class classT : public mainclass
{
public:
	void function(float _val)
	{
		printf("class Function : %f\n", _val);
	}
};

void func(float _val)
{
	printf("Func : %f\n", _val);
}

class delegate
{
public:
	typedef void(*method)(float);

	method func = nullptr;

	delegate()
	{

	}

	delegate(method _func)
	{
		func = _func;
	}

	void invoke(float _val)
	{
		if (func == nullptr)
		{
			printf("Error: func ptr is null\n");
			return;
		}

		func(_val);
	}

	bool isValid() const
	{
		return func != nullptr;
	}
};

template<class Class>
class classdelegate
{
	typedef void(Class::* method)(float);

public:
	Class* owner = nullptr;
	method func = nullptr;

	classdelegate()
	{

	}

	classdelegate(Class* _owner, method _func)
	{
		set(_owner, _func);
	}

	void set(Class* _owner, method _func)
	{
		owner = _owner;
		func = _func;
	}

	void invoke(float _val)
	{
		if (!isValid())
		{
			printf("Error: delegeate invalid (owner or func is null)\n");
			return;
		}

		(owner->*func)(_val);
	}

	bool isValid() const { return owner != nullptr && func != nullptr; }

};

class ActionDelegate
{
	typedef void(*method)(float);

	std::vector<delegate*> delegates;

public:
	//	with the given func, create and add a new delegate. return created delegate
	delegate& add(method _func)
	{
		delegate d(func);
		add(d);
		return d;
	}

	//	add a delegate
	void add(delegate& _delegate)
	{
		if (_delegate.isValid())
		{
			//	std::cout << "Add Address : " << &_delegate << std::endl;
			delegates.push_back(&_delegate);
		}
	}

	//	remove delegate if func is the same (will remove every delegate having the given func)
	void remove(method _func)
	{
		int _max = delegates.size();
		for (int i = 0; i < _max; ++i)
		{
			delegate& d = *delegates[i];
			//	std::cout << "Address : " << &d << " == " << &_delegate << std::endl;
			if (d.func == _func)
			{
				delegates.erase(delegates.begin() + i);
				i--;
				_max--;
			}
		}
	}

	//	remove only the given delegate even if other delegate have the same func
	void remove(delegate& _delegate)
	{
		int _max = delegates.size();
		for (int i = 0; i < _max; ++i)
		{
			delegate& d = *delegates[i];
			//	std::cout << "Address : " << &d << " == " << &_delegate << std::endl;
			if (&d == &_delegate)
			{
				delegates.erase(delegates.begin() + i);
				i--;
				_max--;
			}
		}
	}

	void broadcast(float _val)
	{
		const int& _max = delegates.size();
		for (int i = 0; i < _max; ++i)
		{
			delegate& d = *delegates[i];
			if (d.isValid())
				d.invoke(_val);
		}
	}
};


template<class Class>
class ActionClassDelegate
{
	typedef void(Class::* method)(float);
	std::vector<classdelegate<Class>*> delegates;

public:
	void add(Class* _owner, method _func)
	{
		add(classdelegate<Class>(_owner, _func));
	}
	void add(classdelegate<Class>& _delegate)
	{
		delegates.push_back(&_delegate);
	}

	// remove any delegate than have the same owner and func
	void remove(Class* _owner, method _func)
	{
		int _max = delegates.size();
		for (int i = 0; i < _max; ++i)
		{
			classdelegate<Class>* d = delegates[i];
			if (d->owner == _owner && d->func == _func)
			{
				delegates.erase(delegates.begin() + i);
				i--;
				_max--;
			}
		}
	}

	// remove only the given delegate even if other delegate have the same owner and func
	void remove(classdelegate<Class>& _delegate)
	{
		int _max = delegates.size();
		for (int i = 0; i < _max; ++i)
		{
			classdelegate<Class>& d = *delegates[i];
			if (&d == &_delegate)
			{
				delegates.erase(delegates.begin() + i);
				i--;
				_max--;
			}
		}
	}

	void broadcast(float _val)
	{
		const int& _max = delegates.size();
		for (int i = 0; i < _max; ++i)
		{
			classdelegate<Class>* d = delegates[i];
			if (d->isValid())
				d->invoke(_val);
		}
	}
};

template<class... Args>
struct turple
{
	void func(Args...)
	{

	}
};

class classargs : public mainclass
{
public:
	void funcArgs(int iVal, float fVal)
	{
		printf("int : %d | float : %f\n", iVal, fVal);
	}
};
class classargs2 : public mainclass
{
public:
	void funcArgs(int iVal, float fVal)
	{
		printf("le petit int : %d || et le petit float : %f\n", iVal, fVal);
	}
};

void funcArgs(int iVal, float fVal)
{
	printf("int : %d | float : %f", iVal, fVal);
}


template<typename... Args>
class delegateArgs
{
public:
	typedef void(mainclass::* method)(Args...);
	mainclass* owner = nullptr;
	method func = nullptr;

public:
	delegateArgs()
	{

	}
	delegateArgs(mainclass* _owner, method _func)
	{
		owner = _owner;
		func = _func;
	}

	~delegateArgs()
	{

	}

	bool isvalid() const { return owner != nullptr && func != nullptr; }

	void invoke(Args... params)
	{
		if (!isvalid())
		{
			printf("\nError: invalid delegate\n");
			return;
		}
		(owner->*func)(params...);
	}

};


template<typename... Args>
class ActionArgs
{
	std::vector<delegateArgs<Args...>*> delegates;

public:

	void add(delegateArgs<Args...>& _delegate)
	{
		delegates.push_back(&_delegate);
	}

	void broadcast(Args... params)
	{
		const int& _max = delegates.size();
		for (int i = 0; i < _max; ++i)
		{
			delegateArgs<Args...>& _d = delegates[i];
			_d.invoke(params...);
		}
	}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	typedef int coucou;
	typedef void(*funcptr)(float);

	printf("\nFunc ptr\n");
	funcptr f = &func;
	f(59);

	void(*methodPtr)(float);
	methodPtr = &func;
	methodPtr(15);

	void(classT:: * methodClass)(float);
	methodClass = &classT::function;

	printf("\nClass Func ptr\n");
	classT test;
	classT test2;
	(test.*methodClass)(25);

	printf("\nDelegate\n");
	delegate deleg1(&func);
	deleg1.invoke(50);
	delegate deleg2(&func);
	delegate deleg3(&func);
	delegate deleg4(&func);
	delegate deleg5(&func);

	printf("\nAction Delegate\n");
	ActionDelegate actionD;
	actionD.add(deleg1);
	actionD.add(deleg2);
	actionD.add(deleg3);
	actionD.add(deleg4);
	actionD.add(deleg5);
	//std::cout << "Address : " << &deleg2 << std::endl;
	actionD.remove(deleg2);
	actionD.broadcast(150);

	printf("\nAction Class Delegate\n");
	classdelegate<classT> cDeleg1(&test, &classT::function);
	classdelegate<classT> cDeleg2(&test, &classT::function);
	classdelegate<classT> cDeleg3(&test, &classT::function);
	classdelegate<classT> cDeleg4(&test2, &classT::function);
	classdelegate<classT> cDeleg5(&test, &classT::function);
	classdelegate<classT> cDeleg6(&test, &classT::function);

	ActionClassDelegate<classT> actionCD;
	actionCD.add(cDeleg1);
	actionCD.add(cDeleg2);
	actionCD.add(cDeleg3);
	actionCD.add(cDeleg4);
	actionCD.add(cDeleg5);
	actionCD.add(cDeleg6);
	//actionCD.remove(&test, &classT::function);
	actionCD.remove(cDeleg4);
	actionCD.broadcast(950);

	printf("\nTemplate param list\n");

	turple<int> t1;
	turple<int, int, int> t2;
	t1.func(1);
	t2.func(1, 2, 3);

	mainclass classmain;
	classargs classArgs;

	delegateArgs<int, float> delegateArgs1(&classmain, &mainclass::functionParent);
	delegateArgs<int, float> delegateArgs2(&classArgs, &classargs::functionParent);
	delegateArgs<int, float> delegateArgs3(&classArgs, &classargs::funcArgs);

	//typedef void(mainclass::* ptr)(int, float);
	//typedef void(classargs::* ptr1)(int, float);

	ActionArgs<int, float> action;
	action.add(delegateArgs1);
	action.add(delegateArgs2);
	action.add(delegateArgs3);

	printf("\n\n");
	return 0;
}

*/