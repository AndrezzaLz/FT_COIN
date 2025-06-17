#ifndef RECORDNOTFOUNDEXCEPTION_HPP
#define RECORDNOTFOUNDEXCEPTION_HPP

#include <stdexcept>
#include <string>

// ADDED: A custom exception class in English.
class RecordNotFoundException : public std::runtime_error {
public:
    explicit RecordNotFoundException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif
