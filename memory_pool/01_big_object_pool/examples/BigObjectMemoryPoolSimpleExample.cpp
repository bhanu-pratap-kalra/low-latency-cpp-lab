#include "BigObjectMemoryPool.h"

#include <iostream>

int main()
{
	BigObjectMemoryPool big_object_memory_pool{2};
	BigObject* first_big_object = big_object_memory_pool.acquire();
	BigObject* second_big_object = big_object_memory_pool.acquire();
	
	if (first_big_object == nullptr || second_big_object == nullptr)
	{
		return -1;
	}
	else
	{
		std::cout << "Two BigObject(s) allocated succesfully." << std::endl;
	}
	
	BigObject* third_big_object = big_object_memory_pool.acquire();
	
	if (third_big_object == nullptr)
	{
		std::cout << "Big Object Memory Pool exhausted as expected." << std::endl;
	}
	
	big_object_memory_pool.release(first_big_object);

	std::cout << "First BigObject released, available slots = " << big_object_memory_pool.available() << std::endl;
	
	std::cout << "Lets try to reuse first BigObject" << std::endl;

	BigObject* reused_big_object = big_object_memory_pool.acquire();

	if (reused_big_object == first_big_object)
	{
		std::cout << "First BigObject reused" << std::endl;
	}
	
	big_object_memory_pool.release(reused_big_object);
	big_object_memory_pool.release(second_big_object);
	
	return 0;
}

