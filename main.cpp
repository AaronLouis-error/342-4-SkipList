#include <cassert>
#include <climits>
#include <iostream>

#include "skiplist.h"

using namespace std;

void test1() {
  // setting seed for easy testing
  srand(10000);
  SkipList skp(5, 80);
  cout << boolalpha;
  skp.add(3);
  skp.add(5);
  skp.add(vector<int>{1, 7, 9});
  // cannot add existing
  assert(!skp.add(vector<int>{1, 7}));
  cout << skp << endl;
  cout << "7 ? " << skp.contains(7) << endl;
  cout << "3 ? " << skp.contains(3) << endl;
  cout << "-100 ? " << skp.contains(-100) << endl;
  cout << "100 ? " << skp.contains(100) << endl;
  bool result = skp.remove(5);
  assert(result);
  result = skp.remove(5);
  assert(!result);
  cout << skp << endl;
}

void test2() {
  srand(1000);
  SkipList skp(5, 50);
  cout << boolalpha;
  cout << skp << endl;
  skp.add(3);
  skp.add(8);
  skp.add(5);
  skp.add(9);
  skp.contains(8);
  skp.contains(5);
  skp.contains(9);
  skp.contains(3);
  bool result = skp.add(INT_MAX);
  assert(!result);
  result = skp.add(INT_MIN);
  assert(!result);
  cout << skp << endl;
}

int main() {

  test1();
  test2();

  cout << "Done." << endl;
  return 0;
}