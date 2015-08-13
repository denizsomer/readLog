
#ifndef incl_MathVector_h
#define incl_MathVector_h


#include <cmath>

#include "ContainerBase.h"
#include "LinkedListBase.h"
#include "MathBasic.h"



//  vector classes
//
//  VectorBase     - abstract base class
//  Vector         - vector based on a linked list
//  VectorArray    - vector based on an array
//  VectorFixed    - vector based on an array of fixed length
//  VectorInfinite - vector based on an infinite linked list


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// vector coefficient class needed for Vector

template<typename Type> class VectorCoeff: public ListItem
{
  public:
  
    // implicit copy constructor
    // implicit destructor
    // implicit default assignemt operator

    VectorCoeff(void) { val = (Type) 0; }

    define_generateCopy(VectorCoeff<Type>)

    Type val;

    template<class Type2> bool operator>(Type2 &b) { return val > b.val; }

    template<class Type2> bool operator<(Type2 &b) { return val < b.val; }
};


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// abstract vector base class

template<typename Type> class VectorBase: public virtual ContainerBase, public ListItem
{
  public:

    // implicit default constructor
    // implicit copy constructor
    // implicit destructor
    // implicit default assignment operator

    virtual Type &operator[](int) = 0;

    virtual Type &operator()(int);

    template<class Type2> bool operator==(Type2 &);
    
    virtual Type &firstCoeff(void);

    virtual Type &lastCoeff(void);

    virtual void print(std::ostream &os = std::cout);
    
    virtual void zero(void);

    virtual Type norm(void);

    virtual bool contains(Type, int *indx = NULL);

    template<class Type2> bool containsAllOf(Type2 &);

    template<class Type2> bool containsTheSameAs(Type2 &);

    virtual bool containsAllOf(Type*, int);

    template<class Type2> bool containsAtLeastOneOf(Type2 &, int *indx = NULL, int *indx2 = NULL);

    template<class Type2> bool containsSomeOf(int, Type2 &);

    virtual bool distinct(void);

    virtual void permute(int *, int *flagPtr = NULL);
};


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// vector based on a linked list

template<typename Type> class Vector: public VectorBase<Type>, 
                                      public LinkedListBase< VectorCoeff<Type> >
{
  public:

    // implicit default constructor
    // implicit copy constructor
    // implicit desctructor
    // implicit default assignment operator

    define_generateCopy(Vector<Type>)

    Type &operator[](int);
       
    template<class Type2> Vector<Type> &operator=(Type2 &);

    Type* append(Type val = (Type) 0);

    void insert(Type,int);

    void del(int);

    Type &firstCoeff(void);

    Type &lastCoeff(void);
};


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// vector based on an array

template<typename Type> class VectorArray: public VectorBase<Type>
{
  public:

    VectorArray(void) { x = NULL; }

    VectorArray(VectorArray<Type> &that): VectorBase<Type>(that) { *this = that; }
       
    virtual ~VectorArray() { this->free(); }

    define_generateCopy(VectorArray<Type>) 

    virtual Type &operator[](int);       
    
    VectorArray<Type> &operator=(VectorArray<Type> &); // default assignment operator

    template<class Type2> VectorArray<Type> &operator=(Type2 &);

    void setDim(int);
    
    virtual void free(void);

    virtual void expand(int);

    Type *x;
};

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// simple vector based on array of fixed length

template<typename Type, int dimension> class VectorFixed: public VectorBase<Type>
{
  public:

    // implicit copy constructor
    // implicit destructur
    // implicit default assignment operator

    VectorFixed(void) { this->n = dimension; }

    VectorFixed<Type,dimension> *generateCopy(void) 
      { return new VectorFixed<Type,dimension>(*this); }

    template<class Type2> VectorFixed<Type,dimension> &operator=(Type2 &b)
      { if (this->n == b.n) for (int i=0; i<this->n; i++) x[i] = b[i]; return *this; }

    virtual Type &operator[](int i) { return x[i]; }

    Type x[dimension];
};


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// vector based on an infinite linked list

template<typename Type> class VectorInfinite: public Vector<Type>
{
  public:

    // implicit default constructor
    // implicit copy constructor
    // implicit destructur
    // implicit default assignment operator

    define_generateCopy(VectorInfinite<Type>)

    template<class Type2> VectorInfinite<Type> &operator=(Type2 &b)
      { this->Vector<Type>::operator=(b); return *this; }

    virtual Type &operator[](int i) 
      { while (i > this->n-1) this->append((Type) 0);
        return LinkedListBase< VectorCoeff<Type> >::getItem(i).val; }
};


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


