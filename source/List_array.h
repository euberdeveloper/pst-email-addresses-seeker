#ifndef LIST
#define LIST

#include <iostream>
using namespace std;

typedef int Position;
template<class T_elem>
class List
{
 T_elem *v;
 int dim;
 Position last;
 public:
  List(int maxdim=100);
  ~List();
  Position First();
  Position End();
  int Empty();
  int Full();
  Position Subsequent(Position);
  Position Previous(Position);
  void Pop_back();
  void Delete(Position);
  void Push_back(const T_elem);
  void Insert(const T_elem&, Position);
  Position Find(const T_elem&);
  T_elem& Value(Position);
  void Reverse();
  void Ascending();
  void Descending();
  void Del_duplicates();
  Position Max();
  Position Min();
  void Print();
  void Print(Position);
};

//constructor of List(it makes an empty list)
template<class T_elem>
List<T_elem>::List(int maxdim)
{
 dim=maxdim;
 last=-1;
 v=new T_elem[dim];
}

//destructor of List
template<class T_elem>
List<T_elem>::~List()
{
 delete [] v;
}

//it returns true if the list is empty
template<class T_elem>
int List<T_elem>::Empty()
{
 return (last<0);
}

//it returns true if the list is full
template<class T_elem>
int List<T_elem>::Full()
{
 return (last==dim-1);
}

//it returns the position of the first element of the list
template<class T_elem>
Position List<T_elem>::First()
{
 return 0;
}

//it returns the position of the element subsequent of the last element
template<class T_elem>
Position List<T_elem>::End()
{
 return (last+1);
}

//it returns the subsequent position of the position p
template<class T_elem>
Position List<T_elem>::Subsequent(Position p)
{
 if(p<0||p>=last)
  {
   cout<<"Error: the position doesn't exist or it isn't allowed to this operation\n";
   system("pause");
   exit(1);
  }
 else
  return ++p;
}

//it returns the previous position of the position p
template<class T_elem>
Position List<T_elem>::Previous(Position p)
{
 if(p<=0||p>last)
  {
   cout<<"Error: the position doesn't exist or it isn't allowed to this operation\n";
   system("pause");
   exit(1);
  }
 else
  return --p;
}

//it deletes the last element and if the list is empty it does nothing
template<class T_elem>
void List<T_elem>::Pop_back()
{
 if(!Empty())
  last--;
 return;
}

//it deletes the element in position p and all the subsequent elements go back of one position
template<class T_elem>
void List<T_elem>::Delete(Position p)
{
 if(p<0||p>last)
 {
  cout<<"Error: the position doesn't exist\n";
  system("pause");
  exit(1);
 }
 for(Position i=p; i<last; i++)
  v[i]=v[i+1];
 last--;
 return;
}

//it inserts an element at the end of the List 
template<class T_elem>
void List<T_elem>::Push_back(const T_elem el)
{
 if(Full())
 {
  cout<<"Error: the list is Full\n";
  system("pause");
  exit(1);
 }
 last++;
 v[last]=el;
 return;
}

//it inserts an element in position p with value el and all the subsequent elements go after of one position 
template<class T_elem>
void List<T_elem>::Insert(const T_elem& el, Position p)
{
 if(Full())
 {
  cout<<"Error: the list is Full\n";
  system("pause");
  exit(1);
 }
 if(p<0||p>(last+1))
 {
  cout<<"Error: the position doesn't exist\n";
  system("pause");
  exit(1);
 }
 Position i;
 for(Position i=last; i>=p; i--)
  v[i+1]=v[i];
 v[p]=el;
 last++;
 return;
}

//it returns the position of the first element with value el
template<class T_elem>
Position List<T_elem>::Find(const T_elem& el)
{
 Position p;
 for(p=0; p<=last&&el!=v[p]; p++)
  ;
 if(el==v[p])
  return p;
 else
  return (last+1);
}

//it returns the value of the element in position p
template<class T_elem>
T_elem& List<T_elem>::Value(Position p)
{
 if(p<0||p>last)
 {
  cout<<"Error: the position doesn't exist\n";
  system("pause");
  exit(1);
 }
 return v[p];
}

//this function swaps a's and b's places and is used by the functions Reverse(),Ascending() and Descending()
template<class T_elem>
void Swap(T_elem &a,T_elem &b)
 {
  T_elem c;
  c=a;
  a=b;
  b=c;
  return;
 }

