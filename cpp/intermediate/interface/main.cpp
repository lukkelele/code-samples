#include <cstdio>

#include "interface.h"

/**
 * An interface is a great way to enforce a contract between different parts in your code.
 * A guarantee that certain functions exist, without knowing how they are implemented.
 * This makes the code easier to replace, extend and test.
 */

class LulleBulle : public Interface
{
public:
	LulleBulle() = default;
	~LulleBulle() = default;

	/**
	 * Virtual functions are implemented in this fashion.
	 * It is not required to write 'virtual' and 'override' but it is good
	 * to be explicit in cases like this.
	 *
	 * Add '//' before the 'virtual' to comment out the function and try build
	 * the sample. As you will see it will no longer compile.
	 */
	virtual bool IsValid() const override { return true; }

	virtual std::string GetName() const override { return "LulleBulle"; }

	/**
	 * This is a normal class function.
	 * It is OK to use the interface functions in 'normal' functions
	 * The interface is merely a way of enforcing rules to be followed,
	 * i.e that some functions must be available or else the program will not compile.
	 */
	void SayGoodbye() const
	{
		std::printf("This is captain %s speaking, goodbye!\n", GetName().c_str());
	}
};

class Ullis : public Interface
{
public:
	Ullis() = default;
	~Ullis() = default;

	virtual bool IsValid() const override { return false; }
	virtual std::string GetName() const override { return "Ullis"; }

	void SayGoodbye() const
	{
		std::printf("My name is %s and I'm leaving, peace out nerds!\n", GetName().c_str());
	}
};

/**
 * THIS FUNCTION USES THE INTERFACE AND NOT THE OBJECT TYPE DIRECTLY!
 *
 * Look at the type passed in to this function, it is a reference of type Interface.
 * Because LulleBulle implements the interface (by implementing the purely abstract functions)
 * it is possible to pass LulleBulle to this function without any issues.
 */
static void PrintObjectName(Interface& Object)
{
	std::printf("Object name: %s\n", Object.GetName().c_str());
}

/**
 * THIS FUNCTION USES THE INTERFACE AND NOT THE OBJECT TYPE DIRECTLY!
 */
static bool IsObjectValid(Interface& Object)
{
	return Object.IsValid();
}

int main()
{
	std::printf("sample: interface\n\n");

	LulleBulle lulle;
	PrintObjectName(lulle);
	if (IsObjectValid(lulle)) {
		std::printf("Object is valid\n");
	} else {
		std::printf("Object is NOT valid: %s\n", lulle.GetName().c_str());
	}

	std::printf("\n");

	Ullis ullis;
	PrintObjectName(ullis);
	if (IsObjectValid(ullis)) {
		std::printf("Object is valid\n");
	} else {
		std::printf("Object is NOT valid: %s\n", ullis.GetName().c_str());
	}

	std::printf("\n");

	lulle.SayGoodbye();
	ullis.SayGoodbye();

	return 0;
}
