#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include "analizador.h"
#include "semantica.h"
#include "arbol.h"

using namespace std;

enum {q0,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q13,q15,
      q16,q17,q18,q19,q20,q21,q22,q23,q24,q25,q26,q27,q28,q29,q30,Estados
     };

enum
{
    tokenId,tokenNum,tokenPlus,tokenMinus,tokenTimes,tokenDivided,tokenMod,tokenGt,
    tokenLt,tokenNeg,tokenEq,tokenLEt,tokenGEt,tokenAsig,tokenDif,tokenPrL,tokenPrR,
    tokenKeyL,tokenKeyR,tokenEnd,tokenComa,tokenInv,RW_VOID,RW_INT,RW_IF,RW_ELSE,RW_FOR,
    RW_WHILE,RW_DO,RW_RETURN
};
std::map<int, std::string> mapaSimbolos =
{
    {tokenId, "Id"},
    {tokenNum, "NUM"},
    {tokenPlus, "PLUS"},
    {tokenMinus, "MENOS"},
    {tokenTimes, "MULT"},
    {tokenDivided, "DIV"},
    {tokenMod, "MOD"},
    {tokenGt, "mayor que"},
    {tokenLt,"menor que"},
    {tokenNeg,"neg"},
    {tokenEq,"igual"},
    {tokenLEt,"menorigual"},
    {tokenGEt,"mayoigual"},
    {tokenAsig,"ASIG"},
    {tokenDif,"diferente"},
    {tokenPrL,"parentesisizq"},
    {tokenPrR,"parentesisDER"},
    {tokenKeyL,"llave izq"},
    {tokenKeyR,"llaveder"},
    {tokenEnd,"punto y coma"},
    {tokenComa,"coma"},
    {tokenInv,"invalido"},
    {RW_VOID,"voidpr"},
    {RW_INT,"intPR"},
    {RW_IF,"IF"},
    {RW_ELSE,"else"},
    {RW_FOR,"FORpr"},
    {RW_WHILE,"whilePR"},
    {RW_DO,"DOpr"},
    {RW_RETURN,"return"}
};
enum /*NO TERMINALES*/
{

    PROGRAMA=31,
    DECLARACION,
    FUNCIONVOID,
    ARGUMENTO,
    LISTA_ARGUMENTOS,
    FUNCIONDECLARACION,
    FUNCIONDECLARACION2,
    DECLARACIONVARIABLE,
    DECLARACIONVARIABLE2,
    DECLARACIONVARIABLE3,
    LISTA_VARIABLES,
    LISTA_VARIABLES2,
    EXPRESION_ARITMETICA,
    EXPRESION_ARITMETICA1,
    EXPRESION_ARITMETICA2,
    FACTOR,
    CIERREFUNCION,
    INSTRUCCION,
    INSTRUCCION2,
    ESTRUCTURA_IDENTIFICADOR,
    LISTA_EXPRESIONES,
    ESTRUCTURAIF,
    ESTRUCTURAIF2,
    EXPRESION_COMPARACION,
    EXPRESION_COMPARACION1,
    EXPRESION_COMPARACION2,
    FACTOR_COMPARACION,
    CIERREIF,
    POSIBILIDADELSE,
    POSIBILIDADELSE2,
    ESTRUCTURAFOR,
    ESTRUCTURAFOR2,
    ESTRUCTURAFOR3,
    ESTRUCTURAFOR4,
    CIERREFOR,
    ESTRUCTURADO,
    ESTRUCTURAWHILE,
    ESTRUCTURARETURN,
    ESTRUCTURAIDENTIDICADOR2,
    POSIBILIDADFUNCION,
    CIERREFUNCION2,
    ESTRUCTURAIDENTIDICADOR3
};

int tabTrans[Estados][20]=
{
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q29,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q4,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q4,q4,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q23,q10,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q0,q11,q30},
    {q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q11,q0,q11,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q16,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q5,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q19,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q21,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q21,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q22,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q21,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q21,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q23,q23,q23,q23,q24,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q30},
    {q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q23,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q29,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30},
    {q1,q2,q3,q5,q6,q7,q8,q17,q18,q20,q15,q9,q13,q25,q26,q27,q0,q0,q28,q30}
};

int tabSalidas[Estados][20]=
{
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId},
    {-1,-1,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum,tokenNum},
    {tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus,tokenPlus},
    {-1,-1,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv,tokenInv},
    {tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus,tokenMinus},
    {tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes,tokenTimes},
    {tokenDivided,tokenDivided,tokenDivided,tokenDivided,-1,-1,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided,tokenDivided},
    {tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod,tokenMod},
    {tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL,tokenPrL},
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrR,tokenPrL},
    {tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,-1,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig,tokenAsig},
    {tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq,tokenEq},
    {tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,-1,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt,tokenLt},
    {tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,-1,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt,tokenGt},
    {tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt,tokenLEt},
    {tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,-1,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg,tokenNeg},
    {tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt,tokenGEt},
    {tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif,tokenDif},
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL,tokenKeyL},
    {tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR,tokenKeyR},
    {tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd,tokenEnd},
    {tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa,tokenComa},
    {-1,-1,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId,tokenId},
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}

};
Analizador::Analizador() {}

int Analizador::funcionClasificar(string sim)
{

    if (sim>="a" && sim<="z")
    {
        return 0;
    }
    else if(sim>="A" && sim<="Z")
    {
        return 0;
    }
    else if(sim=="_")
    {
        return 0;

    }
    else if(sim>="0" && sim<="9")
    {
        return 1;
    }
    else if(sim=="+")
    {

        return 2;
    }
    else if(sim=="-")
    {
        return 3;
    }
    else if(sim=="*")
    {
        return 4;
    }
    else if(sim=="/")
    {
        return 5;
    }
    else if(sim=="%")
    {
        return 6;
    }
    else if(sim=="<")
    {
        return 7;
    }
    else if(sim==">")
    {
        return 8;
    }
    else if(sim=="!")
    {
        return 9;
    }
    else if(sim=="=")
    {
        return 10;
    }
    else if(sim=="(")
    {
        return 11;
    }
    else if(sim==")")
    {
        return 12;
    }
    else if(sim=="{")
    {
        return 13;
    }
    else if(sim=="}")
    {
        return 14;
    }
    else if(sim==";")
    {
        return 15;
    }
    else if(sim==" "||sim=="\t")
    {
        return 16;
    }
    else if(sim=="\n")
    {
        return 17;
    }
    else if(sim==",")
    {
        return 18;
    }
    else
    {
        return 19;
    }
}

