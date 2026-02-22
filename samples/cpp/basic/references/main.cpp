#include <cstdio>
#include <string>

struct DataObject
{
	std::string name;
	int data;
};

static void modify_object(DataObject& object, int new_data)
{
	object.data = new_data;
}

static void modify_object(DataObject& object, const std::string& new_name)
{
	object.name = new_name;
}

static void modify_object_without_ref(DataObject object, int new_data)
{
	object.data = new_data;
}

static void modify_object_without_ref(DataObject object, const std::string& new_name)
{
	object.name = new_name;
}

static void print_object(DataObject& object)
{
	std::printf("[DataObject] name=\"%s\" data=%d\n", object.name.c_str(), object.data);
}

int main()
{
	std::printf("\n==============================\n");
	std::printf("  sample: references\n");
	std::printf("==============================\n\n");

	DataObject object1;
	object1.name = "object-1";
	object1.data = 13;
	print_object(object1);

	modify_object(object1, 150);
	modify_object(object1, "OBJECT_1");
	print_object(object1);

	/**
	 * The printed output will be the same as the code above.
	 * This is because the 'modify_object_without_ref' functions do not take
	 * references of objects. What gets modified in those functions is a new
	 * DataObject instance.
	 */
	modify_object_without_ref(object1, 532);
	modify_object_without_ref(object1, "___OBJ-1___");
	print_object(object1);

	return 0;
}
