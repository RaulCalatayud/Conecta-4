#include "jugadormaquina.h"
#include <iostream>
#include "tablero.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int valor1 = 1;
int valor2 = 10;
int valor3 = 100;
int valor4 = 10000; // inf

bool primera;

// //                        A-D    D     B-D      B     B-I     I      A-I    A
//vector<Direccion> Dirs = {{1,1},{0,1},{-1,+1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0}};
vector<Direccion> Dirs = {{0,1},{1,0},{1,1},{1,-1},{0,-1},{-1,0},{-1,-1},{-1,+1}};

vector<Direccion> sola = {{0,-1},{1,0},{0,1},{1,-1},{1,1}};

/**
 * @brief JugadorMaquina::JugadorMaquina Función que se ejecuta en el thread
 */
JugadorMaquina::JugadorMaquina(Jugador *parent): Jugador(parent)
{
    tiempoExcedido = false;
}

void JugadorMaquina::comenzarBusqueda(const int jug)
{
    jugador = jug;
    //Llama a la función Minimax que implementa el algoritmo para calcular la jugada

    primera = true;
    NIVEL_DEFECTO = 6;
    minimax(jugador,0,INT_MIN,INT_MAX);
    columna = iterador;
    //No borrar esta línea!!
    emit trabajoFinalizado(columna, jugador);
}
void JugadorMaquina::setTiempoExcedido(bool excedido) {
    tiempoExcedido = excedido;
}

/**
 * @brief JugadorMaquina::minimax Se debe determinar la mejor jugada mediante el algoritmo Minimax.
 * El tablero de juego se encuentra en la variable tablero.
 * Al final de la función de la variable columna debe contener la tirada.
 */

void JugadorMaquina::imprimirTablero(){
    for(int i = FILAS_TABLERO-1; i >= 0; i--){
        for(int j = 0; j < COLS_TABLERO; j++){
            if(tablero->obtenerCasilla(i,j) == 0){
                cout << "-" << "\t";
            }
            else if(tablero->obtenerCasilla(i,j) == 1){
                cout << "X" << "\t";
            }
            else{
                cout << "O" << "\t";
            }
        }
        cout << endl;
    }
    cout << "--------------------------" << endl;
}

//Devuelve true si la direccion que se le pasa esta dentro de los limites del tablero
bool JugadorMaquina::dentroBordes(int i, int j){
    if(i < FILAS_TABLERO && j < COLS_TABLERO && i >= 0 && j >= 0){ // Menor que los valores maximos de las filas y columnas
                                                                   // y mayor o igual a 0
        return true;
    }
    return false;
}

//Devuelve true si una ficha no tiene ninguna otra de su tipo alrededor
bool JugadorMaquina::fichaSola(int i, int j, int jug){
    bool sola = true;
    //Devuelve false si no esta sola y true si lo está
    for(int k = 0; k < 8; k++){
        if(dentroBordes(i+Dirs[k].x,j+Dirs[k].y) && (tablero->obtenerCasilla(i+Dirs[k].x, j+Dirs[k].y) == jug)){
            sola = false;
        }
    }
    return sola;
}

//Devuelve el numero de fichas que hay en una direccion
int JugadorMaquina::casillasEnDireccion(Direccion dir, int k, int jug){
    int multi = 1;
    int puntos = 0;

    if(dir.x == 0){
        while(dentroBordes(dir.x + multi*Dirs[k].x,dir.y + multi*Dirs[k].y) && tablero->obtenerCasilla(dir.x + multi*Dirs[k].x, dir.y + multi*Dirs[k].y) == jug && multi <=3){
            if(tablero->obtenerCasilla(dir.x + multi*Dirs[k].x, dir.y + multi*Dirs[k].y) == 0){
                multi++;
            }
            else if(tablero->obtenerCasilla(dir.x + multi*Dirs[k].x, dir.y + multi*Dirs[k].y) == jug){
                puntos++;
                multi++;
            }
            else{
                break;
            }
        }
    }
    else{
        if(dentroBordes(dir.x - Dirs[k].x,dir.y - Dirs[k].y) && tablero->obtenerCasilla(dir.x - Dirs[k].x, dir.y - Dirs[k].y) != jug){
            while(dentroBordes(dir.x + multi*Dirs[k].x,dir.y + multi*Dirs[k].y) && tablero->obtenerCasilla(dir.x + multi*Dirs[k].x, dir.y + multi*Dirs[k].y) == jug && multi <=3){
                if(tablero->obtenerCasilla(dir.x + multi*Dirs[k].x, dir.y + multi*Dirs[k].y) == 0){
                    multi++;
                }
                else if(tablero->obtenerCasilla(dir.x + multi*Dirs[k].x, dir.y + multi*Dirs[k].y) == jug){
                    puntos++;
                    multi++;
                }
                else{
                    break;
                }
            }
        }
    }
    return puntos;
}

int JugadorMaquina::contarFichas(){
    int fichas = 0;
    for(int i = 0; i < FILAS_TABLERO; i++){
        for(int j = 0; j < COLS_TABLERO; j++){
            if(tablero->obtenerCasilla(i,j) != 0){
                fichas++;
            }
        }
    }
    return fichas;
}

int JugadorMaquina::posibilidadesEnDireccion(Direccion dir, int k, int mult){
    int multi = mult + 1;
    int puntos = 0;
    while(dentroBordes(dir.x + multi*Dirs[k].x, dir.y + multi*Dirs[k].y) && tablero->obtenerCasilla(dir.x + Dirs[k].x,dir.y + multi*Dirs[k].y) == 0 && multi <= 3){
        puntos++;
        multi++;
    }
    return puntos;
}

