
#ifndef MUTEXWRITEREAD_H
#define MUTEXWRITEREAD_H

#include <iostream>
#include <mutex>
#include <shared_mutex>

using namespace std;
class nodo_wr
{
public:
    int value;
    nodo_wr* next;
    nodo_wr(int valor, nodo_wr* sigue = NULL)
    {
        value = valor;
        next = sigue;
    }
    ~nodo_wr()
    {
        next = NULL;
    }
};

bool ifEqual2(nodo_wr**& next, int value)
{
    return (*next)->value == value;
}

bool ifGreater2(nodo_wr**& next, int value)
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

class lista_write_read
{
public:
    nodo_wr* Head;
    std::shared_mutex mutex;
    lista_write_read()
    {
        Head = NULL;
    }

    lista_write_read(int valor)
    {
        Head = new nodo_wr(valor);
    }

    friend bool ifEqual2(nodo_wr**& next, int value);
    friend bool ifGreater2(nodo_wr**& next, int value);

    bool buscar(int valor, bool(*p)(nodo_wr**&, int), nodo_wr**& next)
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
        nodo_wr** next;
        return buscar(valor, ifEqual2, next);

    }
    void operacion(int operacion, int valor) {
        switch (operacion) {
        case 1:
            mutex.lock_shared();
            buscarGenerico(valor);
            mutex.unlock_shared();
            break;
        case 2:
            mutex.lock();
            add(valor);
            mutex.unlock();
            break;
        case 3:
            mutex.lock();
            borrar(valor);
            mutex.unlock();
            break;
        }
    }
    void borrar(int valor)
    {
        nodo_wr** next;
        if (buscar(valor, ifEqual2, next))
        {
            if (Head->value == valor)
            {
                nodo_wr* next = Head->next;
                delete Head;
                Head = next;
                return;
            }
            nodo_wr* NNext = (*next)->next;
            delete (*next);
            (*next) = NNext;
        }
    }

    void add(int valor)
    {
        //if lista vacia
        if (Head == 0)
        {
            Head = new nodo_wr(valor);
            return;
        }
        if (Head->value > valor)
        {
            Head = new nodo_wr(valor, Head);
            return;
        }
        nodo_wr** next;
        if (buscar(valor, ifGreater2, next))
        {
            /*            if (*next == 0)
                        {
                            *next = new nodo(valor);
                            //temp->next = new nodo(valor);
                            return;
                        }*/
            *next = new nodo_wr(valor, *next);
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
            nodo_wr* temp = Head;
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
            cout << "No existe ningun elemento burro" << endl << endl;
        }
    }

    void print_address()
    {
        if (Head != 0)
        {
            cout << "\nDirecciones de memoria" << endl;
            nodo_wr* temp = Head;
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

    ~lista_write_read()
    {
        if (Head != 0)
        {
            nodo_wr* temp = Head;
            nodo_wr* next;
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
