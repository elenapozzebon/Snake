#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#include "dvector.c"
#include "dvector.h"

/*colori caratteri*/
#define ANSI_RESET_ALL          "\x1b[0m"
#define ANSI_COLOR_BLACK        "\x1b[30m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_COLOR_WHITE        "\x1b[37m"

struct list{
	char data;
	struct list *next;
};

struct allmoves{
	struct list *head;
	struct list *tail;
};

char *board;

//------------------------Modalita AI-----------------------------------
int AI_moves(int rows, int cols, int *current_position, int final_position, char move, struct allmoves *m);
void print_lab(int rows, int cols);

void push_back_list(struct allmoves* m, char move){
	if(m->head == NULL){
		m->head = (struct list*)malloc(sizeof(struct list));
		m->head->data = move;
		m->head->next = NULL;
		m->tail = m->head;
	}else{
		m->tail->next = (struct list*)malloc(sizeof(struct list));
		m->tail = m->tail->next;
		m->tail->data = move;
		m->tail->next = NULL;
	}
}

void print_list(struct allmoves m){
	struct list *ph = m.head;
	while(ph){
		printf("%c", ph->data);
		ph=ph->next;
	}
}

void AI_modality(int rows, int cols, int final_position, int *current_position){
	//creazione lista
	struct allmoves m;
	m.head=NULL;
	m.tail=NULL;
	char nord='n', est='o', sud='s', ovest='e', temp;
	while(*current_position != final_position){
		if(AI_moves(rows,cols,current_position,final_position,nord,&m)){
			temp = nord;
			nord = ovest;
			ovest = sud;
			sud = est;
			est = temp;
		}else{
			temp = nord;
			nord = est;
			est = sud;
			sud = ovest;
			ovest = temp;
		}
		system("cls");
		print_lab(rows,cols);
		
	}
	print_list(m);
	struct list* ph = m.head;
	while(m.head){
		m.head=m.head->next;
		free(ph);
		ph=m.head;
	}
}

int AI_moves(int rows, int cols, int *current_position, int final_position, char move, struct allmoves *m){
	int possible = 1;
	int position = 0;
	if(move=='n'){
		position = (*current_position)-cols;
		if(*current_position>=0 && *current_position<cols){
			possible = 0;
		}
	}
	if(move=='s'){
		position = *current_position+cols;
		if(*current_position>=(rows*cols-cols) && *current_position<(rows*cols)){
			possible = 0;
		}
	}
	if(move=='e'){
		position = *current_position+1;
		if((*current_position+1)%cols == 0){
			possible = 0;
		}
	}
	if(move=='o'){
		position = *current_position-1;
		if((*current_position)%cols == 0){
			possible = 0;
		}
	}
	if(possible == 1){
		if(board[position]=='#'){
			//dove voglio andare c'e # non posso andare
			possible = 0;
		}else{
			board[*current_position]='.';
			board[position]='o';
			*current_position = position;
			//v_push_back(allmoves, move);
			push_back_list(m,move);
		}
	}
	return possible;
}

//----------------------------------------------------------------------

