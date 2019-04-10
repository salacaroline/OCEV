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


Estrutura do arquivo de entrada:
Codigo(Binárioa, Real, Inteira ou Permutada) População Cromossomo Problema tipoSeleção QtdsVariaveis
LimiteInfeiror LimiteSuperior

Problemas:
0 - N-rainhas
1 - Função algébrica
2 - Fábrica de rádios

Tipo de Seleção:
0 - Roleta
1 - Distribuição uniforme
2 - 
3 - 

*/


#include <bits/stdc++.h>

using namespace std;

vector<double> avaliaSolucaoRainhas(vector<vector <double>> &populacao, int POP, int D);
vector<double> avaliaSolucaoFx(vector<vector <double>> &populacao, int POP, int D, int L, int U, string maxmin);
double funcaoAlgebrica(double x);
vector<double> avaliaSolucaoFabricaRadios(vector<vector <double>> &populacao, int POP, int D, int QtsVariaveis, vector<double> &L, vector<double> &U);
vector<int> converterBinario2Decimal(vector<double> &binarios, int QtsVariaveis);
vector<double> calculaFitnessRelativo(vector<double> &fitness, int indiceRetirar);
void roleta(vector<double> &fitness, vector<double> &fitnessRelativo, double checkpoint, 
  vector<vector<double> > &populacao, vector<vector <double>> &populacaoIntermediaria, int POP );

