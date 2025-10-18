#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;

int main(){
    mutex m;
    condition_variable cv;
    bool is_pong = false;

    auto ping = [&]() ->void{
        while(true){
            unique_lock<mutex> lock(m);
            while(is_pong){
                cv.wait(lock);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            cout << "ping" << endl;
            is_pong = !is_pong;
            cv.notify_one();
        }
    };

    auto pong = [&]() ->void{
        while(true){
            unique_lock<mutex> lock(m);
            while(!is_pong){
                cv.wait(lock);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            cout << "pong" << endl;
            is_pong = !is_pong;
            cv.notify_one();
        }
    };


    thread pong_thread(pong);
    thread ping_thread(ping);

    pong_thread.join();
    ping_thread.join();
}