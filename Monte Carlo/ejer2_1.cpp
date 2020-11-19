#include <iostream>
#include <cmath>

using namespace std;

// Estructura que almacena la probabilidad acumulada y la posición
struct tabla{
  unsigned pos;
  float prob;
};

//Genera un número uniformemente distribuido en el
//intervalo [0,1) a partir de uno de los generadores
//disponibles en C. Lo utiliza el generador de demanda
double uniforme(){
 int t = random();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

//Construye la tabla de búsqueda de
//tamaño n para la distribución de
//la demanda del apartado (a).
tabla* construye_prop_a(int n) {
tabla* temp;

if ((temp = (tabla*) malloc(n*sizeof(tabla))) == NULL)
  {
   fputs("Error reservando memoria para generador uniforme\n",stderr);
   exit(1);
 }


temp[0].pos = 0;
temp[0].prob = 1/n;

for (unsigned i=1;i<n;i++){
  temp[i].prob = temp[i-1].prob+1.0/n;
  temp[i].pos = i;
}

return temp;
}

//Construye la tabla de búsqueda de
//tamaño n para la distribución de
//la demanda del apartado (b).

tabla* construye_prop_b(int n){
int max;
tabla* temp;
if ((temp = (tabla*) malloc(n*sizeof(tabla))) == NULL)
  {
   fputs("Error reservando memoria para generador proporcional\n",stderr);
   exit(1);
  }
max = (n/2)*(n+1);
temp[0].prob = n*1.0/max;
temp[0].pos = 0;
for (unsigned i=1;i<n;i++){
  temp[i].prob = temp[i-1].prob+(float)(n-i)/max;
  temp[i].pos = i;
}
return temp;
}

//Construye la tabla de búsqueda de
//tamaño n para la distribución de
//la demanda del apartado (c).
tabla* construye_prop_c(int n){
  int max;
  tabla* temp;

  if ((temp = (tabla*) malloc(n*sizeof(tabla))) == NULL){
     fputs("Error reservando memoria para generador triangular\n",stderr);
     exit(1);
    }
  max = n*n/4;

  int mitad = (int)n/2;

  temp[0].prob = mitad/max;
  temp[0].pos = mitad;

  unsigned aux, actual = 0;
  float aux_prob;

  for (unsigned i = mitad - 1; i > 0; i--){
    ++actual;
    aux = --mitad;
    aux_prob = (float)i / max;

    temp[actual].prob = temp[actual-1].prob+aux_prob;
    temp[actual].pos = i;

    ++actual;
    temp[actual].prob = temp[actual-1].prob+aux_prob;
    temp[actual].pos = mitad + aux;
  }
return temp;
}


// Genera un valor de la
// distribución de la demanda codificada en tabla, por el
// método de tablas de búsqueda.
// tama es el tamaño de la tabla, 100 en nuestro caso particular
int genera_demanda_tabla(tabla* tabla,int tama){
  int i;
  double u = uniforme();
  i = 0;
  while((i<tama) && (u>=tabla[i].prob)){
    i++;
  }
  return tabla[i].pos;
}



int main(int argc, char **argv){
  long veces = 0;
  long apartado = 0;
  tabla *tabla_demanda;
  auto ejecuciones = 100;

  if(argc != 3){
    cout << "\nNúmero de incorrecto argumentos\n";
    cout << "\nFormato de argumentos: <Apartado> <Veces>\n";
    exit(1);
  }

  apartado = atoi(argv[1]);
  veces = atoi(argv[2]);

  srand(time(NULL)); //Inicializa el generador de números pseudoaleatorios
  if(apartado == 1){
    tabla_demanda = construye_prop_a(ejecuciones);
  }
  else if(apartado == 2){
    tabla_demanda = construye_prop_b(ejecuciones);
  }
  else if(apartado == 3){
    tabla_demanda = construye_prop_c(ejecuciones);
  }
  else{
    cout << "\nNúmero de bloque incorrecto\n";
    exit(1);
  }
int resultado;
cout << "\nDatos a generar : " << veces << endl;

clock_t init = clock();
for (unsigned i{0}; i < veces; i++){
  resultado = genera_demanda_tabla(tabla_demanda, 100);
}

clock_t fin = clock();
double tiempo = (double) (fin - init) / CLOCKS_PER_SEC;

cout << "Segundos : " << tiempo << endl;

}