void Analizador::leerArchivo()
{
    string linea;
    string nombreArch;
    cout<<"dame el nombre del archivo: ";
    cin>>nombreArch;
    archivo.open(nombreArch, ios::in);
    cadena="";
    while(!archivo.eof())
    {
        getline(archivo,linea);
        cadena+=linea+"\n";

        if (archivo.eof())
        {
            break;
        }

    }

    cout<<cadena<<endl;
    archivo.close();



}
void Analizador::TomarReservada(string copiaCadena)
{
    /*RW_VOID,RW_INT,RW_IF,RW_ELSE,RW_FOR,
        RW_WHILE,RW_DO,RW_RETURN*/
    if (copiaCadena=="void")
        tokensPro.insert(tokensPro.end(),RW_VOID);
    if (copiaCadena=="int")
        tokensPro.insert(tokensPro.end(),RW_INT);
    if (copiaCadena=="if")
        tokensPro.insert(tokensPro.end(),RW_IF);
    if (copiaCadena=="else")
        tokensPro.insert(tokensPro.end(),RW_ELSE);
    if (copiaCadena=="for")
        tokensPro.insert(tokensPro.end(),RW_FOR);
    if (copiaCadena=="while")
        tokensPro.insert(tokensPro.end(),RW_WHILE);
    if (copiaCadena=="do")
        tokensPro.insert(tokensPro.end(),RW_DO);
    if (copiaCadena=="return")
        tokensPro.insert(tokensPro.end(),RW_RETURN);

}
void Analizador::Analizar()
{
    tokensPro.clear();
    identificadores.clear();
    numeros.clear();
    simbolos.clear();
    PalabrasReservadas();
    int tamCad,estado,in,out,errores=0;
    int i;
    bool bandera=false;
    string caracterCadena,copiaCadena="";

    tamCad=cadena.size();
    estado=q0;


    for(i=0; i<tamCad; i++)
    {
        caracterCadena=cadena.at(i);
        in=funcionClasificar(caracterCadena);


        out=tabSalidas[estado][in];
        if(out!=-1)
        {

            switch(out)
            {
            case 0:
                for(unsigned int i=0; i<paReservadas.size(); i++)
                {
                    if(copiaCadena==paReservadas.at(i))
                        bandera=true;
                }
                if(bandera==true)
                {
                    TomarReservada(copiaCadena);
                    cout<<copiaCadena<<" : Palabra Reservada"<<endl;
                }
                else
                {
                    identificadores.insert(identificadores.end(),copiaCadena);
                    tokensPro.insert(tokensPro.end(),tokenId);
                    cout<<copiaCadena<<" : Identificador"<<endl;
                    TablaDeSimbolos(copiaCadena);
                }
                copiaCadena="";
                break;
            case 1:
                numeros.insert(numeros.end(),copiaCadena);
                tokensPro.insert(tokensPro.end(),tokenNum);
                cout<<copiaCadena<<" : Numero"<<endl;
                copiaCadena="";
                break;
            case 2:
                tokensPro.insert(tokensPro.end(),tokenPlus);
                cout<<copiaCadena<<" : Op Aritmetico Suma"<<endl;
                copiaCadena="";
                break;
            case 3:
                tokensPro.insert(tokensPro.end(),tokenMinus);
                cout<<copiaCadena<<" : Op Aritmetico Resta"<<endl;
                copiaCadena="";
                break;
            case 4:
                tokensPro.insert(tokensPro.end(),tokenTimes);
                cout<<copiaCadena<<" : Op Aritmetico Multiplicacion"<<endl;
                copiaCadena="";
                break;
            case 5:
                tokensPro.insert(tokensPro.end(),tokenDivided);

                cout<<copiaCadena<<" : Op Aritmetico Division"<<endl;
                copiaCadena="";
                break;
            case 6:
                tokensPro.insert(tokensPro.end(),tokenMod);

                cout<<copiaCadena<<" : Op Aritmetico Modulo"<<endl;
                copiaCadena="";
                break;
            case 7:
                tokensPro.insert(tokensPro.end(),tokenGt);

                cout<<copiaCadena<<" : Op Mayor que"<<endl;
                copiaCadena="";
                break;
            case 8:
                tokensPro.insert(tokensPro.end(),tokenLt);

                cout<<copiaCadena<<" : Op Menor que"<<endl;
                copiaCadena="";
                break;
            case 9:
                tokensPro.insert(tokensPro.end(),tokenNeg);

                cout<<copiaCadena<<" : Op Negacion"<<endl;
                copiaCadena="";
                break;
            case 10:
                tokensPro.insert(tokensPro.end(),tokenEq);

                cout<<copiaCadena<<" : Igual"<<endl;
                copiaCadena="";
                break;
            case 11 :
                tokensPro.insert(tokensPro.end(),tokenLEt);

                cout<<copiaCadena<<" : Op Menor Igual"<<endl;
                copiaCadena="";
                break;
            case 12:
                tokensPro.insert(tokensPro.end(),tokenGEt);

                cout<<copiaCadena<<" : Op Mayor Igual"<<endl;
                copiaCadena="";
                break;
            case 13:
                tokensPro.insert(tokensPro.end(),tokenAsig);

                cout<<copiaCadena<<" : Asignacion"<<endl;
                copiaCadena="";
                break;
            case 14:
                tokensPro.insert(tokensPro.end(),tokenDif);

                cout<<copiaCadena<<" : Diferente"<<endl;
                copiaCadena="";
                break;
            case 15:
                tokensPro.insert(tokensPro.end(),tokenPrL);

                cout<<copiaCadena<<" : Parentesis izquierda"<<endl;
                copiaCadena="";
                break;
            case 16:
                tokensPro.insert(tokensPro.end(),tokenPrR);

                cout<<copiaCadena<<" : Parentesis derecho"<<endl;
                copiaCadena="";
                break;
            case 17:
                tokensPro.insert(tokensPro.end(),tokenKeyL);

                cout<<copiaCadena<<" : Llave izquierda"<<endl;
                copiaCadena="";
                break;
            case 18:
                tokensPro.insert(tokensPro.end(),tokenKeyR);

                cout<<copiaCadena<<" : Llave derecha"<<endl;
                copiaCadena="";
                break;
            case 19:
                tokensPro.insert(tokensPro.end(),tokenEnd);

                cout<<copiaCadena<<" : Punto y Coma"<<endl;
                copiaCadena="";
                break;
            case 20:
                tokensPro.insert(tokensPro.end(),tokenComa);

                cout<<copiaCadena<<" : Coma"<<endl;
                copiaCadena="";
                break;
            case 21:
                errores++;
                tokensPro.insert(tokensPro.end(),tokenInv);

                cout<<copiaCadena<<" : Invalido"<<endl;
                copiaCadena="";
                break;
            default:
                cout<<copiaCadena<<" : Error"<<endl;
                copiaCadena="";
                break;

            }
        }

        estado=tabTrans[estado][in];
        if(caracterCadena!="\t" && caracterCadena!=" " && caracterCadena!="\n" && estado!=q23 && estado!=q10 && estado!=q11 && estado!=q24)
        {
            copiaCadena+=caracterCadena;
            if(estado==q30)
            {
                errores++;
                cout<<copiaCadena<<": Cadena o Simbolo no encontrado"<<endl;
                copiaCadena="";
            }
        }
        else
            copiaCadena="";
        bandera=false;
    }
    if(errores==0)
    {
        cout<<"sin errores"<<endl;
    }
    else
    {
        cout<<"hay errores "<<endl;
    }
    //cin.ignore();

}
void Analizador::AnalizarSintaxis()
{
    varGlobal=true;
    retorna=false;
    hayParametros=false;
    llamadoFuncion=false;

    simboloTab.clear();
    tipo.clear();
    tipoDato.clear();
    argument.clear();
    contexto.clear();

    simboloStatement="";
    contextoStatement="";
    contextoPerteneciente="";
    funcionPerteneciente="";

    resultadoParametros=0;

    varSinDeclarar.clear();
    listaContextos.clear();
    funcParametro.clear();
    numParametros.clear();
    errorParametro.clear();
    noRetorna.clear();
    funcsReturn.clear();
    contextoRepetido.clear();
    varRedeclarada.clear();
    variables.clear();
    varSinDeclarar.insert(varSinDeclarar.begin()," ");
    listaContextos.insert(listaContextos.begin()," ");


    bool terminal=false,noTerminal=false;
    objArbol=new Arbol();
    gramatica.clear();
    fallos=0;
    gramatica.insert(gramatica.begin(),PROGRAMA);
    objArbol->Inicializar();
    objArbol->InsertarHijo("Prog",PROGRAMA);
    while(gramatica.size()!=0)
    {
        if(tokensPro.size()!=0)
        {
            //cout<<"muere"<<endl;
            noTerminal=ClasificarNoTerminales();
            terminal=ClasificarTerminales();
            //cout<<noTerminal<<" : noterminal"<<endl;
            //cout<<terminal<<" : terminal"<<endl<<endl;
            if(noTerminal==false && terminal==false)
            {
                FuncionErrores(gramatica.at(0));
                gramatica.clear();
            }
            else
            {
                noTerminal=false;
                terminal=false;
            }
        }
        else
        {
            if(gramatica.size()!=1)
            {
                FuncionErrores(gramatica.at(0));
                gramatica.clear();
            }
            gramatica.clear();
        }
    }
    bool bandera=true;
    //cout<<"varibales sin declarar: "<<varSinDeclarar.back()<<endl;
    if (varSinDeclarar.size()!=1){
        bandera=false;
        cout<<"Semantica incorrecta D:"<<endl<<endl;
        for(unsigned int i=1;i<varSinDeclarar.size();i++){
            if(listaContextos.at(i)=="Global"){
                cout<<"Funcion: ' "<<varSinDeclarar.at(i)<<" ' sin declarar"<<endl;
            }
            else{
                cout<<"En contexto ' "<<listaContextos.at(i)<<" ' variable ' "<<varSinDeclarar.at(i)<<"' sin declarar"<<endl;
            }
        }
        //cin.ignore();
        if(errorParametro.size()!=0){
                bandera=false;
            for(unsigned int j=0;j<errorParametro.size();j++){
                cout<<"' "<<errorParametro.at(j)<<" ' funcion fallo argumentos"<<endl;
            }
            //cin.ignore();
        }
        if(varRedeclarada.size()!=0){
            bandera=false;
            for(unsigned int l=0;l<varRedeclarada.size();l++){
                if(contextoRepetido.at(l)=="Global"){
                    cout<<"Redeclaracion de funcion: "<<varRedeclarada.at(l)<<endl;
                }
                else{
                    cout<<"En contexto: "<<contextoRepetido.at(l)<<" "<<"' "<<varRedeclarada.at(l)<<" ' variable redeclarada"<<endl;
                }
            }
        }
        if(noRetorna.size()!=0){
            bandera=false;
            for(unsigned int m=0;m<noRetorna.size();m++){
                cout<<"Contexto: "<<noRetorna.at(m)<<" no retorna"<<endl;
            }
        }

    }
    else{
        if(errorParametro.size()!=0){
                bandera=false;
            for(unsigned int k=0;k<errorParametro.size();k++){
                cout<<"' "<<errorParametro.at(k)<<"  ' funcion fallo argumentos"<<endl;
            }
        }
        if(varRedeclarada.size()!=0){
            bandera=false;
            for(unsigned int m=0;m<varRedeclarada.size();m++){
                if(contextoRepetido.at(m)=="Global"){
                    cout<<"Redeclaracion de funcion: "<<varRedeclarada.at(m)<<endl;
                }
                else{
                    cout<<"En contexto: "<<contextoRepetido.at(m)<<" "<<"' "<<varRedeclarada.at(m)<<" ' variable redeclarada"<<endl;
                }
            }
        }
        if(noRetorna.size()!=0){
            bandera=false;
            for(unsigned int n=0;n<noRetorna.size();n++){
                cout<<"funcion: "<<noRetorna.at(n)<<" no retorna ni madres"<<endl;
            }
        }
    }

    if(fallos==0 && bandera==true){
            cout<<"sintaxis correcta :D \n"<<endl;
            objArbol->MostrarXML();
            cout<<endl;
            MostrarTabla();
            cout<<endl;
            cout<<"semantica correcta :3"<<endl;
           /* for(unsigned int i=0;i<simboloTab.size();i++)
            cout<<"simbolos: "<<simboloTab.at(i)<<endl;
            cout<<"\n";
            for(unsigned int i=0;i<variables.size();i++)
            cout<<"simbolos: "<<variables.at(i)<<endl;
            cout<<"\n";
            for(unsigned int i=0;i<contexto.size();i++)
            cout<<"simbolos: "<<contexto.at(i)<<endl;
            cout<<"\n";
            for(unsigned int i=0;i<tipo.size();i++)
            cout<<"simbolos: "<<tipo.at(i)<<endl;
            cout<<"\n";
            for(unsigned int i=0;i<simboloTab.size();i++)
            cout<<"simbolos: "<<simboloTab.at(i)<<endl;
            cout<<"\n";*/
            ObjSemantico.GeneracionIntermediaFuncion(simboloTab,contexto,tipo,variables);
            //cout<<"sale"<<endl;
            cin.ignore();
            //cout<<"BACK: "<<simboloTab.back()<<endl;

    }




}

