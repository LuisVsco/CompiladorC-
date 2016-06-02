#ifndef SEMANTICA_H_INCLUDED
#define SEMANTICA_H_INCLUDED
#include <iostream>
#include <vector>
using namespace std;
class Semantico{
public:
    Semantico();
    string cadena;
    vector<string> varAnterior,contextoAnterior;
    bool ChecarVariables(string,string,vector<string>,vector<string>);
    bool ChecarRedeclaracion(string,string,vector<string>,vector<string>);
    int ChecarParametros(string,vector<string>,vector<int>);
    void GeneracionIntermediaFuncion(vector<string>,vector<string>,vector<string>,vector<string>);
    void AsignarNumeracion(vector<string>,string);
};


#endif // SEMANTICA_H_INCLUDED
