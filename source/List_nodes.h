#ifndef LIST
#define LIST

#include <iostream>
using namespace std;

template<class T_elem>
class Node
{
 public:
 T_elem v;
 Node *next;
};

template<class T_elem>
class List
{
 Node<T_elem> *begin, *last;
 void Delete(Node<T_elem>*);
 void Insert(const T_elem, Node<T_elem>*);
 T_elem& Value(Node<T_elem>*);
 Node<T_elem>* Subsequent(Node<T_elem>*);
 Node<T_elem>* Previous(Node<T_elem>*);
 Node<T_elem>* First();
 Node<T_elem>* End();
 public:
  List();
  ~List();
  int Empty();
  int Full();
  int Size();
  void Pop_back();
  void Delete(int);
  void Push_back(const T_elem);
  void Insert(const T_elem, int);
  int Find(const T_elem&);
  T_elem& Value(int);
  void Reverse();
  void Ascending();
  void Descending();
  void Del_duplicates();
  int Max();
  int Min();
  void Print();
  void Print(int);
};

//constructor of List(it makes an empty list)
template<class T_elem>
List<T_elem>::List()
{
 begin=new Node<T_elem>;
 begin->next=NULL;
 last=begin;
}

//destructor of List
template<class T_elem>
List<T_elem>::~List()
{
 Node<T_elem> *psub;
 for(Node<T_elem> *p=begin; p!=NULL; p=psub)
 {
  psub=p->next;
  delete p;
 }
}

//it returns begin(service function)
template<class T_elem>
Node<T_elem>* List<T_elem>::First()
{
 return begin;
}

//it returns the last element(service function)
template<class T_elem>
Node<T_elem>* List<T_elem>::End()
{
 return last;
}

//it returns true if the list is empty
template<class T_elem>
int List<T_elem>::Empty()
{
 return (begin->next==NULL);
}

//it returns true if the list is full(never)
template<class T_elem>
int List<T_elem>::Full()
{
 return 0;
}

//it returns the number of the elements in the list
template<class T_elem>
int List<T_elem>::Size()
{
 int n=0;
 for(Node<T_elem>* p=begin->next; p!=NULL; p=p->next)
  n++;
 return n;
}

//it returns the subsequent punct-node of the position p(service function)
template<class T_elem>
Node<T_elem>* List<T_elem>::Subsequent(Node<T_elem>* p)
{
 return p->next;
} 

//it returns the previous punct-node of the position p(service function)
template<class T_elem>
Node<T_elem>* List<T_elem>::Previous(Node<T_elem>* p)
{
 Node<T_elem>* temp;
 for(temp=begin; temp!=NULL&&temp->next!=p; temp=temp->next)
  ;
 if(temp!=NULL)
  return temp;
 else
 {
  cout<<"Error: previous element doesn't exist\n";
  system("pause"); exit(1);
 } 
}  

//it deletes the subsequent punct-node of p(service function)
template<class T_elem>
void List<T_elem>::Delete(Node<T_elem>* p)  
{
 Node<T_elem>* t=p->next;
 if(t==last)
  last=p;
 p->next=p->next->next;
 delete t;
 return; 
} 

//it deletes the last element and if the list is empty it does nothing
template<class T_elem>
void List<T_elem>::Pop_back()
{
 if(!Empty())	
  Delete(Previous(last));
 return;
}

//it deletes the element in position n
template<class T_elem>
void List<T_elem>::Delete(int n)  
{
 Node<T_elem> *p=begin;
 for(int i=0; i<n; i++)
 {
  p=p->next;
  if(p->next==NULL)
  {
   cout<<"Error: wrong position\n";
   system("pause"); exit(1);
  }
 }
 Delete(p);
 return; 
} 

//it inserts a punct-node in the subsequent position of p with value el(service function)
template<class T_elem>
void List<T_elem>::Insert(const T_elem el, Node<T_elem>* p)  
{
 Node<T_elem> *t=new Node<T_elem>;
 t->v=el;
 t->next=p->next;
 p->next=t;
 if(p==last)
  last=t;
 return;
} 

//it inserts an element at the end of the list
template<class T_elem>
void List<T_elem>::Push_back(const T_elem el)
{
 Insert(el,End());
 return;
}


//it inserts an element with value el in position n
template<class T_elem>
void List<T_elem>::Insert(const T_elem el, int n)
{
 Node<T_elem>* p=begin;
 for(int i=0; i<n; i++)
 {
  p=p->next;
  if(p==NULL)
  {
   cout<<"Error: wrong position\n";
   system("pause"); exit(1);
  }
 }
 Insert(el,p);
 return;
}