bool Analizador::ClasificarNoTerminales()
{

    if(gramatica.at(0)== PROGRAMA)
    {
        return Programa();

    }
    if(gramatica.at(0)== DECLARACION)
    {
        return  Declaracion();

    }
    if(gramatica.at(0)== FUNCIONVOID)
    {
        return  FuncionVoid();

    }
    if(
        gramatica.at(0)== ARGUMENTO)
    {
        return  Argumento();

    }

    if(gramatica.at(0)== LISTA_ARGUMENTOS)
    {
        return  ListaArgumentos();

    }

    if(gramatica.at(0)== CIERREFUNCION)
    {
        return  CierreFuncion();

    }

    if(gramatica.at(0)== FUNCIONDECLARACION)
    {
        return  FuncionDeclaracion();

    }

    if(gramatica.at(0)== FUNCIONDECLARACION2)
    {
        return  FuncionDeclaracion2();

    }

    if(gramatica.at(0)== DECLARACIONVARIABLE)
    {
        return  DeclaracionVariable();

    }

    if(gramatica.at(0)== DECLARACIONVARIABLE2)
    {
        return  DeclaracionVariable2();

    }
    if(gramatica.at(0)== DECLARACIONVARIABLE3)
    {
        return  DeclaracionVariable3();

    }
    if(gramatica.at(0)== LISTA_VARIABLES)
    {
        return  ListaVariables();

    }
    if(gramatica.at(0)== LISTA_VARIABLES2)
    {
        return  ListaVariables2();

    }

    if(gramatica.at(0)== EXPRESION_ARITMETICA)
    {
        return  ExpresionAritmetica();

    }

    if(gramatica.at(0)== EXPRESION_ARITMETICA1)
    {
        return  ExpresionAritmetica1();

    }

    if(
        gramatica.at(0)== EXPRESION_ARITMETICA2)
    {
        return  ExpresionAritmetica2();

    }

    if(gramatica.at(0)== FACTOR)
    {
        return  Factor();

    }

    if(gramatica.at(0)== INSTRUCCION)
    {
        return  InstruccionCondicionyBucle();

    }

    if(gramatica.at(0)== INSTRUCCION2)
    {
        return  Instruccion2();

    }

    if(gramatica.at(0)== ESTRUCTURA_IDENTIFICADOR)
    {
        return  EstructuraIdentificador();

    }

    if(gramatica.at(0)== LISTA_EXPRESIONES)
    {
        return  ListaExpresiones();

    }

    if(gramatica.at(0)== ESTRUCTURAIF)
    {
        return  EstructuraIF();

    }

    if(gramatica.at(0)== ESTRUCTURAIF2)
    {
        return  EstructuraIF2();

    }

    if(gramatica.at(0)== EXPRESION_COMPARACION)
    {
        return  ExpresionComparacion();

    }

    if(gramatica.at(0)== EXPRESION_COMPARACION1)
    {
        return  ExpresionComparacion1();

    }

    if(gramatica.at(0)== EXPRESION_COMPARACION2)
    {
        return  ExpresionComparacion2();

    }

    if(gramatica.at(0)== FACTOR_COMPARACION)
    {
        return  FactorComparacion();

    }

    if(gramatica.at(0)== CIERREIF)
    {
        return  CierreIF();

    }

    if(gramatica.at(0)== POSIBILIDADELSE)
    {
        return  PosibilidadElse();

    }

    if(gramatica.at(0)== POSIBILIDADELSE2)
    {
        return  PosibilidadElse2();

    }

    if(gramatica.at(0)== ESTRUCTURAFOR)
    {
        return  EstructuraFor();

    }

    if(gramatica.at(0)== ESTRUCTURAFOR2)
    {
        return  EstructuraFor2();

    }

    if(gramatica.at(0)== ESTRUCTURAFOR3)
    {
        return  EstructuraFor3();

    }

    if(gramatica.at(0)== ESTRUCTURAFOR4)
    {
        return  EstructuraFor4();

    }

    if(gramatica.at(0)== CIERREFOR)
    {
        return  CierreFor();

    }

    if(gramatica.at(0)== ESTRUCTURADO)
    {
        return  EstructuraDo();

    }

    if(gramatica.at(0)== ESTRUCTURAWHILE)
    {
        return  EstructuraWhile();

    }

    if(gramatica.at(0)== ESTRUCTURARETURN)
    {
        return  EstructuraReturn();

    }
    if(gramatica.at(0)==ESTRUCTURAIDENTIDICADOR2)
    {

        return EstructuraIdentificador2();

    }
    if(gramatica.at(0)==POSIBILIDADFUNCION){
        return PosibilidadFuncion();
    }
    if(gramatica.at(0)==CIERREFUNCION2){
        return CierreFuncion2();
    }
    if(gramatica.at(0)==ESTRUCTURAIDENTIDICADOR3){
        return EstructuraIdentificador3();
    }
    return false;
}

bool Analizador::Programa()
{
    varInstruccion=false;
    hayParametros=false;
    contParametros=0;
    resultadoParametros=0;
    llamadoFuncion=false;
    retorna=false;
    existente=false;
    objArbol->RevisarXML2();
    if(tokensPro.size()!=0)
    {
        objArbol->InsertarHijo("Definicion",DECLARACION);
        objArbol->InsertarHijo2("Prog",PROGRAMA);
        objArbol->InsertarNoTerminaXML("Prog");
        objArbol->InsertarNoTerminaXML("Definicion");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),PROGRAMA);
        gramatica.insert(gramatica.begin(),DECLARACION);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::Declaracion()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)==RW_VOID)
    {
        varGlobal=false;
        //contexto.insert(contexto.end(),"Global");
        tipo.insert(tipo.end(),"Funcion");
        tipoDato.insert(tipoDato.end(),"void");
        tmpContexto="Global";
        objArbol->InsertarHijo("void",RW_VOID);
        objArbol->InsertarHijo2("FuncionVoid",FUNCIONVOID);
        objArbol->InsertarNoTerminaXML("FuncionVoid");
        objArbol->InsertarTerminalXML("<RW>void</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FUNCIONVOID);
        gramatica.insert(gramatica.begin(),RW_VOID);
        return true;
    }
    if(tokensPro.at(0)==RW_INT)
    {
        tipoDato.insert(tipoDato.end(),"int");
        objArbol->InsertarHijo("int",RW_INT);
        objArbol->InsertarHijo2("FuncionDeclaracion",FUNCIONDECLARACION);
        objArbol->InsertarNoTerminaXML("FuncionDeclaracion");
        objArbol->InsertarTerminalXML("<RW>int</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FUNCIONDECLARACION);
        gramatica.insert(gramatica.begin(),RW_INT);
        return true;
    }
    return false;
}

bool Analizador::FuncionVoid()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)==tokenId)
    {
        objArbol->InsertarHijo("Id",tokenId);
        objArbol->InsertarHijo2("(",tokenPrL);
        objArbol->InsertarHijo2("Argumento",ARGUMENTO);
        objArbol->InsertarHijo2(")",FUNCIONVOID);
        objArbol->InsertarHijo2("CerrarFuncion",CIERREFUNCION);
        objArbol->InsertarNoTerminaXML("CerrarFuncion");
        objArbol->InsertarNoTerminaXML("Argumento");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        //simboloTab.insert(simboloTab.end(),identificadores.at(0));
        tmpSimbolo=identificadores.at(0);
        estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
        if(!estaRedeclarada){
            simboloTab.insert(simboloTab.end(),tmpSimbolo);
            contexto.insert(contexto.end(),tmpContexto);
        }
        else{
            varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
            contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
        }
        funcParametro.insert(funcParametro.end(),identificadores.at(0));
        contextoPerteneciente=identificadores.at(0);
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),CIERREFUNCION);
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),ARGUMENTO);
        gramatica.insert(gramatica.begin(),tokenPrL);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }
    return false;
}

bool Analizador::Argumento()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)==RW_INT)
    {
        resultadoParametros++;
        objArbol->InsertarHijo("int",RW_INT);
        objArbol->InsertarHijo2("Id",tokenId);
        objArbol->InsertarHijo2("ListaArgumentos",LISTA_ARGUMENTOS);
        objArbol->InsertarNoTerminaXML("ListaArgumentos");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        objArbol->InsertarTerminalXML("<RW>int</RW>");
        argument.insert(argument.end(),"int");
        tipo.insert(tipo.end(),"Variable");
        tipoDato.insert(tipoDato.end(),"int");
        //contexto.insert(contexto.end(),contextoPerteneciente);
        //simboloTab.insert(simboloTab.end(),identificadores.at(0));
        tmpSimbolo=identificadores.at(0);
        tmpContexto=contextoPerteneciente;
        estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
        if(!estaRedeclarada){
            simboloTab.insert(simboloTab.end(),tmpSimbolo);
            contexto.insert(contexto.end(),tmpContexto);
            variables.insert(variables.end(),tmpContexto);
        }
        else{
            varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
            contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
        }
        argument.insert(argument.end()," ");
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),LISTA_ARGUMENTOS);
        gramatica.insert(gramatica.begin(),tokenId);
        gramatica.insert(gramatica.begin(),RW_INT);
        return true;
    }
    argument.insert(argument.end()," ");
    //numParametros.insert(numParametros.end(),resultadoParametros);
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::ListaArgumentos()
{
    objArbol->RecorrerArbol();
      objArbol->RevisarXML();
    if(tokensPro.at(0)==tokenComa)
    {
        resultadoParametros++;
        objArbol->InsertarHijo(",",tokenComa);
        objArbol->InsertarHijo2("int",RW_INT);
        objArbol->InsertarHijo2("Id",tokenId);
        objArbol->InsertarHijo2("ListaArgumentos",LISTA_ARGUMENTOS);
        objArbol->InsertarNoTerminaXML("ListaArgumentos");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        objArbol->InsertarTerminalXML("<RW>int</RW>");
        //simboloTab.insert(simboloTab.end(),identificadores.at(0));
        tipo.insert(tipo.end(),"variable");
        tipoDato.insert(tipoDato.end(),"int");
        //contexto.insert(contexto.end(),contextoPerteneciente);
        tmpSimbolo=identificadores.at(0);
        tmpContexto=contextoPerteneciente;
        estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
        if(!estaRedeclarada){
            simboloTab.insert(simboloTab.end(),tmpSimbolo);
            contexto.insert(contexto.end(),tmpContexto);
            variables.insert(variables.end(),tmpContexto);
        }
        else{
            varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
            contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
        }
        argument.insert(argument.end()," ");
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),LISTA_ARGUMENTOS);
        gramatica.insert(gramatica.begin(),tokenId);
        gramatica.insert(gramatica.begin(),RW_INT);
        gramatica.insert(gramatica.begin(),tokenComa);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::CierreFuncion()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    numParametros.insert(numParametros.end(),resultadoParametros);
    if(tokensPro.at(0) == tokenEnd )
    {
         objArbol->InsertarHijo(";",tokenEnd);
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        return true;
    }
    if(tokensPro.at(0) == tokenKeyL )
    {
        objArbol->InsertarHijo("{",tokenKeyL);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarHijo2("}",tokenKeyR);
          objArbol->InsertarNoTerminaXML("Instruccion");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenKeyR);
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),tokenKeyL);
        return true;
    }

    return false;
}

