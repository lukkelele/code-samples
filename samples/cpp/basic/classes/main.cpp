#include <cstdint>
#include <cstdio>
#include <string>

class older_brother
{
public:
	older_brother()
	{
		std::printf("[older_brother] created, age=%u name=\"%s\" favorite_number=%d\n", age, name.c_str(), favorite_number);
	}

	~older_brother()
	{
		std::printf("[older_brother] destroyed, age=%u name=\"%s\" favorite_number=%d\n", age, name.c_str(), favorite_number);
	}

	uint8_t get_age() const { return age; }
	void set_age(uint8_t new_age) { age = new_age; }
	const std::string& get_name() const { return name; }
	void set_name(const std::string& new_name) { name = new_name; }

private:
	uint8_t age = 32;
	std::string name;
	int favorite_number;
};

class little_brother
{
public:
	/**
	 * A constructor set as 'default' is basically the same as:
	 *    little_brother()
	 *    {
	 *    }
	 *
	 * I recommend reading about constructors.
	 * There is lots of stuff to learn about, explicit/implicit etc...
	 * Learn about one thing at a time, no stress. It will make sense eventually.
	 */
	little_brother() = default;

	/**
	 * Constructor with arguments.
	 * The ':' is used to assign the arguments to each member.
	 */
	little_brother(uint8_t in_age, const std::string& in_name = "Lulle")
		: age(in_age)
		, name(in_name)
	{
		std::printf("[little_brother] created, age=%u name=\"%s\" favorite_number=%d\n", age, name.c_str(), favorite_number);
	}

	~little_brother()
	{
		std::printf("[little_brother] destroyed, age=%u name=\"%s\" favorite_number=%d\n", age, name.c_str(), favorite_number);
	}

private:
	uint8_t age = 26;
	std::string name = "Lukas";
	int favorite_number;
};


int main()
{
	std::printf("\n==============================\n");
	std::printf("  sample: classes\n");
	std::printf("==============================\n\n");

	/**
	 * This is called a 'scope'.
	 * The objects/instances created on the stack in this scope (I know, "What does stack mean??", bear with me)
	 * live inside the scope and once we go 'out of scope' at the end, the created instances will be destroyed.
	 *
	 * Read about:
	 *  1) Stack-based memory allocation
	 *  2) Heap-based memory allocation (also known as dynamic memory allocation)
	 */
	std::printf("Before scope 1\n");
	{
		older_brother older_bro;
		little_brother lil_bro;
	}
	std::printf("After scope 1\n");

	std::printf("\n---------------------------\n\n");

	/**
	 * Using file scopes is a great way to re-use variable names
	 * because a local variable declared in a file scope is only accessable from there.
	 */
	std::printf("Before scope 2\n");
	{
		older_brother older_bro;
		older_bro.set_name("ludde");

		little_brother lil_bro(26, "lukkelele"); /* The second constructor of 'little_brother' is used here. */
	}
	std::printf("After scope 2\n");

	std::printf("\n---------------------------\n\n");

	std::printf("Before scope 3\n");
	{
		older_brother older_bro;
		older_bro.set_name("ullis");

		/* The second constructor of 'little_brother' is used here as well, it uses the default value for the name. */
		little_brother lil_bro(26);
	}
	std::printf("After scope 3\n");

	return 0;
}
