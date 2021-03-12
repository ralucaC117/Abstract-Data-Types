#include "Iterator.h"
#include "DO.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

/*
best case = worst case = avg case = theta(n)
overall: theta(n)
*/
DO::DO(Relatie r) {
	/* de adaugat */
	capacitate = 2;
	e = new TElem[capacitate];
	urm = new int[capacitate];

	prim = -1;
	primLiber = 0;

	for (int i = 0; i < capacitate - 1; i++)
		urm[i] = i + 1;
	urm[capacitate - 1] = -1;

	relatie = r;
}


//adauga o pereche (cheie, valoare) in dictionar
//daca exista deja cheia in dictionar, inlocuieste valoarea asociata cheii si returneaza vechea valoare
//daca nu exista cheia, adauga perechea si returneaza null
/*
best case: trebuie adaugat pe prima pozitie => theta(1)
avg case = worst case: => theta(n)
overall: O(n)
*/
TValoare DO::adauga(TCheie c, TValoare v) {
	/* de adaugat */
	int curent = prim;
	int anteCurent;
	while (curent != -1 && e[curent].first != c && relatie(e[curent].first, c)) {
		anteCurent = curent;
		curent = urm[curent];
	}
	if (e[curent].first == c) {
		TValoare oldValue = e[curent].second;
		e[curent].second = v;
		return oldValue;
	}
	if (prim == -1)
	{//daca e goala lista
		prim = creeazaNod(make_pair(c, v));
		return NULL_TVALOARE;
	}
	else
	{//daca nu
		if (curent == prim)
		{//daca trebuie inserat pe prima pozitie
			int next = prim;
			prim = creeazaNod(make_pair(c, v));
			urm[prim] = next;
		}
		else {
		//daca nu
			int pos = creeazaNod(make_pair(c, v));
			urm[pos] = curent;
			urm[anteCurent] = pos;
		}
		return NULL_TVALOARE;
	}
}
/*
best case: elementul se afla pe prima poz. => theta(1)
avg case = worst case => theta(n) 
Overall: O(n)
*/
//cauta o cheie si returneaza valoarea asociata (daca dictionarul contine cheia) sau null
TValoare DO::cauta(TCheie c) const {
	/* de adaugat */
	int curent = prim;
	while (curent != -1 && e[curent].first != c && relatie(e[curent].first, c)) {
		curent = urm[curent];
	}
	if (e[curent].first == c)
		return e[curent].second;
	else
		return NULL_TVALOARE;
}

//sterge o cheie si returneaza valoarea asociata (daca exista) sau null

/*best case: elementul se afla pe prima poz. = > theta(1)
avg case = worst case = > theta(n)
Overall: O(n)*/

TValoare DO::sterge(TCheie c) {
	/* de adaugat */
	int curent = prim;
	int anteCurent;
	TValoare oldValue;
	while (curent != -1 && e[curent].first != c && relatie(e[curent].first, c)) {
		anteCurent = curent;
		curent = urm[curent];
	}
	if (e[curent].first == c) {
		oldValue = e[curent].second;
		if (curent == prim) 
			prim = urm[prim];
		
		else 
			urm[anteCurent] = urm[curent];
		dealoca(curent);
		return oldValue;
	}
	return NULL_TVALOARE;
}

//returneaza numarul de perechi (cheie, valoare) din dictionar
/*
best case = worst case = avg case = theta(n)
overall: theta(n)
*/
int DO::dim() const {
	/* de adaugat */
	int curent = prim;
	int size = 0;
	while (curent != -1) {
		curent = urm[curent];
		size++;
	}

	return size;
}
/*
theta(1)
*/
//verifica daca dictionarul e vid
bool DO::vid() const {
	/* de adaugat */
	if (prim == -1)
		return true;
	return false;

}
/*
theta(1)
*/
Iterator DO::iterator() const {
	return  Iterator(*this);
}

DO::~DO() {
	/* de adaugat */
	delete[] e;
	delete[] urm;
}
/*
theta(1)
*/
int DO::aloca() {
	int i = primLiber;
	if (i != -1) {
		primLiber = urm[primLiber];
	}
	return i;
}
/*
theta(1)
*/
void DO::dealoca(int i) {

	urm[i] = primLiber;
	primLiber = i;

}
/*
best case: theta(1)
avg case = worst case: theta(n)
Overall: O(n)
*/
int DO::creeazaNod(TElem e) {
	int i = aloca();
	if (i != -1) {
		this->e[i] = e;
		this->urm[i] = -1;
	}
	else {
		int* newUrm = new int[capacitate * 2];
		TElem* newE = new TElem[capacitate * 2];

		for (int i = 0; i < capacitate; i++)
			newUrm[i] = urm[i];
		for (int i = 0; i < capacitate; i++)
			newE[i] = this->e[i];
		for (int i = capacitate; i < capacitate * 2 - 1; i++)
			newUrm[i] = i + 1;
		newUrm[capacitate * 2 - 1] = -1;

		primLiber = capacitate;
		capacitate *= 2;

		delete[] this->urm;
		delete[] this->e;

		this->urm = newUrm;
		this->e = newE;

		i = aloca();
		this->e[i] = e;
		urm[i] = -1;
	}
	return i;

}
/*
best case = worst case = avg case = theta(n)
overall: theta(n)
*/
vector<TCheie> DO::multimeaCheilor() const {
	int curent = prim;
	vector<TCheie> multimeChei;

	while (curent != -1) {
		multimeChei.push_back(e[curent].first);
		curent = urm[curent];
	}
	return multimeChei;
}
