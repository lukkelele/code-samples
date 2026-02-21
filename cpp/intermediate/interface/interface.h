#pragma once

#include <string>

/**
 * A basic interface.
 * It is a class but with pure virtual functions.
 * The '= 0' declares a function as a pure virtual function.
 * That is the way how functions are implemented in an interface.
 * There is more info on what an interface is in the main.cpp file.
 *
 * You can code as you like but there are some standards that people follow when
 * creating interfaces and similar stuff like this.
 * One of the rules to follow when creating an interface is to NOT create any members in the class.
 * Only pure virtual functions are 'allowed'.
 * There are lots of reasons to why this is. You will read about it when you start learning about
 * Object Oriented Programming (OOP), for sure.
 *
 * For the time being, just follow this rule and trust me :)
 * Keep it simple.
 */
class Interface
{
public:
	virtual ~Interface() = default;

	virtual bool IsValid() const = 0;
	virtual std::string GetName() const = 0;
};