bool Analizador::FuncionDeclaracion()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenId )
    {

         objArbol->InsertarHijo("Id",tokenId);
          objArbol->InsertarHijo2("FuncionDeclaracion2",FUNCIONDECLARACION2);
          objArbol->InsertarNoTerminaXML("FuncionDeclaracion2");
          objArbol->InsertarIDNum("Id",identificadores.at(0));
          //simboloTab.insert(simboloTab.end(),identificadores.at(0));
          tmpSimbolo=identificadores.at(0);
          identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FUNCIONDECLARACION2);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }
    return false;
}

bool Analizador::FuncionDeclaracion2()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenPrL )
    {
        varGlobal=false;
        objArbol->InsertarHijo("(",tokenPrL);
        objArbol->InsertarHijo2("Argumento",ARGUMENTO);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarHijo2("CerrarFuncion",CIERREFUNCION2);
        objArbol->InsertarNoTerminaXML("CerrarFuncion");
        objArbol->InsertarNoTerminaXML("Argumento");
        tipo.insert(tipo.end(),"Funcion");
        //contexto.insert(contexto.end(),"Global");
        tmpContexto="Global";
        estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
        if(!estaRedeclarada){
            simboloTab.insert(simboloTab.end(),tmpSimbolo);
            contexto.insert(contexto.end(),tmpContexto);
            funcsReturn.insert(funcsReturn.end(),tmpSimbolo);
        }
        else{
            varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
            contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
        }
        contextoPerteneciente=simboloTab.back();
        funcParametro.insert(funcParametro.end(),simboloTab.back());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),CIERREFUNCION2);
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),ARGUMENTO);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }
    objArbol->InsertarHijo("DeclaracionVariable",DECLARACIONVARIABLE);
    objArbol->InsertarNoTerminaXML("DeclaracionVariable");
    if (varGlobal){
        tmpContexto="Global";
        //contexto.insert(contexto.end(),"Global");
        }
    else{
          if(!varInstruccion){
        contextoPerteneciente=simboloTab.back();
        tmpContexto=contextoPerteneciente;
        //contexto.insert(contexto.end(),contextoPerteneciente);
          }
          else{
            tmpContexto=contextoPerteneciente;
            //contexto.insert(contexto.end(),contextoPerteneciente);
          }
        }
    estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
    if(!estaRedeclarada){
        simboloTab.insert(simboloTab.end(),tmpSimbolo);
        contexto.insert(contexto.end(),tmpContexto);
    }
    else{
        varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
        contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
    }
    tipo.insert(tipo.end(),"Variable");
    argument.insert(argument.end()," ");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),DECLARACIONVARIABLE);
    return true;
}

bool Analizador::DeclaracionVariable()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenEnd )
    {
        objArbol->InsertarHijo(";",tokenEnd);
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        return true;
    }
    if(tokensPro.at(0) == tokenAsig )
    {
         objArbol->InsertarHijo("=",tokenAsig);
          objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
          objArbol->InsertarHijo2("ListVar2",LISTA_VARIABLES2);
          objArbol->InsertarHijo2(";",tokenEnd);
          objArbol->InsertarNoTerminaXML("ListVar2");
          objArbol->InsertarNoTerminaXML("ExpAritmetica");
          objArbol->InsertarTerminalXML("<Asig>=</Asig>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        gramatica.insert(gramatica.begin(),LISTA_VARIABLES2);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenAsig);
        return true;
    }
    if(tokensPro.at(0)==tokenComa){
        objArbol->InsertarHijo(",",tokenComa);
        objArbol->InsertarHijo2("ListVar",LISTA_VARIABLES);
        objArbol->InsertarNoTerminaXML("ListVar");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),LISTA_VARIABLES);
        gramatica.insert(gramatica.begin(),tokenComa);
    return true;
    }
    return false;
}

bool Analizador::DeclaracionVariable2()
{
     objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenEnd )
    {
        objArbol->InsertarHijo(";",tokenEnd);
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        return true;
    }
    if(tokensPro.at(0) == tokenAsig )
    {
        objArbol->InsertarHijo("=",tokenAsig);
        objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarHijo2("ListVar2",LISTA_VARIABLES2);
        objArbol->InsertarHijo2(";",tokenEnd);
        objArbol->InsertarNoTerminaXML("ListVar2");
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        objArbol->InsertarTerminalXML("<Asig>=</Asig>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        gramatica.insert(gramatica.begin(),LISTA_VARIABLES2);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenAsig);
        return true;
    }
    if(tokensPro.at(0)==tokenComa){
        objArbol->InsertarHijo(",",tokenComa);
        objArbol->InsertarHijo2("ListVar",LISTA_VARIABLES);
        objArbol->InsertarNoTerminaXML("ListVar");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),LISTA_VARIABLES);
        gramatica.insert(gramatica.begin(),tokenComa);
        return true;
    }
    return false;
}
bool Analizador::DeclaracionVariable3(){
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)==tokenAsig){
        objArbol->InsertarHijo("=",tokenAsig);
        objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarHijo2("ListVar2",LISTA_VARIABLES2);
        objArbol->InsertarNoTerminaXML("ListVar2");
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        objArbol->InsertarTerminalXML("<Asig>=</Asig>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),LISTA_VARIABLES2);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenAsig);
        return true;
    }
    if(tokensPro.at(0)==tokenComa){
        objArbol->InsertarHijo(",",tokenComa);
        objArbol->InsertarHijo2("Id",tokenId);
        objArbol->InsertarHijo2("DeclaracionVar3",DECLARACIONVARIABLE3);
        objArbol->InsertarNoTerminaXML("DeclaracionVar3");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        tipoDato.insert(tipoDato.end(),"int");
        tipo.insert(tipo.end(),"Variable");
        argument.insert(argument.end()," ");
        //simboloTab.insert(simboloTab.end(),identificadores.at(0));
        tmpSimbolo=identificadores.at(0);
        if (varGlobal){
            tmpContexto="Global";
        //contexto.insert(contexto.end(),"Global");
        }
        else{
            if(!varInstruccion){
                contextoPerteneciente=simboloTab.back();
                tmpContexto=contextoPerteneciente;
                //contexto.insert(contexto.end(),contextoPerteneciente);
            }
            else{
                //contexto.insert(contexto.end(),contextoPerteneciente);
                tmpContexto=contextoPerteneciente;
            }
        }
        estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
        if(!estaRedeclarada){
            simboloTab.insert(simboloTab.end(),tmpSimbolo);
            contexto.insert(contexto.end(),tmpContexto);
        }
        else{
            varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
            contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
        }
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),DECLARACIONVARIABLE3);
        gramatica.insert(gramatica.begin(),tokenId);
        gramatica.insert(gramatica.begin(),tokenComa);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;

}
bool Analizador::ListaVariables()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenId )
    {
        objArbol->InsertarHijo("Id",tokenId);
        objArbol->InsertarHijo2("DeclaracionVar2",DECLARACIONVARIABLE2);
        objArbol->InsertarNoTerminaXML("DeclaracionVar2");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        tipo.insert(tipo.end(),"Variable");
        tipoDato.insert(tipoDato.end(),"int");
        argument.insert(argument.end()," ");
        //simboloTab.insert(simboloTab.end(),identificadores.at(0));
        tmpSimbolo=identificadores.at(0);
        if(varGlobal){
            tmpContexto="Global";
        //contexto.insert(contexto.end(),"Global");
        }
        else{
            if(!varInstruccion){
                contextoPerteneciente=simboloTab.back();
                //contexto.insert(contexto.end(),contextoPerteneciente);
                tmpContexto=contextoPerteneciente;
            }
            else{
                tmpContexto=contextoPerteneciente;
                //contexto.insert(contexto.end(),contextoPerteneciente);
            }
        }
        estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
        if(!estaRedeclarada){
            simboloTab.insert(simboloTab.end(),tmpSimbolo);
            contexto.insert(contexto.end(),tmpContexto);
        }
        else{
            varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
            contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
        }
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),DECLARACIONVARIABLE2);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}
bool Analizador::ListaVariables2(){
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)==tokenComa){
        objArbol->InsertarHijo(",",tokenComa);
        objArbol->InsertarHijo2("Id",tokenId);
        objArbol->InsertarHijo2("DeclaracionVar3",DECLARACIONVARIABLE3);
        objArbol->InsertarNoTerminaXML("DeclaracionVar3");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        tipoDato.insert(tipoDato.end(),"int");
        tipo.insert(tipo.end(),"Variable");
        argument.insert(argument.end()," ");
        tmpSimbolo=identificadores.at(0);
        if(varGlobal){
            tmpContexto="Global";
        //contexto.insert(contexto.end(),"Global");
        }
        else{
            if(!varInstruccion){
                contextoPerteneciente=simboloTab.back();
                //contexto.insert(contexto.end(),contextoPerteneciente);
                tmpContexto=contextoPerteneciente;
            }
            else{
                tmpContexto=contextoPerteneciente;
                //contexto.insert(contexto.end(),contextoPerteneciente);
            }
        }
        estaRedeclarada=ObjSemantico.ChecarRedeclaracion(tmpSimbolo,tmpContexto,simboloTab,contexto);
        if(!estaRedeclarada){
            simboloTab.insert(simboloTab.end(),tmpSimbolo);
            contexto.insert(contexto.end(),tmpContexto);
        }
        else{
            varRedeclarada.insert(varRedeclarada.end(),tmpSimbolo);
            contextoRepetido.insert(contextoRepetido.end(),tmpContexto);
        }
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),DECLARACIONVARIABLE3);
        gramatica.insert(gramatica.begin(),tokenId);
        gramatica.insert(gramatica.begin(),tokenComa);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::ExpresionAritmetica()
{
    if (llamadoFuncion){
        contParametros++;
    }
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    objArbol->InsertarHijo("Factor",FACTOR);
    objArbol->InsertarHijo2("ExprArit2",EXPRESION_ARITMETICA2);
    objArbol->InsertarNoTerminaXML("ExprArit2");
    objArbol->InsertarNoTerminaXML("Factor");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA2);
    gramatica.insert(gramatica.begin(),FACTOR);
    return true;
}

