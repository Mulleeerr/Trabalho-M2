#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <fstream>

using namespace std;

struct Jogador {
    int x;
    int y;
};

struct Bomba {
    int x;
    int y;
    bool ativa;
    clock_t tempoExplosao = 0;
    clock_t tempoBomba = 0;
    bool explosaoAtiva = false;
    int raio = 1;
    bool atravessaParedes = false; // Adiciona a capacidade de atravessar paredes
    int bombasJogadas = 0; // Adiciona um contador para as bombas jogadas
    bool novaBomba = false; // Adiciona uma variável para rastrear quando uma nova bomba é colocada
};

struct Inimigos {
    int x;
    int y;
    bool vivo;
};

struct Poder {
    int x;
    int y;
    int tipo; // 1 para aumentar a área de explosão, 2 para atravessar paredes
    bool ativo;
    clock_t tempoAtivacao;
};

struct Mapas{
    int linha;
    int coluna;
    bool JogoPausado = false;
    bool JogoEmAndamento = true;
};

    void imprimir(Jogador player, int **m,Mapas mapa) {
    ///Imprime o jogo: mapa e personagem.
    for(int i=0;i<mapa.linha;i++){
        for(int j=0;j<mapa.coluna;j++){
            if(i==player.x&& j==player.y) {
                if(m[player.x][player.y] == 3) {
                    cout<<char(2);
                } else {
                    cout<<char(1); //personagem
                }
            } else {
                switch (m[i][j]){
                    case 0: cout<<" "; break; //caminho
                    case 1: cout<<char(219); break; //parede
                    case 2: cout<<char(177); break; //parede quebravel
                    case 3: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
                    cout<<char(169); break; //bomba
                    case 4: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
                    cout<<char(244); break; // inimigo
                    case 5: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
                    cout<<char(42); break; // explosao bomba
                    case 6: cout <<char(168); break; // poder de aumentar a área de explosão
                    case 7: cout <<char(169); break; // poder de atravessar paredes


                } //fim switch
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            }
        }
    cout<<"\n";
    }
}
///===================================INICIO FUNCOES MOVIMENTO==================================
void movimento(Jogador &player, int **m,Mapas mapa,char &tecla, Bomba &bomba, clock_t &fim, Poder poder) {
        if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            { 
                case 72: case 'w': ///cima
                    if(m[player.x-1][player.y]!=2 && m[player.x-1][player.y]!=1){
                        player.x--;
                    }
                break;
                case 80: case 's': ///baixo
                    if(m[player.x+1][player.y]!=2 && m[player.x+1][player.y]!=1){
                        player.x++;
                    }
                break;
                case 75:case 'a': ///esquerda
                    if(m[player.x][player.y-1]!=2 && m[player.x][player.y-1]!=1){
                        player.y--;
                    }
                break;
                case 77: case 'd': ///direita
                    if(m[player.x][player.y+1]!=2 && m[player.x][player.y+1]!=1){
					    player.y++;
                    }
                break;
                case 'g':
                if (bomba.ativa == false) { // Verifica se a bomba não está ativa
                        bomba.ativa = true;
                        bomba.x = player.x;
                        bomba.y = player.y;
                        bomba.tempoBomba = (fim / CLOCKS_PER_SEC) + 5;
                        m[player.x][player.y] = 3;

                        if (bomba.raio > 1 || bomba.atravessaParedes) {
                            bomba.bombasJogadas++;
                            if (bomba.bombasJogadas >= 3) {
                                bomba.raio = 1;
                                bomba.atravessaParedes = false;
                                bomba.bombasJogadas = 0;
                            }
                        }
                }
                break;
            }
         }
    }

