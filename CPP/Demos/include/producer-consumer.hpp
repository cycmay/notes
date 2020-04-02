#ifndef PRODUCER_CONSUMER_HPP__
#define PRODUCER_CONSUMER_HPP__

/*
 * 对于生产者消费者问题，使用C++11的多线程库thread以及mutex和condition-variable
 * 多个生产者和多个生产者的问题。生产者不断的向仓库放入产品，消费者不断的从仓库取出产品，
 * 仓库的容量是有限的。因此，当仓库处于满状态时，生产者必须等待消费者取出 1 个或多个产品后
 * 才能继续生产；同理，当仓库处于空状态时，消费者必须等待生产者将产品放入仓库后才能消费
 * （取出）产品。
 * 
 * 解决方案：
 * 使用数组模拟仓库，需要记录下一次生产和消费在数组中的位置。
 * 用生产和消费在数组中的位置判断仓库是否为空或者为满：
 * 假设仓库容量为 N：
 * 
 * -   `(produce_position+1)%N == consume_position` 满
 *     因为初始位置都是 0，当两者相差一个位置时，定义满状态。（最多存储N-1个)
 * -   `consume_position == produce_position` 空 
 * 
 * 当仓库满时，阻塞生产者；当一个消费行为后，仓库非满，唤醒生产者；
 * 当仓库空时，阻塞消费者；当一个生产行为后，仓库非空，唤醒消费者；
 * 因此需要引入，仓库非满条件变量和仓库非空条件变量。
 * 
 * 由于生产和消费行为都会修改数据，因此两者操作必须互斥，需引入生产消费互斥锁。
 * 当我们要生产（或消费）一定数量的产品时，需要计数判断是否已经完成工作；多个生产者进行生产时，
 * 都会对生产的计数变量进行修改，因此需引入**生产计数互斥锁**和**消费计数互斥锁**，保证同时只有
 * 一个生产（或消费）进程对计数变量进行修改。
 * 
 */

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <random>

#include <ctime>
#include <unistd.h>

// 目标产品数量
const int kProduceItems = 100;
// 仓库容量
const int kRepositorySize = 8;

template <typename Object>
class Repository{
public:
    Object items_buff[kRepositorySize];
    // 生产者消费者互斥量 共享资源为仓库以及其属性
    std::mutex PCmutex;
    // 生产者资源互斥量
    std::mutex produce_mutex;
    // 消费者资源互斥量
    std::mutex consume_mutex;

    // 生产的产品数量
    size_t produce_item_count;
    // 消费的产品数量
    size_t consume_item_count;
    size_t produce_position;
    size_t consume_position;

    // 仓库不满条件变量
    std::condition_variable repo_full;
    // 仓库不空条件变量
    std::condition_variable repo_empty;

    Repository():
        produce_item_count(0),
        consume_item_count(0),
        produce_position(0),
        consume_position(0)
    {
        
    }

    void Init(){
        std::fill_n(items_buff, sizeof(items_buff)/sizeof(items_buff[0]), 0);
        produce_item_count(0);
        consume_item_count(0);
        produce_position(0);
        consume_position(0);
    }
};

template<typename Object>
class Factory{
private:
// 工厂内仓库对象
    Repository<Object> repo;
    // 模拟时间
    
    // static std::default_random_engine e(static_cast<unsigned long>(time(0)));
    // static std::uniform_int_distribution<unsigned> _u(1, 10);

    static std::default_random_engine _e;
    static std::uniform_int_distribution<unsigned> _u;

