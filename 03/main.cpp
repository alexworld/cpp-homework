#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

class thread_pool {
    vector <thread> pool;
    queue <function <void()> > Q;
    mutex m;
    condition_variable cv;

    void thread_func() {
        while (true) {
            unique_lock <mutex> lk(m);
            cv.wait(lk, [this]{return !Q.empty();});

            auto handler = Q.front();
            Q.pop();

            lk.unlock();
            cv.notify_one();

            handler();
        }
    }

public: 
    thread_pool(int num_threads) {
        for (int i = 0; i < num_threads; i++) {
            pool.push_back(thread(bind(&thread_pool::thread_func, this)));
        }
    }

    template <typename T> 
    void push(T handler) {
        m.lock();
        Q.push(handler);
        m.unlock();
        cv.notify_one();
    }

    ~thread_pool() {
        for (auto &x: pool) {
            x.join();
        }
    }
}; 

int main() { 
    thread_pool pool(5); 
    map <int, unique_ptr <mutex> > files;

    while (true) {
        int filesize;
        cin >> filesize;

        pool.push([filesize, &files]() {
            if (!files.count(filesize)) {
                files.emplace(filesize, make_unique <mutex>());
            }
            files[filesize]->lock();

            stringstream ss;
            ss << filesize;
            string filename;
            ss >> filename;
            filename += ".txt";
            
            ofstream fout(filename, ofstream::app);

            for (int i = 0; i < filesize; i++) {
                fout << char('a' + rand() % ('z' - 'a' + 1));
            }
            fout << endl;

            fout.close();

            files[filesize]->unlock();
        }); 
    }
    return 0; 
}
