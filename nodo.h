#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED
#include <iostream>
using namespace std;

class Nodo
{
public:
  Nodo();
  Nodo *siguiente;
  Nodo *anterior;
  Nodo *Hijo;
  Nodo *Padre;
  string dato;
  bool esTerminal;
  bool Revisado;

};
typedef Nodo *pnodo;


#endif // NODO_H_INCLUDED
