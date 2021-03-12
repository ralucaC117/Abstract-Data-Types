#include "IteratorMDO.h"
#include "MDO.h"

/*
theta(n)
*/
IteratorMDO::IteratorMDO(const MDO& d) : dict(d){
	/* de adaugat */
	curent = dict.radacina;
	curentValori = 0;
	while (curent != -999) {
		s.push(curent);
		curent = dict.stanga[curent];
		if (dict.dreapta[curent] != -999) {
			int nod = s.top();
			nod = dict.dreapta[nod];
			while (nod != -999) {
				s.push(nod);
				nod = dict.stanga[nod];
			}
		}
	}
	if (s.empty() == false) 
		curent = s.top();
	
	else
		curent = -999;

}
/*
theta(1)
*/
void IteratorMDO::prim(){
	/* de adaugat */
	if (s.empty() == false)
		curent = s.top();
	else
		curent = -999;
}
/*
theta(1)
*/
void IteratorMDO::urmator(){
	/* de adaugat */
	if (!valid())
		throw std::exception();

	int nod = s.top();
	if (dict.valori[nod][curentValori] != -999 && dict.cap_valori>curentValori) {
		curent = nod;
		curentValori++;
		return;

	}
	if (dict.valori[nod][curentValori] == -999){
		s.pop();
		if (s.empty() == false){
			curent = s.top();
			s.pop();
			curentValori = -1;
		}
		else
			curent = -999;
	}
}

/*
theta(1)
*/
bool IteratorMDO::valid() const{
	/* de adaugat */
	if (curent == -999 || dict.valori[curent][curentValori]==-999)
		return false;
	return true;
}
/*
theta(1)
*/
TElem IteratorMDO::element() const{
	/* de adaugat */
	if (!valid()) 
		throw std::exception();
	return pair <TCheie, TValoare>  (dict.chei[curent], dict.valori[curent][curentValori] );
}