template<typename Type> Type &VectorBase<Type>::operator()(int i)
{
  return (*this)[i-1];
}



template<typename Type> Type &VectorBase<Type>::firstCoeff(void)
{
  return (*this)[0];
}




template<typename Type> Type &VectorBase<Type>::lastCoeff(void)
{
  return (*this)[this->n-1];
}




template<typename Type> void VectorBase<Type>::print(std::ostream &os)
{
  os << "{"; 
  if (this->n > 0) os << (*this)[0]; else os << " ";
  for (int i=1; i<this->n; i++) os << "," << (*this)[i]; 
  os << "}";
  return;
}




template<typename Type> void VectorBase<Type>::zero(void)
{
  for (int i=0; i<this->n; i++) (*this)[i] = (Type) 0;
  return;
}




template<typename Type> Type VectorBase<Type>::norm(void)
{
  std::cout << " Warning! norm only works for Vector<double> or Vector<int>!\n\n";
      
  return (Type) 0;
}




template<> inline double VectorBase<double>::norm(void)
{
  double y = 0.;

  for (int i=0; i<this->n; i++) y += (*this)[i] * (*this)[i];

  return sqrt(y);
}




template<> inline int VectorBase<int>::norm(void)
{
  int y = 0;

  for (int i=0; i<this->n; i++) y += (*this)[i] * (*this)[i]; 

  return roundToInt(sqrt((double)y));
}




template<> inline unsigned int VectorBase<unsigned int>::norm(void)
{
  unsigned int y = 0;

  for (int i=0; i<this->n; i++) y += (*this)[i] * (*this)[i]; 

  return (unsigned int) roundToInt(sqrt((double)y));
}




template<typename Type> bool VectorBase<Type>::contains(Type xx, int *indx)
{
  int i = 0; while (i < this->n) if ((*this)[i] == xx) break; else i++;
  if (i < this->n)
  {
    if (indx != NULL) *indx = i;
    return true;
  }
  if (indx != NULL) *indx = -1;
  return false;
}




template<typename Type> template<class Type2> bool VectorBase<Type>::containsAllOf(Type2 &vec2)
{
  for (int i=0; i<vec2.n; i++)  if (!this->contains(vec2[i])) return false;

  return true;
}




template<typename Type> template<class Type2> bool VectorBase<Type>::containsTheSameAs(Type2 &vec2)
{
  int i;

  if (this->n != vec2.n) return false;

  // returns false as soon as one element of myself is not found in vec2  

  for (int i=0; i<vec2.n; i++)  if (!this->contains(vec2[i])) return false;

  return true;
}




template<typename Type> bool VectorBase<Type>::containsAllOf(Type *a, int m)
{
  for (int i=0; i<m; i++)  if (!this->contains(a[i])) return false;

  return true;
}




template<typename Type> template<class Type2> 
   bool VectorBase<Type>::containsAtLeastOneOf(Type2 &vec2, int *indx, int *indx2)
{
  int i = 0, j;
  while (i < this->n)
  {
    j = 0;
    while (j < vec2.n && (*this)[i] != vec2[j]) j++;
    if (j < vec2.n) 
    {
      if (indx  != NULL) *indx  = i;
      if (indx2 != NULL) *indx2 = j;
      return true;
    }
    i++;
  }
  return false;
}





template<typename Type> template<class Type2>
   bool VectorBase<Type>::containsSomeOf(int m, Type2 &vec2)
{
  int i = 0, j, c = 0;
  while (i < this->n)
  {
    j = 0;
    while (j < vec2.n && (*this)[i] != vec2[j]) j++;
    if (j < vec2.n) { c++; if (c == m) return true; }
    i++;
  }
  return false;
}





template<typename Type> bool VectorBase<Type>::distinct(void)
{
  int i, j;

  for (i=1; i<this->n; i++)
    for (j=0; j<i; j++)
      if ((*this)[i] == (*this)[j]) return false;

  return true;
}





template<typename Type> template<class Type2> bool VectorBase<Type>::operator==(Type2 &b)
{
  if (this->n != b.n) return false;

  int i = 0;

  while (i < this->n) if ((*this)[i] == b[i]) i++; else return false;

  return true;
}