    void ProduceItem(Repository<Object> &repo, Object item){
        // 生产产品函数 
        // @repo: 仓库对象
        // @item: 生产产品序列号

        // 获取PCmutex, 防止共享资源争抢
        std::unique_lock<std::mutex> lck(repo.PCmutex);
        
        // 判断仓库是否满
        while((repo.produce_position+1) % kRepositorySize == 
                repo.consume_position){
            std::cout << "The repository is full, waiting..." << std::endl;
            // 阻塞线程并自动释放锁lck 直到另外一个线程修改条件变量 唤醒本线程重新获取锁
            // 等待仓库是否满的信号 
            (repo.repo_full).wait(lck);            
        }
        // 唤醒并获取锁后执行
        // 生产者生产一份产品 并将指针移动
        repo.items_buff[repo.produce_position]=item;
        repo.produce_position = (repo.produce_position+1)% kRepositorySize;
        // 此时仓库不空
        // 唤醒其他因空阻塞的线程
        (repo.repo_empty).notify_all();
        // 释放锁
        lck.unlock();
    }

    Object ConsumeItem(Repository<Object> &repo){
        // 消费产品函数
        std::unique_lock<std::mutex> lck(repo.PCmutex);
        // 判断仓库是否满
        while(repo.consume_position == repo.produce_position){
            std::cout << "Repository is empty, waiting..." << std::endl;
            // 仓库空 等待该条件变量唤醒该进程
            (repo.repo_empty).wait(lck);
        }
        // 仓库不空 条件变量改变 并获得互斥量的锁
        Object data = repo.items_buff[repo.consume_position];
        // 并将消费序列号后指针后移
        repo.consume_position = (repo.consume_position + 1) % kRepositorySize;
        // 此时仓库不满
        // 唤醒其他因仓库满阻塞的生产者线程
        (repo.repo_full).notify_all();
        // 释放锁
        lck.unlock();
        return data;
    }

public:
    void reset(){
        repo.Init();
    }

    void produceTask(){
        bool ready_to_exit = false;
        while(true){
            // 互斥量为多个生产者共享资源 已经生产的总数量
            std::unique_lock<std::mutex> lck(repo.produce_mutex);
        
            if(repo.produce_item_count < kProduceItems){
                // 目标产量未完成
                ++(repo.produce_item_count);
                Object item = repo.produce_item_count;
                std::cout << "producer id: " << std::this_thread::get_id() <<
                    "is producing " << item << "^th item..." << std::endl;
                // 上报仓库
                ProduceItem(repo, item);
            }else{
                // 产量完成 退出
                ready_to_exit = true;
            }

            lck.unlock();
            if(ready_to_exit)
                break;
            //休眠一段随机时间,代表生产过程
            sleep(_u(_e));
        }
        std::cout << "Producer thread " << std::this_thread::get_id() <<
            "has finished, is exiting..." << std::endl;
    }

    void consumeTask(){
        bool ready_to_exist = false;
        while(true){
            // 互斥量为多个消费者线程共享在资源 当前总共消费的产品数 consume_item_count
            std::unique_lock<std::mutex> lck(repo.consume_mutex);
            // 消费未达标
            if(repo.consume_item_count < kProduceItems){
                Object item = ConsumeItem(repo);
                std::cout << "consumer id: " << std::this_thread::get_id() <<
                    "is consuming, " << item << "^th item..." << std::endl;
                // 预先消费完成上报仓库
                ++(repo.consume_item_count);  
            }else{
                ready_to_exist = true;
            }
            // 释放锁
            lck.unlock();
            if(ready_to_exist)
                break;
            // 模拟消费过程
            sleep(_u(_e));
        }
        // 退出
        std::cout<< "consumer thread" << std::this_thread::get_id() << 
            "has finished, is exiting..." << std::endl;
    }
};

#if 0
// int main(){
//     std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

//     Factory<int> testFactory;
//     std::thread producer1(Factory<int>::produceTask, testFactory);
//     std::thread producer2(Factory<int>::produceTask, testFactory);
//     std::thread producer3(Factory<int>::produceTask, testFactory);

//     std::thread consumer1(Factory<int>::consumeTask, testFactory);
//     std::thread consumer2(Factory<int>::consumeTask, testFactory);
//     std::thread consumer3(Factory<int>::consumeTask, testFactory);

//     producer1.join();
//     producer2.join();
//     producer3.join();

//     consumer1.join();
//     consumer2.join();
//     consumer3.join();

//     return 0;
// }
#endif

#endif