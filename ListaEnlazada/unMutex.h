
#ifndef UNMUTEX_H
#define UNMUTEX_H

#include <iostream>
#include <mutex>
#include <shared_mutex>

using namespace std;
class nodo
{
public:
    int value;
    nodo* next;
    nodo(int valor, nodo* sigue = NULL)
    {
        value = valor;
        next = sigue;
    }
    ~nodo()
    {
        next = NULL;
    }
};

bool ifEqual(nodo**& next, int value)
{
    return (*next)->value == value;
}

bool ifGreater(nodo**& next, int value)
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

class lista_one_mutex
{
public:
    nodo* Head;
    shared_mutex oneMutex;
    lista_one_mutex()
    {
        Head = NULL;
    }

    lista_one_mutex(int valor)
    {
        Head = new nodo(valor);
    }

    friend bool ifEqual(nodo**& next, int value);
    friend bool ifGreater(nodo**& next, int value);

    /*    nodo** buscar(int valor, bool(*p)(nodo*, int))
        {
            if (Head != 0)
            {
                nodo* temp = Head;
                if (Head->value == valor)
                    return &Head;
                do
                {
                    if (p(temp, valor))
                        return &(temp->next);
                    temp = temp->next;
                } while (temp);
            }
            return 0;
        }*/

    bool buscar(int valor, bool(*p)(nodo**&, int), nodo**& next)
    {
        if (Head != 0)
        {
            next = &Head;
            do
            {
                if (p(next, valor))
                    return true;
                next = &(*next)->next;
            } while (*next);
        }
        return false;
    }
    bool buscarGenerico(int valor) {
        nodo** next;
        return buscar(valor, ifEqual, next);

    }
    void operacion(int operacion, int valor) {
        
        switch (operacion) {
        case 1:
            oneMutex.lock();
            buscarGenerico(valor);
            oneMutex.unlock();
            break;
        case 2:
            oneMutex.lock();
            add(valor);
            oneMutex.unlock();
            break;
        case 3:
            oneMutex.lock();
            borrar(valor);
            oneMutex.unlock();
            break;
        }
        //oneMutex.unlock();
    }
    void borrar(int valor)
    {
        nodo** next;
        if (buscar(valor, ifEqual, next))
        {
            if (Head->value == valor)
            {
                nodo* next = Head->next;
                delete Head;
                Head = next;
                return;
            }
            nodo* NNext = (*next)->next;
            delete (*next);
            (*next) = NNext;
        }
    }

    void add(int valor)
    {
        //if lista vacia
        if (Head == 0)
        {
            Head = new nodo(valor);
            return;
        }
        if (Head->value > valor)
        {
            Head = new nodo(valor, Head);
            return;
        }
        nodo** next;
        if (buscar(valor, ifGreater, next))
        {
            /*            if (*next == 0)
                        {
                            *next = new nodo(valor);
                            //temp->next = new nodo(valor);
                            return;
                        }*/
            *next = new nodo(valor, *next);
            //if (temp->value != valor && temp->next->value != valor)
            //{
            //	temp->next = new nodo(valor, temp->next);
            //}
        }
    }

    void print()
    {
        if (Head != 0)
        {
            cout << "\nValores dentro de la lista\n";
            nodo* temp = Head;
            do
            {
                cout << temp->value;
                if (temp->next)
                    cout << " -> " << temp->next->value;
                cout << endl;
                temp = temp->next;
            } while (temp);
        }
        else
        {
            cout << "No existe ningun elemento" << endl << endl;
        }
    }

    void print_address()
    {
        if (Head != 0)
        {
            cout << "\nDirecciones de memoria" << endl;
            nodo* temp = Head;
            do
            {
                cout << temp;
                if (temp->next)
                    cout << " -> " << temp->next;
                cout << endl;
                temp = temp->next;
            } while (temp);
            cout << endl;
        }
    }

    ~lista_one_mutex()
    {
        if (Head != 0)
        {
            nodo* temp = Head;
            nodo* next;
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
