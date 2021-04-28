#ifndef MUCHOSMUTEX_H
#define MUCHOSMUTEX_H

#include <iostream>
#include <mutex>
//#include <pthread.h>

using namespace std;
class nodo_mutex
{
public:
	int value;
	nodo_mutex* next;
	// pthread_mutex_t my_mutex;
	std::mutex my_mutex;

	nodo_mutex(int valor, nodo_mutex* sigue = NULL)
	{
		my_mutex.lock();
		value = valor;
		next = sigue;
		my_mutex.unlock();
	}
	~nodo_mutex()
	{
		next = NULL;
	}
};

bool ifEqual(nodo_mutex**& next, int value)
{
	return (*next)->value == value;
}

bool ifGreater(nodo_mutex**& next, int value)
{
	if ((*next)->value != value and (*next)->value < value)
	{
		if ((*next)->value > value)
		{
			return true;
		}
		if ((*next)->next == 0 or (*next)->next->value > value)
		{
			next = &(*next)->next;
			return true;
		}
		/*if ((*next)->next->value > value)
		{
			return true;
		}*/
	}
	return false;
}

class lista_mutex
{
public:
	nodo_mutex* Head;
	mutex my_mutex;

	lista_mutex()
	{
		Head = NULL;
	}

	lista_mutex(int valor)
	{
		Head = new nodo_mutex(valor);
	}

	friend bool ifEqual(nodo_mutex**& next, int value);
	friend bool ifGreater(nodo_mutex**& next, int value);
	void operacion(int operacion, int valor) {

		switch (operacion) {
		case 1:
			buscarGenerico(valor);
			break;
		case 2:
			add(valor);
			break;
		case 3:
			borrar(valor);
			break;
		}

	}

	bool buscar(int valor, bool(*p)(nodo_mutex**&, int), nodo_mutex**& next)
	{
		if (Head != 0) {
			// pthread_mutex_lock(&(Head->my_mutex));
			Head->my_mutex.lock();
			next = &Head;
			Head->my_mutex.unlock();
			do
			{
				if (p(next, valor)) {
					//(*next)->my_mutex.unlock();
					return true;
				}

				if ((*next)->next)
					// pthread_mutex_lock(&((*next)->next->my_mutex));
					(*next)->next->my_mutex.lock();
				next = &(*next)->next;
				// pthread_mutex_unlock(&((*next)->my_mutex));
				if ((*next))
					(*next)->my_mutex.unlock();
			} while ((*next));
		}
		return false;
	}
	bool buscarGenerico(int valor) {
		nodo_mutex** next;
		return buscar(valor, ifEqual, next);
	}
	void borrar(int valor)
	{
		nodo_mutex** next;
		if (buscar(valor, ifEqual, next))
		{
			if (Head->value == valor)
			{
				//pthread_mutex_lock(&((*Head)->my_mutex));
				Head->my_mutex.lock();
				if ((Head->next))
					//pthread_mutex_lock(&((*(Head->next))->my_mutex));
					(Head->next)->my_mutex.lock();
				nodo_mutex* next = Head->next;

				delete Head;
				Head = next;
				//pthread_mutex_unlock(&((*Head)->my_mutex));
				Head->my_mutex.unlock();
				return;
			}

			/// bloquear el actual
			// pthread_mutex_lock(&((*next)->my_mutex));
			(*next)->my_mutex.lock();
			if ((*next)->next) /// bloquear el siguiente si existe
			  // pthread_mutex_lock(&((*next)->next->my_mutex));
				(*next)->next->my_mutex.lock();
			nodo_mutex* NNext = (*next)->next;

			delete (*next);
			(*next) = NNext;
			if ((*next)) /// desbloquear el actual si existe
			  // pthread_mutex_unlock(&((*next)->my_mutex));
				(*next)->my_mutex.unlock();
		}
	}


	void add(int valor)
	{
		nodo_mutex** next;
		//if lista vacia
		if (Head == 0) {
			my_mutex.lock();
			Head = new nodo_mutex(valor);
			my_mutex.unlock();
		}
		else if (Head->value > valor) {
			Head->my_mutex.lock();
			Head = new nodo_mutex(valor, Head);
			Head->next->my_mutex.unlock();
		}
		else if (buscar(valor, ifGreater, next)) {
			nodo_mutex** tmp = NULL;
			if ((*next)) {
				tmp = next;
				(*tmp)->my_mutex.lock();
			}
			*next = new nodo_mutex(valor, *next);
			if (tmp)
				(*tmp)->my_mutex.unlock();
			// (*((*next)->next))->my_mutex.unlock();
		}
	}

	~lista_mutex()
	{
		if (Head != 0)
		{
			nodo_mutex* temp = Head;
			nodo_mutex* next;
			do
			{
				next = temp->next;
				delete temp;
				temp = next;
			} while (temp);
		}
	}
};

#endif