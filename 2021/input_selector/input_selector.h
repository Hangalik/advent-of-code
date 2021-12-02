#ifndef INPUT_SELECTOR_H_
#define INPUT_SELECTOR_H_

#include <functional>
#include <istream>

using TestFunction = std::function<int(std::istream&)>;

int runWithProperInput(int argc, const char* argv[], TestFunction func1, TestFunction func2);

#endif