bool Analizador::ExpresionAritmetica1()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenTimes )
    {
        objArbol->InsertarHijo("*",tokenTimes);
        objArbol->InsertarHijo2("Factor",FACTOR);
        objArbol->InsertarNoTerminaXML("Factor");
        objArbol->InsertarTerminalXML("<Aritmetico>times</Aritmetico>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FACTOR);
        gramatica.insert(gramatica.begin(),tokenTimes);
        return true;
    }
    if(tokensPro.at(0) == tokenDivided )
    {
         objArbol->InsertarHijo("/",tokenDivided);
          objArbol->InsertarHijo2("Factor",FACTOR);
          objArbol->InsertarNoTerminaXML("Factor");
          objArbol->InsertarTerminalXML("<Aritmetico>divided</Aritmetico>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FACTOR);
        gramatica.insert(gramatica.begin(),tokenDivided);
        return true;
    }
    if(tokensPro.at(0) == tokenMod)
    {
         objArbol->InsertarHijo("%",tokenMod);
          objArbol->InsertarHijo2("Factor",FACTOR);
          objArbol->InsertarNoTerminaXML("Factor");
          objArbol->InsertarTerminalXML("<Aritmetico>mod</Aritmetico>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FACTOR);
        gramatica.insert(gramatica.begin(),tokenMod);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::ExpresionAritmetica2()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenPlus )
    {
        objArbol->InsertarHijo("+",tokenTimes);
          objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
          objArbol->InsertarNoTerminaXML("ExpAritmetica");
          objArbol->InsertarTerminalXML("<Aritmetico>plus</Aritmetico>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenPlus);
        return true;
    }
    if(tokensPro.at(0) == tokenMinus )
    {
        objArbol->InsertarHijo("-",tokenTimes);
          objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
          objArbol->InsertarNoTerminaXML("ExpAritmetica");
          objArbol->InsertarTerminalXML("<Aritmetico>minus</Aritmetico>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenMinus);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::Factor()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenId )
    {
        objArbol->InsertarHijo("Id",tokenId);
        objArbol->InsertarHijo2("FuncionExpect",POSIBILIDADFUNCION);
        objArbol->InsertarHijo2("ExprAritmetica1",EXPRESION_ARITMETICA1);
        objArbol->InsertarNoTerminaXML("ExprAritmetica1");
        objArbol->InsertarNoTerminaXML("FuncionExpect");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        simboloStatement=identificadores.at(0);
        //cout<<"simbolos statement: "<<simboloStatement<<endl;
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA1);
        gramatica.insert(gramatica.begin(),POSIBILIDADFUNCION);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }
    if(tokensPro.at(0) == tokenNum )
    {
         objArbol->InsertarHijo("Num",tokenNum);
          objArbol->InsertarHijo2("ExprAritmetica1",EXPRESION_ARITMETICA1);
          objArbol->InsertarNoTerminaXML("ExprAritmetica1");
          objArbol->InsertarIDNum("entero",numeros.at(0));
          numeros.erase(numeros.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA1);
        gramatica.insert(gramatica.begin(),tokenNum);
        return true;
    }
    if(tokensPro.at(0) == tokenPrL)
    {
        objArbol->InsertarHijo("(",tokenPrL);
          objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
          objArbol->InsertarHijo2(")",tokenPrR);
          objArbol->InsertarNoTerminaXML("ExpAritmetica");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }

    return false;
}
bool Analizador::PosibilidadFuncion(){
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)==tokenPrL){
        objArbol->InsertarHijo(")",tokenPrL);
        objArbol->InsertarHijo2("EstructuraIdentificador3",ESTRUCTURAIDENTIDICADOR3);
        objArbol->InsertarNoTerminaXML("EstructuraIdentificador3");
        contextoStatement="Global";
        //cout<<"simbolos statement: "<<simboloStatement<<endl;
        varDeclarada=ObjSemantico.ChecarVariables(simboloStatement,contextoStatement,simboloTab,contexto);
        if(!varDeclarada){
            varSinDeclarar.insert(varSinDeclarar.end(),simboloStatement);
            listaContextos.insert(listaContextos.end(),contextoStatement);
        }
        else{
           for(unsigned int i=0;i<funcsReturn.size();i++){
                if(simboloStatement==funcsReturn.at(i)){
                    retorna=true;
                }
           }
           if(!retorna){
                noRetorna.insert(noRetorna.end(),simboloStatement);
           }
           else{
                funcionPerteneciente=simboloStatement;
                llamadoFuncion=true;
                contParametros=0;
           }
        }
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURAIDENTIDICADOR3);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }
    contextoStatement=contextoPerteneciente;
    varDeclarada=ObjSemantico.ChecarVariables(simboloStatement,contextoStatement,simboloTab,contexto);
    if(!varDeclarada){
        for(unsigned int i=0;i<varSinDeclarar.size();i++){
            if(simboloStatement==varSinDeclarar.at(i)&& contextoStatement==listaContextos.at(i)){
                existente=true;
            }
        }
        if(!existente){
            varSinDeclarar.insert(varSinDeclarar.end(),simboloStatement);
            listaContextos.insert(listaContextos.end(),contextoStatement);
        }
        existente=false;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::InstruccionCondicionyBucle()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(llamadoFuncion){
    for(unsigned int i=0;i<funcParametro.size();i++){
        if(funcionPerteneciente==funcParametro.at(i)&&contParametros==numParametros.at(i)){
            hayParametros=true;
        }
    }
    if(!hayParametros){
        errorParametro.insert(errorParametro.end(),funcionPerteneciente);
    }
    }
    llamadoFuncion=false;
    contParametros=0;
    if(tokensPro.at(0) == RW_VOID )
    {
           objArbol->InsertarHijo("void",RW_VOID);
          objArbol->InsertarHijo2("FuncionVoid",FUNCIONVOID);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarNoTerminaXML("Instruccion");
          objArbol->InsertarNoTerminaXML("FuncionVoid");
          objArbol->InsertarTerminalXML("<RW>void</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),FUNCIONVOID);
        gramatica.insert(gramatica.begin(),RW_VOID);
        return true;
    }
    if(tokensPro.at(0) == RW_INT )
    {
        varInstruccion=true;
        tipoDato.insert(tipoDato.end(),"int");
        objArbol->InsertarHijo("int",RW_VOID);
          objArbol->InsertarHijo2("FuncionDeclaracion",FUNCIONDECLARACION);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarNoTerminaXML("Instruccion");
          objArbol->InsertarNoTerminaXML("FuncionDeclaracion");
          objArbol->InsertarTerminalXML("<RW>int</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),FUNCIONDECLARACION);
        gramatica.insert(gramatica.begin(),RW_INT);
        return true;
    }
    if(tokensPro.at(0) == tokenId )
    {
        objArbol->InsertarHijo("Id",tokenId);
        objArbol->InsertarHijo2("EstrId",ESTRUCTURA_IDENTIFICADOR);
        objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
        objArbol->InsertarNoTerminaXML("Instruccion");
        objArbol->InsertarNoTerminaXML("EstrId");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        simboloStatement=identificadores.at(0);
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),ESTRUCTURA_IDENTIFICADOR);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }

    if(tokensPro.at(0) == RW_IF )
    {
        objArbol->InsertarHijo("if",RW_IF);
          objArbol->InsertarHijo2("EstructuraIf",ESTRUCTURAIF);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarNoTerminaXML("Instruccion");
          objArbol->InsertarNoTerminaXML("EstructuraIf");
          objArbol->InsertarTerminalXML("<RW>if</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),ESTRUCTURAIF);
        gramatica.insert(gramatica.begin(),RW_IF);
        return true;
    }

    if(tokensPro.at(0) == RW_FOR )
    {
        objArbol->InsertarHijo("for",RW_FOR);
          objArbol->InsertarHijo2("EstructuraFor",ESTRUCTURAFOR);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarNoTerminaXML("Instruccion");
          objArbol->InsertarNoTerminaXML("EstructuraFor");
          objArbol->InsertarTerminalXML("<RW>for</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),ESTRUCTURAFOR);
        gramatica.insert(gramatica.begin(),RW_FOR);
        return true;
    }

    if(tokensPro.at(0) == RW_DO )
    {
         objArbol->InsertarHijo("Do",RW_DO);
          objArbol->InsertarHijo2("EstructuraDo",ESTRUCTURADO);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarNoTerminaXML("Instruccion");
          objArbol->InsertarNoTerminaXML("EstructuraDo");
          objArbol->InsertarTerminalXML("<RW>do</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),ESTRUCTURADO);
        gramatica.insert(gramatica.begin(),RW_DO);
        return true;
    }

    if(tokensPro.at(0) == RW_WHILE)
    {
        objArbol->InsertarHijo("while",RW_WHILE);
          objArbol->InsertarHijo2("EstructuraWhile",ESTRUCTURAWHILE);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarNoTerminaXML("Instruccion");
          objArbol->InsertarNoTerminaXML("EstructuraWhile");
          objArbol->InsertarTerminalXML("<RW>while</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),ESTRUCTURAWHILE);
        gramatica.insert(gramatica.begin(),RW_WHILE);
        return true;
    }

    /*if(tokensPro.at(0) == RW_RETURN)
    {
        objArbol->InsertarHijo("return",RW_RETURN);
          objArbol->InsertarHijo2("EstructuraReturn",ESTRUCTURARETURN);
          objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
          objArbol->InsertarNoTerminaXML("Instruccion");
          objArbol->InsertarNoTerminaXML("EstructuraReturn");
          objArbol->InsertarTerminalXML("<RW>return</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),ESTRUCTURARETURN);
        gramatica.insert(gramatica.begin(),RW_RETURN);
        return true;
    }*/
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::Instruccion2()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == RW_VOID )
    {
        objArbol->InsertarHijo("void",RW_VOID);
          objArbol->InsertarHijo2("FuncionVoid",FUNCIONVOID);
          objArbol->InsertarNoTerminaXML("FuncionVoid");
          objArbol->InsertarTerminalXML("<RW>void</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FUNCIONVOID);
        gramatica.insert(gramatica.begin(),RW_VOID);
        return true;
    }
    if(tokensPro.at(0) == RW_INT )
    {
        //varInstruccion=true; //que onda aqui
        tipoDato.insert(tipoDato.end(),"int");
        objArbol->InsertarHijo("int",RW_INT);
          objArbol->InsertarHijo2("FuncionDeclaracion",FUNCIONDECLARACION);
          objArbol->InsertarNoTerminaXML("FuncionDeclaracion");
          objArbol->InsertarTerminalXML("<RW>int</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FUNCIONDECLARACION);
        gramatica.insert(gramatica.begin(),RW_INT);
        return true;
    }
    if(tokensPro.at(0) == tokenId )
    {
        objArbol->InsertarHijo("Id",tokenId);
        objArbol->InsertarHijo2("EstrId",ESTRUCTURA_IDENTIFICADOR);
        objArbol->InsertarNoTerminaXML("EstrId");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        contextoStatement=contextoPerteneciente;
        varDeclarada=ObjSemantico.ChecarVariables(simboloStatement,contextoStatement,simboloTab,contexto);
        if(!varDeclarada){
            varSinDeclarar.insert(varSinDeclarar.end(),simboloStatement);
        }
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURA_IDENTIFICADOR);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }
    if(tokensPro.at(0) == RW_IF )
    {
        objArbol->InsertarHijo("if",RW_IF);
          objArbol->InsertarHijo2("EstructuraIf",ESTRUCTURAIF);
          objArbol->InsertarNoTerminaXML("EstructuraIf");
          objArbol->InsertarTerminalXML("<RW>if</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURAIF);
        gramatica.insert(gramatica.begin(),RW_IF);
        return true;
    }
    if(tokensPro.at(0) == RW_FOR )
    {
        objArbol->InsertarHijo("for",RW_FOR);
          objArbol->InsertarHijo2("EstructuraFor",ESTRUCTURAFOR);
          objArbol->InsertarNoTerminaXML("EstructuraFor");
          objArbol->InsertarTerminalXML("<RW>for</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURAFOR);
        gramatica.insert(gramatica.begin(),RW_FOR);
        return true;
    }
    if(tokensPro.at(0) == RW_DO )
    {
        objArbol->InsertarHijo("do",RW_DO);
          objArbol->InsertarHijo2("EstructuraDo",ESTRUCTURADO);
          objArbol->InsertarNoTerminaXML("EstructuraDo");
          objArbol->InsertarTerminalXML("<RW>do</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURADO);
        gramatica.insert(gramatica.begin(),RW_DO);
        return true;
    }
    if(tokensPro.at(0) == RW_WHILE)
    {
        objArbol->InsertarHijo("while",RW_WHILE);
          objArbol->InsertarHijo2("EstructuraWhile",ESTRUCTURAWHILE);
          objArbol->InsertarNoTerminaXML("EstructuraWhile");
          objArbol->InsertarTerminalXML("<RW>while</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURAWHILE);
        gramatica.insert(gramatica.begin(),RW_WHILE);
        return true;
    }
    /*if(tokensPro.at(0) == RW_RETURN)
    {
        objArbol->InsertarHijo("Return",RW_RETURN);
          objArbol->InsertarHijo2("EstructuraReturn",ESTRUCTURARETURN);
          objArbol->InsertarNoTerminaXML("EstructuraReturn");
          objArbol->InsertarTerminalXML("<RW>return</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURARETURN);
        gramatica.insert(gramatica.begin(),RW_RETURN);
        return true;
    }*/
    gramatica.erase(gramatica.begin());
    return false;
}

bool Analizador::EstructuraIdentificador()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenPrL)
    {
        objArbol->InsertarHijo(")",tokenPrL);
        objArbol->InsertarHijo2("EstrId2",ESTRUCTURAIDENTIDICADOR2);
        objArbol->InsertarNoTerminaXML("EstrId2");
        contextoStatement="Global";
        varDeclarada=ObjSemantico.ChecarVariables(simboloStatement,contextoStatement,simboloTab,contexto);
        if(!varDeclarada){
            varSinDeclarar.insert(varSinDeclarar.end(),simboloStatement);
            listaContextos.insert(listaContextos.end(),contextoStatement);
        }
        else{
            funcionPerteneciente=simboloStatement;
            llamadoFuncion=true;
            contParametros=0;
        }
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURAIDENTIDICADOR2);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }
    if(tokensPro.at(0) == tokenAsig)
    {
        objArbol->InsertarHijo("=",tokenAsig);
        objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarHijo2(";",tokenEnd);
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        objArbol->InsertarTerminalXML("<Asig>=</Asig>");
        contextoStatement=contextoPerteneciente;
        varDeclarada=ObjSemantico.ChecarVariables(simboloStatement,contextoStatement,simboloTab,contexto);
        if(!varDeclarada){
            for(unsigned int i=0;i<varSinDeclarar.size();i++){
                if(simboloStatement==varSinDeclarar.at(i)&& contextoStatement==listaContextos.at(i)){
                existente=true;
                }
            }
            if (!existente){
                varSinDeclarar.insert(varSinDeclarar.end(),simboloStatement);
                listaContextos.insert(listaContextos.end(),contextoStatement);
        }
        existente=false;
        }
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenAsig);
        return true;
    }
    return false;
}
bool Analizador::EstructuraIdentificador2()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)!=tokenPrR)
    {
        objArbol->InsertarHijo("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarHijo2("ListaExpresiones",LISTA_EXPRESIONES);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarHijo2(";",tokenEnd);
        objArbol->InsertarNoTerminaXML("ListaExpresiones");
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),LISTA_EXPRESIONES);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        return true;
    }
    objArbol->InsertarHijo(")",tokenPrR);
    objArbol->InsertarHijo2(";",tokenEnd);
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),tokenEnd);
    gramatica.insert(gramatica.begin(),tokenPrR);
    return true;

}
bool Analizador::EstructuraIdentificador3(){
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0)!=tokenPrR){
        objArbol->InsertarHijo("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarHijo2("ListaExpresiones",LISTA_EXPRESIONES);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarNoTerminaXML("ListaExpresiones");
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),LISTA_EXPRESIONES);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        return true;
    }
    objArbol->InsertarHijo(")",tokenPrR);
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),tokenPrR);
    return true;
}
bool Analizador::ListaExpresiones()
{
      objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenComa)
    {
        objArbol->InsertarHijo(",",tokenComa);
          objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
          objArbol->InsertarHijo2("ListaExpresiones",LISTA_EXPRESIONES);
          objArbol->InsertarNoTerminaXML("ListaExpresiones");
          objArbol->InsertarNoTerminaXML("ExpAritmetica");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),LISTA_EXPRESIONES);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenComa);
        return true;
    }

    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::EstructuraIF()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenPrL)
    {
        objArbol->InsertarHijo("(",tokenPrL);
          objArbol->InsertarHijo2("EstructuraIf2",ESTRUCTURAIF2);
          objArbol->InsertarHijo2(")",tokenPrR);
          objArbol->InsertarHijo2("CierreIf",CIERREIF);
          objArbol->InsertarNoTerminaXML("CierreIf");
          objArbol->InsertarNoTerminaXML("EstructuraIf2");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),CIERREIF);
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),ESTRUCTURAIF2);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }

    return false;
}

