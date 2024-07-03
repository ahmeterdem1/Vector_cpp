//
// Created by AHMET ERDEM on 5.05.2024.
//

#include "../include/helpers.h"
#include "../include/exceptions.h"
#include <ostream>
#include <cstdio>
#include <ctime>


template <typename T>
Range<T>::Range() = default;

template <typename T>
Range<T>::Range(const T &_last) {
    this->last = _last;
}

template <typename T>
Range<T>::Range(const T& _first, const T& _last) {
    if (_first > _last) throw RangeError();
    this->first = _first;
    this->last = _last;
}

template <typename T>
Range<T>::Range(const T& _first, const T& _last, const T& _step) {
    if (_first > _last) throw RangeError();
    this->first = _first;
    this->last = _last;
    this->step = _step;
}

template <typename T>
Range<T>::Iterator Range<T>::begin() {
    return Range<T>::Iterator(&(this->first), this->step);
}

template <typename T>
Range<T>::Iterator Range<T>::end() {
    return Range<T>::Iterator(&(this->last), this->step);
}

NaN::NaN() = default;

NaN::NaN(const unsigned int &s) {
    if (s == 0) this->state = 0;
    else if (s == 1) this->state = 1;
    else this->state = 2;
}

/*
std::ostream& operator<< (std::ostream& o, const NaN& n) {
    if (n.state == 0) o << "Infinity(-)";
    else if (n.state == 1) o << "Infinity(+)";
    else o << "Undefined";
    return o;
}
*/

NaN NaN::operator+ (const NaN& n) const {
    if (this->state == 2 or n.state == 2) return undefined;
    if (this->state ^ n.state) return undefined;
    return NaN(this->state);
}

template <typename T>
NaN NaN::operator+ (const T& anything) const {
    return NaN(this->state);
}

/*
template <typename T>
NaN operator+ (const T& anything, const NaN& n) {
    return NaN(n.state);
}
*/

NaN NaN::operator- (const NaN& n) const {
    if (this->state == 2 or n.state == 2) return undefined;
    if (!(this->state ^ n.state)) return undefined;
    return NaN(this->state);
}

template <typename T>
NaN NaN::operator- (const T& anything) const {
    return NaN(this->state);
}

/*
template <typename T>
NaN operator- (const T& anything, const NaN& n) {
    if (n.state == 2) return undefined;
    if (n.state == 1) return NaN(0);
    return NaN(1);
}
*/

NaN NaN::operator* (const NaN& n) const {
    if (this->state == 2 or n.state == 2) return undefined;
    return NaN(this->state and n.state);
}

template <typename T>
NaN NaN::operator* (const T& anything) const {
    if (this->state == 2) return undefined;
    if (anything > 0) return NaN(this->state);
    if (anything < 0) {
        if (this->state == 1) return NaN(0);
        return NaN(1);
    }
    if (anything == 0) return undefined;
}

/*
template <typename T>
NaN operator* (const T& anything, const NaN& n) {
    if (n.state == 2) return undefined;
    if (anything > 0) return NaN(n.state);
    if (anything < 0) {
        if (n.state == 1) return NaN(0);
        return NaN(1);
    }
    if (anything == 0) return undefined;
}
*/

NaN NaN::operator/ (const NaN& n) const {
    return undefined;
}

template <typename T>
NaN NaN::operator/ (const T& anything) const {
    if (this->state == 2) return undefined;
    if (anything > 0) return NaN(this->state);
    if (anything < 0) {
        if (this->state == 1) return NaN(0);
        return NaN(1);
    }
    if (anything == 0) return undefined;
}

/*
template <typename T>
NaN operator/ (const T& anything, const NaN& n) {
    return undefined;
}
*/

template <typename T>
NaN NaN::operator== (const T& anything) const {
    return undefined;
}

template <typename T>
NaN NaN::operator!= (const T& anything) const {
    return undefined;
}

bool NaN::operator> (const NaN& n) const {
    if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
    if (this->state) return true;
    return false;
}

template <typename T>
bool NaN::operator> (const T& anything) const {
    if (this->state == 2 or this->state == 0) return false;
    return true;
}

bool NaN::operator>= (const NaN& n) const {
    if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
    if (this->state) return true;
    return false;
}

template <typename T>
bool NaN::operator>= (const T& anything) const {
    if (this->state == 2 or this->state == 0) return false;
    return true;
}

bool NaN::operator< (const NaN& n) const {
    if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
    if (this->state) return false;
    return true;
}

template <typename T>
bool NaN::operator< (const T& anything) const {
    if (this->state == 2 or this->state == 0) return true;
    return false;
}

bool NaN::operator<= (const NaN& n) const {
    if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
    if (this->state) return false;
    return true;
}

template <typename T>
bool NaN::operator<= (const T& anything) const {
    if (this->state == 2 or this->state == 0) return true;
    return false;
}

Logger::Logger(const std::string &alias) {
    this->name = alias;
}

void Logger::setLevel(const unsigned int& level_in) {
    this->level = level_in % 5;
}

void Logger::setFormat(const std::string& f) {
    this->format = f;
}

unsigned int Logger::getLevel() const {
    return this->level;
}

std::string Logger::getName() const {
    return this->name;
}

std::string Logger::getFormat() const {
    return this->format;
}

void Logger::debug(const std::string& message) const {
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

void Logger::info(const std::string& message) const {
    if (this->level <= INFO) {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        printf((GREEN + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "INFO", message.c_str());
    }
}

void Logger::warning(const std::string& message) const {
    if (this->level <= WARNING) {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        printf((YELLOW + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "WARNING", message.c_str());
    }
}

void Logger::critical(const std::string& message) const {
    if (this->level <= CRITICAL) {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        printf((RED + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "CRITICAL", message.c_str());
    }
}

void Logger::fatal(const std::string& message) const {
    if (this->level <= FATAL) {  // Which is always true in our case
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        printf((RED + this->format + RESET + "\n").c_str(), buf, this->name.c_str(), "FATAL", message.c_str());
    }
}

template class Range<double>;
