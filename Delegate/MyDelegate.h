#pragma once

#include <vector>

//	Delegate Interface
template<typename... Params>
class idelegate
{
public:
	virtual bool isvalid() const = 0;
	virtual void invoke(Params... _params) = 0;
};

//	Function Delegate
template<typename... Params>
class delegate : public  idelegate<Params...>
{
	typedef void(*method)(Params...);
	method func = nullptr;

public:
	delegate() { }
	delegate(method _func)
	{
		func = _func;
	}

	virtual bool isvalid() const
	{
		return func != nullptr;
	}
	virtual void invoke(Params... _params)
	{
		if (!isvalid())
			throw std::exception("Invalid Delegate\n");

		(*func)(_params...);
	}
};

//	Member Delegate
template<class Class, typename... Params>
class memberdelegate : public idelegate<Params...>
{
	typedef void(Class::* method)(Params...);
	Class* owner = nullptr;
	method func = nullptr;

public:
	memberdelegate() { }
	memberdelegate(Class* _owner, method _func)
	{
		owner = _owner;
		func = _func;
	}

	virtual bool isvalid() const
	{
		return owner != nullptr && func != nullptr;
	}
	virtual void invoke(Params... _params) override
	{
		if (!isvalid())
			throw std::exception("Invalid Member Delegate\n");

		(owner->*func)(_params...);
	}
};

//	Action
template<typename... Params>
class action
{
protected:
	std::vector<idelegate<Params...>*> delegates = std::vector<idelegate<Params...>*>();
	typedef typename std::vector<idelegate<Params...>*>::iterator iterator;

private:
	bool exist(idelegate<Params...>& _delegate)
	{
		iterator it = delegates.begin();
		while (it != delegates.cend())
		{
			if (&*(*it) == &_delegate)		//check adresses of delegates
				return true;
			++it;
		}
		return false;
	}

public:
	//template<class Class>
	//idelegate<Params...>& addDynamic(Class* _owner, void(Class::*Method)(Params...))
	//{
	//	memberdelegate<Class, Params...>* _delegate = new memberdelegate<Class, Params...>(_owner, Method);
	//	dynamicDelegates.push_back(_delegate);
	//	delegates.push_back(_delegate);
	//	return *_delegate;
	//}
	//template<class Class> 
	//void removeDynamic(Class* _owner, void (Class::*Method)(Params...))
	//{
	//	
	//}

	void add(idelegate<Params...>& _delegate)
	{
		delegates.push_back(&_delegate);
	}

	void addunique(idelegate<Params...>& _delegate)
	{
		if (exist(_delegate)) return;
		add(_delegate);
	}


	void remove(idelegate<Params...>& _delegate)
	{
		iterator it = delegates.begin();
		while (it != delegates.cend())
		{
			if (&*(*it) == &_delegate)	//check adresses of delegates
				it = delegates.erase(it);
			else
				++it;
		}
	}

	void clear()
	{
		delegates.clear();
	}

	void broadcast(Params... _params)
	{
		iterator it = delegates.begin();
		while (it != delegates.cend())
		{
			try
			{
				(*it)->invoke(_params...);
				++it;
			}
			catch (const std::exception& e)
			{
				printf("Broadcast Error: %s", e.what());
				it = delegates.erase(it);
			}
		}
	}
};
