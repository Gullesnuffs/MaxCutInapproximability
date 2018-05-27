#pragma once

using namespace std;

template <class T>
T gcd(T a, T b) {
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    return b ? gcd(b, a%b) : a;
}

template <class T>
T lcm(T a, T b) {
    return (a/gcd(a, b))*b;
}

template <class T>
struct Rational {

    T a, b;

    Rational(T _a, T _b) {
        a = _a;
        b = _b;
        assert(a != 0 || b != 0);
        fix();
    }

    Rational(T _a) {
        a = _a;
        b = 1;
    }

    Rational() {
        a = 0;
        b = 1;
    }

    Rational<T>& operator=(T _a) {
        a = _a;
        b = 1;
        return *this;
    }

    // Makes sure the denominator is positive and that gcd(a,b)=1
    void fix() {
        if (b < 0) {
            a *= -1;
            b *= -1;
        }
        T g = gcd(a, b);
        a /= g;
        b /= g;
    }

    Rational<T> inverse() const {
        return Rational<T>(b, a);
    }

    Rational<T> operator+(const Rational& other) const {
        return Rational<T>(a*other.b+b*other.a, b*other.b);
    }

    Rational<T> operator-(const Rational& other) const {
        return Rational<T>(a*other.b-b*other.a, b*other.b);
    }

    Rational<T> operator*(const Rational& other) const {
        return Rational<T>(a*other.a, b*other.b);
    }

    Rational<T> operator/(const Rational& other) const {
        return Rational<T>(a*other.b, b*other.a);
    }

    Rational<T> operator-() const {
        return Rational<T>(-a, b);
    }

    Rational<T> operator+(T other) const {
        return Rational<T>(a+other*b, b);
    }

    Rational<T> operator-(T other) const {
        return Rational<T>(a-other*b, b);
    }
    
    Rational<T> operator*(T other) const {
        return Rational<T>(a*other, b);
    }
    
    Rational<T> operator/(T other) const {
        return Rational<T>(a, b*other);
    }

    void operator+=(const Rational& other) {
        *this = (*this + other);
    }

    void operator-=(const Rational& other) {
        *this = (*this - other);
    }

    void operator*=(const Rational& other) {
        *this = (*this * other);
    }

    void operator/=(const Rational& other) {
        *this = (*this / other);
    }

    void operator+=(T other) {
        *this = (*this + other);
    }

    void operator-=(T other) {
        *this = (*this - other);
    }

    void operator*=(T other) {
        *this = (*this * other);
    }

    void operator/=(T other) {
        *this = (*this / other);
    }

    bool operator<(const Rational& other) const {
        return a * other.b < b * other.a;
    }

    bool operator<=(const Rational& other) const {
        return a * other.b <= b * other.a;
    }

    bool operator>(const Rational& other) const {
        return a * other.b > b * other.a;
    }

    bool operator>=(const Rational& other) const {
        return a * other.b >= b * other.a;
    }

    bool operator<(const T& other) const {
        return a < b * other;
    }

    bool operator<=(const T& other) const {
        return a <= b * other;
    }

    bool operator>(const T& other) const {
        return a > b * other;
    }

    bool operator>=(const T& other) const {
        return a >= b * other;
    }

    bool operator==(const T& other) const {
        return b * other == a;
    }

    bool operator!=(const T& other) const {
        return b * other != a;
    }
};
