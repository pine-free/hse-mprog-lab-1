#include <ctime>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

struct Passenger {
  time_t flight_date;
  int flight_num;
  std::string name;
  int seat_num;

  friend bool operator<(const Passenger &l, const Passenger &r) {
    return std::tie(l.flight_date, l.flight_num, l.name, l.seat_num) <
           std::tie(r.flight_date, r.flight_num, r.name, r.seat_num);
  }

  friend inline bool operator>(const Passenger &lhs, const Passenger &rhs) {
    return rhs < lhs;
  }
  friend inline bool operator<=(const Passenger &lhs, const Passenger &rhs) {
    return !(lhs > rhs);
  }
  friend inline bool operator>=(const Passenger &lhs, const Passenger &rhs) {
    return !(lhs < rhs);
  }

  friend std::istream &operator>>(std::istream &is, Passenger &p) {
    is >> p.flight_date >> p.flight_num >> p.name >> p.seat_num;
    return is;
  }
};

template <class T> class Sort {
public:
  static void sort(std::vector<T> &vec);
};

template <class T> class BubbleSort : Sort<T> {
public:
  static void sort(std::vector<T> &vec) {
    for (int i = 0; i < vec.size(); ++i) {
      for (int j = vec.size() - 1; j > i; --j) {
        if (vec[j - 1] > vec[j]) {
          std::swap(vec[j - 1], vec[j]);
        }
      }
    }
  }
};

int main() {
  auto nums = std::vector({1, 10, 4, 2, 1, 3, 4, 5, 3, 2, 1});
  BubbleSort<int>::sort(nums);
  for (auto v: nums) {
    std::cout << v << " ";
  }
  std::cout << "\n";
  return 0;
}