void moverInimigos(Inimigos inimigo[], int **m,Mapas mapa, int numInimigos) {
    for (int i = 0; i < numInimigos; i++) {
        if(inimigo[i].vivo == true) {
            int movimentos = rand() % 3 + 1; // Movimento aleatório de 1 a 3 posições

            for (int j = 0; j < movimentos; j++) {
                int direcao = rand() % 4;
                int novoX = inimigo[i].x;
                int novoY = inimigo[i].y;
                    if(m[inimigo[i].x - 1][inimigo[i].y] == 1 || m[inimigo[i].x + 1][inimigo[i].y] == 1 | 
                            m[inimigo[i].x][inimigo[i].y-1] == 1 || m[inimigo[i].x][inimigo[i].y+1] ==1 ) {
                                 int movimentos = rand() % 3 + 1; // se encontrar uma parede, altera a direcao
                            }
                switch (direcao) {
                    case 0: // CIMA
                        if (m[inimigo[i].x - 1][inimigo[i].y] != 2 && m[inimigo[i].x - 1][inimigo[i].y] != 1) {
                            novoX--;
                        }
                        break;
                    case 1: // BAIXO
                        if (m[inimigo[i].x + 1][inimigo[i].y] != 2 && m[inimigo[i].x + 1][inimigo[i].y] != 1) {
                            novoX++;
                        }
                        break;
                    case 2: // Esquerda
                        if (m[inimigo[i].x][inimigo[i].y - 1] != 2 && m[inimigo[i].x][inimigo[i].y - 1] != 1) {
                            novoY--;
                        }
                        break;
                    case 3: // Direita
                        if (m[inimigo[i].x][inimigo[i].y + 1] != 2 && m[inimigo[i].x][inimigo[i].y + 1] != 1) {
                            novoY++;
                        }
                        break;
                }

                // Limpa a posição anterior do inimigo
                m[inimigo[i].x][inimigo[i].y] = 0;
                // Atualiza a nova posição do inimigo
                inimigo[i].x = novoX;
                inimigo[i].y = novoY;
                m[inimigo[i].x][inimigo[i].y] = 4;
            }
        }
    }
}
///===================================FIM FUNCOES MOVIMENTO=================================

///===================================INICIO FUNCOES BOMBA==================================
void explodirBomba(int **m, Bomba &bomba, int &InimigosRestantes, Inimigos inimigo[], int numInimigos, Mapas mapa) {
    int explosaoX, explosaoY;

    // Explode a bomba na posição atual
    m[bomba.x][bomba.y] = 5;

    // Explode a bomba nas direções vertical e horizontal de acordo com o raio
    for (int i = 1; i <= bomba.raio; i++) {
        // Para cima
        if (bomba.x - i >= 0 && (m[bomba.x - i][bomba.y] == 0 || m[bomba.x - i][bomba.y] == 2 || m[bomba.x - i][bomba.y] != 1)) {
            explosaoX = bomba.x - i;
            explosaoY = bomba.y;
            m[explosaoX][explosaoY] = 5;
        }

        // Para baixo
        if (bomba.x + i < mapa.linha && (m[bomba.x + i][bomba.y] == 0 || m[bomba.x + i][bomba.y] == 2 || m[bomba.x + i][bomba.y] != 1)) {
            explosaoX = bomba.x + i;
            explosaoY = bomba.y;
            m[explosaoX][explosaoY] = 5;
        }

        // Para a esquerda
        if (bomba.y - i >= 0 && (m[bomba.x][bomba.y - i] == 0 || m[bomba.x][bomba.y - i] == 2 || m[bomba.x][bomba.y - i] != 1)) {
            explosaoX = bomba.x;
            explosaoY = bomba.y - i;
            m[explosaoX][explosaoY] = 5;
        }

        // Para a direita
        if (bomba.y + i < mapa.coluna && (m[bomba.x][bomba.y + i] == 0 || m[bomba.x][bomba.y + i] == 2 || m[bomba.x][bomba.y + i] != 1)) {
            explosaoX = bomba.x;
            explosaoY = bomba.y + i;
            m[explosaoX][explosaoY] = 5;
        }
    }

    // Verificar se algum inimigo está dentro da área da bomba na hora em que ela explode
    for (int k = 0; k < numInimigos; k++) {
        if (inimigo[k].vivo == true && ((inimigo[k].x >= bomba.x - bomba.raio && inimigo[k].x <= bomba.x + bomba.raio && inimigo[k].y == bomba.y) ||
                                        (inimigo[k].y >= bomba.y - bomba.raio && inimigo[k].y <= bomba.y + bomba.raio && inimigo[k].x == bomba.x))) {
            InimigosRestantes--; // inimigo morre para a explosão
            inimigo[k].vivo = false; // marcar o inimigo em especifico como morto
            m[inimigo[k].x][inimigo[k].y] = 0; // Remover o inimigo do mapa
        }
    }
}
///===================================FIM FUNÇOES BOMBA==================================

