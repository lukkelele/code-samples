#include <cstdint>
#include <cstdio>
#include <chrono>
#include <thread>

class older_brother
{
public:
    /* Constructor -> runs on instance creation */
    older_brother()
    {
        std::printf("[older_brother] instance created, age=%u name=\"%s\" favorite_number=%d\n", age, name.c_str(), favorite_number);
    }

    /* Destructor -> runs on instance destruction */
    ~older_brother()
    {
        std::printf("[older_brother] instance destroyed, age=%u name=\"%s\" favorite_number=%d\n", age, name.c_str(), favorite_number);
    }

    /* Class functions */
    uint8_t get_age() const { return age; }
    void set_age(uint8_t new_age) { age = new_age; }

    /* The '&' (ampersand) means "reference", very important to learn about. */
    const std::string& get_name() const { return name; }
    void set_name(const std::string& new_name) { name = new_name; }

private:
    /* Member variable can have default-values set like this. */
    uint8_t age = 32;

    /**
     * Uninitialized members are also valid, but if this is the case then
     * the constructor should assign a value to it. It will run either way
     * but if left without initialization, the value CAN be undefined depending
     * on the type. A std::string is fine due to the string class having a constructor
     * that causes it to be fully constructed and valid (due to how classes work).
     * Look at the other variable called 'favorite_number' for an explanation on when it is bad.
     */
    std::string name;

    /**
     * This can be problematic if the constructor does not initialize this member variable
     * by assigning it a value. The reason is because this is a 'fundamental type' that doesn't
     * have a constructor. Therefore the value is undefined.
     * I will demonstrate why this is problematic by logging this member in the constructor.
     */
    int favorite_number;
};

/**
 * This is a "static free function", it only visible in this source file.
 * Internal implementation details are usually declared 'static'.
 * The 'static' keyword prevents other files from linking against it.
 *
 * I know it is many new words and lots of info,
 * for the time being just think of this as a function.
 */
static void create_older_brother_instance_once()
{
    std::printf("enter function: create_older_brother_instance_once\n");
    older_brother older_bro;
    std::printf("exiting function: creating_older_brother_instance_once\n");
}

static void create_older_brother_instance_twice()
{
    std::printf("enter function: create_older_brother_instance_twice\n");
    std::printf(">> creating first instance\n");
    older_brother older_bro_1;

    std::printf("\n>> creating second instance\n");
    older_brother older_bro_2;
    std::printf("exiting function: creating_older_brother_instance_twice\n");
}

static void create_older_brother_instance_and_set_age(uint8_t age)
{
    std::printf("enter function: create_older_brother_instance_and_set_age\n");
    older_brother bro;
    bro.set_age(age);
    std::printf("exit function: create_older_brother_instance_and_set_age\n");
}

int main()
{
    std::printf("\n==============================\n");
    std::printf("\tsample: class\n");
    std::printf("==============================\n\n");

    create_older_brother_instance_once();

    std::printf("\n---------------------------\n\n");

    create_older_brother_instance_twice();

    std::printf("\n---------------------------\n\n");

    create_older_brother_instance_and_set_age(33);

    std::printf("\n---------------------------\n\n");

    older_brother bro;
    bro.set_name("ludvi"); /* Call function on instance */

    const uint8_t age = bro.get_age(); /* const means "this value is constant and cannot change" */
    //age = 25; /* Uncomment this to view the error caused by trying to modify a const variable */

    return 0;
}