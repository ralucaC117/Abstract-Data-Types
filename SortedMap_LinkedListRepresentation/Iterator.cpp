#include "Iterator.h"
#include "DO.h"
#include <exception>
using namespace std;
/*
theta(1)
*/
Iterator::Iterator(const DO& d) : dict(d){
	/* de adaugat */
	curent = dict.prim;
}

/*
theta(1)
*/
void Iterator::prim(){
	/* de adaugat */
	curent = dict.prim;
}
/*
theta(1)
*/
void Iterator::urmator(){
	/* de adaugat */
	if(!valid())
		throw std::exception();
	curent = dict.urm[curent];
}
/*
theta(1)
*/
bool Iterator::valid() const{
	/* de adaugat */
	if(curent<0 || curent>dict.capacitate)
		return false;
	return true;

}
/*
theta(1)
*/
TElem Iterator::element() const{
	/* de adaugat */
	if(!valid())
		throw std::exception();
	return dict.e[curent];
}