bool Analizador::EstructuraIF2()
{
     objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenNeg)
    {
        objArbol->InsertarHijo("!",tokenNeg);
        objArbol->InsertarHijo2("(",tokenPrL);
        objArbol->InsertarHijo2("ExprCmp",EXPRESION_COMPARACION);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarNoTerminaXML("ExprCmp");
        objArbol->InsertarTerminalXML("<Negacion>!</Negacion>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenPrL);
        gramatica.insert(gramatica.begin(),tokenNeg);
        return true;
    }
    objArbol->InsertarHijo("ExprCmp",EXPRESION_COMPARACION);
    objArbol->InsertarNoTerminaXML("ExprCmp");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
    return true;
}

bool Analizador::ExpresionComparacion()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    objArbol->InsertarHijo("FactorComparacion",FACTOR_COMPARACION);
    objArbol->InsertarHijo2("ExprCmp2",EXPRESION_COMPARACION2);
    objArbol->InsertarNoTerminaXML("ExprCmp2");
    objArbol->InsertarNoTerminaXML("FactorComparacion");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION2);
    gramatica.insert(gramatica.begin(),FACTOR_COMPARACION);
    return true;
}

bool Analizador::ExpresionComparacion1()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenGt)
    {
        objArbol->InsertarHijo(">",tokenGt);
        objArbol->InsertarHijo2("FACTOR_COMPARACION",FACTOR_COMPARACION);
        objArbol->InsertarNoTerminaXML("FactorComparacion");
        objArbol->InsertarTerminalXML("<OperadorComp>&gt;</OperadorComp>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FACTOR_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenGt);
        return true;
    }
    if(tokensPro.at(0) == tokenLt)
    {
        objArbol->InsertarHijo("<",tokenLt);
        objArbol->InsertarHijo2("FACTOR_COMPARACION",FACTOR_COMPARACION);
        objArbol->InsertarNoTerminaXML("FactorComparacion");
        objArbol->InsertarTerminalXML("<OperadorComp>&lt;</OperadorComp>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FACTOR_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenLt);
        return true;
    }
    if(tokensPro.at(0) == tokenGEt)
    {
        objArbol->InsertarHijo(">=",tokenGEt);
        objArbol->InsertarHijo2("FACTOR_COMPARACION",FACTOR_COMPARACION);
        objArbol->InsertarNoTerminaXML("FactorComparacion");
        objArbol->InsertarTerminalXML("<OperadorComp>&me;</OperadorComp>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FACTOR_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenGEt);
        return true;
    }
    if(tokensPro.at(0) == tokenLEt)
    {
        objArbol->InsertarHijo("<=",tokenLEt);
        objArbol->InsertarHijo2("FACTOR_COMPARACION",FACTOR_COMPARACION);
        objArbol->InsertarNoTerminaXML("FactorComparacion");
        objArbol->InsertarTerminalXML("<OperadorComp>&le;</OperadorComp>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),FACTOR_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenLEt);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::ExpresionComparacion2()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenEq)
    {
        objArbol->InsertarHijo("==",tokenEq);
        objArbol->InsertarHijo2("ExprCmp",EXPRESION_COMPARACION);
        objArbol->InsertarNoTerminaXML("ExprCmp");
        objArbol->InsertarTerminalXML("<Igual>==</Igual>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenEq);
        return true;
    }
    if(tokensPro.at(0) == tokenDif)
    {
        objArbol->InsertarHijo("!=",tokenDif);
        objArbol->InsertarHijo2("ExprCmp",EXPRESION_COMPARACION);
        objArbol->InsertarNoTerminaXML("ExprCmp");
        objArbol->InsertarTerminalXML("<Diferente>!=</Diferente>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenDif);
        return true;
    }

    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::FactorComparacion()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenPrL)
    {
        cout<<"entra al if"<<endl;
        objArbol->InsertarHijo("(",tokenPrL);
        objArbol->InsertarHijo2("ExprCmp",EXPRESION_COMPARACION);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarNoTerminaXML("ExprCmp");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }
    cout<<"no entra al if"<<endl;
    objArbol->InsertarHijo("ExpAritmetica",EXPRESION_ARITMETICA);
    objArbol->InsertarHijo2("ExprCmp1",EXPRESION_COMPARACION1);
    objArbol->InsertarNoTerminaXML("ExprCmp1");
    objArbol->InsertarNoTerminaXML("ExpAritmetica");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION1);
    gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
    return true;
}

bool Analizador::CierreIF()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenKeyL )
    {
        objArbol->InsertarHijo("{",tokenKeyL);
        objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
        objArbol->InsertarHijo2("}",tokenKeyR);
        objArbol->InsertarHijo2("PosibilidadElse",POSIBILIDADELSE);
        objArbol->InsertarNoTerminaXML("PosibilidadElse");
        objArbol->InsertarNoTerminaXML("Instruccion");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),POSIBILIDADELSE);
        gramatica.insert(gramatica.begin(),tokenKeyR);
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),tokenKeyL);
        return true;
    }
    objArbol->InsertarHijo("PosibilidadElse",POSIBILIDADELSE);
    objArbol->InsertarHijo2("Instruccion2",INSTRUCCION2);
    objArbol->InsertarNoTerminaXML("Instruccion2");
    objArbol->InsertarNoTerminaXML("PosibilidadElse");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),INSTRUCCION2);
    gramatica.insert(gramatica.begin(),POSIBILIDADELSE);
    return true;
}

