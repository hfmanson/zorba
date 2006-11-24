/* -*- mode: c++; indent-tabs-mode: nil -*-
 *
 *  $Id: rchandle.h,v 1.1 2006/10/09 07:08:02 Paul Pedersen Exp $
 *
 *  Copyright 2005-2006 FLWOR Foundation.  All Rights Reserved.
 *
 */

#ifndef XQP_RCHANDLE_H
#define XQP_RCHANDLE_H

#include <string>
#include <sstream>

namespace xqp {

/*__________________________________________________________________________  
|                                                                           |  
|  Base class for reference counted objects                                 |
|                                                                           |  
|  Any class wishing to take advantage of automatic reference counting      |
|  must inherit from this class.                                            |
|                                                                           |  
|  rcobject encapsulates the reference count, as well as functions          |
|  for incrementing and decrementing the count. It also contains code       |
|  for destroying a value when it is no longer in use, i.e., when its       |
|  reference count becomes 0.  Finally, it contains a field that keeps      |
|  track of whether this value is shareable, and it provides functions      |
|  to query this value and set it to false.  There is no need for a         |
|  function to set the shareability field to true, because all values       |
|  are shareable by default.  Once an object has been tagged unshareable,   |
|  there is no way to make it shareable again.                              |
|___________________________________________________________________________|*/

class rcobject
{
private:
  int refCount;
  bool shareable;

public:	// ctor,dtor
  rcobject();
  rcobject(const rcobject& rhs);
  virtual ~rcobject() = 0;

public:	// refcounting
  void addReference()			 { ++refCount; }
  void removeReference()	 { if (--refCount == 0) delete this; }
  bool isShared() const		 { return refCount > 1; }

public:	// sharing
  void markUnshareable()	 { shareable = false; }
  bool isShareable() const { return shareable; }
	int get_refCount() const { return refCount; }

public:	// operator overloading
	rcobject& operator=(const rcobject&) { return *this; }

};


inline rcobject::rcobject()
:	refCount(0),
	shareable(true)
{}


inline rcobject::rcobject(const rcobject& o)
:	refCount(0),
	shareable(true) 
{}


// Virtual dtors must always be implemented, 
// even if they are pure virtual and do nothing.
inline rcobject::~rcobject() {}



/*__________________________________________________________________________  
|                                                                           |
|  rchandle = r(ef)c(ount)handle                                            |
|                                                                           |
|  Template class for smart pointers-to-T objects. T must support the       |
|  rcobject interface, typically by inheriting from rcobject								| 
|___________________________________________________________________________|*/

template<class T>
class rchandle
{
private:
  T *p;        // dumb pointer this object is emulating
  void init(); // common initialization

public:	// ctor, dtor
  rchandle(T* realPtr = 0);
  rchandle(const rchandle& rhs);
  ~rchandle();

public:	// operator overloading
  rchandle& operator=(const rchandle& rhs);
  T* operator->() const; 
  T& operator*() const;  
	bool operator==(const rchandle& h) const;
	bool operator!=(const rchandle& h) const;
	bool operator==(const T* pp) const;
	bool operator!=(const T* pp) const;

public:
	std::string debug() const;

}; 


template<class T> inline rchandle<T>::rchandle(T* realPtr) : p(realPtr) { init(); }
template<class T> inline rchandle<T>::rchandle(const rchandle& rhs) : p(rhs.p) { init(); }
template<class T> inline rchandle<T>::~rchandle() { if (p) p->removeReference(); } 
template<class T> inline T* rchandle<T>::operator->() const { return p; } 
template<class T> inline T& rchandle<T>::operator*() const { return *p; } 
template<class T> inline bool rchandle<T>::operator==(const rchandle& h) const { return p==h.p; }
template<class T> inline bool rchandle<T>::operator!=(const rchandle& h) const { return p!=h.p; }
template<class T> inline bool rchandle<T>::operator==(const T* pp) const { return p==pp; } 
template<class T> inline bool rchandle<T>::operator!=(const T* pp) const { return p!=pp; }


template<class T>
inline void rchandle<T>::init()
{
  if (p == 0) return;
  if (p->isShareable() == false) {
		p = new T(*p);
  }
  p->addReference();
}


template<class T>
inline rchandle<T>& rchandle<T>::operator=(const rchandle& rhs)
{
  if (p != rhs.p) {
    if (p) p->removeReference();
    p = rhs.p;
    init();
  }
  return *this;
}


template<class T>
inline std::string rchandle<T>::debug() const
{
	std::ostringstream oss;
	oss << "rchandle[refcount="<<p->get_refCount()<<",shareable="<<p->isShareable()<<']';
	return oss.str();
}


/*__________________________________________________________________________  
|                                                                           |           
|  rcihandle = r(ef)c(ount)i(ndirect)handle                                 |
|                                                                           |
|  rcihandle differs from rchandle in 2 ways:                               |
|                                                                           |
|    (1) rchandle objects point to values directly, while rcihandle objects |
|        point to values through intervening 'countholder' objects.         |
|                                                                           |
|    (2) rcihandle overloads operator-> and operator* so that a             |
|        copy-on-write is automatically performed whenever a non-const      |
|        access is made to a pointed-to object.                             |
|___________________________________________________________________________|*/

template<class T>
class rcihandle
{
private:
  struct CountHolder: public rcobject
	{
    ~CountHolder() { delete p; }
    T *p;
  };

