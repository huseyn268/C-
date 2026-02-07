#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define shipsPlaces "ships.txt"
#define shipLogs "battleships_log.txt"
#define size 10
#define max 100

char map[size][size];
int hit_map[size][size];
int health_ship[max] = {0} ;
int total_ships_health = 0 ;
int player_shots = 0 ; 


void draw_map(){
    int i ;
    int j = 0 ;

    for(i = 0 ; i<size ; i++){

            for(j = 0 ; j<size ; j++){

               map[i][j] =  '-';
               hit_map[i][j] = 0 ;
            }
    }

    for(i= 0 ; i<100 ; i++){
        health_ship[i] = 0 ;

    }
    total_ships_health = 0 ;

}


void createPlace_ship(int area ,int hit ,FILE *ships){
    int firstX , firstY , direction ;
    int j , bool ;
    do{
        direction = rand() %2 ;
        bool = 1 ;
        if(direction ==0){
            firstX = rand() % (size - area);
            firstY = rand() % size ;
        }
        else{
            firstX = rand() % size ;
            firstY = rand() % (size -area); }

                j = 0;

            while(j<area){

                int x = firstX + (direction == 0 ? j : 0);
                int y = firstY + (direction == 1 ? j : 0);
                if(map[y][x] != '-'){
                    bool = 0;
                    break ;
                }
                j++ ;
    }}while(!bool);
                j = 0 ;
               while(j<area){

                int x = firstX + (direction == 0 ? j : 0);
                int y = firstY + (direction== 1 ? j : 0);
                map[y][x] = 'S';
                hit_map[y][x] = hit ;
                fprintf(ships,"%d %d %d\n",hit,y,x);
                total_ships_health++;
                j++;
               }          health_ship[hit] = area ;
}

void newMap(){

    FILE* ships = fopen(shipsPlaces,"w");
    FILE * logs = fopen(shipLogs,"w");
    if(logs == NULL){
        printf("ERROR: Cannot open file %s\n",shipLogs);
        return ;
    }
    
    if(ships == NULL){
        printf("ERROR: Cannot open file %s\n",shipsPlaces);
        return ;
    }
    srand(time(NULL));
    draw_map();
    int shipSize[3] = {4,3,2};
    int i , h ,j;
    int shipCount[3] = {1,2,1};  
    for(h = 0 ; h<3 ; h++){         
            for(j = 0 ; j<shipCount[h] ; j++){
                int hit = shipSize[h]*10 + j;
                createPlace_ship(shipSize[h],hit,ships);
            }
    }
    player_shots = 0 ;
    fclose(ships);
    fclose(logs);

}
    int shot(int x , int y){

    if(x<0 || x>=size || y<0 || y>= size){
        printf("ERROR: Invalid coordinates.\n");
        return 2;
    }
    char d = map[y][x];
    if(d== 'X' || d == 'O'){
        printf("ERROR: You've already hit this spot!\n");
        return 1;
    }
    FILE *p = fopen(shipLogs,"a");
    fprintf(p,"Shot: %d %d - ",y,x);   
    player_shots++;
    if(d == '-'){
        printf("MISS!\n");
        map[y][x] = 'O';
        fprintf(p,"MISS\n");
        fclose(p);
        return -1;
    }
if(d == 'S'){

    printf("HIT!\n");
    map[y][x] = 'X';
    fprintf(p,"HIT\n");

    int hit = hit_map[y][x];
    int remaining = --health_ship[hit];

    if(remaining == 0){
        printf("Congratulations!! You've sunk %d-cell ship.\n",hit/10);
    }

    if(--total_ships_health == 0){
        printf("You've sunk all ships! It took %d shots.\n",player_shots);
        fprintf(p,"All ships sunk in %d shots\n",player_shots);
        fclose(p);
        return -2;
    }
    fclose(p);
    return-3;

}   
fclose(p);
return 0;
}
        
void displayMap(int k) {
    int x, y;

  
    printf("   ");
    for (x = 0; x < size; x++){
        printf("%2d ", x);
    }
    printf("\n");

   
    for (y = 0; y < size; y++){
        printf("%2d ", y); 
       for (x = 0; x < size; x++){
            char c = map[y][x];
            if (k == 1) {
                
        printf(" %c ", c);
            } else {
           
       if (c == 'S' || c == '-') {
                    printf(" - ");        } else {
                 printf(" %c ", c); 
                }
          }
       }
        printf("\n");
    }
}


int gameplay(){
    while(1){
        displayMap(0);
        printf("Enter coordinate (X to quit) : ");
        char k ;
        int x ,y ,c;

   if(scanf("%d %d",&y,&x) == 2){
    int h = shot(x,y);
    printf("\n");
    
    if(h ==-2){
        break;
    }
    
}
else{   

    scanf("%c",&k);

if(k == 'X' ||  k  == 'x'){

    return 120 ;}

printf("Invalid input.\n");

    for(c = getchar(); c!= '\n' && c != EOF ; c = getchar());

} 

}return 0 ;
}


int newGame(){

    newMap();
    gameplay();
    return 0;

}


int main(){

    printf("Welcome to Battleship Game!!\n\n");

    while(1){

        int a = newGame();

        if(a == 120)
            break;

        int h ;
        while((h = getchar()) != '\n' && h != EOF);

        printf("Game over! Press 'N' to start a new game or 'X' to quit: ");
        char l ;
        scanf("%c",&l);

        if(l == 'N' || l == 'n')
            continue;
        else if(l == 'X' || l == 'x')
            break;
    }
}
