#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <stdexcept>

class thread_pool
{
private:
    std::vector<std::thread> workers;               // 工作线程数组
    std::queue<std::function<void()>> tasks;        // 任务队列

    std::mutex queueMtx;                            //队列锁
    std::condition_variable cv;                     //条件变量（控制队列状态）
    bool stop;

    // 删除拷贝构造、赋值
    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

public:
    // 构造：指定工作线程数量
    explicit thread_pool(size_t thread_num) 
        : stop(false)
    {
        for(size_t i = 0;i < thread_num;++i)
        {
            workers.emplace_back([this](){
                for(;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMtx);
                        // 等待任务 / 线程池停止
                        this->cv.wait(lock,[this](){
                            return this->stop || !this->tasks.empty();
                        });
                        // 收到停止信号，并且任务队列为空,退出线程
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    // 提交任务，返回future获取返回值
    template<class F,class... Args>
    auto submit(F&& f,Args&&... args) // 打包函数 f，参数 args，异步执行，future获取返回值
        -> std::future<typename std::result_of<F(Args...)>::type> // 获取函数类型
    {
        using return_type = typename std::result_of<F(Args...)>::type; // 接收函数类型

        // 使用智能指针shared计数，防止悬垂
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f),std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        {
            std::lock_guard<std::mutex> lock(queueMtx);

            if(stop)
                throw std::runtime_error("submit on stopped thread_pool");
            
            tasks.emplace([task](){
                (*task)();
            });

            cv.notify_one();
            return res;
        }
    }

    // 析构：停止线程池，等待所有工作退出
    ~thread_pool()
    {
        {
            std::lock_guard<std::mutex> lock(queueMtx);
            stop = true;
        }
        cv.notify_all();
        for(std::thread &workers : workers)
            workers.join();
    }

};

int main()
{
    thread_pool pool(4); // 创建四个线程

    std::vector<std::future<int>> results;

    // 计算0 - 7 的平方，存入results里面
    for(int i = 0; i < 100; ++i)
    {
        auto fut = pool.submit([](int x){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return x * x;
        }, i);
        results.emplace_back(std::move(fut));
    }

    // 输出results的结果
    for(auto& fut : results)
    {
        std::cout << fut.get() << " ";
    }
    std::cout << "\n";

    return 0;
}