///===================================INICIO FUNCAO LER MAPA==================================
    int** lerMapa(const char* nomeArquivo, Mapas& mapa) {
        ifstream arquivo(nomeArquivo);
        if (!arquivo) {
            cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
            exit(1);
        }

        // Leia a primeira linha do arquivo para obter informações de linhas e colunas.
        arquivo >> mapa.linha >> mapa.coluna;

        // Aloque memória para a matriz m com base nas informações lidas.
        int** m = new int*[mapa.linha];
        for (int i = 0; i < mapa.linha; i++) {
            m[i] = new int[mapa.coluna];
        }

        // Leia o restante do arquivo para preencher a matriz.
        for (int i = 0; i < mapa.linha; i++) {
            for (int j = 0; j < mapa.coluna; j++) {
                arquivo >> m[i][j];
            }
        }
        arquivo.close();
        return m; // Retorne a matriz alocada dinamicamente.
    }
///===================================FIM FUNCAO LER MAPA==================================

    void gerarPoder(Poder& poder, int **m,Mapas mapa, clock_t& fim, Bomba& bomba) { // Adicionado Bomba& bomba como parâmetro
        if (!poder.ativo && fim / CLOCKS_PER_SEC % 20 == 0 && !(bomba.raio > 1 || bomba.atravessaParedes)) { // Adicionada condição para verificar se o jogador tem um poder ativo
            do {
                poder.x = rand() % mapa.linha;
                poder.y = rand() % mapa.coluna;
            } while (m[poder.x][poder.y] != 0); // Garante que o poder não seja gerado em uma parede

            poder.tipo = rand() % 2 + 1; // Gera um tipo de poder aleatório
            poder.ativo = true;
            if(poder.tipo == 1) {
                m[poder.x][poder.y] = 6; // Poder aumento Explosao
            } else {
                m[poder.x][poder.y] = 7; // Poder atravessar paredes
            }
        }
    }

    void pegarPoder(Jogador& jogador, Poder& poder, Bomba& bomba, int **m,Mapas mapa) {
        if (jogador.x == poder.x && jogador.y == poder.y && poder.ativo) { // O jogador pegou o poder
            if (poder.tipo == 1) {
                bomba.raio += 1; // Aumenta a área de explosão da bomba
            } else if (poder.tipo == 2) {
                bomba.atravessaParedes = true; // Permite que a bomba atravesse paredes sólidas
            }

            poder.ativo = false; // Desativa o poder depois de ser pego
            m[poder.x][poder.y] = 0; // Remove o poder do mapa

            if (bomba.bombasJogadas == 3 && (bomba.raio > 1 || bomba.atravessaParedes)) {
                bomba.novaBomba = false; // Redefine a variável novaBomba
                bomba.raio = 1; // Redefine o raio da bomba
                bomba.atravessaParedes = false; // Desativa a capacidade de atravessar paredes
            }
        }
    }

