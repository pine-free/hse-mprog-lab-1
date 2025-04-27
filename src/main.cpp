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

template <class T> class InsertionSort : Sort<T> {
public:
  static void sort(std::vector<T> &vec) {
    int j = 0;
    T val;
    for (int i = 0; i < vec.size(); ++i) {
      // NOTE: Здесь важно именно присваивать, а не просто брать из массива,
      // иначе всё ломается на этапе сдвига
      val = vec[i];
      for (j = i - 1; j >= 0 && vec[j] > val; --j) {
        // "Сдвигаем" элементы направо, освободжая "свободное место" под
        // вставляемый
        vec[j + 1] = vec[j];
      }
      vec[j + 1] = val;
    }
  }
};

template <class T> class HeapSort : Sort<T> {
  /*
  Пирамиду (кучу) определяем как бинарное дерево высоты k, где:
  - Все вершины имеют глубину k или k - 1 (сбалансированное)
  - Уровень k - 1 заполнен целиком, а уровень k -- слева направо
  - Каждый элемент меньше или равен родителю

  Сортировка производится в 2 фазы:
  1. Построение пирамиды
  2. Сама сортировка

  Построение пирамиды производится следующим образом:

  Хранить пирамиду предлагается в массиве. Для любого элемента с индексом i его
  детьми будут элементы с индексами 2i + 1 и 2i + 2

  Строить пирамиду предлагается "расширением":

  Если на индексах vec[i+1] .. vec[n] хранится пирамида,
  то мы можем расширить её до индекса vec[i] следующим образом:
  1. Выбираем наибольшего ребенка из vec[2i+1], vec[2i+2]
  2. Если наибольший ребенок больше vec[i] -- меняем их местами и повторяем
  процедуру, иначе останавливаем процедуру

  Когда пирамида была построена, остаётся всего лишь взять первый элемент
  массива (самый верхний в пирамиде), поменять местами с нижним (самым
  последним) и повторить процесс построения пирамиды, "забыв" о последнем
  (теперь самом большом) элементе
  */
private:
  static void buildHeap(std::vector<T> &vec, int root_idx, int pyramid_size) {
    /*
    Построение пирамиды
    */
    T new_elem = vec[root_idx];
    int child_idx;
    while (root_idx <= pyramid_size / 2) {
      child_idx = 2 * root_idx;
      if (child_idx < pyramid_size && vec[child_idx] < vec[child_idx + 1]) {
        ++child_idx;
      }

      if (new_elem >= vec[child_idx]) {
        break;
      }

      vec[root_idx] = vec[child_idx];
      root_idx = child_idx;
    }

    vec[root_idx] = new_elem;
  }

public:
  static void sort(std::vector<T> &vec) {
    for (int i = vec.size() / 2 - 1; i >= 0; --i) {
        buildHeap(vec, i, vec.size() - 1);
    }

    for (int i = vec.size() - 1; i > 0; --i) {
      std::swap(vec[i], vec[0]);
      buildHeap(vec, 0, i - 1);
    }
  }
};

int main() {
  auto nums = std::vector({1, 10, 4, 2, 1, 3, 4, 5, 3, 2, 1});
  HeapSort<int>::sort(nums);
  for (auto v : nums) {
    std::cout << v << " ";
  }
  std::cout << "\n";
  return 0;
}