//crea la matrice statica 10x19
void create_static_lab(int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (i==0 || i == 9 || (j==0 && i != 1) || (j==18 && i != 5) || (j==5 && i!= 8 && i!= 7) || (j==12 && i!=1 && i!=2)){
				board[i * 19 + j] = 35;//stampa #
			}else if ((i==8 && 5<=j && j<=8) || (j == 16 && 1 <= i && i <= 4)) {
				board[i * 19 + j] = 36;//stampa $
			}else if (i==3 && j==9) {
				board[i * 19 + j] = 33;//stampa !
			}else if(i==1 && j==0){
                board[i * 19 + j] = 111;//stampa o
            }else if(i==5 && j==18){
                board [i * 19 + j] = 95;//stampa _
            }else if(i==3 && j==8){
				board [i * 19 + j] = 84;//stampa T
			}else{
				board[i * 19 + j] = 32;//stampa ' '
			}
		}
	}
}
//crea la matrice dinamica ROWSxCOLS
void create_dynamic_lab(int rows, int cols){
	char element;

	for (int i = 0; i<rows  ; i++) {
		for (int j = 0; j<cols; j++) {
			scanf(" %c", &element);
			if(element=='x'){
				board[i*cols+j]=32;//stampa lo spazio
			}else{
				board[i*cols+j]=element;
			}
		}
	}
}
//funzione per stampare il labirinto
void print_lab(int rows, int cols) {
	//system(CLEAR);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%c", board[i*cols+j]);
		}
		printf("\n");
	}
}
//funzione che trova la testa dello snake
int find_start(int rows, int cols){
    int current_position;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			if(board[i*cols+j]=='o'){
				current_position=i*cols+j;
			}
			/*if(board[i*COLS+j]=='_'){
				End_position=i*COLS+j;
			}*/
		}
	}
    return current_position;
}
//funzione che trova la fine del labirinto
int find_finish(int rows, int cols){
    int final_position;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			if(board[i*cols+j]=='_'){
				final_position=i*cols+j;
			}
		}
	}
    return final_position;
}

//elimina lo snake dal labirinto
void clean(vector_t *v){
	for(int i=0; i<v->size; i++){
		board[v->data[i]] = ' ';
	}
}

//inserisce lo snake nel labirinto
void printsnake(vector_t *v){
	board[v->data[0]] = 'o';
	for(int i=1; i<v->size; i++){
		board[v->data[i]] = '.';
	}
}

//cambio posizioni Snake
void change(vector_t *v, int p){
	if(v->size > 1){
		v_push_front(v,p);
		v_pop_back(v);
	}else{
		v->data[0] = p;
	}
}

//se va sopra s� stesso
void find_himself(vector_t *v, int p, int *coins){
	int pos = 0;
	for(int i=1; i<v->size; i++){
		if(v->data[i] == p){
			pos = i;
		}
	}
	
	if(pos != 0){
		int size = v->size;
		for(int i=pos; i<size; i++){
			v_pop_back(v);
		}
	}
	*coins = v->size - 1;
}

//gestione $, !, T
void coins_unexp_drill(int p, int *coins, int *current_position, int *T, vector_t *v){
	clean(v);
	if(board[p]=='$'){
		*coins += 1;
		v_push_front(v,p);
	}
	if(board[p]=='!'){
		*coins = (*coins)/2;
		for(int i=0; i<v->size/2; i++){
			v_pop_back(v);
		}
	}
	if(board[p]=='T'){
		*T += 3;
		//printf("sono su T+3\n\n");
	}
	if(board[p]=='#'){
		*T -= 1;
		//printf("sono su T-1\n\n");
	}
	//find_himself(v,p);

	if(board[p]!='$'){
		change(v,p);
	}
	find_himself(v,p,coins);
	printsnake(v);
	
	*current_position = p;
}
//gestisco le mosse
void moves(char move, int *current_position, int cols, int rows, int *coins, int *score, int *T, vector_t *v){
	int position;
	int possible = 1;
	if(move!='n' && move!='s' && move!='e' && move!='o'){
		printf("Move not possible!\n");
	}else{
		if(move=='n'){
			position = *current_position-cols;
			if(v->data[0]>=0 && v->data[0]<cols){
				printf("Move not possible!\n");
				possible = 0;
			}
		}
		if(move=='s'){
			position = *current_position+cols;
			if(v->data[0]>=(rows*cols-cols) && v->data[0]<(rows*cols)){
				printf("Move not possible!\n");
				possible = 0;
			}
        }
		if(move=='e'){
			position = *current_position+1;
			if((v->data[0]+1)%cols == 0){
				printf("Move not possible!\n");
				possible = 0;
			}
		}
		if(move=='o'){
			position = *current_position-1;
			if(v->data[0]%cols == 0){
				printf("Move not possible!\n");
				possible = 0;
			}
		}
		if(possible == 1){
			if(board[position]=='#' && *T==0){
				printf("Move not possible!\n");
			}else{
				coins_unexp_drill(position,coins,current_position,T,v);
				*score -= 1;
			}
		}
		
	}
}