bool Analizador::PosibilidadElse()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == RW_ELSE )
    {
        objArbol->InsertarHijo("else",RW_ELSE);
        objArbol->InsertarHijo2("PosibilidadElse2",POSIBILIDADELSE2);
        objArbol->InsertarNoTerminaXML("PosibilidadElse2");
        objArbol->InsertarTerminalXML("<RW>else</RW>");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),POSIBILIDADELSE2);
        gramatica.insert(gramatica.begin(),RW_ELSE);
        return true;
    }
    gramatica.erase(gramatica.begin());
    return true;
}

bool Analizador::PosibilidadElse2()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenKeyL )
    {
        objArbol->InsertarHijo("{",tokenKeyL);
        objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
        objArbol->InsertarHijo2("}",tokenKeyR);
        objArbol->InsertarNoTerminaXML("Instruccion");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenKeyR);
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),tokenKeyL);
        return true;
    }
    objArbol->InsertarHijo("Instruccion2",INSTRUCCION2);
    objArbol->InsertarNoTerminaXML("Instruccion2");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),INSTRUCCION2);
    return true;
}

bool Analizador::EstructuraFor()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenPrL)
    {
        objArbol->InsertarHijo("(",tokenPrL);
        objArbol->InsertarHijo2("EstructuraFor2",ESTRUCTURAFOR2);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarHijo2("CierreFor",CIERREFOR);
        objArbol->InsertarNoTerminaXML("CierreFor");
        objArbol->InsertarNoTerminaXML("EstructuraFor2");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),CIERREFOR);
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),ESTRUCTURAFOR2);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }

    return false;
}

bool Analizador::EstructuraFor2()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == RW_INT )
    {
        objArbol->InsertarHijo("int",RW_INT);
        objArbol->InsertarHijo2("Id",tokenId);
        objArbol->InsertarHijo2("=",tokenAsig);
        objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarHijo2(";",tokenEnd);
        objArbol->InsertarHijo2("EstructuraFor3",ESTRUCTURAFOR3);
        objArbol->InsertarNoTerminaXML("EstructuraFor3");
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        objArbol->InsertarTerminalXML("<Asig>=</Asig>");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        objArbol->InsertarTerminalXML("<RW>int</RW>");
        simboloTab.insert(simboloTab.end(),identificadores.at(0));
        tipo.insert(tipo.end(),"Variable");
        tipoDato.insert(tipoDato.end(),"int");
        contexto.insert(contexto.end(),contextoPerteneciente);
        argument.insert(argument.end()," ");
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURAFOR3);
        gramatica.insert(gramatica.begin(),tokenEnd);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenAsig);
        gramatica.insert(gramatica.begin(),tokenId);
        gramatica.insert(gramatica.begin(),RW_INT);
        return true;
    }

    if(tokensPro.at(0) == tokenId )
    {
        int i;
        objArbol->InsertarHijo("Id",tokenId);
        objArbol->InsertarHijo2("=",tokenAsig);
        objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarHijo2(";",tokenEnd);
        objArbol->InsertarHijo2("EstructuraFor3",ESTRUCTURAFOR3);
        objArbol->InsertarNoTerminaXML("EstructuraFor3");
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        objArbol->InsertarTerminalXML("<Asig>=</Asig>");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        simboloStatement=identificadores.at(0);
        contextoStatement=contextoPerteneciente;
        varDeclarada=ObjSemantico.ChecarVariables(simboloStatement,contextoStatement,simboloTab,contexto);
        if(!varDeclarada){
                for(i=0;i<varSinDeclarar.size();i++){
                    if(simboloStatement==varSinDeclarar.at(i)&& contextoStatement==listaContextos.at(i)){
                        existente=true;
                    }
                }
                if(!existente){
                    varSinDeclarar.insert(varSinDeclarar.end(),simboloStatement);
                    listaContextos.insert(listaContextos.end(),contextoStatement);
                }
                existente=false;

        }
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),ESTRUCTURAFOR3);
        gramatica.insert(gramatica.begin(),tokenEnd);
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenAsig);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }
    return false;
}

bool Analizador::EstructuraFor3()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    objArbol->InsertarHijo("ExprCmp",EXPRESION_COMPARACION);
    objArbol->InsertarHijo2(";",tokenEnd);
    objArbol->InsertarHijo2("EstructuraFor4",ESTRUCTURAFOR4);
    objArbol->InsertarNoTerminaXML("EstructuraFor4");
    objArbol->InsertarNoTerminaXML("ExprCmp");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),ESTRUCTURAFOR4);
    gramatica.insert(gramatica.begin(),tokenEnd);
    gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
    return true;
}

bool Analizador::EstructuraFor4()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenId )
    {
        objArbol->InsertarHijo("Id",tokenId);
        objArbol->InsertarHijo2("=",tokenAsig);
        objArbol->InsertarHijo2("ExpAritmetica",EXPRESION_ARITMETICA);
        objArbol->InsertarNoTerminaXML("ExpAritmetica");
        objArbol->InsertarTerminalXML("<Asig>=</Asig>");
        objArbol->InsertarIDNum("Id",identificadores.at(0));
        simboloStatement=identificadores.at(0);
        contextoStatement=contextoPerteneciente;
        varDeclarada=ObjSemantico.ChecarVariables(simboloStatement,contextoStatement,simboloTab,contexto);
        if(!varDeclarada){
            for(unsigned int i=0;i<varSinDeclarar.size();i++){
                if(simboloStatement==varSinDeclarar.at(i)&&contextoStatement==listaContextos.at(i)){
                    existente=true;
                }
            }
            if(!existente){
                varSinDeclarar.insert(varSinDeclarar.end(),simboloStatement);
                listaContextos.insert(listaContextos.end(),contextoStatement);
            }
            existente=false;
        }
        identificadores.erase(identificadores.begin());
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
        gramatica.insert(gramatica.begin(),tokenAsig);
        gramatica.insert(gramatica.begin(),tokenId);
        return true;
    }
    return false;
}

bool Analizador::CierreFor()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenKeyL )
    {
        objArbol->InsertarHijo("{",tokenKeyL);
        objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
        objArbol->InsertarHijo2("}",tokenKeyR);
        objArbol->InsertarNoTerminaXML("Instruccion");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenKeyR);
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),tokenKeyL);
        return true;
    }
    objArbol->InsertarHijo("Instruccion2",INSTRUCCION2);
    objArbol->InsertarNoTerminaXML("Instruccion2");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),INSTRUCCION2);
    return true;
}

bool Analizador::EstructuraDo()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenKeyL )
    {
        objArbol->InsertarHijo("{",tokenKeyL);
        objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
        objArbol->InsertarHijo2("}",tokenKeyR);
        objArbol->InsertarHijo2("while",RW_WHILE);
        objArbol->InsertarHijo2("(",tokenPrL);
        objArbol->InsertarHijo2("ExprCmp",EXPRESION_COMPARACION);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarHijo2(";",tokenEnd);
        objArbol->InsertarNoTerminaXML("ExprCmp");
        objArbol->InsertarTerminalXML("<RW>while</RW>");
        objArbol->InsertarNoTerminaXML("Instruccion");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenPrL);
        gramatica.insert(gramatica.begin(),RW_WHILE);
        gramatica.insert(gramatica.begin(),tokenKeyR);
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),tokenKeyL);
        return true;
    }
    return false;
}

