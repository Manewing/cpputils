#include "utils_tuple.h"

#include <iostream>

#define TPSTR     "1 c 0.5"
#define TPSTR_ERR "c 0.5 1"

int main(int argc, char* argv[]) {
  std::istringstream iss_1(TPSTR);
  std::istringstream iss_2(TPSTR_ERR);
  std::ostringstream oss;
  std::tuple<int, char, float> t;

  try {
    iss_1 >> t;
  } catch(std::istream::failure const& f) {
    return 1;
  }

  if (std::get<0>(t) != 1 ||
      std::get<1>(t) != 'c' ||
      std::get<2>(t) != 0.5) {
    return 2;
  }

  oss << t;
  if (oss.str() != TPSTR)
    return 3;

  try {
    // TODO check the issue with setting at this level iss_2.exceptions()
    t = utils::typesafe_read_tuple<int, char, float>(iss_2);
  } catch(std::istream::failure const& f) {
    std::cout << "error" << std::endl;
  }

  return 0;
}
