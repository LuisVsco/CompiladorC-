#include "arbol.h"
#include "nodo.h"
#include <iostream>
#include <fstream>
Arbol::Arbol() {}

bool Arbol::EsTerminal(int token)
{
    if(token>=0 && token<=29)
    {
        return true;
    }
    else if(token>=31 && token<=67){
        return false;
    }
    else if(token==68){
        return true;
    }
    return false;
}

void Arbol::Inicializar()
{
    actual=NULL;
    raiz=NULL;
    hijos=0;
    pos=0;
    xml.insert(xml.end(),"<Prog>");
    xml.insert(xml.end(),"</Prog>");
}

void Arbol::InsertarHijo(string dato,int token)
{
    string lista="";
    pnodo nuevo=new Nodo();
    nuevo->dato=dato;
    if(hijos==0)
    {
        actual=nuevo;
        raiz=nuevo;
        actual->siguiente=NULL;
        actual->Hijo=NULL;
        actual->Revisado=false;
        actual->esTerminal=false;
        hijos++;
    }
    else
    {
        if(!EsTerminal(token))
        {
            actual->Revisado=true;
            actual->Hijo=nuevo;
            nuevo->Padre=actual;
            actual=nuevo;
            actual->siguiente=NULL;
            actual->anterior=NULL;
            actual->Hijo=NULL;
            actual->Revisado=false;
            actual->esTerminal=false;
            hijos++;
        }
        else
        {
            actual->Hijo=nuevo;
            nuevo->Padre=actual;
            actual=nuevo;
            actual->siguiente=NULL;
            actual->anterior=NULL;
            actual->Hijo=NULL;
            actual->esTerminal=true;
            hijos++;
        }
    }
}

void Arbol::InsertarHijo2(string dato,int token)
{
    pnodo nuevo=new Nodo();
    nuevo->dato=dato;
    if(!EsTerminal(token))
    {
        actual->siguiente=nuevo;
        actual->siguiente->anterior=actual;
        actual=nuevo;
        actual->siguiente=NULL;
        actual->Revisado=false;
        actual->esTerminal=false;
        hijos++;
    }
    else
    {
        actual->siguiente=nuevo;
        actual->siguiente->anterior=actual;
        actual=nuevo;
        actual->siguiente=NULL;
        actual->esTerminal=true;
        hijos++;
    }
}

void Arbol::RecorrerArbol()
{
    while(actual->anterior!=NULL)
        actual=actual->anterior;

    while(actual->siguiente!=NULL)
    {
        if(!actual->esTerminal && !actual->Revisado)
            break;
        else
            actual=actual->siguiente;
    }

    if(actual->siguiente==NULL && actual->esTerminal)
    {
        while(actual->anterior!=NULL)
            actual=actual->anterior;
        actual=actual->Padre;
        RecorrerArbol();
    }
    else
    {
        if(!actual->esTerminal && !actual->Revisado)
            actual=actual;
        else
        {
            while(actual->anterior!=NULL)
                actual=actual->anterior;
            actual=actual->Padre;
            RecorrerArbol();
        }
    }
}

void Arbol::InsertarNoTerminaXML(string etiqueta)
{
    xml.insert(xml.begin()+pos,"<"+etiqueta+">");
    xml.insert(xml.begin()+pos+1,"</"+etiqueta+">");
}

void Arbol::InsertarTerminalXML(string etiqueta)
{
    xml.insert(xml.begin()+pos,etiqueta);
}

void Arbol::RevisarXML()
{
    while(xml.at(pos).at(1)=='/')
        pos++;
    for(int i=pos; i<xml.size(); i++)
    {
        if(xml.at(i).at(1)=='/')
        {
            string compara="";
            compara=xml.at(i-1);
            compara.insert(compara.begin()+1,'/');
            if(compara==xml.at(i))
            {
                pos=i;
                i=xml.size();
            }
        }
    }
}

void Arbol::InsertarIDNum(string etiqueta,string dato)
{
    xml.insert(xml.begin()+pos,"<"+etiqueta+">"+dato+"</"+etiqueta+">");

}
void Arbol::MostrarXML()
{
    archivoArbol.open("arbol.xml",ios::trunc);
    string cadena="";
    int i;
    for(i=0; i<xml.size(); i++){
        //cout<<xml.at(i)<<endl;
        cadena+=xml.at(i);
    }
    if(archivoArbol.is_open()){
        archivoArbol<<cadena;
        cout<<"Arbol Generado :D"<<endl;
        archivoArbol.close();
    }

}
void Arbol::RevisarXML2()
{
    while(xml.at(pos)!="</Prog>"){
        pos++;
    }
    for(int i=pos; i<xml.size(); i++)
    {
        if(xml.at(i).at(1)=='/')
        {
            string compara="";
            compara=xml.at(i-1);
            compara.insert(compara.begin()+1,'/');
            if(compara==xml.at(i))
            {
                pos=i;
                i=xml.size();
            }
        }
    }
}
