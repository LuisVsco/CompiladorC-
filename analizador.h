#ifndef ANALIZADOR_H_INCLUDED
#define ANALIZADOR_H_INCLUDED
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "semantica.h"
#include "arbol.h"
using namespace std;
class Analizador{
public:
    Semantico ObjSemantico;
    int contParametros,resultadoParametros;
    bool retorna;
    bool varGlobal,varInstruccion,varDeclarada,existente,llamadoFuncion,hayParametros,checarReturn,estaRedeclarada;
    string cadena,contextoPerteneciente,simboloStatement,contextoStatement,funcionPerteneciente,tmpContexto;
    string etiquetaXml,tmpSimbolo;
    ifstream archivo;
    vector<string> paReservadas;
    vector<string> simbolos;
    vector<string> arbolXml,noRetorna,variables;
    vector<string> identificadores,numeros,tipo,tipoDato,contexto,simboloTab,argument,varSinDeclarar,varRedeclarada;
    vector<string> listaContextos,funcParametro,errorParametro,contextoRepetido,funcsReturn;
    vector<int>tokensPro;
    vector<int>gramatica;
    vector<int>numParametros;
    Arbol *objArbol;
    int fallos;
    int posicion;
    Analizador();
    void leerArchivo();
    int funcionClasificar(string);
    void Analizar();
    void AnalizarSintaxis();
    void PalabrasReservadas();
    bool ClasificarNoTerminales();
    bool ClasificarTerminales();
    void TablaDeSimbolos(string);
    void TomarReservada(string);
    bool Programa();
    bool Declaracion();
    bool FuncionVoid();
    bool Argumento();
    bool ListaArgumentos();
    bool CierreFuncion();
    bool FuncionDeclaracion();
    bool FuncionDeclaracion2();
    bool DeclaracionVariable();
    bool DeclaracionVariable2();
    bool DeclaracionVariable3();
    bool ListaVariables();
    bool ListaVariables2();
    bool ExpresionAritmetica();
    bool ExpresionAritmetica1();
    bool ExpresionAritmetica2();
    bool Factor();
    bool InstruccionCondicionyBucle();
    bool Instruccion2();
    bool EstructuraIdentificador();
    bool ListaExpresiones();
    bool EstructuraIF();
    bool EstructuraIF2();
    bool ExpresionComparacion();
    bool ExpresionComparacion1();
    bool ExpresionComparacion2();
    bool FactorComparacion();
    bool CierreIF();
    bool PosibilidadElse();
    bool PosibilidadElse2();
    bool EstructuraFor();
    bool EstructuraFor2();
    bool EstructuraFor3();
    bool EstructuraFor4();
    bool CierreFor();
    bool EstructuraDo();
    bool EstructuraWhile();
    bool EstructuraReturn();
    bool EstructuraIdentificador2();
    bool PosibilidadFuncion();
    bool CierreFuncion2();
    bool EstructuraIdentificador3();
    void Mostrar();
    void FuncionErrores(int);
    void MostrarTabla();
};

#endif // ANALIZADOR_H_INCLUDED