///=================================== FIM FUNCOES PODER==================================
    void salvarJogo(Jogador& jogador, Bomba& bomba, Inimigos inimigos[], int &numInimigos, Mapas &mapa) {
        ofstream arquivo("jogo_salvo.txt");
        if (arquivo.is_open()) {
            // Salvar informações do jogador
            arquivo << jogador.x << " " << jogador.y << endl;

            // Salvar informações da bomba
            arquivo << bomba.x << " " << bomba.y << " " << bomba.ativa << " " << bomba.tempoExplosao << endl;

            // Salvar informações dos inimigos
            for (int i = 0; i < numInimigos; i++) {
                arquivo << inimigos[i].x << " " << inimigos[i].y << " " << inimigos[i].vivo << endl;
            }
            cout << "\nO Jogo foi Salvo com Sucesso\n";
            arquivo.close();
        } else {
            cout << "Não foi possível abrir o arquivo para salvar o jogo." << endl;
        }
    }
    void carregarJogo(Jogador &player, Bomba &bomba, Inimigos inimigo[], int &numInimigos, Mapas &mapa) {
    ifstream arquivo("jogo_salvo.txt");
    if (arquivo.is_open()) {
        // Informacoes do jogador
        arquivo >> player.x >> player.y;
        
        // Pnformacoes da bomba
        arquivo >> bomba.x >> bomba.y >> bomba.ativa >> bomba.tempoExplosao;
        
        // Informacoes dos inimigos
        for (int i = 0; i < numInimigos; i++) {
            arquivo >> inimigo[i].x >> inimigo[i].y >> inimigo[i].vivo;
        }
        
        arquivo.close();
        
    } else {
        cout << "Não foi possível abrir o arquivo de jogo salvo." << endl;
    }
}

    void menuJogo(Mapas &mapa,Jogador &player, Bomba &bomba, Inimigos inimigo[],int numInimigos,clock_t &fim,char &tecla) {
        int escolha1;

        cout << "Jogo Pausado!" << endl;
        cout << "1 - Continuar" << endl;
        cout << "2 - Salvar Jogo" << endl;
        cout << "3 - Sair do Jogo" << endl;
            cin >> escolha1;

        switch (escolha1) {
            case 1:
                cout << "Continuando Partida.\n";
                tecla = 'y';
                system("pause");
                mapa.JogoPausado = false;
                system("cls");
                break;
            case 2:
                salvarJogo(player,bomba,inimigo,numInimigos,mapa);
                system("pause");
                break;
            case 3: 
                cout << "\nDesenvolvido Por:\n";
                cout << "Felipe Muller , Gabriel Pavan e Luigi Pofo\n";
                cout << "Sobre o Codigo: Implementacao de novas funcoes para o Bomberman criado na M1.\n";
                cout << "Pressione qualquer tecla para sair.";
                system("pause");
                system("cls");  
                exit(1);
            break;
        }
    }
    void liberarMemoria(int** m, int linhas) {
        for (int i = 0; i < linhas; i++) {
            delete[] m[i];
        }
        delete[] m;
        //cout << "Memoria Alocada Liberada.\n";
    }
    void reiniciarJogo(Jogador &player, Bomba &bomba, Inimigos inimigo[], int numInimigos, Mapas &mapa,clock_t &inicio) {
    // Reinicialize o jogador
    player.x = 1;
    player.y = 1;
    // Reinicialize a bomba
    bomba.ativa = false;
    bomba.explosaoAtiva = false;
    bomba.tempoBomba = 0;
    bomba.tempoExplosao = 0;
    bomba.raio = 1;
    bomba.atravessaParedes = false;
    bomba.bombasJogadas = 0;
    bomba.novaBomba = false;
    inicio = clock();
    mapa.JogoEmAndamento = true;
    mapa.JogoPausado = false;
}

    void jogo(int **m, int &numInimigos, Inimigos inimigo[],char &tecla,Jogador &player,
        Bomba &bomba,Poder &poder,clock_t &inicio, clock_t &fim, int InimigosRestantes,clock_t &tempo_antes_pausa,clock_t &tempo_corrido){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
    short int CX=0, CY=0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;

    srand(time(NULL));
    inicio = clock();
    ///===================FIM VARIAVEIS TEMPO/INIMIGO=================

    ///===================INICIO CHAMADA ESCOLHER MAPA================
    cout << "Digite o Mapa que quer jogar (mapa1.txt ou mapa2.txt)" << endl;
        string s;
            cin >> s;
            const char* nomeArquivo = s.c_str(); // Converte de string para char*
        Mapas mapa;

        mapa.JogoPausado = false;
        m = lerMapa(nomeArquivo, mapa);
    //POSICIONA OS INIMIGOS NO MAPA
    for (int i = 0; i < numInimigos; i++){
        do {
            inimigo[i].vivo = true;
            inimigo[i].x = rand()%mapa.linha;
            inimigo[i].y = rand()%mapa.coluna;
        } while (m[inimigo[i].x][inimigo[i].y] != 0);
            if(m[inimigo[i].x][inimigo[i].y] != 2 && m[inimigo[i].x][inimigo[i].y] != 1) {
            m[inimigo[i].x][inimigo[i].y] = 4;
            }
    }


        system("cls");
    ///===================FIM CHAMADA ESCOLHER MAPA====================
    while(mapa.JogoEmAndamento == true){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); //COLOCAR CURSOR NO INICIO DA TELA
         if(tecla == 'p') {
             mapa.JogoPausado = true;
         }
        if(mapa.JogoPausado == true) {
            system("cls");
            menuJogo(mapa,player,bomba,inimigo,numInimigos,fim,tecla);
        } if(mapa.JogoPausado == false) {

        imprimir(player,m,mapa); //IMPRIMIR O PLAYER,MAPA,ETC

        gerarPoder(poder,m,mapa,fim,bomba); // Chama a Funçao que vai gerar o poder na tela        

        pegarPoder(player,poder,bomba,m,mapa);     
    ///===================INICIO CONTAGEM TEMPO/MOVIMENTO DO INIMIGO A CADA X TEMPO====================
    fim = clock();
    if((fim - inicio) / CLOCKS_PER_SEC == 1) {

    moverInimigos(inimigo,m,mapa,numInimigos); //FUNÇAO DE MOVIMENTAR OS INIMIGOS
    inicio = clock();
    ///===================FIM CONTAGEM TEMPO/MOVIMENTO DO INIMIGO A CADA X TEMPO=======================
    }
    movimento(player,m,mapa,tecla,bomba,fim,poder);  // Chama a Funçao que executa os movimentos do player

    ///===================INICIO TRECHO EXPLOSAO DA BOMBA, RETIRANDO A EXPLOSAO APOS 1 SEGUNDO====================

        if (bomba.ativa) {
            if (fim / CLOCKS_PER_SEC >= bomba.tempoBomba) {
                bomba.ativa = false;
                m[bomba.x][bomba.y] = 0;
                explodirBomba(m,bomba,InimigosRestantes,inimigo,numInimigos,mapa); // Faz a animacao de explosao
                bomba.tempoExplosao = (fim / CLOCKS_PER_SEC) + 1; 

                if (bomba.bombasJogadas >= 4 && (bomba.raio > 1 || bomba.atravessaParedes)) {
                    bomba.raio = 1;
                    bomba.atravessaParedes = false;
                    bomba.bombasJogadas = 0;
                }
            }
        }
            if (fim / CLOCKS_PER_SEC >= bomba.tempoExplosao) { // Retira a particula da bomba do mapa
                for(int i = 0 ; i < mapa.linha ; i++) {
                    for(int j = 0 ; j < mapa.coluna ; j++) {
                        if(m[i][j] == 5) {
                        m[i][j] = 0;
                }
            }
        }
    }
    ///===================FIM TRECHO EXPLOSAO DA BOMBA, RETIRANDO A EXPLOSAO APOS 1 SEGUNDO====================
cout << bomba.raio << endl;
cout <<"Tempo de Partida: " << (fim / CLOCKS_PER_SEC) << " segundos.\n";

    ///===================INICIO COLISOES E CONDICOES VITORIA / DERROTA====================

    //Colisao do jogador com a explosao da bomba e com os inimigoss
    if(m[player.x][player.y] == 5) { //morre para bomba
            system("cls");
        cout << "Jogo Encerrado, voce perdeu morrendo para a Bomba.\n";
            cout << "\nDuracao da Partida: " << (fim / CLOCKS_PER_SEC) << " segundos.\n";
            inicio = clock();
            liberarMemoria(m,mapa.linha);
            mapa.JogoEmAndamento = false;
            system("pause");
            break;
    } else if(m[player.x][player.y] == 4) { //morre para inimigo
        system("cls");
        cout << "Jogo Encerrado, voce perdeu, foi eliminado por um inimigo";
            cout << "\nDuracao da Partida: " << (fim / CLOCKS_PER_SEC) << " segundos.\n";
            inicio = clock();
            liberarMemoria(m,mapa.linha);
            mapa.JogoEmAndamento = false;
            system("pause");
        break;
    }
    if(InimigosRestantes == 0) { // ganhou pois matou os inimigos
        system("cls");
        cout << "Voce venceu, conseguiu derrotar todos os inimigos !\n";
            inicio = clock();
            liberarMemoria(m,mapa.linha);
            mapa.JogoEmAndamento = false;
            system("pause");
        break;
            }
        }
    }
}
    ///======================FIM COLISOES E CONDICOES VITORIA / DERROTA====================


