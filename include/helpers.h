//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_HELPERS_H
#define VECTOR_CPP_HELPERS_H

// General mathematical constants
#define PI 3.1415926535
#define E 2.718281828459
#define DtoR 0.01745329252
#define ln2 0.6931471805599569
#define log2E 1.4426950408889392
#define log2_10 3.3219280948873626
#define sqrtPI 1.77245385091
#define sqrt2 1.41421356237
#define sqrt2PI 2.50662827463
#define in :    // This is for Pythonic for loops. I don't use it, but you may if you wish.
#define undefined NaN(2) // Global undefined object, like Python-None, there is just one of it.
                         // However, if you want, you can construct as many as you want.

// Logger levels
#define DEBUG 0
#define INFO 1
#define WARNING 2
#define CRITICAL 3
#define FATAL 4

// Logger colors
// These color definitions are from GitHub:
// https://gist.github.com/Kielx/2917687bc30f567d45e15a4577772b02
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"   //CRITICAL - FATAL
#define GREEN   "\033[32m"   //INFO
#define YELLOW  "\033[33m"   //WARNING
#define BLUE    "\033[34m"   //DEBUG
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

#include "exceptions.h"
#include <ostream>
#include <cstdio>
#include <ctime>

template <typename T>
class Range {
public:
    T step = 1;
    T first = 0;
    T last = 0;

    class Iterator {
        T* current = nullptr;
        T step = 1;
    public:
        explicit Iterator(T* inn, const T& s) {
            this->current = inn;
            this->step = s;
        }

        T& operator* () const {
            return *this->current;
        }

        bool operator!= (const Iterator& i) const {
            // lol
            return *this->current <= *i.current;
        }

        Iterator& operator++ () {
            *(this->current) += this->step;
            return *this;
        }
    };

    Range() = default;

    explicit Range(const T& _last) {
        this->last = _last;
    }

    Range(const T& _first, const T& _last) {
        if (_first > _last) throw RangeError();
        this->first = _first;
        this->last = _last;
    }

    Range(const T& _first, const T& _last, const T& _step) {
        if (_first > _last) throw RangeError();
        this->first = _first;
        this->last = _last;
        this->step = _step;
    }

    Iterator begin() {
        return Iterator(&(this->first), this->step);
    }

    Iterator end() {
        return Iterator(&(this->last), this->step);
    }
};

class NaN {
public:
    unsigned int state = 1;

    NaN() = default;

    explicit NaN (const unsigned int& s) {
        if (s == 0) this->state = 0;
        else if (s == 1) this->state = 1;
        else this->state = 2;
    }

    friend std::ostream& operator<< (std::ostream& o, const NaN& n) {
        if (n.state == 0) o << "Infinity(-)";
        else if (n.state == 1) o << "Infinity(+)";
        else o << "Undefined";
        return o;
    }

    NaN operator+ (const NaN& n) const {
        if (this->state == 2 or n.state == 2) return undefined;
        if (this->state ^ n.state) return undefined;
        return NaN(this->state);
    }

    template <typename T>
    NaN operator+ (const T& anything) const {
        return NaN(this->state);
    }

    template <typename T>
    friend NaN operator+ (const T& anything, const NaN& n) {
        return NaN(n.state);
    }

    NaN operator- (const NaN& n) const {
        if (this->state == 2 or n.state == 2) return undefined;
        if (!(this->state ^ n.state)) return undefined;
        return NaN(this->state);
    }

    template <typename T>
    NaN operator- (const T& anything) const {
        return NaN(this->state);
    }

    template <typename T>
    friend NaN operator- (const T& anything, const NaN& n) {
        if (n.state == 2) return undefined;
        if (n.state == 1) return NaN(0);
        return NaN(1);
    }

    NaN operator* (const NaN& n) const {
        if (this->state == 2 or n.state == 2) return undefined;
        return NaN(this->state and n.state);
    }

