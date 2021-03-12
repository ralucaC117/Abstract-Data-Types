#include "IteratorMDO.h"
#include "MDO.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <exception>

using namespace std;
/*
theta(1)
*/
int hashCode(TElem e){
	return abs(e.first);
}
/*
theta(1)
*/
int MDO::d(TElem e) const  {
	return hashCode(e) % capacitate;
}
/*
theta(capacitate)
*/
MDO::MDO(Relatie r) {
	/* de adaugat */
	capacitate = 13;
	tabelValori = new vector<TValoare>[capacitate];
	tabelChei  = new TCheie[capacitate];
	urmator =  new int[capacitate];
	m = 0;
	for (int i = 0; i < capacitate; i++) {
		tabelChei[i] = -10000;
		urmator[i] = -10000;
	}
	primLiber = 0;
	rel = r;
}

/*
best case: theta(1)
worst case = theta(m)
mediu: theta(alpha)
*/
void MDO::adauga(TCheie c, TValoare v) {
	/* de adaugat */
	TElem e = std::make_pair(c, v);
	int pos = d(e); //calculez valoarea de dispersie

	if (tabelChei[pos] == -10000) { //daca nu se afla nimic la indexul egal cu valoarea de dispersie
		tabelChei[pos] = c;
		tabelValori[pos].push_back(v);
		m++;
		if (pos == primLiber)
			actPrimLiber();  //daca a fost ocupata pozitia lui prim liber, acesta se actualizeaza si se face resize + redispersare daca e cazul
		return;
	}
	if (c == tabelChei[pos]) { //daca cheia deja exista, se adauga valoarea asociata cheii in vectorul de valori
		tabelValori[pos].push_back(v);
		m++;
		return;
	}
	int anteCurent = -10000;
	while (pos != -10000) //cazul in care indicele la care trebuie dispersata cheia este ocupat
	{
		anteCurent = pos;
		pos = urmator[pos];
	}
	tabelChei[primLiber] = c;
	tabelValori[primLiber].push_back(v);
	urmator[anteCurent] = primLiber;
	m++;
	actPrimLiber();
}
/*
best: theta(1)
worst case = theta(m)
mediu: theta(alpha)
*/
vector<TValoare> MDO::cauta(TCheie c) const {
	/* de adaugat */
	TElem e = std::make_pair(c, 0);
	int pos = d(e);
	while (pos != -10000 && tabelChei[pos] != c ) {
		pos = urmator[pos];
	}
	if(pos == -10000)
		return vector<TValoare>();
	if (tabelChei[pos] == c)
		return tabelValori[pos];
	
}
/*
pp ca valoarea v se afla la pozitia n in vectorul de valori
best case theta(1)
in medie theta(alpha)
worst case theta(m+n)

*/
bool MDO::sterge(TCheie c, TValoare v) {
	/* de adaugat */
	TElem e = std::make_pair(c, v);
	int i = d(e); //calculeaza valoarea de dispersie

	while (i != -10000 && tabelChei[i] != c ) {
		i = urmator[i];
	}
	if (i == -10000)
		return false;
	if (tabelChei[i] == c) {//a gasit cheia care trebuie stearsa
		int pos= -10000;
		for (int j = 0; j < tabelValori[i].size(); j++) {
			if (tabelValori[i][j] == v) {
				pos = j;
				break;
			}
		}
		if (pos == -10000) {
			return false; //nu exista valoarea in vectorul asociat cheii
		}
		else if (tabelValori[i].size() > 1) {
			tabelValori[i].erase(tabelValori[i].begin() + pos);
			m--;
			return true;
		}
	
		if(tabelValori[i].size() == 1) { //trebuie stearsa cheia cu totul
			int anterior = -10000;
			int j;
			bool found = false;

			while (found == false) {
				j = urmator[i];
				TElem e2;
				if (j != -10000)
					    e2 = std::make_pair(tabelChei[j], 0);
				while ( j != -10000 && i != d(e2) ) {
					j = urmator[j];
					if (j != -10000)
						e2 = std::make_pair(tabelChei[j], 0);
				}
				if (j!= -10000 && d(e2) == i) { //daca s-a gasit un element care poate fi dispersat la indicele i
					tabelChei[i] = tabelChei[j];
					anterior = i;
					i = j;
				}
				else if (j == -10000) {//daca nu mai exista niciun element care poate fi dispersat la indicele i, valoarea cheii se reseteaza la -1
					tabelChei[i] = -10000;
					tabelValori[i].clear();
					if (anterior != -10000) {
						urmator[anterior] = urmator[i];
						urmator[i] = -10000;
					}
					m--;
					found = true;
				}
			}
			return true;
		}
	}
}
/*
theta(1)
*/
int MDO::dim() const {
	/* de adaugat */
	return m;
}
/*
theta(1)
*/
bool MDO::vid() const {
	/* de adaugat */
	return m==0;
}
/*
theta(1)
*/
IteratorMDO MDO::iterator() const {
	return IteratorMDO(*this);
}
/*
theta(1)
*/
MDO::~MDO() {
	/* de adaugat */
	delete[] tabelChei;
	delete[] tabelValori;
	delete[] urmator;
}

/*
theta(capacitate)
*/
void MDO::goleste() {
	bool empty = false;
	int i = 0;
	while (i<capacitate) {
		tabelChei[i] = 10000;
		m -= tabelValori[i].size();
		tabelValori[i].clear();
		i++;
	}
}
void MDO::rehash() {
	int* newUrmator = new int[2 * capacitate];
	std::vector<TValoare> *tabelValoriVechi = new vector<TValoare>[2 * capacitate];
	std::vector<TValoare> *tabelValoriNou = new vector<TValoare>[2 * capacitate];
	TCheie *tabelCheiVechi = new TCheie[2 * capacitate];
	TCheie *tabelCheiNou = new TCheie[2 * capacitate];

	for (int i = 0; i < capacitate; i++) {
		tabelValoriVechi[i] = tabelValori[i];
		tabelCheiVechi[i] = tabelChei[i];
	}

	for (int i = 0; i < (2 * capacitate); i++) {
		tabelCheiNou[i] = -10000;
		newUrmator[i] = -10000;
	}

	delete[] this->tabelChei;
	delete[] this->tabelValori;
	delete[] this->urmator;

	this->tabelChei = tabelCheiNou;
	this->tabelValori = tabelValoriNou;
	this->urmator = newUrmator;
	m = 0;
	capacitate *= 2;
	for (int i = 0; i < capacitate / 2; i++) {
		for (int j = 0; j < tabelValoriVechi[i].size(); j++)
			adauga(tabelCheiVechi[i], tabelValoriVechi[i].at(j));
	}

}
void MDO::actPrimLiber() {
	primLiber++;
	while (primLiber < capacitate && tabelChei[primLiber] != -10000)
		primLiber++;
	if (primLiber == capacitate) {
		rehash();
	}
}