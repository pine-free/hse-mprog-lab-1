#include <ctime>
#include <iostream>
#include <istream>
#include <string>

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

  friend std::istream& operator>>(std::istream& is, Passenger& p) {
    is >> p.flight_date >> p.flight_num >> p.name >> p.seat_num;
    return is;
  }
};

int main() { return 0; }
