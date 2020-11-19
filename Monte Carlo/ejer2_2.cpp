#include <iostream>
#include <cmath>

using namespace std;

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
float* construye_prop_a(int n) {
int i;
float* temp;
if ((temp = (float*) malloc(n*sizeof(float))) == NULL)
  {
   fputs("Error reservando memoria para generador uniforme\n",stderr);
   exit(1);
  }
temp[0] = 1.0/n;
for (i=1;i<n;i++)
  temp[i] = temp[i-1]+1.0/n;
return temp;
}

//Construye la tabla de búsqueda de
//tamaño n para la distribución de
//la demanda del apartado (b).
float* construye_prop_b(int n){
int i, max;
float* temp;
if ((temp = (float*) malloc(n*sizeof(float))) == NULL)
  {
   fputs("Error reservando memoria para generador proporcional\n",stderr);
   exit(1);
  }
max = (n/2)*(n+1);
temp[0] = n*1.0/max;
for (i=1;i<n;i++)
  temp[i] = temp[i-1]+(float)(n-i)/max;
return temp;
}

//Construye la tabla de búsqueda de
//tamaño n para la distribución de
//la demanda del apartado (c).
float* construye_prop_c(int n){
  int i, max;
  float* temp;
  if ((temp = (float*) malloc(n*sizeof(float))) == NULL){
     fputs("Error reservando memoria para generador triangular\n",stderr);
     exit(1);
    }
  max = n*n/4;
  temp[0] = 0.0;
  for (i=1;i<(n/2);i++)
    temp[i] = temp[i-1]+(float)i/max;
  for (i=(n/2);i<n;i++)
    temp[i] = temp[i-1]+(float)(n-i)/max;
  return temp;
}

// Genera un valor de la
// distribución de la demanda codificada en tabla, por el
// método de tablas de búsqueda.
// tama es el tamaño de la tabla, 100 en nuestro caso particular

int genera_demanda_1(float* tabla,int tama){
  int i;
  double u = uniforme();
  i = 0;
  while((i<tama) && (u>=tabla[i])){
    i++;
  }
  return i;
}

int genera_demanda(float* tabla,int tama){
  int centro;
  double u = uniforme();
  int izda = 0, dcha = tama - 1;
  bool fin = false;
  while(izda <= dcha && !fin){
    centro = izda + (dcha - izda) / 2;
    if (tabla[centro] == u) return centro;
    else if ( tabla[centro] >= u ) dcha = centro - 1;
    else  izda = centro + 1;
  }
  return -1;
}

//El programa principal debe también incluir lo siguiente:
int main(int argc, char **argv){
  long veces = 0;
  long apartado = 0;
  float *tabla_demanda;
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

cout << "\nDatos a generar : " << veces << endl;

clock_t init = clock();
for (unsigned i{0}; i < veces; i++){
  genera_demanda(tabla_demanda, 100);
}

clock_t fin = clock();
double tiempo = (double) (fin - init) / CLOCKS_PER_SEC;

cout << "Segundos : " << tiempo << endl;
}