int main(){
    //variables
    int rows=10, cols=19;
    int score = 1000;
    int type_of_lab = 1;
    int current_position, final_position;
    vector_t *snake=v_create();
    int coins = 0;
    int T = 0;
	int modality;
 //Game splash screen
 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);		
    printf("                  ____   \n");
    printf("                 / . .\\\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);	
    printf("                 \\  ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	printf("-- - <\n");	
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("                  \\  /\n");
    printf("        __________/ /\n");
    printf("   -= :___________ /\n");		
	printf("**************************************************\n");
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    printf("  /$$$$$$  /$$   /$$  /$$$$$$  /$$   /$$ /$$$$$$$$\n");
    printf(" /$$__  $$| $$$ | $$ /$$__  $$| $$  /$$/| $$_____/\n");
    printf("| $$  \\__/| $$$$| $$| $$  \\ $$| $$ /$$/ | $$      \n");
    printf("|  $$$$$$ | $$ $$ $$| $$$$$$$$| $$$$$/  | $$$$$   \n");
    printf(" \\____  $$| $$  $$$$| $$__  $$| $$  $$  | $$__/   \n");
    printf(" /$$  \\ $$| $$\\  $$$| $$  | $$| $$\\  $$ | $$      \n");
    printf("|  $$$$$$/| $$ \\  $$| $$  | $$| $$ \\  $$| $$$$$$$$\n");
    printf(" \\______/ |__/  \\__/|__/  |__/|__/  \\__/|________/\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);		
	printf("**************************************************\n");
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf( "Choose the modality:\n" ); 
	printf("Insert 1 for standard modality\n");
	printf("Insert 0 for AI modality\n");
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	scanf(" %d",&modality);

		printf("\n\nSNAKE LABYRINTH GAME\npress 1 to select the default labyrinth, press 0 to create a new labyrinth: ");
		scanf(" %d", &type_of_lab);

		//inserisco numero righe e colonne
		if(type_of_lab==0){
			printf("enter columns number: "); scanf(" %d", &cols);
			printf("enter rows number: "); scanf(" %d", &rows);
		}
		
		//alloco la memoria
		board=(char*)malloc(rows*cols*sizeof(char));

		if(type_of_lab==1){
			create_static_lab(rows,cols);
		}else{
			printf("\n\nCREATE YOUR LABYRINTH:\npress 'x' for space\n\n");
			create_dynamic_lab(rows,cols);
		}

		//stampo il campo di gioco
		printf("\n\n\nYOUR PLAYING FIELD\n\n\n");
		print_lab(rows,cols);

		current_position=find_start(rows,cols);
		final_position=find_finish(rows,cols);
		v_push_front(snake, current_position);
	if(modality){	
		char move;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		printf("Le mosse possibili sono:\n");
		printf("n -> nord\n");
		printf("s -> sud\n");
		printf("e -> est\n");
		printf("o -> ovest\n\n");
		printf("lo score iniziale � 1000");
    	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
		while(current_position!=final_position){
			printf("Inserisci la mossa: ");
			scanf(" %c",&move);
			printf("\n");
			moves(move,&current_position,cols,rows,&coins,&score,&T,snake);
			//clrscr();
			system("cls");
			print_lab(rows,cols);
			//v_print(snake);
			printf(ANSI_COLOR_YELLOW "SCORE:" ANSI_RESET_ALL);
	        printf("%d",score );
	        printf("\n");
		}
	}else{
		AI_modality(rows,cols,final_position,&current_position);
		
	}
    v_free(snake);
}    
