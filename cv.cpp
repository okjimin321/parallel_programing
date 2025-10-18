#include<iostream>
#include<mutex>
#include<thread>
#include<condition_variable>
using namespace std;

mutex m;
condition_variable cv;
bool breakfast = false;

void student(){
    unique_lock<std::mutex> lock(m);
    while(breakfast == false){
        cv.wait(lock);
    }
    cout << "eat breakfast" << endl;
}

int main(){
    thread stu(student);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    {
        unique_lock<std::mutex> lock(m);
        breakfast = true;
    }
    cv.notify_one();
    stu.join();
}