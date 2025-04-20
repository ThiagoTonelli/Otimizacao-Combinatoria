# Otimizacao-Combinatoria
Trabalho da disciplina métodos de otimização do 5 período da faculdade.

# Otimização da Localização de Hubs em Redes de Transporte

Este projeto implementa um algoritmo metaheurístico para resolver o Problema de Localização de Hubs (Hub Location Problem - HLP). O objetivo principal é determinar a localização ideal de um número predefinido de hubs em uma rede, de forma a minimizar o custo total de transporte entre todos os pares de nós da rede.

## Problema Abordado

O Problema de Localização de Hubs é um problema clássico de otimização combinatória com diversas aplicações em logística, transporte e telecomunicações. Dada uma rede de nós e os custos de transporte entre eles, o desafio é selecionar um subconjunto de nós para atuarem como hubs. O tráfego entre quaisquer dois nós da rede pode passar diretamente ou ser roteado através de um ou dois hubs. O objetivo é minimizar o custo total de transporte, considerando os custos de conexão dos nós aos hubs e o custo de transporte entre os próprios hubs (que geralmente possui um desconto).

Este código aborda uma variante do HLP que permite o fluxo de tráfego entre dois nós passar por no máximo dois hubs. A escolha de quais nós se tornam hubs e como o tráfego é roteado impacta diretamente o custo total do sistema.

## Utilização do Código

Para utilizar este código, siga os passos abaixo:

1.  **Pré-requisitos:**
    * Um compilador C++ compatível (como g++).
    * O arquivo de cabeçalho `estruturas.h` (que deve definir as estruturas de dados utilizadas, como `solucao`, `matriz_custo`, `vet_dist`, etc.).

2.  **Compilação:**
    Compile o código utilizando um compilador C++. Por exemplo, utilizando o g++:

    ```bash
    g++ seu_arquivo_codigo.cpp estruturas.h -o executavel -lm
    ```

    `-lm` é utilizado para linkar a biblioteca matemática (necessária para a função `sqrt`).

3.  **Execução:**
    Execute o arquivo compilado:

    ```bash
    ./executavel
    ```

4.  **Entrada de Dados:**
    O programa solicitará as seguintes informações:
    * **Número de hubs:** Digite a quantidade de hubs que você deseja localizar na rede.
    * **Nome do arquivo de entrada:** Digite o nome do arquivo de texto que contém as informações da instância do problema. O arquivo de entrada deve seguir o seguinte formato:
        * A primeira linha contém um inteiro representando o número total de nós na rede.
        * As linhas subsequentes (uma para cada nó) contêm dois números de ponto flutuante representando as coordenadas (x, y) da localização de cada nó. Estas coordenadas são utilizadas para calcular as distâncias euclidianas entre os nós, que representam os custos de transporte direto.

5.  **Saída de Resultados:**
    Após a execução, o programa realizará três execuções do algoritmo metaheurístico Variable Neighborhood Search (VNS) e exibirá o tempo de execução de cada uma. Serão gerados dois arquivos de saída:
    * Um arquivo chamado `hubs_NOME_DO_ARQUIVO_DE_ENTRADA`, onde `NOME_DO_ARQUIVO_DE_ENTRADA` é o nome do arquivo que você forneceu. Este arquivo conterá a melhor solução encontrada ao longo das três execuções, incluindo:
        * O número total de nós (`n`) e o número de hubs (`p`).
        * O valor da função objetivo (maior distância entre dois pontos) da melhor solução (`FO`).
        * A lista dos nós selecionados como hubs.
        * Para cada par de nós da rede (origem e destino), os hubs pelos quais o fluxo é roteado (podendo ser um ou dois hubs) e o custo total de transporte entre eles.
    * Um arquivo chamado `INFO_hubs_NOME_DO_ARQUIVO_DE_ENTRADA` que conterá informações sobre o processo de busca, incluindo:
        * O valor da função objetivo da solução inicial.
        * O tempo médio de execução das três rodadas.
        * Um histórico das melhorias encontradas durante a execução do VNS (a função objetivo e o tempo em que a melhoria foi obtida).

## Metodologia Implementada

O código implementa uma metaheurística Variable Neighborhood Search (VNS) para encontrar boas soluções para o Problema de Localização de Hubs. O VNS é uma técnica de busca local que explora um conjunto de estruturas de vizinhança de forma sistemática para escapar de ótimos locais e explorar diferentes regiões do espaço de soluções.

O algoritmo segue, de forma geral, os seguintes passos:

1.  **Construção da Solução Inicial:** Uma solução inicial é construída utilizando uma heurística construtiva. Neste caso, a heurística seleciona como hubs os nós com as menores e maiores somas de distâncias aos outros nós, além do nó central (após ordenar os nós pela soma de suas distâncias).

