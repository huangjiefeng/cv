/************************************************************************/
/* 做这个类的本意是，因为STL等的智能指针使用起来要求较高，而这只要求    */
/* 实现退出作用域自动清理，为了更好的可控性，实现这个简单的类           */
/************************************************************************/

#ifndef CLASSIC_PTR_HELPER_H
#define CLASSIC_PTR_HELPER_H

//T must be a pointer type
template<typename T>
class ClassicPtrHelper
{
public:
  ClassicPtrHelper<T>(T *p)
    : ptr_(p)
  {
  }

  ~ClassicPtrHelper<T>()
  {
    if (ptr_ && *ptr_) delete (*ptr_);
  }

private:
  T *ptr_;
};

#endif