    template <typename T>
    NaN operator* (const T& anything) const {
        if (this->state == 2) return undefined;
        if (anything > 0) return NaN(this->state);
        if (anything < 0) {
            if (this->state == 1) return NaN(0);
            return NaN(1);
        }
        if (anything == 0) return undefined;
    }

    template <typename T>
    friend NaN operator* (const T& anything, const NaN& n) {
        if (n.state == 2) return undefined;
        if (anything > 0) return NaN(n.state);
        if (anything < 0) {
            if (n.state == 1) return NaN(0);
            return NaN(1);
        }
        if (anything == 0) return undefined;
    }

    NaN operator/ (const NaN& n) const {
        return undefined;
    }

    template <typename T>
    NaN operator/ (const T& anything) const {
        if (this->state == 2) return undefined;
        if (anything > 0) return NaN(this->state);
        if (anything < 0) {
            if (this->state == 1) return NaN(0);
            return NaN(1);
        }
        if (anything == 0) return undefined;
    }

    template <typename T>
    friend NaN operator/ (const T& anything, const NaN& n) {
        return undefined;
    }

    template <typename T>
    NaN operator== (const T& anything) const {
        return undefined;
    }

    template <typename T>
    NaN operator!= (const T& anything) const {
        return undefined;
    }

    bool operator> (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return true;
        return false;
    }

    template <typename T>
    bool operator> (const T& anything) const {
        if (this->state == 2 or this->state == 0) return false;
        return true;
    }

    bool operator>= (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return true;
        return false;
    }

    template <typename T>
    bool operator>= (const T& anything) const {
        if (this->state == 2 or this->state == 0) return false;
        return true;
    }

    bool operator< (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return false;
        return true;
    }

    template <typename T>
    bool operator< (const T& anything) const {
        if (this->state == 2 or this->state == 0) return true;
        return false;
    }

    bool operator<= (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return false;
        return true;
    }

    template <typename T>
    bool operator<= (const T& anything) const {
        if (this->state == 2 or this->state == 0) return true;
        return false;
    }
};

NaN Infinity(const bool& sign = true) {
    if (sign) return NaN(1);
    return NaN(0);
}

NaN Undefined() {
    return undefined;
}

class Logger {
    unsigned int level = INFO;
    std::string name;
    std::string format = "%-24s %-12s %-12s %s";

public:
    explicit Logger(const std::string& alias) {
        this->name = alias;
    }

    void setLevel(const unsigned int& level_in) {
        this->level = level_in % 5;
    }

    void setFormat(const std::string& f) {
        this->format = f;
    }

    [[nodiscard]] unsigned int getLevel() const {
        return this->level;
    }

    [[nodiscard]] std::string getName() const {
        return this->name;
    }

    [[nodiscard]] std::string getFormat() const {
        return this->format;
    }

    void debug(const std::string& message) const {
        if (this->level <= DEBUG) {
            // The time formatter in this class is from stack overflow:
            // https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
            time_t     now = time(0);
            struct tm  tstruct;
            char       buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
            printf((BLUE + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "DEBUG", message.c_str());
        }
    }

    void info(const std::string& message) const {
        if (this->level <= INFO) {
            time_t     now = time(0);
            struct tm  tstruct;
            char       buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
            printf((GREEN + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "INFO", message.c_str());
        }
    }

    void warning(const std::string& message) const {
        if (this->level <= WARNING) {
            time_t     now = time(0);
            struct tm  tstruct;
            char       buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
            printf((YELLOW + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "WARNING", message.c_str());
        }
    }

    void critical(const std::string& message) const {
        if (this->level <= CRITICAL) {
            time_t     now = time(0);
            struct tm  tstruct;
            char       buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
            printf((RED + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "CRITICAL", message.c_str());
        }
    }

    void fatal(const std::string& message) const {
        if (this->level <= FATAL) {  // Which is always true in our case
            time_t     now = time(0);
            struct tm  tstruct;
            char       buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
            printf((RED + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "FATAL", message.c_str());
        }
    }
};

#endif //VECTOR_CPP_HELPERS_H
