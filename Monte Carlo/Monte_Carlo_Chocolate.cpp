#include <iostream>
#include <cmath>

using namespace std;

int a = 2000, b = 3000, c = 2600;


float uniforme(){
  float u;
  u = (float)random();
  u = u/(float)(RAND_MAX+1.0);

  return u;
}

int genera_demanda(){
  float u, x;
  u = uniforme();

  if (u < ( c - a) / (b - a) ){
    x = a + sqrt((b - a) * (c - a) * u);
  }

  else{
    x = b - sqrt((b - a)*(b - c)*(1 - u));
  }

  return (int)x;
}

int main(int argc, char **argv){
  if (argc != 2 and argc != 1){
    cout << "\nNúmero incorrecto de argumentos." << endl;
    cout << "\nFormato: <número de ejecuciones>." << endl;
    exit(1);
  }
  srand (time(NULL));
  int ejecuciones = 100;

  if (argc == 2){
    ejecuciones = atoi(argv[1]);
  }


  float suma, total;
  int demanda_actual;
  float beneficios = 0.0f, perdidas = 0.0f, resultado_operacion = 0.0f;
  float total_esperado = 0.0f;
  float desv = 0.0f;

  int mejorS = 0;

  float ganancia_max = 0.0f, desv_max = 0.0f;

  for(auto s = a; s <= b; s++){
    suma = 0.0f;
    total = 0.0f;

    for(int i = 0; i < ejecuciones; i++){
      demanda_actual = genera_demanda();

      if(s < demanda_actual){  // Se tienen que comprar productos en diciembre

        beneficios = (float)demanda_actual * 0.6;

        // si 1 euro por kilo son 0.25, 1.5 euros por kilos serían 0.375
        // es más simple emplearlo en términos de peso que de euros.
        perdidas = (float)(demanda_actual-s) * 0.375 + (float)s*0.25;

      }

      else{
        beneficios = (float)demanda_actual * 0.6;
        perdidas = (float)s * 0.25;

      }

      resultado_operacion = beneficios - perdidas;
      suma = suma + resultado_operacion;
      total = total + (pow(resultado_operacion, 2));

    }

    total_esperado = suma / ejecuciones;
    desv = sqrt((total-ejecuciones*total_esperado*total_esperado)/(ejecuciones-1));

    if (total_esperado > ganancia_max){
      ganancia_max = total_esperado;
      desv_max = desv;
      mejorS = s;
    }

  }
	
	cout << mejorS << ", " << ganancia_max << ", " << mejorS*0.25 << endl;
/*
  cout << "Mejor S : " << mejorS << ". Mejor ganancia esperada : " << ganancia_max \
        << ". Desviacion : " << desv_max;
  cout << ". Kilos de chocolate : " << mejorS*0.25 << endl;
*/
}
