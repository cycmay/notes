#include "producer-consumer_test.h"

template <typename Object>
std::default_random_engine Factory<Object>::_e(static_cast<unsigned long>(time(0)));

template <typename Object>
std::uniform_int_distribution<unsigned> Factory<Object>::_u{1,3};

void producer_consumer_test(){
    std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

    Factory<int> testFactory;

    std::thread producer1(&Factory<int>::produceTask, &testFactory);
    std::thread producer2(&Factory<int>::produceTask, &testFactory);
    std::thread producer3(&Factory<int>::produceTask, &testFactory);
    std::thread producer4(&Factory<int>::produceTask, &testFactory);

    std::thread consumer1(&Factory<int>::consumeTask, &testFactory);
    std::thread consumer2(&Factory<int>::consumeTask, &testFactory);

    producer1.join();
    producer2.join();
    producer3.join();
    producer4.join();

    consumer1.join();
    consumer2.join();

    return ;
}