//this function reverses the elements of the list
template<class T_elem>
void List<T_elem>::Reverse()
{
 for(Position i=0; i<last; i++)
  for(Position k=0; k<last-i; k++)
   Swap(v[k],v[k+1]);
 return;
}
 
//this function sorts the list in ascending order
template<class T_elem>
void List<T_elem>::Ascending()
{
 for(Position i=0; i<(last); i++)
  for(Position k=0; k<(last-i); k++)
   if(v[k]>v[k+1])
    Swap(v[k],v[k+1]);
 return;
}

//this function sorts the list in descending order
template<class T_elem>
void List<T_elem>::Descending()
{
 for(Position i=0; i<(last); i++)
  for(Position k=0; k<(last-i); k++)
   if(v[k]<v[k+1])
    Swap(v[k],v[k+1]);
 return;
}

//this function deletes the duplicates(elements with the same value) in the list
template<class T_elem>
void List<T_elem>::Del_duplicates()
{
 for(Position i=0; i<last+1; i++)
 {
  for(Position k=i+1; k<(last+1); k++)
   if(v[i]==v[k])
   {
    this->Delete(k);
    k--;
   }
 }
 return;
}

//it returns the position of the element with the biggest value
template<class T_elem>
Position List<T_elem>::Max()
{
 Position p;
 T_elem max=0;
 for(Position i=0; i<(last+1); i++)
 {
  if(v[i]>max)
   {
    max=v[i];
    p=i;
   }
 }
 return p;
}

//it returns the position of the element with the smallest value
template<class T_elem>
Position List<T_elem>::Min()
{
 Position p;
 T_elem min;
 if(sizeof(min)==sizeof(int))
  min=INT_MAX;
 else if(sizeof(min)==sizeof(long int))
  min=LONG_MAX;
 else if(sizeof(min)==sizeof(unsigned int))
  min=UINT_MAX;
 else if(sizeof(min)==sizeof(unsigned long int))
  min=ULONG_MAX;
 else if(sizeof(min)==sizeof(long long int))
  min=LLONG_MAX;
 else if(sizeof(min)==sizeof(unsigned long long int))
  min=ULLONG_MAX;
 else if(sizeof(min)==sizeof(short int))
  min=SHRT_MAX;
 else if(sizeof(min)==sizeof(unsigned short int))
  min=USHRT_MAX;
 else if(sizeof(min)==sizeof(char))
  min=CHAR_MAX;
 else if(sizeof(min)==sizeof(unsigned char))
  min=UCHAR_MAX;
 else
  min=999999999;
 for(Position i=0; i<(last+1); i++)
 {
  if(v[i]<min)
   {
    min=v[i];
    p=i;
   }
 }
 return p;
}

//it prints the contents of the list
template<class T_elem>
void List<T_elem>::Print()
{
 for(Position p=0; p<=last; p++)
 {
  if(p==0)
   cout<<"Content of the list:\n";
  cout<<"List("<<p<<") => "<<v[p]<<"\t";
  if((p+1)%4==0)
   cout<<endl;
 }
 cout<<endl;
 return;
}

//it prints the contents of the element in position p
template<class T_elem>
void List<T_elem>::Print(Position p)
{
 if(p>last)
 {
  cout<<"Error: position doesn't exist\n";
  system("pause"); exit(1);
 }
 cout<<"List("<<p<<") => "<<v[p];
 return;
}


/*                                      STURCTURE OF A LIST OF N ELEMENTS:

                                        v=value
                                        * is a punct to an array         
                                        [ | |...|n] is an array of n elements

                                        EMPTY LIST
                                        * -> [ - | - | - | - | - | - | - | - | ... | - ]
                                          -1   0   1   2   3   4   5   6   7   ...   n
                                         last
                                        NOT EMPTY LIST
                                        * -> [ v | v | v | v | v | - | - | - | ... | - ]
                                          -1   0   1   2   3   4   5   6   7   ...   n
                                                              last
                                        FULL LIST
                                        * -> [ v | v | v | v | v | v | v | v | ... | v ]
                                          -1   0   1   2   3   4   5   6   7   ...   n
                                                                                    last                                                    */
#endif

