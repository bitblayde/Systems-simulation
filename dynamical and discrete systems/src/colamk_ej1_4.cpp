#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>
#include <vector>

using namespace std;

#define suceso_llegada 0
#define suceso_salida 1
#define suceso_finsimulacion 3
#define suceso_monitor 4

typedef struct {
	int suceso;
	float tiempo;
	float retraso;
	} suc;
list<suc> lsuc;
suc nodo;
int nada = 0;
list<float> cola;
int m; //numero de servidores
float tparada;
float tlleg;
float tserv;
float valor;

float reloj;;
int libres;
int encola;
int ensistema;
int atendidos;
float acum_cola;
float acum_sistema;
float acum_ocio;
float acum_retraso;
float tultsuc_cola;
float tultsuc_ocio;
float tultsuc_sistema;
float acum_sincola;
float init_sincola;
int maximacola;
bool parar;

vector<vector<float>> desv;
vector<int> total_desv;
//float res[6]={0.0};

float med[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
int total_med = 0;
bool mostrar_por_pantalla = true;

bool compare(const suc &s1, const suc &s2)
{ return s1.tiempo < s2.tiempo; }


/* Inserta de forma ordenada un elemento en la lista de sucesos */
void insertar_lsuc(suc n)
{
  lsuc.push_back(n);
  // Mantener ordenada la lista por el tiempo de los sucesos
  lsuc.sort(compare);
}

float generador_uni(float media)
{
  float u;
  u = (float) random();
  u = u/(float)(RAND_MAX+media);
  return u;
}



// Generador de tiempos entre llegadas (exponencial)
float generallegada(float media)
{ return generador_uni(media); }

// Generador de tiempos de servicio (exponencial)
float generaservicio(float media)
{ return generador_uni(media); }


/* Procedimiento inicializacion */
void inicializacion()
{

srandom(time(NULL));
reloj = 0.0;
libres = m;
encola = 0;
ensistema = 0;
atendidos = 0;
acum_cola = 0.0;
acum_sistema = 0.0;
acum_ocio = 0.0;
acum_retraso = 0.0;
tultsuc_cola = reloj;
tultsuc_ocio = reloj;
tultsuc_sistema = reloj;
acum_sincola = 0.0;
init_sincola = reloj;
maximacola = 0;
nodo.suceso = suceso_llegada;
nodo.tiempo = reloj+generallegada(tlleg);
nodo.retraso = nada;
insertar_lsuc(nodo);
nodo.suceso = suceso_finsimulacion;
nodo.tiempo = reloj+tparada;
nodo.retraso = nada;
insertar_lsuc(nodo); //tparada es un par�metro de entrada al programa
//Inicializar el suceso monitor si que quiere trazar alguna medida del rendimiento a lo largo del tiempo, no solo al final

med[0] = 0.0f;
med[1] = 0.0f;
med[2] = 0.0f;
med[3] = 0.0f;
med[4] = 0.0f;
med[5] = 0.0f;

total_med = 0;

parar = false;
}

/* Procedimiento temporizacion */
void temporizacion()
{
  nodo = lsuc.front();
  lsuc.pop_front();
  reloj = nodo.tiempo;
//  printf("\n%.3f",reloj);
}

void llegada(){
	acum_sistema += (reloj - tultsuc_sistema) * ensistema;
	tultsuc_sistema = reloj;
	ensistema ++;
	nodo.suceso = suceso_llegada;
	nodo.tiempo = reloj+generallegada(tlleg);
	nodo.retraso = nada;
	insertar_lsuc(nodo);

	if (libres > 0){
	      acum_ocio += (reloj - tultsuc_ocio) * libres;
	      tultsuc_ocio = reloj;
	      libres --;
	      nodo.suceso = suceso_salida;
	      nodo.tiempo = reloj+generaservicio(tserv);
	      nodo.retraso = nada;
	      insertar_lsuc(nodo);
	}
	else {
	      if (encola == 0){
					acum_sincola += reloj - init_sincola;
				}
	      acum_cola += (reloj - tultsuc_cola) * encola;
	      tultsuc_cola = reloj;
	      encola++;
	      if (encola > maximacola){
					maximacola = encola;
				}
	      cola.push_back(reloj);
	}
}

void salida(){
acum_sistema += (reloj - tultsuc_sistema) * ensistema;
tultsuc_sistema = reloj;
ensistema --;
atendidos ++;
acum_retraso += nodo.retraso;
if (encola > 0){
      acum_cola += (reloj - tultsuc_cola) * encola;
      tultsuc_cola = reloj;
      encola --;
      if (encola == 0){
				init_sincola = reloj;
			}
      valor = cola.front();
      cola.pop_front();
      nodo.suceso = suceso_salida;
      nodo.tiempo = reloj+generaservicio(tserv);
      nodo.retraso = reloj-valor;
      insertar_lsuc(nodo);
}
else {
      acum_ocio += (reloj - tultsuc_ocio) * libres;
      tultsuc_ocio = reloj;
      libres ++;
     }
}

void fin(int iteracion){
	parar = true; //para detener la simulaci�n
	              //habr� que hacer las �ltimas actualizaciones de algunas variables
	float retrasomedio = acum_retraso/atendidos;
	float estanciamedia = retrasomedio + tserv;
	acum_cola += (reloj - tultsuc_cola) * encola;
	float encolamedio = acum_cola/reloj;
	acum_sistema += (reloj - tultsuc_sistema) * ensistema;
	float ensistemamedio = acum_sistema/reloj;
	if (encola == 0){
		acum_sincola += reloj - init_sincola;
	}
	float colasnovaciasmedio = acum_cola/(reloj - acum_sincola);
	acum_ocio += (reloj - tultsuc_ocio) * libres;
	float porcentajemedioocio = 100*acum_ocio/(m*reloj);

	if (!mostrar_por_pantalla){
		printf("\nTiempo medio de espera en cola = %.3f",retrasomedio);
		printf("\nTiempo medio de estancia en el sistema = %.3f",estanciamedia);
		//printf("\nTiempo medio de espera en cola bis = %.3f",acum_cola/atendidos);
		//printf("\nTiempo medio de estancia en el sistema bis= %.3f",acum_cola/atendidos+tserv);
		printf("\nNumero medio de personas en cola = %.3f",encolamedio);
		printf("\nNumero medio de personas en el sistema = %.3f",ensistemamedio);
		printf("\nLongitud media de colas no vacias = %.3f",colasnovaciasmedio);
		printf("\nporcentaje medio de tiempo de ocio por servidor = %.3f",porcentajemedioocio);
		printf("\nLongitud m�xima de la cola = %d",maximacola);
		printf("\n");
	}
	else{

		total_med = total_med + maximacola;
		total_desv.push_back(maximacola);


		med[0] += retrasomedio;
		med[1] += estanciamedia;
		med[2] += encolamedio;
		med[3] += ensistemamedio;
		med[4] += colasnovaciasmedio;
		med[5] += porcentajemedioocio;


		desv[0][iteracion] = retrasomedio;
		desv[1][iteracion] = estanciamedia;
		desv[2][iteracion] = encolamedio;
		desv[3][iteracion] = ensistemamedio;
		desv[4][iteracion] = colasnovaciasmedio;
		desv[5][iteracion] = porcentajemedioocio;


	}
}

//Si se desea monitorizar, por ejemplo el n�mero medio de clientes en sistema a lo largo del tiempo se puede usar el suceso monitor siguiente, que habr� que inicializar
void monitor()
{
nodo.suceso = suceso_monitor;
nodo.tiempo = reloj+100.0;
nodo.retraso = nada;
insertar_lsuc(nodo);
cout<<"tiempo y media en sistema "<<reloj<<" "<<(acum_sistema+(reloj - tultsuc_sistema) * ensistema)/reloj<<endl;
//printf("\n %.3f, %3f","tiempo y media en sistema",reloj,(acum_sistema+(reloj - tultsuc_sistema) * ensistema)/reloj);
}

/* Procedimiento suceso */
void suceso(int iteracion)
{
 switch(nodo.suceso)
                {
                 case suceso_llegada: llegada(); break;
                 case suceso_salida: salida(); break;
                 case suceso_finsimulacion: fin(iteracion); break;
		 case suceso_monitor: monitor(); break;
                }
}

void init_desv(int ejecuciones){
	for(int i = 0; i < ejecuciones; i++){
		desv.push_back(vector<float>(ejecuciones));
	}
}

int main(int argc, char *argv[]){
	int i;
	int ejecuciones = 100;
	float acc;

  if (argc != 5 && argc !=6){
     printf("\n\nFormato Argumentos -> <numero_servidores> <tiempo de parada> <tlleg> <tserv>\n\n");
		 printf("\n\nFormato Argumentos -> <numero_servidores> <tiempo de parada> <tlleg> <tserv> <ejecuciones>\n\n");
     exit(1);
  }
	else if (argc == 5){
	  sscanf(argv[1],"%d",&m);
	  sscanf(argv[2],"%f",&tparada);
	  sscanf(argv[3],"%f",&tlleg);
	  sscanf(argv[4],"%f",&tserv);
	}
	else{
		sscanf(argv[1],"%d",&m);
		sscanf(argv[2],"%f",&tparada);
		sscanf(argv[3],"%f",&tlleg);
		sscanf(argv[4],"%f",&tserv);
		sscanf(argv[4],"%f",&ejecuciones);
	}

	srandom(time(NULL));

	init_desv(ejecuciones);

	for (int i = 0; i < ejecuciones; i++){
		  inicializacion();

			while (!parar){
			    temporizacion();
			    suceso(i);
			}
	}

	if(mostrar_por_pantalla){
		med[0] = med[0] / ejecuciones;
		med[1] = med[1] / ejecuciones;
		med[2] = med[2] / ejecuciones;
		med[3] = med[3] / ejecuciones;
		med[4] = med[4] / ejecuciones;
		med[5] = med[5] / ejecuciones;

		total_med = total_med / ejecuciones;
		acc = (float) 1 /(ejecuciones - 1);

		for (int i = 0; i < ejecuciones; i++){
			desv[0][i] = (desv[0][i] * desv[0][i]) - ejecuciones*(med[0]*med[0]);
			desv[1][i] = (desv[1][i] * desv[1][i]) - ejecuciones*(med[1]*med[1]);
			desv[2][i] = (desv[2][i] * desv[2][i]) - ejecuciones*(med[2]*med[2]);
			desv[3][i] = (desv[3][i] * desv[3][i]) - ejecuciones*(med[3]*med[3]);
			desv[4][i] = (desv[4][i] * desv[4][i]) - ejecuciones*(med[4]*med[4]);
			desv[5][i] = (desv[5][i] * desv[5][i]) - ejecuciones*(med[5]*med[5]);

			total_desv[i] = total_desv[i]*total_desv[i] - ejecuciones * pow(total_med, 2);

		}


		for (int i = 0; i < ejecuciones; i++){
			desv[0][0] += desv[0][i];
			desv[1][0] += desv[1][i];
			desv[2][0] += desv[2][i];
			desv[3][0] += desv[3][i];
			desv[4][0] += desv[4][i];
			desv[5][0] += desv[5][i];

			total_desv[0] += total_desv[i];
		}

		desv[0][0] = sqrt(desv[0][0]*acc);
		desv[1][0] = sqrt(desv[1][0]*acc);
		desv[2][0] = sqrt(desv[2][0]*acc);
		desv[3][0] = sqrt(desv[3][0]*acc);
		desv[4][0] = sqrt(desv[4][0]*acc);
		desv[5][0] = sqrt(desv[5][0]*acc);

		total_desv[0] = sqrt(total_desv[0]*acc);


		cout << "\nValores teóricos \n";
		cout << "Tiempo medio de espera en cola " << (float) ((tserv*tserv)/(tlleg-tserv)) << endl;
		cout << "Tiempo medio de estancia en el sistema " << (float)(tserv*tlleg)/(tlleg-tserv) << endl;
		cout << "Numero medio de clientes en cola " << (float)(tserv*tserv)/(tlleg*(tlleg-tserv)) << endl;
		cout << "Numero medio de personas en el sistema " << (float) tserv/(tlleg-tserv) << endl;
		cout << "Longitud media de colas no vacias " << (float) tlleg/(tlleg-tserv) << endl;
		cout << "Porcentaje medio de tiempo de ocio por servidor " << (float) (1 - (tserv/tlleg))*100 << endl;

		cout << "\nValores tras la experimentación \n";
		cout << "Tiempo medio de espera en cola " << med[0] << endl;
		cout << "Tiempo medio de estancia en el sistema " << med[1] << endl;
		cout << "Numero medio de clientes en cola " << med[2] << endl;
		cout << "Numero medio de personas en el sistema " << med[3] << endl;
		cout << "Longitud media de colas no vacias " << med[4] << endl;
		cout << "Porcentaje medio de tiempo de ocio por servidor " << med[5] << endl;
		cout << "Media máxima " << total_med << "\n\n";


		cout << "Desviación Típica. Valores tras la experimentación \n";
		cout << "Desviación Típica. Tiempo medio de espera en cola " << desv[0][0] << endl;
		cout << "Desviación Típica. Tiempo medio de estancia en el sistema " << desv[1][0] << endl;
		cout << "Desviación Típica. Numero medio de clientes en cola " << desv[2][0] << endl;
		cout << "Desviación Típica. Numero medio de personas en el sistema " << desv[3][0] << endl;
		cout << "Desviación Típica. Longitud media de colas no vacias " << desv[4][0] << endl;
		cout << "Desviación Típica. Porcentaje medio de tiempo de ocio por servidor " << desv[5][0] << endl;

		cout << "Desviación Típica. Máxima de la cola " << total_desv[0] << "\n\n";
	}

}
