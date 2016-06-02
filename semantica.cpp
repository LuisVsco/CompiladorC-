#include <iostream>
#include "semantica.h"
#include <string>
using namespace std;

Semantico::Semantico(){
//varAnterior = "";
//contextoAnterior ="";

}
bool Semantico::ChecarVariables(string simbolos1,string contexto1,vector<string>simbolos2,vector<string>contexto2){
    unsigned int i;
    for(i=0;i<simbolos2.size();i++){
        if(simbolos1==simbolos2.at(i)&&contexto1==contexto2.at(i)){
            return true;
        }
    }
    return false;
}
bool Semantico::ChecarRedeclaracion(string simbolos1,string contexto1,vector<string>simbolos2,vector<string>contexto2){
   unsigned int i;
    for(i=0;i<simbolos2.size();i++){
        if(simbolos1==simbolos2.at(i)&& contexto1==contexto2.at(i)){
            return true;
        }
    }

    return false;
}
int Semantico::ChecarParametros(string idfuncion,vector<string> parametros, vector<int> contParametros){
unsigned int i,contador;
for(i=0;i<parametros.size();i++){
    if (idfuncion==parametros.at(i)){
        contador=contParametros.at(i);
    }
}
return contador;
}
void Semantico::GeneracionIntermediaFuncion(vector<string>simbolos,vector<string>contexto,vector<string>tipo,vector<string>contexto2){
    //cout<<"entra a generacion"<<endl;
    //cout<<"simbolos: "<<simbolos.data()<<endl;
unsigned int i=0,j=0;
int pila=0,espReservado=0;
while(i<simbolos.size()){
    if(tipo.at(i)=="Funcion"){
       // cout<<"entra????"<<endl;
        cadena+=(simbolos.at(i)+"="+"\n");
        cadena+=" pushq %rbp \n";
        cadena+=" movq %rsp, %rbp \n";
        j=i+1;
        pila=0;
        espReservado=0;
        pila=pila+16;
        while(contexto.at(j)==simbolos.at(i)){
            espReservado++;
            if(espReservado==5){
                pila=pila+16;
                espReservado=0;
            }
            if(j==contexto.size()-1){
                j=i;
            }
            else{
                j++;
            }
        }
        cadena+=" subq ";
        cadena+=pila;
        cadena+=", %rbp \n";
        AsignarNumeracion(contexto2,simbolos.at(i));
        cadena+=" movq %rbp, %rsp \n";
        cadena+=" popq %rbp \n";
        cadena+=" ret\n";
        cadena+=" \n";
    }
    i++;
}
cout<<endl;
cout<<"Esqueleto:D \n\n"<<cadena;
}
void Semantico::AsignarNumeracion(vector<string>contexto,string statement){
int cont=0;
    for(unsigned int i=0;i<contexto.size();i++){
        if(contexto.at(i)==statement){
            cont++;
        }
    }
    if(cont==1){
        cadena+=" movl %edi, -4(%rbp) \n";
    }
    if(cont==2){
        cadena+=" movl %edi, -4(%rbp) \n";
        cadena+=" movl %esi, -8(%rbp) \n";
    }
    if(cont==3){
        cadena+=" movl %edi, -4(%rbp) \n";
        cadena+=" movl %esi, -8(%rbp) \n";
        cadena+=" movl %edx, -12(%rbp) \n";

    }
    if(cont==4){
        cadena+=" movl %edi, -4(%rbp) \n";
        cadena+=" movl %esi, -8(%rbp) \n";
        cadena+=" movl %edx, -12(%rbp) \n";
        cadena+=" movl %ecx ,-16(%rbp) \n";
    }
    if(cont==5){
        cadena+=" movl %edi, -4(%rbp) \n";
        cadena+=" movl %esi, -8(%rbp) \n";
        cadena+=" movl %edx, -12(%rbp) \n";
        cadena+=" movl %ecx, -16(%rbp) \n";
        cadena+=" movl %ebp, -20(%rbp) \n";
    }
    if(cont==6){
        cadena+=" movl %edi, -4(%rbp) \n";
        cadena+=" movl %esi, -8(%rbp) \n";
        cadena+=" movl %edx, -12(%rbp) \n";
        cadena+=" movl %ecx, -16(%rbp) \n";
        cadena+=" movl %ebp, -20(%rbp) \n";
        cadena+=" movl %eip, -24(%rbp) \n";
    }
}
