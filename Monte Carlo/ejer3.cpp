#include <iostream>
#include <cmath>
#include <vector>
#include <optional>
#include <algorithm>

using namespace std;

/*
  Compilar con C++17, es decir, g++ -o ejer3 ejer3.cpp -std=c++17
*/

//Genera un número uniformemente distribuido en el
//intervalo [0,1) a partir de uno de los generadores
//disponibles en C. Lo utiliza el generador de demanda
double uniforme(){
 int t = random();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}


int aritmetica_entera(int a, int x, int c, int m){
  return (a*x+c)%m;
}

long int aritmetica_real_artesanal(int a, int x, int c, int m){
  double aux = (a*(double)x+c)/m;
  aux = (aux - (long int)aux)*m;

  return (long int)aux;
}

long int aritmetica_real_artesanal_corregida(int a, int x, int c, int m){
  double aux = (a*(double)x+c)/m;
  aux = (aux - (long int)aux)*m;

  return (long int)(aux + 0.5);
}

long int aritmetica_real_fmod(int a, double x, int c, int m){
  return (long int)fmod((a*x+c), m);
}

#if __cplusplus == 201703L
  template<size_t index, typename T, typename... Ts,
      typename enable_if<index==0>::type* = nullptr>
  inline constexpr decltype(auto) get_argument(T&& t, Ts&&... ts) {
      return std::forward<T>(t);
  }
  template<size_t index, typename T, typename... Ts,
      typename enable_if<(index > 0 && index <= sizeof...(Ts))>::type* = nullptr>
  inline constexpr decltype(auto) get_argument(T&& t, Ts&&... ts) {
      return get_argument<index-1>(std::forward<Ts>(ts)...);
  }
  template<long long index, typename... Ts>
  inline constexpr bool index_in_range() {
      return index >= 0 && index < (long long)sizeof...(Ts);
  }
  template<long long index, typename T, typename... Ts,
      typename enable_if<(!index_in_range<index, T, Ts...>())>::type* = nullptr>
  inline constexpr decltype(auto) get_argument(T&& t, Ts&&... ts) {
      static_assert(index_in_range<index, T, Ts...>(),
          "bad index");
      return std::forward<T>(t);

  }

  /*
    to check number of params
  */
  template <typename F, typename ... Types> constexpr size_t getArgumentCount( F(*f)(Types ...))
  {
     return sizeof...(Types);
  }

  template <typename F, typename ... args>
  std::optional<int> periods(F f, args&&... vars){
    std::vector<long>p;
    if(getArgumentCount(F()) == 4){
      long int contador = 0;
      auto a = get_argument<0>(vars...);
      auto c = get_argument<2>(vars...);
      auto m = get_argument<3>(vars...);
      auto x = get_argument<1>(vars...);

      auto x_def = f(std::forward<args>(vars) ...);

      while(find(p.begin(), p.end(), x_def) == p.end()){
        p.emplace_back( x_def );
        ++contador;
        x_def = f( a, x_def, c, m );
      }
      return contador;
    }
    return std::nullopt;
  }
#endif

enum tipo { entero_type, artesanal_type, corregido_type, fmod_type };
int periods_2(int a, long int x, int c, int m, tipo flag){
  long int contador = 0;
  long int x_def;
  std::vector<long>p;
  switch (flag) {
    case entero_type:{
        x_def = aritmetica_entera(a, x, c, m);
        while(find(p.begin(), p.end(), x_def) == p.end()){
          p.emplace_back( x_def );
          ++contador;
          x_def = aritmetica_entera(a, x_def, c, m);
        }
        break;
    }
    case artesanal_type:{
      x_def = aritmetica_real_artesanal(a, x, c, m);
      while(find(p.begin(), p.end(), x_def) == p.end()){
        p.emplace_back( x_def );
        ++contador;
        x_def = aritmetica_real_artesanal(a, x_def, c, m);
      }
      break;
    }
    case corregido_type:{
      x_def = aritmetica_real_artesanal_corregida(a, x, c, m);
      while(find(p.begin(), p.end(), x_def) == p.end()){
        p.emplace_back( x_def );
        ++contador;
        x_def = aritmetica_real_artesanal_corregida(a, x_def, c, m);
      }
      break;
    }
    case fmod_type:{
      x_def = aritmetica_real_fmod(a, x, c, m);
      while(find(p.begin(), p.end(), x_def) == p.end()){
        p.emplace_back( x_def );
        ++contador;
        x_def = aritmetica_real_fmod(a, x_def, c, m);
      }
      break;
    }
  }
  return contador;
}

