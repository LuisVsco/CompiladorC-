#include <iostream>
#include <stdlib.h>
#include "menu.h"
#include "analizador.h"
enum {ABRIR=1,TABLA_SIMBOLOS,ANALISIS_SINTACTICO,SALIR};
using namespace std;

void Menu::menuPrincipal(){
int opcionMenu;
    Analizador datos;
    bool SeguirPrograma=true;
    bool bandera=false;
    do
    {
        system("cls");
        cout<<"\nAnalizador \n";
        cout<<"1. Leer y analizar"<<endl;
        cout<<"2. Tabla de simbolos"<<endl;
        cout<<"3. Analisis sintactico :v"<<endl;
        //cout<<"4. Analizar Semantica."<<endl;
        cout<<"4. Salir" <<endl <<endl;
        cout<<"Selecciona una opcion: ";
        cin>>opcionMenu;

        switch(opcionMenu)
        {
        case ABRIR:
            datos.leerArchivo();
            datos.Analizar();
            cin.ignore();
            cin.ignore();
            bandera=true;
            break;
        case TABLA_SIMBOLOS:
            if(bandera==true)
            datos.Mostrar();
            cin.ignore();
            cin.ignore();
            break;
        case ANALISIS_SINTACTICO:
            datos.AnalizarSintaxis();
            cin.ignore();
            bandera=true;
            break;
        case SALIR:
            SeguirPrograma=false;
            break;
        default:
            cout<<"\nOPCION INCORRECTA!" <<endl <<endl <<endl;
            cin.ignore();
            break;
        }
        cin.ignore();
    }
    while(SeguirPrograma);
}
