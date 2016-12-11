#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

template <class T>
class hashed_vector {
private:
    vector <T> v;
    unordered_map <T, unordered_set <size_t> > s;

    void addmap(int left) {
        for (int i = left; i < int(v.size()); i++) {
            s[v[i]].insert(i);
        }
    }

    void delmap(int left) {
        for (int i = left; i < int(v.size()); i++) {
            s[v[i]].erase(i);
        }
    }

    class element {
    private:
        hashed_vector <T> *v;
        int pos;

    public:
        element(hashed_vector <T> *_v, int _pos): v(_v), pos(_pos) {}

        element &operator =(T another) {
            v->s[v->v[pos]].erase(pos);
            v->v[pos] = another;
            v->s[v->v[pos]].insert(pos);
            return *this;
        }

        operator T() {
            return v->v[pos];
        }
    };

public:
    class iterator {
    private:
        hashed_vector <T> *v;
        int pos;
    
    public:
        iterator(hashed_vector <T> *_v, int _pos): v(_v), pos(_pos) {}

        element operator *() {
            return element(v, pos);
        }

        iterator operator +(int num) {
            return iterator(pos + num);
        }

        iterator operator -(int num) {
            return iterator(pos - num);
        }

        iterator &operator ++() {
            pos++;
            return *this;
        }

        iterator operator ++(int) {
            iterator res = *this;
            pos++;
            return res;
        }

        bool operator ==(const iterator &other) {
            return pos == other.pos;
        }

        bool operator !=(const iterator &other) {
            return pos != other.pos;
        }

        int operator -(iterator other) {
            return pos - other.pos;
        }
    };


    hashed_vector(size_t count, const T &value): v(count, value) {
        addmap(0);
    }

    hashed_vector(const vector <T> &other): v(other) {
        addmap(0);
    }

    hashed_vector(vector <T> &&other): v(other) {
        addmap(0);
    }

    ~hashed_vector() {}

    hashed_vector &operator =(const vector <T> &other) {
        s.clear();
        v = other;
        addmap(0);
    }

    hashed_vector &operator =(vector <T> &&other) {
        s.clear();
        v = other;
        addmap(0);
    }

    void assign(int count, const T &value) {
        s.clear();
        v.assign(count, value);
        addmap(0);
    }


    element operator [](int pos) {
        return element(this, pos);
    }

    //T operator [](int pos) const {
    //    return v[pos];
    //}

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, v.size());
    }

    bool empty() const {
        return v.empty();
    }

    size_t size() const {
        return v.size();
    }

    size_t max_size() const {
        return v.max_size();
    }

    void reserve(int new_cap) {
        v.reserve(new_cap);
    }

    size_t capacity() const {
        return v.capacity();
    }

    void shrink_to_fit() {
        v.shrink_to_fit();
    }

    void clear() {
        s.clear();
        v.clear();
    }

    void insert(iterator poss, const T &value) {
        int pos = poss.pos;
        delmap(pos);
        v.insert(v.begin() + pos, value);
        addmap(pos);
    }

    iterator insert(iterator poss, size_t count, const T &value) {
        int pos = poss.pos;
        delmap(pos);
        v.insert(v.begin() + pos, count, value);
        addmap(pos);
    }

    iterator erase(iterator poss) {
        int pos = poss.pos;
        delmap(pos);
        v.erase(v.begin() + pos);
        addmap(pos);
    }

    void push_back(const T &value) {
        v.push_back(value);
        addmap(v.size() - 1);
    }

    void pop_back() {
        delmap(v.size() - 1);
        v.pop_back();
    }

    iterator find(const T &value) {
        return iterator(this, !s[value].empty() ? *s[value].begin() : v.size());
    }
};

int main() {
    hashed_vector <int> A(5, 1);
    A[2] = 3;
    cout << A[2] << endl;

    for (int i = 0; i < 5; i++) {
        A[i] = A[i] + 1;
    }
    A.push_back(100);
    A[5] = A[5] - 1;

    for (int i = 0; i < int(A.size()); i++) {
        cout << A[i] << ' ';
    }
    cout << '\n';

    cout << *A.find(2) << '\n';
    cout << *A.find(4) << '\n';
    cout << *A.find(99) << '\n';

    cout << (A.find(2) - A.begin()) << '\n';
    cout << (A.find(4) - A.begin()) << '\n';
    cout << (A.find(99) - A.begin()) << '\n';
    cout << (A.find(100) - A.begin()) << '\n';

    for_each(A.begin(), A.end(), [](auto n) {n = n * 2;});

    for (int i = 0; i < 6; i++) {
        cout << A[i] << ' ';
    }
    cout << "\n";
}