  CountHolder *counter;
  void init();
  void makeCopy(); 

public:	// ctor, dtor
  rcihandle(T* realPtr = 0);
  rcihandle(const rcihandle& rhs);
  ~rcihandle();

public:	// operator overloading
  rcihandle& operator=(const rcihandle& rhs);
  const T* operator->() const; 
  T* operator->();            
  const T& operator*() const;
  T& operator*();           

	bool operator==(const rcihandle& h) const { return counter->p==h.counter->p; }
	bool operator!=(const rcihandle& h) const { return counter->p!=h.counter->p; }
	bool operator==(const T* pp) const { return counter->p==pp; }
	bool operator!=(const T* pp) const { return counter->p!=pp; }

};


template<class T>
inline void rcihandle<T>::init()
{
	if (counter->isShareable() == false) {
		T *oldValue = counter->p;
		counter = new CountHolder;
		counter->p = new T(*oldValue);
	}
  counter->addReference();
}


template<class T>
inline rcihandle<T>::rcihandle(T* realPtr)
: counter(new CountHolder)
{
  counter->p = realPtr;
  init();
}


template<class T>
inline rcihandle<T>::rcihandle(const rcihandle& rhs)
: counter(rhs.counter)
{ init(); }


template<class T>
inline rcihandle<T>::~rcihandle()
{ counter->removeReference(); }


template<class T>
inline rcihandle<T>& rcihandle<T>::operator=(const rcihandle& rhs)
{
  if (counter != rhs.counter) {
    counter->removeReference();
    counter = rhs.counter;
    init();
  }
  return *this;
}


template<class T>																	// implement the copy part
inline void rcihandle<T>::makeCopy()							// of copy-on-write (conw)
{ 
  if (counter->isShared()) {
    T *oldValue = counter->p;
    counter->removeReference();
    counter = new CountHolder;
    counter->p = new T(*oldValue);
    counter->addReference();
  }
}


template<class T>																	// const access,
inline const T* rcihandle<T>::operator->() const	// no conw needed
{ return counter->p; }


template<class T>																	// non-const access,
inline T* rcihandle<T>::operator->()							// conw needed
{ makeCopy(); return counter->p; } 


template<class T>																	// const access,
inline const T& rcihandle<T>::operator*() const		// no conw needed
{ return *(counter->p); }


template<class T>																	// non-const access,
inline T& rcihandle<T>::operator*()								// conw needed
{ makeCopy(); return *(counter->p); } 


}	/* namespace xqp */
#endif	/* XQP_RCHANDLE_H */