int main(){
  /*Inicio das declarações*/
  char COD;
  int POP, D, aux, problema, QtsVariaveis,  i, selecao;
  double max, min;
  int Laux, Uaux;
  //Vetor pois pode ter mais de uma variável e double pra servir para inteiro e real
  vector<double> L, U;
  //Vetor do inteiro permutado
  int permuta[D];
  //Zerei vetor
  for(int i = 0; i < D; i++){
    permuta[i] = 0;
  }
  scanf("%c %d %d %d %d %d", &COD, &POP, &D, &problema, &selecao, &QtsVariaveis);
  //cin >> COD >> POP >> D >> problema >> QtsVariaveis;
  getchar();
  for (int i = 0; i < QtsVariaveis; i++)
  {
    scanf("%d %d", &Laux, &Uaux);
    getchar();
    //cin >> Laux >> Uaux;
    L.push_back((double)Laux);
    U.push_back((double)Uaux);
  }
  unsigned semente = chrono::system_clock::now().time_since_epoch().count();
  uniform_real_distribution<double> distribuicaoReal(L[0], U[0]);
  uniform_int_distribution<int> distribuicaoBinaria(0, 1);
  uniform_int_distribution<int> distribuicaoInteira((int)L[0], (int)U[0]);
  mt19937_64 gerador(semente);

  //Espaço de busca em double para ser utilizado também por inteiros e binários
  vector<vector <double>> populacao (POP, vector<double>());
  vector<vector <double>> populacaoIntermediaria (POP, vector<double>());
  //inicializa populacaoIntermediaria
  for (int i=0; i < POP; i++) {
    for(int j=0; j< D; j++){
      populacaoIntermediaria[i].push_back(0);
    }
  }
  //vetor de fitness
  vector<double> fitness (POP, 0.0);
  vector<double> fitness2;
  for (int i=0; i < POP; i++) {
    fitness2.push_back(0.0);
  }
  
  /*Inicio das verificações para a codificação*/
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
      //assumindo uma variável somente L[0] e U[0]
      if(!(L[0]==0 && U[0]==D-1)){
        cout << "Erro: Corrija os parametros L e U para L=0 e U=D-1" << endl;
        break;
      }
      //gera a população inicial
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
      //printar
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
    //gera a população inicial
      for (int i=0; i < POP; i++) {
        for(int j=0; j< D; j++){
          populacao[i].push_back(distribuicaoReal(gerador));
        }
      }
      //printar
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

  /*Inicio da escolha do problema a ser resolvido*/

  switch (problema) {
    //se 0 -> problema das N-rainhas
    case 0:
    fitness = avaliaSolucaoRainhas(populacao, POP, D);
    //Pegar o melhor e pior indivíduo
    /*for (i = 0; i < fitness.size(); i++) {
      if(i == 0){
        max = fitness[i];
        min = fitness[i];
        //cout <<"Max e Min: " << max << min << endl;
      }else {
        if(fitness[i] > max ){
          max = fitness[i];
        }
        if(fitness[i] < min){
          min = fitness[i];
        }
      }
    }*/
    cout << "\nMax: " << *max_element(fitness.begin(), fitness.end()) << endl;
    cout << "Min: " << *min_element(fitness.begin(), fitness.end()) << endl;
    //printar
    /*cout << "\nfitness: " << endl;
    for(int j=0; j< fitness.size(); j++){
        //  printf("%02d   ", (int)populacao[i][j] );
          cout << fitness[j]<< "    ";
    }*/
    //cout << "\nO melhor indivíduo alcançou: " << max << endl;
    //cout << "O pior indivíduo alcançou: " << min << endl;
    break;

    //Se 1 -> problema da função algébrica
    case 1:
      //assumindo uma variável somente
      fitness = avaliaSolucaoFx(populacao, POP, D, L[0], U[0], "max");
      cout << "\nMax: " << *max_element(fitness.begin(), fitness.end()) << endl;
      cout << "Min: " << *min_element(fitness.begin(), fitness.end()) << endl;
    break;
    //Se 2 -> problema  da fábrica de rádios
    case 2:
      fitness = avaliaSolucaoFabricaRadios(populacao, POP, D, QtsVariaveis, L, U);
      cout << "\nMax: " << *max_element(fitness.begin(), fitness.end()) << endl;
      cout << "Min: " << *min_element(fitness.begin(), fitness.end()) << endl;
    break;

  }

  /*Inicio da escolha da rotina de seleção*/

  switch(selecao){
    case 0:{
      uniform_real_distribution<double> distribuicaoReal2(0.0, 1.0);
      double checkpoint = distribuicaoReal2(gerador);
      double soma = 0;
      int indice = 0;
      roleta(fitness, fitness2, checkpoint, populacao, populacaoIntermediaria, POP);
      
      for(int i = 0; i < POP; i++){
        for(int j=0; j< D; j++){
        //  printf("%02d   ", (int)populacao[i][j] );
          cout << populacaoIntermediaria[i][j] << "    ";
        }
        cout << endl;
      }
      
    }
    break;
    case 1:{
      fitness2 = fitness;
      double max, min;
      int POP2 = POP, aux = fitness2.size();
      while(POP2 > 0) {
      //min = *min_element(fitness2.begin(), fitness2.end());
        max = *max_element(fitness2.begin(), fitness2.end());
        for(int i = 0; i < POP; i++){
          if(fitness[i] == max){
            fitness[i] = POP2;
            POP2--;
            fitness2[i] = -1;
//            fitness2.erase(fitness2.begin()+i-apagado);
            break;
          }
        }
       
      }
      /*printando o fitness
      cout << "Fitnes: " << endl;
      for(int j = 0; j < POP; j++){
         cout << fitness[j] << endl;
      }*/
      //checkpoint
      uniform_real_distribution<double> distribuicaoReal2(0.0, 1.0);
      double checkpoint = distribuicaoReal2(gerador);
      //chama a roleta
      roleta(fitness, fitness2, checkpoint, populacao, populacaoIntermediaria, POP);
      for(int i = 0; i < POP; i++){
        for(int j=0; j< D; j++){
        //  printf("%02d   ", (int)populacao[i][j] );
          cout << populacaoIntermediaria[i][j] << "    ";
        }
        cout << endl;
      }
    }
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
vector<double> avaliaSolucaoRainhas(vector<vector <double>> &populacao, int POP, int D){
  int colisao = 0, aux = 0, i, j, k;
  vector<int> colisoes;
  vector<double> fitness;
  for (i = 0; i < POP; i++){
    for(j = 0; j < D; j++){
      //loop para percorrer para frente
      for(k = j+1; k < D; k++){
        if(abs(populacao[i][j]-populacao[i][k]) == abs(j-k)){ aux++;}
      }
      //loop para percorrer para trás
      for(k = j-1; k>0; k--){
        if(abs(populacao[i][j]-populacao[i][k]) == abs(j-k)){ aux++;}
      }
    }
    //cria vetor com o número de colisões de cada indivíduo
    colisoes.push_back(aux);
    //cout << "Numero colisao: " << aux << endl;
    //colisao+=aux;
    aux = 0;
  }
  //aplica o fitness para descobrir melhor e pior indivíduo
  for (i = 0; i < colisoes.size(); i++) {
    //O intuito é min o número de colisões por isso o fitness deve ser positivo e
    //o número de colisões subtraido do máx de colisões possíveis -> Max de colisões na diagonal D*(D-1), ex 8x8 é 8*7
    fitness.push_back(D*(D-1) - colisoes[i]);
  }
  //retorna o vetor de fitness
  return fitness;
}
/*Função algébrica: F(x)=cos(20x) - |x|/2 + x³/4
L = -2
U = 2
Precisão de 4 casas decimais
Espaço de busca = (Xmax - Xmin)/ 10^-4 = 40.001
bits = 16
*/
vector<double> avaliaSolucaoFx(vector<vector <double>> &populacao, int POP, int D, int L, int U, string maxmin){
  int i, j, k = 0;
  double soma = 0, ajuste = 0, fx = 0;
  vector<double> fitness;
  for (i = 0; i < POP; i++){
    soma = k = 0;
    //decodificação -> Alterar para a função
    for(j = D; j > 0; j--){
      //soma será o valor binário convertido em decimal
      soma += populacao[i][j] * pow(2, k);
      k++;
    }
    //ajuste de escala (fórmula)
    ajuste = L + ((U - L)/(pow(2, D) - 1))*soma;
    //função fitness = função objetivo + limites
    fx = funcaoAlgebrica(ajuste);
    //cout << "Valor: " << fx << endl;
    if(maxmin == "max"){
      fitness.push_back(4 + fx);
    }else if(maxmin == "min"){
      fitness.push_back(2 - fx);
    }
    //cout << "Valor para max: " << 4 + fx << endl;
    //cout << "Valor para min: " << 2 - fx << endl;
  }
  return fitness;
}

//Função algébrica f(x) == função objetivo
double funcaoAlgebrica(double x){
  return (cos(20*x) - (abs(x)/2) + (pow(x,3)/4));
}

vector<double> avaliaSolucaoFabricaRadios(vector<vector <double>> &populacao, int POP, int D, int QtsVariaveis, vector<double> &L, vector<double> &U){
  int i, j, ajuste;
  double fo, fit;
  vector<int> binario2decimal, decimalMapeado;
  vector<double> fitness;
  for (i = 0; i < POP; i++){
    //Decodifica os binários para decimal
    binario2decimal = converterBinario2Decimal(populacao[i], QtsVariaveis);
    //Ajuste de escala dos intervalos em mais de uma variável concatenada
    for (j = 0; j < binario2decimal.size(); j++){
      ajuste = L[j] + ((U[j] - L[j])/(pow(2, D/QtsVariaveis) - 1))*binario2decimal[j];
      decimalMapeado.push_back(ajuste);
      //cout << "Decimal: "<<binario2decimal[j] << " Ajuste: " << ajuste << endl;
    }
    //Fução objetivo para a fábrica de rádios
    fo = (double) (30*decimalMapeado[0]+40*decimalMapeado[1])/1360.0;
    //cout << "Função: " << fo << endl;
    //Aplicando a função fitness
    fit = fo - max(0.0, (decimalMapeado[0]+2*decimalMapeado[1]-40)/16.0);
    //cout << "Fitness: "<< fit << endl;
    fitness.push_back(fit);
  }
  return fitness;

}

/*Funções úteis*/

//Converte um cromossomo com strings concatenadas em um vetor com o valor em decimal dessas variáveis
vector<int> converterBinario2Decimal(vector<double> &binarios, int QtsVariaveis){
  int j, i, k = 0, soma = 0, numCortes = binarios.size()/QtsVariaveis;
  vector<int> v;
  //ordem inversa de significatividade dos bits. O primeiro é o menos significativo
  for(j = 0 ; j < binarios.size(); j++){
    //soma será o valor binário convertido em decimal
    soma += binarios[j] * pow(2, k);
    k++;
    //Necessidade de fazer essa parte do código para dividir o vetor de binários em duas variáveis
    if(k == numCortes-1){
      //cout << "soma: " <<  soma << endl;
      v.push_back(soma);
      soma = 0;
      k = 0;
    }
  }
  return v;
}

vector<double> calculaFitnessRelativo(vector<double> &fitness, int indiceRetirar){
  double soma = 0;
  vector<double> fitnessRelativo;

  for(int i = 0; i < fitness.size(); i++){
    if(i != indiceRetirar){
      soma += fitness[i];
    }
  }
  for(int i = 0; i < fitness.size(); i++){
    if(i != indiceRetirar){
      fitnessRelativo.push_back((double)fitness[i]/(double)soma);
    }
  }
  return fitnessRelativo;
} 

void roleta(vector<double> &fitness, vector<double> &fitnessRelativo, 
  double checkpoint, vector<vector<double> > &populacao, 
  vector<vector <double>> &populacaoIntermediaria, int POP ){
  double soma = 0.0;
  int indice;
  for(int i = 0; i < POP-1; i+=2){
        //loop para percorrer o vetor fitnessRelativo e pegar o elemento no checkpoint
        soma = 0;
        fitnessRelativo = calculaFitnessRelativo(fitness, -1);
        for(int j = 0; j < POP; j++){
          soma+=fitnessRelativo[j];
          if(soma >= checkpoint){
            populacaoIntermediaria[i] = populacao[j];
            indice = j;
            break; //?
          }
        }
        soma = 0;
        fitnessRelativo = calculaFitnessRelativo(fitness, indice);
        for(int j = 0; j < POP-1; j++){
          soma+=fitnessRelativo[j];
          if(soma >= checkpoint){
            if(j >= indice){
              populacaoIntermediaria[i+1] = populacao[j+1];
            }else {
              populacaoIntermediaria[i+1] = populacao[j];
            } 
            break; //?
          }
        }
      }
}
