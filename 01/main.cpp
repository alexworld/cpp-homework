#include <bits/stdc++.h>

using namespace std;

class BigNum {
private:
    long long *number;
    int len;
    static const int base;

public:
    BigNum() = delete;

    BigNum(int size) {
        len = size;
        number = new long long[len];

        for (int i = 0; i < len; i++) {
            number[i] = 0;
        }
    }

    BigNum(const string &s) {
        len = (int(s.size()) + 8) / 9;
        number = new long long[len];

        for (int i = 0; i < len; i++) {
            number[i] = 0;

            for (int j = (i + 1) * 9 - 1; j >= i * 9; j--) {
                if (int(s.size()) - j - 1 >= 0) {
                    number[i] = number[i] * 10 + (s[int(s.size()) - j - 1] - '0');
                }
            }
        }
    }

    BigNum(const BigNum &num) {
        len = num.len;
        number = new long long[len];
        memcpy(number, num.number, len * sizeof(long long));
    }

    BigNum(BigNum &&num): number(nullptr) {
        swap(num);
    }

    void swap(BigNum &num) {
        std::swap(len, num.len);
        std::swap(number, num.number);
    }

    BigNum &operator =(BigNum num) {
        swap(num);
        return *this;
    }

    friend BigNum operator +(const BigNum &one, const BigNum &two) {
        BigNum res(max(one.len, two.len) + 1);
        int add = 0;

        for (int i = 0; i < res.len; i++) {
            res.number[i] = (i < one.len ? one.number[i] : 0) + (i < two.len ? two.number[i] : 0) + add;
            add = int(res.number[i] / base);
            res.number[i] %= base;

        }
        return res;
    }

    friend BigNum operator -(const BigNum &one, const BigNum &two) {
        BigNum res(max(one.len, two.len) + 1);
        int add = 0;

        for (int i = 0; i < res.len; i++) {
            res.number[i] = (i < one.len ? one.number[i] : 0) - (i < two.len ? two.number[i] : 0) - add;

            if (res.number[i] < 0) {
                add = 1;
                res.number[i] += base;
            } else {
                add = 0;
            }
        }
        return res;
    }

    friend BigNum operator *(const BigNum &one, const BigNum &two) {
        BigNum res(one.len * two.len + 1);
        long long add = 0;

        for (int i = 0; i < one.len; i++) {
            for (int j = 0; j < two.len; j++) {
                res.number[i + j] += one.number[i] * two.number[j];
            }
        }

        for (int i = 0; i < res.len; i++) {
            res.number[i] += add;
            add = res.number[i] / base;
            res.number[i] %= base;
        }
        return res;
    }

    string tostr() const {
        int pos = 0;
        stringstream ss;

        for (int i = 0; i < len; i++) {
            if (number[i] != 0) {
                pos = i;
            }
        }
        ss << number[pos];

        for (int i = pos - 1; i >= 0; i--) {
            ss << setw(9) << setfill('0') << number[i];
        }
        string res;
        ss >> res;
        return res;
    }

    ~BigNum() {
        if (number) {
            delete[] number;
        }
    }
};

const int BigNum::base = 1000 * 1000 * 1000;

string trunc(string s) {
    while (!s.empty() && s[0] == ' ') {
        s.erase(s.begin());
    }

    while (!s.empty() && s.back() == ' ') {
        s.pop_back();
    }
    return s;
}

int main() {
    string s;
    getline(cin, s);

    auto tmp = s.find_first_not_of("0123456789 ");
    int pos = (tmp == string::npos ? int(s.size()) : int(tmp));
    BigNum left(trunc(s.substr(0, pos))), right(trunc(s.substr(min(int(s.size()), pos + 1))));
    char op = (pos == int(s.size()) ? '+' : s[pos]);

    switch (op) {
        case '+':
            cout << (left + right).tostr() << endl;
            break;
        case '-':
            cout << (left - right).tostr() << endl;
            break;
        case '*':
            cout << (left * right).tostr() << endl;
            break;
    }
    return 0;
}
