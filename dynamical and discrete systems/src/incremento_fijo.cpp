#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>

using namespace std;

#define suceso_llegada 0
#define suceso_salida 1
#define suceso_finsimulacion 3
#define suceso_monitor 4



////////////////////////////////////////
int total_a_atender;
long long infinito;
int inicio_ocio;
bool servidor;
long tiempo_llegada, tiempo_salida;
double ocio = 0;
double tultsuc;
double porcent_ocio, media_encola;

long reloj, atendidos, encola;

double tserv, tlleg, acum_cola;



bool formato_extendido = false;


float generador_exponencial(float media){
  float u = (float) random() / (RAND_MAX+1.0);
  u = ceil(-media*log(1-u));

  if (u == 0){
    return 1.0f;
  }
  else{
    return u;
  }
}

// Generador de tiempos entre llegadas (exponencial)
float generallegada(float media)
{ return generador_exponencial(media); }

// Generador de tiempos de servicio (exponencial)
float generaservicio(float media)
{ return generador_exponencial(media); }


/* Procedimiento inicializacion */
void inicializacion(){
  srandom(time(NULL));
  infinito = 10e14; //tiempo en que ocurren cosas que sabemos no pueden ocurrir
  // 9223372036854775807
  atendidos = 0; //al principio no hay nadie ya atendido
  inicio_ocio = 0.0; //marcará el momento en que el servidor empieza a estar ocioso
  acum_cola = 0.0; //acumulador de número de clientes en cola por el tiempo en que están en
                   //dicha cola. El cliente que está siendo atendido NO está en cola
  reloj = 0; //marca el valor del tiempo simulado, inicialmente cero
  servidor = true; //inicialmente el servidor no está atendiendo a nadie
  encola = 0; //no hay nadie en cola todavía
  tiempo_llegada = reloj + generallegada(tlleg); //en ese tiempo llegará el primer cliente
  tiempo_salida = infinito; //nadie puede salir si nadie ha entrado aun
  ocio = 0;
  tultsuc = reloj;
}

int main(int argc, char *argv[]){
  int i;
  int ejecuciones = 100;

  if(argc == 1){
    tlleg = 0.5;
    tserv = 0.2;
    total_a_atender = 10000;
  }
  else if(argc == 4){
    sscanf(argv[1],"%f",&tlleg);
    sscanf(argv[2],"%f",&tserv);
    sscanf(argv[3],"%ld",&total_a_atender);
  }
  else if(argc == 5){
    sscanf(argv[1],"%f",&tlleg);
    sscanf(argv[2],"%f",&tserv);
    sscanf(argv[3],"%ld",&total_a_atender);
    sscanf(argv[4],"%ld",&ejecuciones);
  }
  else  {
    cout << "\n\nFormato Argumentos -> <tlleg> <tserv> <nº clientes>\n\n";
    cout << "\n\nFormato Argumentos -> <tlleg> <tserv> <nº clientes> <ejecuciones>\n\n";
    exit(1);
  }

  for(int i = 0; i < ejecuciones; i++){
    inicializacion();
    clock_t init = clock();
    while (atendidos < total_a_atender){
      if (reloj == tiempo_llegada){
        tiempo_llegada = reloj + generallegada(tlleg);
        if (servidor){
              servidor = false;
              tiempo_salida = reloj + generaservicio(tserv);
              ocio += reloj - inicio_ocio;
        }
        else {
              acum_cola += (reloj - tultsuc) * encola;
              tultsuc = reloj;
              encola++;
        }
      }
      if (reloj == tiempo_salida){
        atendidos++;
        if (encola > 0){
              acum_cola += (reloj - tultsuc) * encola;
              tultsuc = reloj;
              encola--;
              tiempo_salida = reloj + generaservicio(tserv);
             }
        else {
              servidor = true; //el servidor se queda ocioso por falta de clientes
              inicio_ocio = reloj; //marcamos cuando empieza a estar ocioso
              tiempo_salida = infinito; //nadie puede salir puesto que nadie hay
             }
       }
       reloj++;   //el tiempo se incrementa en una unidad
      }
      clock_t fin = clock();
      double tiempo = (double) (fin - init) / CLOCKS_PER_SEC;
      porcent_ocio = ocio*100/reloj; //calculamos el porcentaje de tiempo de ocio del servidor

      media_encola = acum_cola/reloj; //calculamos el número medio de clientes en cola


      if (formato_extendido){
        cout << "Nº de clientes en la cola " << total_a_atender << endl;
        cout << "Tiempo. Ocio : " << porcent_ocio << endl;
        cout << "Tiempo total. Ejecución  : " << tiempo << endl;
        cout << "Nº medio de clientes en la cola " << media_encola << endl;
      }
      else{
        cout << total_a_atender << " \t" << porcent_ocio << " \t" << tiempo \
             << " \t" << media_encola << endl;
      }
  }
  if (tlleg > tserv){
    cout << "\n\nServicio entre llegada " << tserv / tlleg << endl;

    cout << "PTO " << (1 - (tserv / tlleg)) * 100 << endl;
    cout << "Q(n) " << pow((tserv / tlleg), 2) / (1 - (tserv / tlleg)) << endl;
  }
}
