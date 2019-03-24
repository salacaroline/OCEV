/*
Funcionalidades:
Codificações: Binária, Inteira e Real
Tipo da codificação (COD = BIN, INT, INT-PERM ou REAL)
Tamanho da população (POP)
Tamanho do cromossomo (D)    
Se codificação binária, representa a quantidade de bits do cromossomo
Se codificação real, representa a quantidade de variáveis sendo otimizada (número de dimensões do problema)
Domínio das variáveis: Bounds = [Li, Ui] Є R
Se codificação inteira, representa a quantidade de inteiros a se         
considerar (VERIFICAR SE É PERMUTAÇÃO)
Domínio das variáveis: Bounds = [Li, Ui] Є Z
Se permutação (INT-PERM), D representa a quantidade de inteiros a permutar

*/

/*Compilar com flag 
$ g++ --std=c++11 ag.cpp -o teste
*/


#include <bits/stdc++.h>

using namespace std;

int avaliaSolucaoRainhas(vector<vector <double>> populacao, int POP, int D);
void avaliaSolucaoFx(vector<vector <double>> populacao, int POP, int D, int L, int U);
double funcaoAlgebrica(double x);

int main(){
  char COD, problema;
  int POP, D, aux;
  double L, U;
  
  //default_random_engine generator;
  cin >> COD >> POP >> D >> L >> U >> problema;
  unsigned semente = chrono::system_clock::now().time_since_epoch().count();
  uniform_real_distribution<double> distribuicaoReal(L, U);
  uniform_int_distribution<int> distribuicaoBinaria(0, 1);
  uniform_int_distribution<int> distribuicaoInteira((int)L, (int)U);
  mt19937_64 gerador(semente);
  //Vetor do inteiro permutado
  int permuta[D] = {0};
  //Zerei vetor
  for(int i = 0; i < D; i++){
    permuta[i] = 0;
  }



  //Espaço de busca em double para ser utilizado também por inteiros e binários
  vector<vector <double>> populacao (POP, vector<double>());

  //Verifica qual é a codificação
  switch (COD) {
    /*Se B = Binária
      L=0 e U=1
    */
    case 'B':
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
          populacao[i].push_back(distribuicaoBinaria(gerador));
        }
      }
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
        //  printf("%02d   ", (int)populacao[i][j] );
          cout << populacao[i][j] << "    ";
        }
        cout << endl;
      }
      break;
    
    //Se I = Inteira
    case 'I':
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
          populacao[i].push_back(distribuicaoInteira(gerador));
        }
      }
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
        //  printf("%02d   ", (int)populacao[i][j] );
          cout << populacao[i][j] << "    ";
        }
        cout << endl;
      }
      break;
    /*Se P = Inteiro Permutado
      L=0 e U=D-1
    */
    case 'P':
      //Verifica integridade dos parametros
      if(!(L==0 && U==D-1)){
        cout << "Erro: Corrija os parametros L e U para L=0 e U=D-1" << endl;
        break;
      }
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
          aux = distribuicaoInteira(gerador);
          
          while(permuta[aux] == 1){
            aux = distribuicaoInteira(gerador);
          }
          populacao[i].push_back(aux);
          permuta[aux] = 1;
        }
          for(int k = 0; k < D; k++){
            permuta[k] = 0;
          }
      }
      
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
        //  printf("%02d   ", (int)populacao[i][j] );
          cout << populacao[i][j] << "    ";
        }
        cout << endl;
      }
      break;
    //Se R == Real
    case 'R':
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
          populacao[i].push_back(distribuicaoReal(gerador));
        }
      }
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
        //  printf("%02d   ", (int)populacao[i][j] );
          cout << populacao[i][j] << "    ";
        }
        cout << endl;
      }
      //int a = distribuicaoInteira(gerador);
      break;
  }

  switch (problema) {
    case 'R':
      cout << "Numero de colisão total: " << avaliaSolucaoRainhas(populacao, POP, D) << endl; 
    break;

    case 'F':
      avaliaSolucaoFx(populacao, POP, D, L, U);
    break;

  }

  return 0;
}

/*Função que avalia o problema R das N-Rainhas
população = vetor com os cromossomos
POP = tamanho da população
D = tamanho do cromossomo que forma o individuo e número de rainha no tabuleiro

Essa codificação não permite choque em coluna e linha apenas nas diagonais

*/
int avaliaSolucaoRainhas(vector<vector <double>> populacao, int POP, int D){
  int colisao = 0, aux = 0, i, j, k;
  for (i = 0; i < POP; i++){
    for(j = 0; j < D; j++){
      //loop para percorrer para frente
      for(k = j+1; k < D; k++){
        if(abs(populacao[i][j]-populacao[i][k]) == abs(j-k)){ aux++;}
      }
      for(k = j-1; k>0; k--){
        if(abs(populacao[i][j]-populacao[i][k]) == abs(j-k)){ aux++;}
      }   
    }
    cout << "Numero colisao: " << aux << endl;
    colisao+=aux;
    aux = 0;    
  } 
  //retorna o número de colisões
  return colisao;  
}
/*Função algébrica: F(x)=cos(20x) - |x|/2 + x³/4
L = -2
U = 2
Precisão de 4 casas decimais
Espaço de busca = (Xmax - Xmin)/ 10^-4 = 40.001
bits = 16
*/
void avaliaSolucaoFx(vector<vector <double>> populacao, int POP, int D, int L, int U){
  int i, j, k = 0;
  double soma = 0, ajuste = 0, fx = 0;
  for (i = 0; i < POP; i++){
    soma = k = 0;
    //decodificação
    for(j = D; j > 0; j--){
      soma += populacao[i][j] * pow(2, k);
      k++;
    }
    //ajuste de escala
    ajuste = L + ((U - L)/(pow(2, D) - 1))*soma;
    //função objetivo
    fx = funcaoAlgebrica(ajuste);
    cout << "Valor: " << fx << endl;
  }
}



//Função algébrica f(x) == função objetivo
double funcaoAlgebrica(double x){
  return (cos(20*x) - (abs(x)/2) + (pow(x,3)/4));
}