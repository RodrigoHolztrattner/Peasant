// Peasant.cpp : Defines the entry point for the console application.
//

#include "Peasant.h"

class MyInstance : public Peasant::ObjectInstance
{
	virtual void OnConstruct()
	{

	}
	virtual void OnDependenciesFulfilled()
	{

	}
};

int main()
{
	MyInstance ab;
	MyInstance ba;

	ab = std::move(ba);

    return 0;
}