template<typename Type> void VectorBase<Type>::permute(int *perm, int *flagPtr)
{
  Type dmy, *p1, *p2;
 
  int i, j = 0, *flag = flagPtr, m = this->n;

  if (flag == NULL) flag = new int [m];

  for (i=0; i<m; i++) flag[i] = 1;

  i = 0;

  while (i < m)
  {
    p2 = &((*this)[i]);

    dmy = *p2;

    do {

      p1 = p2;

      p2 = &((*this)[perm[j]]);

      *p1 = *p2;

      //cout << *p1 << " A\n";

      flag[j] = 0;

      j = perm[j];

      //cout << *this << "\n";

    } while (i != perm[j]);

    *p2 = dmy;

    flag[j] = 0;

    //cout << *p2 << " B\n";

    i++;

    while (i < m) if (flag[i]) break; else i++;

    //cout << *this << "," <<  i << "\n";

    j = i; 
  }

  if (flagPtr == NULL) delete [] flag;

  return;
}





template<typename Type> std::ostream &operator<<(std::ostream &os, VectorBase<Type> &vec)
{
  vec.print(os);  return os;
}




 
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


template<typename Type> Type &Vector<Type>::operator[](int i)
{
  return LinkedListBase< VectorCoeff<Type> >::getItem(i).val;
}



//template<typename Type> Vector<Type> &Vector<Type>::operator=(Vector<Type> &b)
//{
//  cout << "Vector<Type>::operator= (1)\n";
//
//  this->free();
//
//  for (int i=0; i<b.n; i++) append(b[i]);
//
//  return *this;
//}



template<typename Type> template<class Type2> Vector<Type> &Vector<Type>::operator=(Type2 &b)
{
  //cout << "Vector<Type>::operator=\n";

  this->free();

  for (int i=0; i<b.n; i++) append(b[i]);

  return *this;
}



template<typename Type> Type *Vector<Type>::append(Type x)
{       
  VectorCoeff<Type> *c = new VectorCoeff<Type>;

  c->val = x;

  this->add(c);

  return &(c->val);
}




template<typename Type> void Vector<Type>::insert(Type x, int pos)
{       
  VectorCoeff<Type> *c = new VectorCoeff<Type>;

  c->val = x;

  LinkedListBase< VectorCoeff<Type> >::insert(c,pos);
	
  return;
}




template<typename Type> void Vector<Type>::del(int pos)
{       
  LinkedListBase< VectorCoeff<Type> >::del(pos);
	
  return;
}




template<typename Type> Type &Vector<Type>::firstCoeff(void)
{
  return ((VectorCoeff<Type>*)(this->first.next))->val;
}




template<typename Type> Type &Vector<Type>::lastCoeff(void)
{
  return ((VectorCoeff<Type>*)(this->last.prev))->val;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



template<typename Type> Type &VectorArray<Type>::operator[](int i)
{
  if (i < 0 || i+1 > this->n) 

    cout << "  ERROR: VectorArray<Type>::operator[]: invalid index!\n\n";

  return x[i];
}






template<typename Type> VectorArray<Type> &VectorArray<Type>::operator=(VectorArray<Type> &b)
{
  setDim(b.n);

  for (int i=0; i<this->n; i++) x[i] = b[i];

  return *this;
}






template<typename Type> template<class Type2> 
                  VectorArray<Type> &VectorArray<Type>::operator=(Type2 &b)
{
  setDim(b.n);

  for (int i=0; i<this->n; i++) x[i] = b[i];

  return *this;
}







template<typename Type> void VectorArray<Type>::setDim(int d)
{       
  if (d != this->n)
  {
    this->free();
    x = new Type [d];
    this->n = d;
  }
  return;
}



template<typename Type> void VectorArray<Type>::free(void)
{       
  if (x != NULL && this->n != 0) delete [] x; x = NULL; this->n = 0;
	
  return;
}







template<typename Type> void VectorArray<Type>::expand(int d)
{       
  if (d <= this->n) return;

  Type *tmp = new Type [d];

  int i = 0;

  while (i < this->n) tmp[i] = x[i++];

  while (i < d) tmp[i++] = (Type) 0;

  delete [] x;

  x = tmp;

  this->n = d;

  return;
}




#endif



