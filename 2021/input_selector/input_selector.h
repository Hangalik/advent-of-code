#ifndef INPUT_SELECTOR_H_
#define INPUT_SELECTOR_H_

#include <functional>

int runWithProperInput(int argc, const char* argv[], std::function<int(std::istream&)>);

#endif