2.  **Cálculo da Função Objetivo:** Para uma dada configuração de hubs, a função `calculoFO` calcula o custo total de transporte entre todos os pares de nós. Ela considera o roteamento direto (se um dos nós for um hub) ou o roteamento através de um ou dois hubs, aplicando um fator de desconto ($\alpha = 0.25$) no transporte entre os hubs e 0 no custo de conexão de um nó a si mesmo se ele for um hub. Para cada par de nós, o caminho de menor custo (direto ou via um ou dois hubs) é determinado.

3.  **Variable Neighborhood Search (VNS):** O algoritmo VNS é aplicado para tentar melhorar a solução inicial. Ele itera através de diferentes estruturas de vizinhança (implementadas na função `geraVizinho`) e aplica uma busca local (neste caso, uma estratégia de "melhor melhora" implementada na função `melhorMelhora`) em cada vizinho gerado. O algoritimo irá rodar por 5 minutos no máx pois foi a restrição do trabalho, porém é possível alterar, para isso apenas mude o parâmetro indicando quantos minutos deseja rodar.
    * **Geração de Vizinhos (`geraVizinho`):** Esta função gera soluções vizinhas à solução atual, realizando perturbações na configuração dos hubs. Três tipos de vizinhança parecem ser implementados, com diferentes intensidades de mudança nos hubs selecionados.
    * **Busca Local (`melhorMelhora`):** A função `melhorMelhora` realiza uma busca local na vizinhança da solução atual. Ela explora a troca de um hub atual por um nó que não é um hub, buscando a melhoria mais significativa na função objetivo. Este processo é repetido até que nenhuma melhora seja encontrada na vizinhança.
    * **Critério de Aceitação:** Se um vizinho com uma função objetivo melhor é encontrado após a busca local, ele se torna a nova solução atual, e a busca retorna à primeira estrutura de vizinhança. Caso contrário, a próxima estrutura de vizinhança é explorada. Este processo continua por um determinado número máximo de vizinhanças ou até que um critério de parada (neste caso, um tempo máximo de execução) seja atingido.

4.  **Múltiplas Execuções:** O algoritmo VNS é executado três vezes para tentar encontrar soluções ainda melhores, já que metaheurísticas podem ser sensíveis à solução inicial e ao processo de busca estocástico. A melhor solução encontrada entre as três execuções é reportada no arquivo de saída.

## Estruturas de Dados (`estruturas.h`)

Para que este código funcione corretamente, o arquivo de cabeçalho `estruturas.h` deve definir as seguintes estruturas e variáveis globais:

* `numeroNos`: Um inteiro representando o número total de nós na rede.
* `numeroHubs`: Um inteiro representando o número de hubs a serem localizados.
* `matriz_cood[MAXN][2]`: Uma matriz de doubles para armazenar as coordenadas (x, y) de cada nó. `MAXN` deve ser uma constante definida com o tamanho máximo esperado para o número de nós.
* `matriz_custo[MAXN][MAXN]`: Uma matriz de doubles para armazenar os custos de transporte direto entre cada par de nós.
* `vet_dist[MAXN]`: Um array de doubles para armazenar a soma das distâncias de cada nó para todos os outros nós.
* `vet_posicoes[MAXN]`: Um array de inteiros para armazenar os índices dos nós, utilizado para ordenação.
* `solucao`: Uma estrutura que deve conter:
    * `hubs[MAXP]`: Um array de inteiros para armazenar os índices dos nós selecionados como hubs. `MAXP` deve ser uma constante definida com o tamanho máximo esperado para o número de hubs.
    * `fo`: Um double para armazenar o valor da função objetivo da solução.
    * `matrizSol[MAXN*MAXN][5]`: Uma matriz de doubles para armazenar informações sobre o roteamento entre cada par de nós (origem, hub 1, hub 2, destino, custo).
* `hubSet`: Uma estrutura de dados (provavelmente `std::unordered_set<int>`) para armazenar os índices dos hubs para facilitar a verificação.
* `vet_naohubs[MAXN]`: Um array de inteiros para armazenar os índices dos nós que não são hubs.
* `sol[1000]`: Um array de estruturas `solucao` para armazenar múltiplas soluções (neste caso, pelo menos para as soluções inicial e vizinha).
* `solAtual`: Um inteiro para controlar o índice da solução atual.
* `nome_saida[30]`: Um array de caracteres para armazenar o nome do arquivo de saída da solução.
* `nome_saida_info[40]`: Um array de caracteres para armazenar o nome do arquivo de saída de informações.
* `fo_inicial[5]`: Um array de doubles para armazenar o valor da função objetivo da solução inicial de cada execução.
* `saidas_tempo`: Um vetor de doubles para armazenar os tempos em que melhorias foram encontradas.
* `saidas_fo`: Um vetor de doubles para armazenar os valores da função objetivo das melhorias encontradas.
* `saidas_exec`: Um vetor de inteiros para armazenar o número da execução em que as melhorias foram encontradas.
* `N` e `P`: Variáveis inteiras para armazenar o número de nós e hubs, respectivamente.

Certifique-se de que este arquivo de cabeçalho esteja presente no mesmo diretório do seu código fonte. 