int JugadorMaquina::posibilidadesFichaSola(Direccion dir, int k){
    int num = 0;
    int multi = 1;
    while(dentroBordes(dir.x+multi*sola[k].x,dir.y+multi*sola[k].y) && tablero->obtenerCasilla(dir.x+multi*sola[k].x,dir.y+multi*sola[k].y) == 0 && num <= 3){
        num++;
        multi++;
    }
    return num;
}

bool JugadorMaquina::tableroVacio(){
    for(int i = 0; i < FILAS_TABLERO; i++){
        for(int j = 0; j < COLS_TABLERO; j++){
            if(tablero->obtenerCasilla(i,j) != 0){
                return false;
            }
        }
    }
    return true;
}

//int JugadorMaquina::jugadasLibro(){

//}

int JugadorMaquina::contarPuntos(int jug){

    Direccion dir;

    int puntos = 0;

    bool finish = false;

    if(tiempoExcedido == true){
        return 0;
    }
    else{
        //Bucle que recorre todo el tablero
        for(int i = 0; i < FILAS_TABLERO && finish == false; i++){
            for(int j = 0; j < COLS_TABLERO && finish == false;j++){
                dir = {i,j};
                if(tablero->obtenerCasilla(dir.x,dir.y) == jug){
                    if(fichaSola(dir.x,dir.y,jug)){
                        puntos += 1;
                        for(int n = 0; n < 5; n++){
                            if(n < 4){
                                puntos += posibilidadesFichaSola(dir,n);
                            }
                            else if(n == 3){
                                if(tablero->obtenerCasilla(dir.x,dir.y-1) != 0){
                                    puntos += posibilidadesFichaSola(dir,n);
                                }
                            }
                            else{
                                if(tablero->obtenerCasilla(dir.x,dir.y+1) != 0){
                                    puntos += posibilidadesFichaSola(dir,n);
                                }
                            }
                        }
                    }
                    else{
                        for(int k = 0; k < 4; k++){
                            int dir1 = casillasEnDireccion(dir,k,jug);
                            int pos1 = posibilidadesEnDireccion(dir,k,dir1);
                            int pos2 = posibilidadesEnDireccion(dir,k+4,0);
                            if(1 + dir1 + pos1 + pos2 >= 4){
                                int fichas = 1 + dir1;
                                if(fichas == 2){
                                    puntos += valor2 + pos1 + pos2;
                                }
                                else if(fichas == 3){
                                    puntos += valor3 + pos1 + pos2;
                                }
                                else if(fichas == 4){
                                    puntos += valor4;
                                    finish = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        //cout << G1 << " " << G2 << " " << G3 << " " << G4 << " " << endl;
        //int puntos;
        //if(jug == jugador){
            //puntos = funcionHeuristica(G1,G2,G3,G4, jugador);
        //}
        //else{
            //puntos = funcionHeuristica(G1,G2,G3,G4, jug);
        //}
        //cout << "Puntos: " << puntos << endl;
        return puntos;
    }
}

bool JugadorMaquina::nodoHoja(int profundidad){
    //4 en raya, tablero lleno, profundidad alcanzada
    if(tablero->cuatroEnRaya() != 0 || tablero->tableroLleno() || profundidad == NIVEL_DEFECTO){
        return true;
    }
    return false;
}

int JugadorMaquina::minimax(int casilla, int profundidad, int alfa, int beta){

    oponente = 1;
    if(jugador == 1){
        oponente = 2;
    }

    if(profundidad == 1 && contarFichas() >= 15){// 15 && contarFichas() < 30){
        NIVEL_DEFECTO = 7;
    }
    //if(profundidad == 1 && contarFichas() >= 30){
    //    NIVEL_DEFECTO = 8
    //}

    if(!tiempoExcedido){
        if (primera && tableroVacio()){
            iterador = 3;
        }
        else{
            if(nodoHoja(profundidad)){
                //imprimirTablero();
                int puntos = contarPuntos(jugador) - contarPuntos(oponente);
                //cout << "Puntos: " << puntos << endl;
                return puntos;
            }
            else{
                if(profundidad%2 == 0){
                    profundidad++;
                    for(int i = 0; i < COLS_TABLERO; i++){
                        int fila = tablero->comprobarColumna(i);
                        if(fila != -1){
                            tablero->ponerFicha(i,casilla);
                            int max = minimax(oponente,profundidad, alfa, beta);
                            if(max > alfa){
                                alfa = max;
                                if(profundidad == 1){
                                    iterador = i;
                                    emit resultadoParcial(iterador);
                                }
                            }
                            tablero->cambiarCasilla(fila, i, 0);
                            if(alfa >= beta){
                                return beta;
                            }
                        }
                    }
                    return alfa;
                }
                else{
                    profundidad++;
                    for(int i = 0; i < COLS_TABLERO; i++){
                        int fila = tablero->comprobarColumna(i);
                        if(fila !=-1){
                            tablero->ponerFicha(i,casilla);
                            beta = min(beta, minimax(jugador, profundidad, alfa, beta));
                            tablero->cambiarCasilla(fila, i, 0);
                            if(alfa >= beta){
                                return alfa;
                            }
                        }
                    }
                    return beta;
                }
            }
        }

    }
    //cout << "Final: " << iterador << endl;
}