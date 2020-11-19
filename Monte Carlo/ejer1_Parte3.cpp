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
int genera_demanda(float* tabla,int tama){
  int i;
  double u = uniforme();
  i = 0;
  while((i<tama) && (u>=tabla[i])){
    i++;
  }
  return i;
}

//El programa principal debe también incluir lo siguiente:
int main(int argc, char **argv){
  bool mostrar = false;
  long veces = 0;
  long demanda = 0, apartado = 0;
  long ejecuciones = 100, x = 0, z = 0, y = 0;

  double sum = 0.0, sum2 = 0.0;
  double desviacion = 0.0, ganancia = 0.0, gananciaesperada = 0.0;
  double mejorganancia = 0.0, mejordesv = 0.0;

  float mejors = -1;
  float *tabla_demanda;


  if(argc == 1){
  x = 10;
  z = 7;
  y = 5;
  veces = 100;
  apartado = 1;
}else if(argc == 6){
  x = atoi(argv[1]);
  z = atoi(argv[2]);
  y = atoi(argv[3]);
  veces = atoi(argv[4]);
  apartado = atoi(argv[5]);
}else if(argc == 7){
  x = atoi(argv[1]);
  z = atoi(argv[2]);
  y = atoi(argv[3]);
  veces = atoi(argv[4]);
  apartado = atoi(argv[5]);
  ejecuciones = atoi(argv[6]);
}else{
  cout << "\nFormato erróneo de argumentos. ";
  cout << "\nPrimera opción. 5 argumentos: <X> <Z> <Y> <Veces> <Apartado>\n";
  cout << "\nSegunda opción. 6 argumentos: <X> <Z> <Y> <Veces> <Apartado> <Ejecuciones>\n";
  exit(1);
}
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
for (long s = 1; s < ejecuciones; s++){
	sum = 0.0;
	sum2 = 0.0;
		for(long i = 1; i < veces; i++){
			demanda = genera_demanda(tabla_demanda, ejecuciones);
			if(s > demanda) ganancia = (x*demanda) - std::min(z, (s-demanda)*y);
			else ganancia = s*x;

			sum += ganancia;
			sum2 += ganancia*ganancia;
		}
	gananciaesperada = sum/veces;
	desviacion = sqrt((sum2-veces*gananciaesperada*gananciaesperada)/(veces-1));
  if (mostrar) {
    cout << "Ganancia esperada : " << gananciaesperada << " S : " << s << endl;
  }
  if(gananciaesperada > mejorganancia){
    mejorganancia = gananciaesperada;
    mejordesv = desviacion;
    mejors = s;
  }
}

  if (!mostrar){
    cout << "\nMejor ganancia esperada : " << mejorganancia << " mejor s : " << mejors << \
            " Mejor desviación : " << mejordesv << endl;
  }
}
