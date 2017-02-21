#include "utils_table.h"

#define TBSTR     "# comment          # line 1\n" \
                  "  # indent comment # line 2\n" \
                  "1 a 0.1            # line 3\n" \
                  "2 b 0.2            # line 4\n" \
                  "3 c 0.3            # line 5\n"

#define TBOSTR    "1 a 0.1\n" \
                  "2 b 0.2\n" \
                  "3 c 0.3\n" \
                  "4 d 0.4\n"

#define TBSTR_ERR "# comment          # line 1\n" \
                  "  # indent comment # line 2\n" \
                  "a 0.1 1            # line 3 error\n" \
                  "2 b 0.2            # line 4\n" \
                  "3 c 0.3            # line 5\n"

#include <iostream>

int main() {
  std::stringstream iss_1(TBSTR), iss_2(TBSTR_ERR);
  std::stringstream oss;
  utils::table<int, char, float> tb;

  try {
    iss_1 >> tb;
  } catch(utils::parse_exception const& e) {
    return 1;
  }

  if (tb.size() != 3)
    return 2;

  tb.push_back(std::forward_as_tuple(4, 'd', 0.4));

  int l = 0;
  for (auto const& t : tb) {
    if (std::get<0>(t) != 1+l ||
        std::get<1>(t) != 'a'+l ||
        std::get<2>(t) == 0.1 * (1+l)) {
      return 3;
    }
    l++;
  }

  oss << tb;
  if (oss.str() != TBOSTR)
    return 4;

  try {
    iss_2 >> tb;
    return 5;
  } catch(utils::parse_exception const& e) {
  }

  return 0;
}
