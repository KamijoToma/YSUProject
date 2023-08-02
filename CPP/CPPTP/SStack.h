//
// Created by SkyRain on 2023/05/03.
//

#ifndef CXXTP_SSTACK_H
#define CXXTP_SSTACK_H
#include "SList.h"
namespace CXXTP {
template<typename T>
class SStack : private SList<T> {
public:
  /**
   * If it is empty
   * @return is empty
   */
  bool empty() const { return SList<T>::empty(); }
  /**
   * Return size
   * @return size
   */
  size_t size() const { return SList<T>::getSize(); }
  /**
   * Return top value
   * @return top value
   */
  T& top() { return SList<T>::back(); }
  /**
   * Return top value constantly
   * @return constant top value
   */
  const T& top() const { return SList<T>::back(); }
  /**
   * Push in top value
   * @param value value
   */
  void push(const T &value) { SList<T>::push_back(value); }
  /**
   * Pop out value
   */
  void pop() { SList<T>::pop_back(); }
};

}
#endif // CXXTP_SSTACK_H
