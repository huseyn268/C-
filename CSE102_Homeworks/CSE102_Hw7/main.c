#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MOVES 100

int move_rows[MAX_MOVES];
int move_cols[MAX_MOVES];
int move_top = 0;


void printf_board(int **map , int **playerMap  , int a ){

    int i ,j ;

    printf(" ");

    for(i = 0  ; i<a ; i++){
        printf("%4d",i);
    }
    printf("\n");
    for (i = 0; i < a; i++)
    {

        printf("%d",i);



        for (j = 0; j < a; j++)
        {
            if(playerMap[i][j] == 1)
                if (map[i][j] ==  -1)
                {
                    printf("%4c",'X');
                }else
                    printf("%4d",map[i][j]);
                else
                    printf("%4c",'#');
        }
        printf("\n");
    }

}

void copyFILE(int **map, int a, FILE *fp) {
    int i, j;

    for (i = 0; i < a; i++) {
        for (j = 0; j < a; j++) {
            if (map[i][j] == -1)
                fprintf(fp, "* ");
            else
                fprintf(fp, ". ");
        }
        fprintf(fp, "\n");
    }
}


void push(int row, int col) {
    if (move_top < MAX_MOVES) {
        move_rows[move_top] = row;
        move_cols[move_top] = col;
        move_top++;
    }
}

void pop() {
    if (move_top > 0)
        move_top--;
}

int isEmpty() {
    return move_top == 0;
}

int cmp(char *map , char *map2 , int a){
    int i ;

    for( i = 0 ; i<a ; i++){
        if(map[i] != map2[i]){
            return 0;
        }
    }
    return 1;
}


int control_cell(int **map , int row , int col , int a){


if(row < 0 || row >= a || col < 0 || col >= a){
        return 0;
    }
    if(map[row][col] == -1){
        return 1;
    }
  else{
        return 0;
    }
  }



void flood_fill(int** playerMap,int** map,int row ,int col , int a ){

    if(row < 0 || row >= a || col < 0 || col >= a){
        return;
    }
    if(playerMap[row][col] == 1){
        return;
    }
    playerMap[row][col] = 1;

     if(map[row][col] !=0)
        return;
     


     flood_fill(playerMap,map,row-1,col,a);
    flood_fill(playerMap,map,row,col -1 ,a);
    flood_fill(playerMap,map,row+1,col,a);
    flood_fill(playerMap,map,row,col + 1,a);


    flood_fill(playerMap,map,row-1,col-1,a);
    flood_fill(playerMap,map,row-1,col+1,a);
    flood_fill(playerMap,map,row+1,col-1,a);
    flood_fill(playerMap,map,row+1,col+1,a);

        
}





void create_cell(int **map , int a ){

    int i , j , k ;
    int count = 0;
    int minePlaced = 0;
    
    for(i = 0 ; i<a ;i++){
        for(j = 0 ; j<a ; j++){
            k = rand() % 10 ;
            if(k<3){
                map[i][j]  = -1; 
                minePlaced++;
            }
            else{
                map[i][j] = 0;
            }  
        }
    }

    if (a == 2 || a == 3) {           
        if (minePlaced == 0) {         
            int randRow = rand() % a;  
            int randCol = rand() % a;  
            map[randRow][randCol] = -1;
        }
    }



for(i = 0 ; i<a ; i++){
    
    for(j = 0 ; j<a ; j++){
            if(map[i][j] == -1){
                continue;
            }

            count = 0 ;

            if(control_cell(map,i-1,j,a)){
                count++;
            }
            if(control_cell(map , i+1,j,a)){
                count++;
            }
            if(control_cell(map , i,j-1,a)){
                count++;
            }
            if(control_cell(map , i,j+1,a)){
                count++;
            }
            if(control_cell(map , i-1,j-1,a)){
                count++;
            }
            if(control_cell(map , i-1,j+1,a)){
                count++;
            }
            if(control_cell(map , i+1,j-1,a)){
                count++;
            }
            if(control_cell(map , i+1,j+1,a)){
                count++;
            }
            map[i][j] = count;
    }

}


}