bool Analizador::EstructuraWhile()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    if(tokensPro.at(0) == tokenPrL )
    {
        objArbol->InsertarHijo("(",tokenPrL);
        objArbol->InsertarHijo2("ExprCmp",EXPRESION_COMPARACION);
        objArbol->InsertarHijo2(")",tokenPrR);
        objArbol->InsertarHijo2("{",tokenKeyL);
        objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
        objArbol->InsertarHijo2("}",tokenKeyR);
        objArbol->InsertarNoTerminaXML("Instruccion");
        objArbol->InsertarNoTerminaXML("ExprCmp");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenKeyR);
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),tokenKeyL);
        gramatica.insert(gramatica.begin(),tokenPrR);
        gramatica.insert(gramatica.begin(),EXPRESION_COMPARACION);
        gramatica.insert(gramatica.begin(),tokenPrL);
        return true;
    }
    return false;
}

bool Analizador::EstructuraReturn()
{
    objArbol->RecorrerArbol();
    objArbol->RevisarXML();
    objArbol->InsertarHijo("ExpAritmetica",EXPRESION_ARITMETICA);
    objArbol->InsertarHijo2(";",tokenEnd);
    objArbol->InsertarNoTerminaXML("ExpAritmetica");
    gramatica.erase(gramatica.begin());
    gramatica.insert(gramatica.begin(),tokenEnd);
    gramatica.insert(gramatica.begin(),EXPRESION_ARITMETICA);
    return true;
}
bool Analizador::CierreFuncion2(){
    numParametros.insert(numParametros.end(),resultadoParametros);
    if(tokensPro.at(0)==tokenEnd){
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenEnd);
        return true;
    }
    if(tokensPro.at(0)==tokenKeyL){
        objArbol->InsertarHijo("{",tokenKeyL);
        objArbol->InsertarHijo2("Instruccion",INSTRUCCION);
        objArbol->InsertarHijo2("return",RW_RETURN);
        objArbol->InsertarHijo2("EstructuraReturn",ESTRUCTURARETURN);
        objArbol->InsertarHijo2("}",tokenKeyR);
        objArbol->InsertarNoTerminaXML("Instruccion");
        objArbol->InsertarTerminalXML("<RW>return</RW>");
        objArbol->InsertarNoTerminaXML("EstructuraReturn");
        gramatica.erase(gramatica.begin());
        gramatica.insert(gramatica.begin(),tokenKeyR);
        gramatica.insert(gramatica.begin(),ESTRUCTURARETURN);
        gramatica.insert(gramatica.begin(),RW_RETURN);
        gramatica.insert(gramatica.begin(),INSTRUCCION);
        gramatica.insert(gramatica.begin(),tokenKeyL);
        return true;
    }
    return false;


}
bool Analizador::ClasificarTerminales()
{
    //cout<< mapaSimbolos.find(tokensPro.at(0))->second << " : "<<tokensPro.at(0)<<endl;
    //cout << mapaSimbolos.find(0)->second << endl;
    //cout<<"Gramatica en: " << gramatica.at(0)<<endl;
    //cout<<"tamaño gramatica: "<<gramatica.size()<<endl;
    if(tokensPro.at(0)==gramatica.at(0))
    {
        tokensPro.erase(tokensPro.begin());
        gramatica.erase(gramatica.begin());
        return true;
    }
    return false;
}
void Analizador::FuncionErrores(int fail)
{
    cout<<"fail = "<<fail<<endl;

    if(fail==0)
    {
        fallos++;
        cout<<"Error se esperaba: Identidicador"<<endl;
        gramatica.clear();
    }
    if(fail==1)
    {
        fallos++;
        cout<<"Error se esperaba: Numero"<<endl;
        gramatica.clear();
    }
    if(fail==2 || fail==3 || fail==4 ||fail==5 || fail==6)
    {
        fallos++;
        cout<<"Error se esperaba: Operador Aritmetico"<<endl;
        gramatica.clear();
    }
    if(fail==7 || fail==8 ||fail==9 ||fail==10 ||fail==11 ||fail==12 ||fail==13 ||fail==14)
    {
        fallos++;
        cout<<"Error se esperaba: Comparacion "<<endl;
        gramatica.clear();
    }
    if(fail==15 || fail==16)
    {
        fallos++;
        cout<<"Error se esperaba: Parentesis"<<endl;
        gramatica.clear();
    }
    if(fail==17 || fail==18)
    {
        fallos++;
        cout<<"Error se esperaba: Llave"<<endl;
        gramatica.clear();
    }
    if(fail==19)
    {
        fallos++;
        cout<<"Error se esperaba: Punto y Coma"<<endl;
        gramatica.clear();
    }
    if(fail==20)
    {
        fallos++;
        cout<<"Error se esperaba coma"<<endl;
        gramatica.clear();
    }
    if(fail==21)
    {
        fallos++;
        cout<<"Error de sintaxis"<<endl;
        gramatica.clear();
    }
    if(fail>=22 && fail<=29)
    {
        fallos++;
        cout<<"Error falta Palabra Reservada"<<endl;
        gramatica.clear();
    }
    if(fail==31)
    {
        fallos++;
        cout<<"Error de Sintaxis"<<endl;
        gramatica.clear();
    }
    if(fail>=32 && fail<=44)
    {
        fallos++;
        cout<<"Error se esperaba Funcion,Declaraicon o Expresion"<<endl;
        gramatica.clear();
    }
    if(fail==45)
    {
        fallos++;
        cout<<"Error se esperaba Cierre de Funcion"<<endl;
        gramatica.clear();
    }
    if(fail>=46 && fail<=49)
    {
        fallos++;
        cout<<"Error se esperaba Funcion,Declaracion,Expresion o Instruccion"<<endl;
        gramatica.clear();
    }
    if(fail>=50 && fail<=56)
    {
        fallos++;
        cout<<"Error Estructura if"<<endl;
        gramatica.clear();
    }
    if(fail>=57 && fail<=58)
    {
        fallos++;
        cout<<"Error Estructura else "<<endl;
        gramatica.clear();
    }
    if(fail>=59 && fail<=63)
    {
        fallos++;
        cout<<"Error Estructura for "<<endl;
        gramatica.clear();
    }
    if(fail==64)
    {
        fallos++;
        cout<<"Error Estructura do "<<endl;
        gramatica.clear();
    }
    if(fail==65)
    {
        fallos++;
        cout<<"Error Estructura while "<<endl;
        gramatica.clear();
    }
    if(fail==66)
    {
        fallos++;
        cout<<"Error Return "<<endl;
        gramatica.clear();
    }

}
void Analizador::PalabrasReservadas()
{
    //cout <<"entro"<<endl;
    paReservadas.insert(paReservadas.end(),"auto");
    paReservadas.insert(paReservadas.end(),"break");
    paReservadas.insert(paReservadas.end(),"case");
    paReservadas.insert(paReservadas.end(),"char");
    paReservadas.insert(paReservadas.end(),"const");
    paReservadas.insert(paReservadas.end(),"continue");
    paReservadas.insert(paReservadas.end(),"default");
    paReservadas.insert(paReservadas.end(),"do");
    paReservadas.insert(paReservadas.end(),"double");
    paReservadas.insert(paReservadas.end(),"else");
    paReservadas.insert(paReservadas.end(),"enum");
    paReservadas.insert(paReservadas.end(),"extern");
    paReservadas.insert(paReservadas.end(),"float");
    paReservadas.insert(paReservadas.end(),"for");
    paReservadas.insert(paReservadas.end(),"goto");
    paReservadas.insert(paReservadas.end(),"if");
    paReservadas.insert(paReservadas.end(),"int");
    paReservadas.insert(paReservadas.end(),"long");
    paReservadas.insert(paReservadas.end(),"register");
    paReservadas.insert(paReservadas.end(),"return");
    paReservadas.insert(paReservadas.end(),"short");
    paReservadas.insert(paReservadas.end(),"signed");
    paReservadas.insert(paReservadas.end(),"sizeof");
    paReservadas.insert(paReservadas.end(),"static");
    paReservadas.insert(paReservadas.end(),"struct");
    paReservadas.insert(paReservadas.end(),"switch");
    paReservadas.insert(paReservadas.end(),"typedef");
    paReservadas.insert(paReservadas.end(),"union");
    paReservadas.insert(paReservadas.end(),"unsigned");
    paReservadas.insert(paReservadas.end(),"void");
    paReservadas.insert(paReservadas.end(),"volatile");
    paReservadas.insert(paReservadas.end(),"while");
}

void Analizador::TablaDeSimbolos(string referencia)
{
    unsigned int i;
    bool bandera;
    bandera=false;
    if(simbolos.empty())
    {
        simbolos.insert(simbolos.begin(),referencia);
    }
    else
    {
        for(i=0; i<simbolos.size(); i++)
        {
            if(simbolos.at(i)==referencia)
            {
                bandera=true;
            }
        }
        if(bandera==false)
            simbolos.insert(simbolos.end(),referencia);
    }
}
void Analizador::Mostrar()
{
    int pos;
    unsigned int i;
    pos=0;
    for(i=0; i<simbolos.size(); i++)
    {
        cout<<"id "<<simbolos[i]<<" : "<<pos<<endl;
        pos++;
    }
}
void Analizador::MostrarTabla(){
int pos;
unsigned int i;
pos=0;
cout<<"Posicion"<<"\t"<<"Simbolos"<<"\t"<<"Tipo"<<"\t\t"<<"Tipo de dato"<<"\t"<<"Contexto"<<"\t"<<"Parametros"<<endl;
for(i=0; i<simboloTab.size(); i++)
    {
        cout<<pos<<"\t\t"<<simboloTab[i]<<"\t\t"<<tipo[i]<<"\t\t"<<tipoDato[i]<<"\t\t"<<contexto[i]<<"\t\t"<<argument[i]<<endl;
        pos++;
    }
}



