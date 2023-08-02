//
// Created by SkyRain on 2023/05/03.
//

#ifndef CXXTP_SSET_H
#define CXXTP_SSET_H
#include "SList.h"
namespace CXXTP {
template <typename T>
class SSet : public SList<T> {
public:
  SSet() = default;

  // 求集合差
  SSet<T> operator- (const SSet<T>& rhs) const {
    SSet<T> result;
    for(int i=0; i<getSize(); i++){
      if(!rhs.contains(this->operator[](i))){
        result.push_back(this->operator[](i));
      }
    }
    return result;
  }

  // 求集合并
  SSet<T> operator+ (const SSet<T>& rhs) const {
    SSet<T> result = *this;
    for(int i=0; i<rhs.getSize(); i++){
      if(!result.contains(rhs[i])){
        result.push_back(rhs[i]);
      }
    }
    return result;
  }

  // 求集合交
  SSet<T> and_(const SSet<T>& rhs) const {
    SSet<T> result;
    for(int i=0; i<getSize(); i++){
      if(rhs.contains(this->operator[](i))){
        result.push_back(this->operator[](i));
      }
    }
    return result;
  }

  // 判断元素是否在集合中
  bool contains(const T& item) const {
    for(int i=0; i<getSize(); i++){
      if(item == this->operator[](i)){
        return true;
      }
    }
    return false;
  }
};

}
#endif // CXXTP_SSET_H
