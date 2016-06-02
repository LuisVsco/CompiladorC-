#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "nododoble.h"
class Lista
{
private:
    NodoDoble* encabezado;
    int contador;
public:
    Lista();
    void inicializa();
    int getContador();
    bool vacia();
    void inserta(NodoDoble* posicion, Pasajero* elemento);
    void Elimina(NodoDoble* posicion);
    NodoDoble* primero();
    NodoDoble* ultimo();
    NodoDoble* anterior(NodoDoble* posicion);
    NodoDoble* siguiente (NodoDoble* posicion);
    void localizaConArbol(Pasajero,ArbolBinarioPasajero*);
    NodoDoble* localiza (Pasajero elemento);
    Pasajero* recupera (NodoDoble* posicion);
    void ordena ();
    void quickSortNombre(NodoDoble**,int,int);
    void anula ();

    friend ostream& operator << (ostream&,Lista&);
    friend istream& operator >> (istream&,Lista&);
};


#endif // LISTA_H_INCLUDED
