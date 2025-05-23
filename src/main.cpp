/// @file main.cpp
/// @brief Основной файл с реализацией сортировок

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <istream>
#include <ratio>
#include <string>
#include <thread>
#include <vector>

struct Passenger {
  std::string flight_date;
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
    is >> p.flight_num >> p.flight_date >> p.name >> p.seat_num;
    return is;
  }

  friend std::ostream &operator<<(std::ostream &os, const Passenger &p) {
    os << p.flight_num << " " << p.flight_date << " " << p.name << " "
       << p.seat_num;
    return os;
  }
};

template <class T> class Sort {
public:
  static void sort(std::vector<T> &vec);
  static std::string classname() { return "Generic Sort"; }
};

/**
Идея заключается в том, чтобы сортировать по шагам. На каждом шаге проходимся
"снизу" (начало массива) "вверх" (конец массива) и попарно сравниваем соседние
элементы. Если элемент "ниже" больше "верхнего", то меняем их местами.
Повторяем эту процедуру N раз, где N -- размер массива.

На каждом шаге бОльшие элементы будут "всплывать" вверх как пузырьки, hence
the name
*/
template <class T> class BubbleSort : public Sort<T> {
public:
  static void sort(std::vector<T> &vec) {
    bool sorted = true;
    for (int i = 0; i < vec.size(); ++i) {
      for (int j = vec.size() - 1; j > i; --j) {
        if (vec[j - 1] > vec[j]) {
          sorted = false;
          std::swap(vec[j - 1], vec[j]);
        }
      }

      if (sorted) {
        break;
      }
    }
  }
  static std::string classname() { return "Bubble Sort"; }
};

/**
Каждый шаг сортировки мы сравниваем i-ый элемент с предшествующими ему (уже
отсортированными). Как только мы находим элемент, который больше i-го -- они
меняются местами и процесс повторяется. Таким образом в начале массива у нас
потихоньку растёт "отсортированная" часть.
*/
template <class T> class InsertionSort : public Sort<T> {
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
  static std::string classname() { return "Insertion Sort"; }
};

/**
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
template <class T> class HeapSort : public Sort<T> {
private:
  static void buildHeap(std::vector<T> &vec, int root_idx, int pyramid_size) {
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
  static std::string classname() { return "Heap Sort"; }
};

template <class T> class FileReader {
public:
  static std::vector<T> read(std::istream &is, int size) {
    auto res = std::vector<T>();
    for (int i = 0; i < size; ++i) {
      T el;
      is >> el;
      res.push_back(el);
    }
    return res;
  }
};

template <class T, class S> void time_sort(std::vector<T> vec) {
  auto t1 = std::chrono::high_resolution_clock::now();
  S::sort(vec);
  auto t2 = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> ms_double = t2 - t1;
  std::cout << S::classname() << "\t" << vec.size() << "\t" << ms_double.count()
            << "ms\n";
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "ERROR: Must have exactly 2 arguments -- name of file to read "
                 "and n of entries"
              << std::endl;
    return 0;
  }

  std::string fpath = argv[1];
  int nrecs = std::stoi(argv[2]);

  std::fstream passengers_file(fpath);
  auto res = FileReader<Passenger>::read(passengers_file, nrecs);
  std::thread bubble(time_sort<Passenger, BubbleSort<Passenger>>, res);
  std::thread insertion(time_sort<Passenger, InsertionSort<Passenger>>, res);
  std::thread heap(time_sort<Passenger, HeapSort<Passenger>>, res);
  std::thread standard([&res, nrecs]() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::sort(res.begin(), res.end());
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "Standart sort" << "\t" << nrecs << "\t" << ms_double.count()
              << "ms\n";
  });

  bubble.join();
  insertion.join();
  heap.join();
  standard.join();

  return 0;
}
