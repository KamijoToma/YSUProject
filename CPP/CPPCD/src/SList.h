//
// Created by SkyRain on 2023/05/03.
//

#ifndef SLIST_H
#define SLIST_H
#include <cstddef>
#include <stdexcept>

template<typename T>
class SList {

private:
  /**
   * List node presentation.
   */
  struct Node {
    /**
     * Inner data
     */
    T data;
    /**
     * Next node pointer. Default nulptr
     */
    Node* next;
    /**
     * Previous node pointer. Default nulptr
     */
    Node* prev;
    explicit Node(const T& d = T(), Node* n = nullptr, Node* p = nullptr) : data(d), next(n), prev(p) {}
  };

  /**
   * First node
   */
  Node* head;
  /**
   * Last node
   */
  Node* tail;
  /**
   * List size
   */
  std::size_t size;

public:
  /**
   * Default constructor
   */
  SList() : head(nullptr), tail(nullptr), size(0) {}
  /**
   * Copy constructor
   * @param other other list
   */
  SList(const SList & other);
  /**
   * Equal constructor
   * @param other Other list
   * @return list reference
   */
  SList & operator=(const SList & other);
  /**
   * Destructor
   */
  ~SList();

  /**
   * Add a value to its head
   * @param val value
   */
  void push_front(const T& val);
  /**
   * Add a value to its tail
   * @param val value
   */
  void push_back(const T& val);
  /**
   * Remove its front value.
   * If it does not have, do nothing.
   */
  void pop_front();
  /**
   * Remove its tail value.
   * If it does not have, do nothing.
   */
  void pop_back();
  /**
   * Clear list. Destruct all node pointer
   */
  void clear();
  /**
   * Indicate if the list is empty
   * @return bool value
   */
  [[nodiscard]] bool empty() const;
  /**
   * Return its size
   * @return size
   */
  [[nodiscard]] std::size_t getSize() const;
  /**
   * Get its front value
   * @return front value reference
   */
  T& front();
  /**
   * Get its front value constantly
   * @return constant front value reference
   */
  const T& front() const;
  /**
   * Get its tail value
   * @return tail value reference
   */
  T& back();
  /**
   * Get its tail value constantly
   * @return constant tail value reference
   */
  const T& back() const;
  /**
   * Insert a value
   * @param val value
   * @param pos position
   */
  void insert(const T& val, std::size_t pos);
  /**
   * Delete a value
   * @param pos position
   */
  void erase(std::size_t pos);
  /**
   * Index operator
   * @param i position
   * @return value
   */
  T& operator[](const size_t i) const;
};

template <typename T> T &SList<T>::operator[](const size_t i) const {
  if(i >= size){
    throw std::out_of_range("List out of range");
  }
  auto curr = head;
  for(int j=0; j<i; j++){
    curr = curr->next;
  }
  return curr->data;
}

template<typename T> SList<T>::SList(const SList<T>& other) {
  head = tail = nullptr;
  size = 0;
  for (Node* it = other.head; it != nullptr; it = it->next) {
    push_back(it->data);
  }
}

template<typename T> SList<T>&SList<T>::operator=(const SList<T>& other) {
  if (this != &other) {
    clear();
    for (Node* it = other.head; it != nullptr; it = it->next) {
      push_back(it->data);
    }
  }
  return *this;
}

template<typename T> SList<T>::~SList() {
  clear();
}

template<typename T>
void SList<T>::push_front(const T& val) {
  Node* newNode = new Node(val, head, nullptr);
  if (head != nullptr) {
    head->prev = newNode;
  } else {
    tail = newNode;
  }
  head = newNode;
  ++size;
}

template<typename T>
void SList<T>::push_back(const T& val) {
  Node* newNode = new Node(val, nullptr, tail);
  if (tail != nullptr) {
    tail->next = newNode;
  } else {
    head = newNode;
  }
  tail = newNode;
  ++size;
}

template<typename T>
void SList<T>::pop_front() {
  if (head != nullptr) {
    Node* tmp = head;
    head = head->next;
    if (head != nullptr) {
      head->prev = nullptr;
    } else {
      tail = nullptr;
    }
    delete tmp;
    --size;
  }
}

template<typename T>
void SList<T>::pop_back() {
  if (tail != nullptr) {
    Node* tmp = tail;
    tail = tail->prev;
    if (tail != nullptr) {
      tail->next = nullptr;
    } else {
      head = nullptr;
    }
    delete tmp;
    --size;
  }
}

template<typename T>
void SList<T>::clear() {
  while (head != nullptr) {
    Node* tmp = head;
    head = head->next;
    delete tmp;
  }
  tail = nullptr;
  size = 0;
}

template<typename T>
bool SList<T>::empty() const{
  return size == 0;
}

template<typename T>
std::size_t SList<T>::getSize() const {
  return size;
}

template<typename T>
T&SList<T>::front() {
  return head->data;
}

template<typename T>
const T&SList<T>::front() const {
  return head->data;
}

template<typename T>
T&SList<T>::back() {
  return tail->data;
}

template<typename T>
const T&SList<T>::back() const {
  return tail->data;
}

template<typename T>
void SList<T>::insert(const T& val, std::size_t pos) {
  if (pos > size) {
    return;
  }
  if (pos == 0) {
    push_front(val);
  } else if (pos == size) {
    push_back(val);
  } else {
    Node* it = head;
    for (std::size_t i = 0; i < pos; ++i) {
      it = it->next;
    }
    Node* newNode = new Node(val, it, it->prev);
    it->prev->next = newNode;
    it->prev = newNode;
    ++size;
  }
}

template<typename T>
void SList<T>::erase(std::size_t pos) {
  if (pos >= size) {
    return;
  }
  if (pos == 0) {
    pop_front();
  } else if (pos == size - 1) {
    pop_back();
  } else {
    Node* it = head;
    for (std::size_t i = 0; i < pos; ++i) {
      it = it->next;
    }
    it->prev->next = it->next;
    it->next->prev = it->prev;
    delete it;
    --size;
  }
}
#endif