int main() {
    ///===================VARIAVEIS JOGO==============
    Jogador player;
        player.x=1, player.y=1;
        char tecla;
    Bomba bomba;
        bomba.explosaoAtiva = false;
        Poder poder;

        clock_t inicio, fim, tempo_antes_pausa = 0 , tempo_corrido = 0;
        inicio = clock();
        Inimigos inimigo[3];
        int numInimigos = 3 , opcao = 0, InimigosRestantes = numInimigos;
        Mapas mapa;
        int **m;
        ///===================VARIAVEIS JOGO==============

        
    while (opcao != 5) {
        system("cls");
        cout << "Ola, qual opcao deseja?" << endl;
        cout << "1 - Jogar Novo Jogo" << endl;
        cout << "2 - Carregar Jogo Salvo" << endl;
        cout << "3 - Salvar Jogo" << endl;
        cout << "4 - Sair do Jogo" << endl;
            cin >> opcao;
        switch (opcao) {
            case 1:
                reiniciarJogo(player,bomba,inimigo,numInimigos,mapa,inicio); // setar as variaveis para o default
                system("cls");
                jogo(m,numInimigos,inimigo,tecla,player,bomba,poder,inicio,fim,InimigosRestantes,tempo_antes_pausa,tempo_corrido);
                break;
            case 2:
                carregarJogo(player, bomba,inimigo,numInimigos,mapa);
                cout << "Jogo carregado com sucesso!" << endl;
                system("pause");
                jogo(m,numInimigos,inimigo,tecla,player,bomba,poder,inicio,fim,InimigosRestantes,tempo_antes_pausa,tempo_antes_pausa);
                break;
            case 3:
                salvarJogo(player, bomba, inimigo, numInimigos,mapa);
                break;
            case 4:
                cout << "\nDesenvolvido Por:\n";
                cout << "Felipe Muller , Gabriel Pavan e Luigi Pofo\n";
                cout << "Sobre o Codigo: Implementacao de novas funcoes para o Bomberman criado na M1.\n";
                cout << "Pressione qualquer tecla para sair.";
                system("pause");
                system("cls");
                break;
            default:
                cout << "Opcao invalida. Por favor, escolha uma opcao valida." << endl;
                break;
        }
    }
}
