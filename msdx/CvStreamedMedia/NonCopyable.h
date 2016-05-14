////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file NonCopyable.h
/// @brief 定义一个不可复制的类，单件类可以继承该类
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RW_NONCOPYABLE_H
#define _RW_NONCOPYABLE_H

/**
 * Ensures derived classes have private copy constructor and copy assignment.
 *
 * Example:
 * <pre>
 * class MyClass : NonCopyable {
 * public:
 *    ...
 * };
 * </pre>
 *
 * Taken from Boost library.
 *
 * @see boost::noncopyable
 * @author Tanguy Krotoff
 */
class NonCopyable
{
protected:

    NonCopyable() {}

    ~NonCopyable() {}

private:

    /**
     * Copy constructor is private.
     */
    NonCopyable(const NonCopyable &);

    /**
     * Copy assignement is private.
     */
    const NonCopyable & operator=(const NonCopyable &);
};

#endif