//El programa principal debe también incluir lo siguiente:
int main(int argc, char **argv){
  if(argc != 2){
    std::cout << "Número incorrecto de args " << endl;

    std::cout << "\nFormato de args : " << endl;
    std::cout << "\t\t   aritmetica_entera\n \
                  aritmetica_real_artesanal\n \
                  aritmetica_real_artesanal_corregida\n \
                  aritmetica_real_fmod\n";


    exit(1);
  }

  long int x_1;
  long int x_2;
  long int x_init = 101;

  int a_1 = 2061;
  int a_2 = 2060;
  int c = 4321;
  int m = 10000;


  vector <long int> p;
  unsigned contador = 0;

// Primer generador  : a1 = 2061
  switch (atoi(argv[1])) {
    case 1:{
        #if __cplusplus == 201703L
          auto str = periods(aritmetica_entera, a_1, x_init, c , m);
          if(str.has_value()){
            cout << "\nPrimer generador. Periodo : " << str.value();
          }

        #else
          int val = periods_2(a_1, x_init, c, m, tipo::entero_type);
          cout << "\nPrimer generador. Periodo : " << val;
        #endif
        break;
    }


    case 2:{
      #if __cplusplus == 201703L
        auto str = periods(aritmetica_real_artesanal, a_1, x_init, c , m);
        if(str.has_value()){
          cout << "\nPrimer generador. Periodo : " << str.value();
        }

      #else
        int val = periods_2(a_1, x_init, c, m, tipo::artesanal_type);
        cout << "\nPrimer generador. Periodo : " << val;
      #endif
      break;
    }

    case 3:{
      #if __cplusplus == 201703L
        auto str = periods(aritmetica_real_artesanal_corregida, a_1, x_init, c , m);
        if(str.has_value()){
          cout << "\nPrimer generador. Periodo : " << str.value();
        }

      #else
        int val = periods_2(a_1, x_init, c, m, tipo::corregido_type);
        cout << "\nPrimer generador. Periodo : " << val;
      #endif
      break;
    }

    case 4:{
      #if __cplusplus == 201703L
        auto str = periods(aritmetica_real_fmod, a_1, x_init, c , m);
        if(str.has_value()){
          cout << "\nPrimer generador. Periodo : " << str.value();
        }

      #else
        int val = periods_2(a_1, x_init, c, m, tipo::fmod_type);
        cout << "\nPrimer generador. Periodo : " << val;
      #endif
      break;
    }

  }

// Segundo generador  : a2 = 2060
  switch (atoi(argv[1])) {
    case 1:{
      #if __cplusplus == 201703L
        auto str = periods(aritmetica_entera, a_2, x_init, c , m);
        if(str.has_value()){
          cout << "\nPrimer generador. Periodo : " << str.value();
        }

      #else
        int val = periods_2(a_2, x_init, c, m, tipo::entero_type);
        cout << "\nPrimer generador. Periodo : " << val;
      #endif
      break;
    }

    case 2:{
      #if __cplusplus == 201703L
          auto str = periods(aritmetica_real_artesanal, a_2, x_init, c , m);
          if(str.has_value()){
            cout << "\nPrimer generador. Periodo : " << str.value();
          }

        #else
          int val = periods_2(a_2, x_init, c, m, tipo::artesanal_type);
          cout << "\nPrimer generador. Periodo : " << val;
        #endif
        break;
    }

    case 3:{
      #if __cplusplus == 201703L
          auto str = periods(aritmetica_real_artesanal_corregida, a_2, x_init, c , m);
          if(str.has_value()){
            cout << "\nPrimer generador. Periodo : " << str.value();
          }

        #else
          int val = periods_2(a_2, x_init, c, m, tipo::corregido_type);
          cout << "\nPrimer generador. Periodo : " << val;
        #endif
        break;
    }

    case 4:{
      #if __cplusplus == 201703L
          auto str = periods(aritmetica_real_fmod, a_2, x_init, c , m);
          if(str.has_value()){
            cout << "\nPrimer generador. Periodo : " << str.value();
          }

        #else
          int val = periods_2(a_2, x_init, c, m, tipo::fmod_type);
          cout << "\nPrimer generador. Periodo : " << val;
        #endif
        break;
    }

  }

}
