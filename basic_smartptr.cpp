#include<iostream>

using namespace std;

class Data{
	int d;
	public:
	Data():d(0){}
	Data(int _d) : d(_d){}
	void Incr(){
		d++;
		cout << "INCR d = " << d << endl;
	}
	void operator *(){
		Display();
	}
	void Display(){ 
		cout << "DISP d = " << d << endl;
	}
};


class Reference{
	size_t count;
	public:
	Reference():count(0){}
	size_t IncrRefCount(){
		return ++count;
	}
	size_t DecrRefCount(){
		return --count;
	}
	void DisplayRefCount(){
		cout << "\tREF COUNT "<< count << endl;
	}
};


template<typename T>
class SmartPointer{
	T *pt;
	Reference *r; //reference count
	public:

	SmartPointer() : pt(NULL), r(NULL) {
		r = new Reference();
		r->IncrRefCount();
	}

	SmartPointer(T *_pt) : pt(_pt), r(NULL){
		r = new Reference();
		r->IncrRefCount();
		cout << "!!! CONSTRUCTOR !!!";
		r->DisplayRefCount();
	}

	~SmartPointer(){ 
		size_t rc = r->DecrRefCount();
		cout << "DESTRUCTOR";
		r->DisplayRefCount();
		if(rc == 0){
			cout << "\t*** DELETING MEMORY ***" << endl;
			delete pt; 
			delete r;
		}
	}

	SmartPointer(SmartPointer &rhs){
		pt = &(*rhs);
		r = rhs.GetReference();
		r->IncrRefCount();
		cout << "COPY CONSTRUCTOR";
		r->DisplayRefCount();
	}	
	
	T &operator *(){
		return *pt;
	}

	T *operator ->(){
		return pt;
	}

	Reference *GetReference(){
		return r;
	}

	SmartPointer<T> &operator=(SmartPointer<T> &rhs){
		
		if(this == &rhs){
			return *this;
		}

		//Get rid of the old data
		if(r->DecrRefCount() == 0){
			delete pt;
			delete r;
		}

		pt = &(*rhs);
		r = rhs.GetReference();
		r->IncrRefCount();
		
		cout << "ASSIGNMENT OPERATOR";
		r->DisplayRefCount();
		return *this;
		
	}	

	/*SmartPointer<T> &operator=(const SmartPointer<T> &rhs){
		if(this == &rhs){
		       return *this;
		}
		pt = &(*rhs);
		rc = GetRC();
		rc++;
		cout << "ASSIGNMENT RC " << rc << endl;

	}*/

};

template<typename T>
void foo(SmartPointer<T> sp){
	cout << endl << "FOO" << endl;
	sp->Incr();
	(*sp).Display();
	cout << "FOO END" << endl << endl;
}

int main(){

	SmartPointer<Data> sp(new Data());
	sp->Incr();
	(*sp).Display();

	{
		SmartPointer<Data> sp2(sp);
		sp->Incr();
		(*sp2).Display();
	}

	cout << "CALLING FOO" << endl;
	foo(sp);

	cout << "CREATING 2 SMART POINTERS" << endl;
	SmartPointer<Data> sp3, sp4;
	sp3 = sp4 = sp;

	(*sp).Incr();
	sp->Display();
	return 0;
}
