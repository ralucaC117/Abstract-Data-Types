#include "IteratorMDO.h"
#include "MDO.h"
/*
theta(capacitate*m)
*/
IteratorMDO::IteratorMDO(const MDO& d) : dict(d){
	/* de adaugat */
	for (int i = 0; i < d.capacitate; i++) {
		if (d.tabelChei[i] != -10000) {
			Node *nodNou = new Node;
			nodNou->elem = {d.tabelChei[i], d.tabelValori[i]};
			if (sorted == nullptr or d.rel(nodNou->elem.first, sorted->elem.first)) {
				nodNou->next = sorted;
				sorted = nodNou;
			}
			else {
				Node *currentN = sorted;
				while (currentN->next != nullptr and d.rel(currentN->next->elem.first, nodNou->elem.first))
					currentN = currentN->next;
				nodNou->next = currentN->next;
				currentN->next = nodNou;
			}
		}
	}
	curent = sorted;
	indexValori = -1;
}
/*
theta(1)
*/
void IteratorMDO::prim(){
	/* de adaugat */
	curent = sorted;
	indexValori = 0;
}
/*
theta(1)
*/
void IteratorMDO::urmator(){
	/* de adaugat */
	if(!valid())
		throw std::runtime_error("Iteratorul nu e valid");
	if (indexValori < curent->elem.second.size())
		indexValori++;
	else
	{
		curent = curent->next;
		indexValori = 0;
	}
	if (indexValori == curent->elem.second.size())
	{
		curent = curent->next;
		indexValori = 0;
	}

}
/*
theta(1)
*/
bool IteratorMDO::valid() const{
	/* de adaugat */
	if(curent==nullptr)
		return false;
	return true;
}
/*
theta(1)
*/
TElem IteratorMDO::element() const{
	/* de adaugat */
	return pair < TCheie, TValoare > (curent->elem.first, curent->elem.second.at(indexValori));
}

IteratorMDO::~IteratorMDO() {
	Node* current;
	current = curent;
	while (curent != nullptr) {
		current = curent;
		curent = curent->next;
		delete current;
	}
}