int playGame(int ** map , int **playerMap , int row , int col , int a ){


    int i , j ;

    if(map[row][col] == -1){
        playerMap[row][col] = 1;
        return 1 ;
    }

else if(map[row][col] == 0 ){

        flood_fill(playerMap,map,row,col,a);

}
else {
    playerMap[row][col] = 1 ;
}



for(i = 0 ; i<a ; i++){

    for(j = 0 ; j<a ; j++){

        if(map[i][j] != -1 && playerMap[i][j]== 0 ){
            return 0 ;
        }

    }



}
    return 2 ;
}

void newGame(int **playerMap , int a ){

    int i , j ;

    for(i = 0 ; i<a ; i++){

        for(j = 0 ; j<a ; j++){
            playerMap[i][j] = 0 ;
        }

    }


}

int main(void){




    int totalM = 0 ;
    int a ,row , col , i , j ;
    int gameOver = 0 ;
    int **map , **playerMap ;

    char cordinate[100];

srand(time(NULL)); 


a = (rand() % 9) + 2 ;

map = (int **)malloc(a * sizeof(int *));

    for(i = 0  ; i<a ; i++){
        map[i] = (int *)malloc(a * sizeof(int));
    }

    playerMap = (int **)malloc(a * sizeof(int *));
    for(i = 0  ; i<a ; i++){
        playerMap[i] = (int *)malloc(a * sizeof(int));
    }

    for(i = 0 ; i<a ; i++){
        for(j = 0 ; j<a ; j++){
            playerMap[i][j] = 0 ;
        }
    }
    create_cell(map , a);



    FILE* move  = fopen("moves.txt","w");
    FILE*  Map = fopen("map.txt","w");


    if(Map == NULL ){

        perror("Error opening map.txt");

    }
    else{
        copyFILE(map , a , Map);
        fclose(Map);
    }


if(move == NULL ){

        perror("Error opening move.txt");

    }
    else{
      fprintf(move," --- Game Moves ---\n");

    }
    printf_board(map, playerMap, a);
    while(!gameOver){

        printf("Enter move (row col) or 'undo': ");
        if (fgets(cordinate,sizeof(cordinate),stdin) == NULL) {
             break; 
        }


if(cmp(cordinate,"undo",4)){

    if(move_top == 0){
        printf("No moves to undo.\n");
        continue; 
}


    totalM++;

    if(move != NULL){
        fprintf(move,"Move %d: undo\n",totalM);
      
    }
    pop();

    newGame(playerMap , a);




    for(i = 0 ; i <move_top ; i++){
        playGame(  map , playerMap , move_rows[i] , move_cols[i] , a);
    }

    printf_board(map , playerMap , a);
    continue;

    }
    if(cordinate[0] <'0' || cordinate[0]> '9' || cordinate[1] != ' ' || cordinate[2]<'0' || cordinate[2] >'9' || cordinate[3] != '\n' && cordinate[3] != '\0'){
        printf("Invalid Input Format! Use 'row col' (single digits) or 'undo'.\n");
        continue;

    }


row = cordinate[0] - '0';
col = cordinate[2] - '0';


if(row<0 || row>= a || col<0 || col>=a){
    printf("Invalid coordinates! Try again. Map is %d * %d\n",a,a);
    continue;
}

totalM++;


if(move != NULL){
    fprintf(move,"Move %d: (Row : %d , Col : %d) \n",totalM,row,col);
    
}

push(row,col);

gameOver = playGame(map , playerMap , row , col , a);
printf_board(map , playerMap , a);


    if(gameOver == 1){
        printf("Boom! You Hit a Mine. Game Over!\n");
      
    }
    else if(gameOver == 2){
        printf("Congratulations, You won the game!\n");
       
    }
    

}

    if(move != NULL){
        fprintf(move,"\n\nTotal Moves: %d\n",totalM);
        
    }

for(i = 0 ; i<a ; i++){
    free(map[i]);
}


for(i = 0 ; i<a ; i++){
    free(playerMap[i]); 
}
fclose(move);
free(map);
free(playerMap);
    return 0;

}













        
