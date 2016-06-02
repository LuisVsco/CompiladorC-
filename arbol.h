#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED
#include <iostream>
#include <vector>
#include "nodo.h"
#include <fstream>

class Arbol
{
public:
    ofstream archivoArbol;
    pnodo raiz,actual;
    int pos,hijos;
    vector<string> xml;
    bool esTerminalArbol;
    void InsertarHijo(string dato,int token);
    void InsertarHijo2(string dato,int token);
    bool EsTerminal(int token);
    void Inicializar();
    void RecorrerArbol();
    void InsertarTerminalXML(string etiqueta);
    void InsertarNoTerminaXML(string etiqueta);
    void InsertarIDNum(string etiqueta,string dato);
    void RevisarXML();
    void RevisarXML2();
    void MostrarXML();
    void GuardarArbol();
    Arbol();
};


#endif // ARBOL_H_INCLUDED