//it returns the position of the first element in the list with value el
template<class T_elem>
int List<T_elem>::Find(const T_elem& el)
{
 Node<T_elem> *p;
 int n=0;
 for(p=begin;p->next!=NULL&&p->next->v!=el;p=p->next)
  n++;
 if(p->next!=NULL)
  return n;
 else
  return -1;
}

//it returns the value of the punct-node p(service function)
template<class T_elem>
T_elem& List<T_elem>::Value(Node<T_elem>* p)
{
 return p->next->v;
}

//it returns the value of the element in position n
template<class T_elem>
T_elem& List<T_elem>::Value(int n)
{
 Node<T_elem> *p=begin;
 for(int i=0; i<n; i++)
 {
  p=p->next;
  if(p->next==NULL)
  {
   cout<<"Error: wrong position\n";
   system("pause"); exit(1);
  }
 }
 return Value(p);
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
 for(int i=0; i<Size(); i++)
 {
  Node<T_elem>* k=begin->next;
  for(int j=i+1; j<Size(); j++)
  {
   Swap(k->v,k->next->v);
   k=k->next;
  }
 }
 return;
}

//this function sorts the list in ascending order
template<class T_elem>
void List<T_elem>::Ascending()
{
 for(Node<T_elem>* i=begin->next; i!=NULL; i=i->next)
  for(Node<T_elem>* k=begin->next; k!=last; k=k->next)
   if(k->v>k->next->v)
    Swap(k->v,k->next->v);
 return;
}

//this function sorts the list in descending order
template<class T_elem>
void List<T_elem>::Descending()
{
 for(Node<T_elem>* i=begin->next; i!=NULL; i=i->next)
  for(Node<T_elem>* k=begin->next; k!=last; k=k->next)
   if(k->v<k->next->v)
    Swap(k->v,k->next->v);
 return;
}

//this function deletes the duplicates(elements with the same value) in the list
template<class T_elem>
void List<T_elem>::Del_duplicates()
{
 for(Node<T_elem>* i=begin; i->next!=NULL; i=i->next)
 {
  for(Node<T_elem>* k=i->next; k->next!=NULL;)
   if(i->next->v==k->next->v)
    Delete(k);
   else
    k=k->next;
 }
 return;
}

//it returns the position of the element with the biggest value
template<class T_elem>
int List<T_elem>::Max()
{
 T_elem max=0;
 int n,i=0;
 for(Node<T_elem>* p=begin; p->next!=NULL; p=p->next)
 {
  if(p->next->v>max)
  {
   max=p->next->v;
   n=i;
  }
  i++;
 }
 return n;
}

//it returns the position of the element with the smallest value
template<class T_elem>
int List<T_elem>::Min()
{
 int p;
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
 int n,i=0;
 for(Node<T_elem>* p=begin; p->next!=NULL; p=p->next)
 {
  if(p->next->v<min)
  {
   min=p->next->v;
   n=i;
  }
  i++;
 }
 return n;
}

//it prints the contents of the list
template<class T_elem>
void List<T_elem>::Print()
{
 int i=0;
 for(Node<T_elem> *p=begin; p!=last; p=p->next)
 {
  if(i==0)
   cout<<"Content of the list:\n";
  cout<<"List("<<i<<") => "<<p->next->v<<"\t";
  if(++i%4==0)
   cout<<endl;
 }
 cout<<endl;
 return;
}

//it prints the contents of the element in position n
template<class T_elem>
void List<T_elem>::Print(int n)  
{
 Node<T_elem> *p=begin;
 for(int i=0; i<n; i++)
 {
  p=p->next;
  if(p->next==NULL)
  {
   cout<<"Error: element not found\n";
   system("pause"); exit(1);
  }
 }
 cout<<"List("<<n<<") => "<<p->next->v;
 return;
}

/*                                      STRUCTURE OF A NODE:
           
                                        v=value("-" is no value)
                                        * is a punct to another node
 
                                        [v|*]
 
 
                                        STRUCTURE OF A LIST OF N ELEMENTS:
                                                  
                                        LIST EMPTY:
                                        [-|*]->NULL
                                        begin
                                        last
                                                 
                                        LIST NOT EMPTY:
                                        [-|*]->[v|*]->[v|*]->[v|*]->...->[v|*]->NULL
                                        begin                            last
                                                 0      1      2    ...    n                                                                */
#endif
