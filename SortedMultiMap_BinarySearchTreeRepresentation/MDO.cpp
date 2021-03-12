#include "IteratorMDO.h"
#include "MDO.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;
/*
theta(capacitate)
*/
MDO::MDO(Relatie r) {
	/* de adaugat */
	this->rel = r;
	this->size = 0;
	this->cap = 10;
	this->cap_valori = 5;
	stanga = new int[cap];
	dreapta = new int[cap];
	chei = new TCheie[cap];
	valori = new TValoare*[cap];
	for (int i = 0; i < cap; i++){
		valori[i] = new TValoare[cap_valori];
		stanga[i] = -999;
		dreapta[i] = -999;
		chei[i] = -999;
		for (int j = 0; j < cap_valori; j++) {
			valori[i][j] = -999;
		}
	}
	primLiber = 0;
	radacina = -999;
}
/*
theta(capacitate)
*/

void MDO::actualizeazaPrimLiber() {
	while (chei[primLiber] != -999 && primLiber != cap)
		primLiber++;
	if (primLiber == cap){

		TCheie* cheiNou = new TCheie[2*cap];
		TValoare** valoriNou = new TValoare*[2*cap];
		int* stangaNou=new int[2*cap];
		int* dreaptaNou = new int[2*cap];

		for (int i = 0; i < cap; i++) {
			cheiNou[i] = chei[i];
			valoriNou[i] = valori[i];
			stangaNou[i] = stanga[i];
			dreaptaNou[i] = dreapta[i];
			valoriNou[i] = new TValoare[cap_valori];
			for (int j = 0; j < cap_valori; j++) {
				int aux = valori[i][j];
				valoriNou[i][j] = aux;
			}	
		}

		for (int i = cap; i < 2 * cap; i++) {
			valoriNou[i] = new TValoare[cap_valori];
			for (int j = 0; j < cap_valori; j++) {
				valoriNou[i][j] = -999;
			}
			stangaNou[i] = -999;
			dreaptaNou[i] = -999;
			cheiNou[i] = -999;
		}

		delete[] chei;
		delete[] dreapta;
		delete[] stanga;
		for (int i = 0; i < cap; i++)
			delete[] valori[i];
		delete[] valori;

		chei = cheiNou;
		valori = valoriNou;
		for (int i = 0; i < 2*cap; i++)
			valori[i] = valoriNou[i];
		
		dreapta = dreaptaNou;
		stanga = stangaNou;
		cap *= 2;
	}

}
/*
complexitate O(h)
best case theta(log2n) n-numarul de noduri
worst case theta(n) 
*/
void MDO::adauga(TCheie c, TValoare v) {
	/* de adaugat */
	if (dim() == 0)
		radacina = 0;
	int curent = radacina;
	int parinte = -1;
	while (curent != -999 && chei[curent]!= c) {
		parinte = curent;
		if (rel(c, chei[curent]) == true)
			curent = stanga[curent];
		else
			curent = dreapta[curent];
	}
	if (curent != -999 && chei[curent] == c) {
		//adaug in vectorul de valori
		int primLiberValori = 0;
		while (curent != -999 && primLiberValori != cap_valori && valori[curent][primLiberValori] != -999)
			primLiberValori++;
		if (primLiberValori < cap_valori) {
			valori[curent][primLiberValori] = v;
		}
		else {//daca trebuie redimensionat vectorul de valori
			TValoare* valoriNou = new TValoare[2 * cap_valori];

			for (int i = 0; i < cap_valori; i++) {
				valoriNou[i] = valori[curent][i];
			}
			for (int i = cap_valori; i < 2 * cap_valori; i++) {
				valoriNou[i] = -999;
			}
			delete[] valori[curent];
			valori[curent] = valoriNou;
			cap_valori *= 2;

			valori[curent][primLiberValori] = v;
		}
		size++;
		return;
	}
	else {
		if (rel(c, chei[parinte]) == true) {
			chei[primLiber] = c;
			valori[primLiber][0] = v;
			if (primLiber != 0)
			stanga[parinte] = primLiber;
		}
			
		else {
			chei[primLiber] = c;
			valori[primLiber][0] = v;
			if(primLiber != 0)
			dreapta[parinte] = primLiber;
		}
	}
	actualizeazaPrimLiber();
	size++;
}
/*
complexitate O(h)
best case theta(log2n) n-numarul de noduri
worst case theta(n)
*/
vector<TValoare> MDO::cauta(TCheie c) const {
	/* de adaugat */
	int aux;
	int nod_curent = radacina;
	vector<TValoare> values;
	while (true) {
		aux = nod_curent;
		if (nod_curent != -999 && chei[nod_curent] == c) {
			for (int i = 0; valori[nod_curent][i] != -999; i++)
				values.push_back(valori[nod_curent][i]);
			return values;
		}
		if ( nod_curent !=-999 && stanga[nod_curent] != -999 && rel(c, chei[nod_curent])==true)
			nod_curent = stanga[nod_curent];
		else if (nod_curent != -999 && dreapta[nod_curent] != -999 && rel(c, chei[nod_curent])==false)
			nod_curent = dreapta[nod_curent];
		if(aux == nod_curent)
			return vector<TValoare>();
		
	}
	
}
/*
complexitate O(h)
best case theta(log2n) n-numarul de noduri
worst case theta(n)
*/
bool MDO::sterge(TCheie c, TValoare v) {
	/* de adaugat */

	bool removed = false;
	TElem e;
	e.first = c;
	e.second = v;
	radacina = removeElem(radacina, e, removed);
	if (removed == true)
		this->size--;
	return removed;
}
/*
theta(1)
*/
int MDO::dim() const {
	/* de adaugat */
	return this->size;
}
/*
theta(1)
*/
bool MDO::vid() const {
	/* de adaugat */
	return this->size==0;
}
/*
theta(1)
*/
IteratorMDO MDO::iterator() const {
	return IteratorMDO(*this);
}
/*
theta(capacitate)
*/
MDO::~MDO() {
	/* de adaugat */
	delete[] chei;
	delete[] stanga;
	delete[] dreapta;
	for (int i = 0; i < cap; i++)
		delete[] valori[i];
	delete[] valori;
}
/*
complexitate O(h)
best case theta(log2n) n-numarul de noduri
worst case theta(n)
*/
int MDO::getMax(int r) {
	int nod_curent = r;
	int maxim = r;
	int maxValue;
	while (chei[nod_curent] != -999) {
		maxValue = chei[nod_curent];
		maxim = nod_curent;
		nod_curent = dreapta[nod_curent];
	}
	return maxim;
}
/*
complexitate O(h)
best case theta(log2n) n-numarul de noduri
worst case theta(n)
*/
int MDO::removeElem(int r, TElem e, bool& removed) {
	if (radacina == -999)
		return r;
	bool found = false;
	int j;
	int l = 0;
	if (e.first == chei[r]) {
		for (j = 0; valori[r][j] != -999; j++) {
			if (valori[r][j] == e.second) {
				found = true;
				break;
			}
		}

		if (found == true && valori[r][1] != -999) {
			for (int i = j; i < cap_valori - 1; i++)
				valori[r][i] = valori[r][i + 1];
			valori[r][cap_valori - 1] = -999;
			removed = true;
			return r;
		}

		if (found == true && valori[r][1] == -999) {
			//trebuie sters cu totul
			removed = true;
			if (stanga[r] == -999) {
				int dreptul = dreapta[r];
				chei[r] = -999;
				valori[r][0] = -999;
				dreapta[r] = -999;
				primLiber = r;
				return dreptul;
			}
			else if (dreapta[r] == -999) {
				int stangul = stanga[r];
				chei[r] = -999;
				valori[r][0] = -999;
				stanga[r] = -999;
				primLiber = r;
				return stangul;
			}
			else {
				int max = getMax(stanga[r]);
				chei[r] = chei[max];
				//dreapta[r] = dreapta[max];
				//stanga[r] = stanga[max];
				TElem e2;
				e2.first = chei[max];
				e2.second = valori[max][0];
				stanga[r] = removeElem(stanga[r], e2, removed);
			}
		}
	}
		else if (rel(e.first, chei[r]) == true && stanga[r] != -999)
			stanga[r] = removeElem(stanga[r], e, removed);
		else if (rel(e.first, chei[r]) == false && dreapta[r] != -999)
			dreapta[r] = removeElem(dreapta[r], e, removed);
		return r;
	
}
/*
theta(capacitate)
*/
vector<TCheie> MDO::multimeaCheilor()const {
	int nod_curent = radacina;
	vector<TCheie> mChei;
	while (nod_curent<cap) {
		if(chei[nod_curent] != -999)
		mChei.push_back(chei[nod_curent]);
		nod_curent++;
	}
	return mChei;
}