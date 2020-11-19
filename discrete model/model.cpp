#include <iostream>
#include <cmath>
#include <list>
#include <vector>

using namespace std;

typedef struct {
	int suceso;
	float tiempo;
	float retraso;
	} suc;

const int suceso_llegada = 0;
const int suceso_finalizacion_servidor_A = 1;
const int suceso_finalizacion_servidor_B = 2;
const int suceso_fin_s = 3;

const double tiempo_servicio_servidor_A = 0.8f;
const double tiempo_servicio_servidor_B = 1.2f;
const double tiempo_llegadas = 1.0f;

list<suc> lsuc;
list<float> tiemposlleg;

int c_atendidos, colaA, colaB;
double crono, n_clientes_sis, t_parada;
bool libreA, libreB;

int ejecuciones;
suc actual;

bool fin_simu;

vector<double> tiempo_estancias;




bool compare(const suc &s1, const suc &s2)
  { return s1.tiempo < s2.tiempo; }

float generador_exponencial(float media){
  float u = (float) random() / (RAND_MAX+1.0);
  u = ceil(-media*log(1-u));
	return u;
}


suc generador_suceso(int suceso, double tiempo){
    suc suc_actual;
    suc_actual.suceso = suceso;
    suc_actual.tiempo = crono + generador_exponencial(tiempo);
    return suc_actual;
}


suc generar_fin(){
    suc suc_actual;
    suc_actual.suceso = suceso_fin_s;
    suc_actual.tiempo = crono + 60.0 * 8;
    return suc_actual;
}


void insertar_lsuc(suc n){
  lsuc.emplace_back(n); lsuc.sort(compare);
}

double calcular_tiempo_estancia(){
    double resultado =  n_clientes_sis / c_atendidos;
    return resultado;
}


void suceso_fin(){
  fin_simu = true;
  tiempo_estancias.emplace_back(calcular_tiempo_estancia());
}


void proximo_suc(){
    actual = lsuc.front();
		crono = actual.tiempo;
    lsuc.pop_front();
}

void inicializa_modelo(){
    libreA = true;
    libreB = true;
		crono = 0.0f;
		t_parada = 60.0 * 8;
    c_atendidos = 0;
    n_clientes_sis = 0.0;

		colaA = 0;
    colaB = 0;

    tiemposlleg.clear();
    lsuc.clear();
    insertar_lsuc(generador_suceso(suceso_llegada, tiempo_llegadas));
    insertar_lsuc(generar_fin());
    fin_simu = false;
}

void llegada_servidor(){
    tiemposlleg.push_back(crono);
    insertar_lsuc(generador_suceso(suceso_llegada, tiempo_llegadas));

    if (!libreA){
				colaA = colaA + 1;
    }
    else{
			libreA = false;
			insertar_lsuc(generador_suceso(suceso_finalizacion_servidor_A, tiempo_servicio_servidor_A));
		}
}

void fin_servicioA(){
  if (!libreB){
			colaB = colaB + 1;
  }
  else{
			libreB = false;
			insertar_lsuc(generador_suceso(suceso_finalizacion_servidor_B, tiempo_servicio_servidor_B));
  }

  if (colaA <= 0){
			libreA = true;
  }
  else{
		colaA = colaA - 1;
		insertar_lsuc(generador_suceso(suceso_finalizacion_servidor_A, tiempo_servicio_servidor_A));
	}
}

void fin_servicioB(){
    c_atendidos = c_atendidos + 1;
    n_clientes_sis = n_clientes_sis + (crono - tiemposlleg.front());

    tiemposlleg.pop_front();
    if (colaB <= 0){
        libreB = true;
    }
    else{
			colaB = colaB - 1;
			insertar_lsuc(generador_suceso(suceso_finalizacion_servidor_B, tiempo_servicio_servidor_B));
    }
}

void muestra_resultados(){
  double total = 0;

  for(auto t = tiempo_estancias.begin(); t != tiempo_estancias.end(); ++t){
    total += *t;
  }

  cout << "Número de simulaciones " << ejecuciones << endl;
  cout << "Tiempo de estancia " << total / ejecuciones << endl;

}

void suceso_tipo(){
    switch(actual.suceso){
        case suceso_llegada:
            llegada_servidor();
            break;
        case suceso_finalizacion_servidor_A:
            fin_servicioA();
            break;
        case suceso_finalizacion_servidor_B:
            fin_servicioB();
            break;
        case suceso_fin_s:
            suceso_fin();
            break;
    }
}


int main(int argc, char **argv){
  srand(time(NULL));

  if(argc != 2){
    cout << "args fmt : <ejecuciones>" << endl;
    exit(-1);
  }

  ejecuciones = atoi(argv[1]);

  for (int i = 0; i < ejecuciones; i++){
    inicializa_modelo();

    while (!fin_simu){
      proximo_suc();
      suceso_tipo();
    }
  }

  muestra_resultados();
}
