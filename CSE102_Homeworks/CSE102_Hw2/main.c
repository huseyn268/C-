#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int main(){
int XDoor , YDoor , width , height ,XPlayer , YPlayer ;
char move ;

    srand(time(NULL));
    FILE * game ;
    game = fopen("game_state.txt","r+");

        if(game ==NULL){
            game = fopen("game_state.txt","w+");
                if(game==NULL) {
                    printf("Opening file failed\n");
                    return 1 ;
                }
                else{
                    printf("Opening file is successful\n"); }
                printf("Generating a new board...\n");
                printf("Enter the width and height of board: ");
                scanf("%d %d",&width , &height);

                    if(width <= 1 || height<=1 ){
                        printf("Width and height must be greater than 1 .Please try again\n ");
                        remove("game_state.txt");
                        return 0 ;}
                        
                XDoor = (rand() %width) +1 ;
                YDoor = (rand() %height)  +1;
                XPlayer = (rand() %width)  +1;
                YPlayer = (rand() %height) +1 ;
                
                    if(XDoor == XPlayer && YDoor == YPlayer){
                        XDoor = (rand() %width) +1 ;
                        YDoor = (rand() %height)  +1;
                        XPlayer = (rand() %width)  +1;
                        YPlayer = (rand() %height) +1 ;
                    }
            printf("Width : %d , Height : %d , Player(%d,%d), Door(%d,%d) \n",width,height,XPlayer,YPlayer,XDoor,YDoor);
            fprintf(game, "%d %d %d %d %d %d\n", width, height, XPlayer, YPlayer, XDoor, YDoor);
            fclose(game);
  
  
                }
            
                else{
                    fscanf(game, "%d %d %d %d %d %d\n", &width, &height, &XPlayer, &YPlayer, &XDoor, &YDoor); 
                        
                    printf("Width : %d , Height : %d , Player(%d,%d), Door(%d,%d)  \n",width,height,XPlayer,YPlayer,XDoor,YDoor);
                            printf("Enter move(WASD): ");
                            scanf(" %c",&move);
        
                            if(XPlayer!=XDoor || YPlayer !=YDoor){
                                if(move =='D' || move == 'd'){
                                    XPlayer = XPlayer +1 ;
                                printf("Player moves  right to (%d,%d)\n",XPlayer,YPlayer);
                                    if(XPlayer>width || XPlayer<1){
                                         printf("Player does not  moves  right to (%d,%d)\n",XPlayer,YPlayer);
                                         printf("Player is hitting the wall !! Please try again\n");
                                        XPlayer = XPlayer -1 ;                                
                                    }                       
                                }
                                else if(move=='A' || move== 'a'){
                                    XPlayer = XPlayer-1 ;
                                printf("Player moves left to (%d,%d)\n",XPlayer,YPlayer);
                                    if(XPlayer>width || XPlayer<1){
                                         printf("Player does not  move  not left to (%d,%d)\n",XPlayer,YPlayer);
                                         printf("Player is hitting the wall !!\n");
                                         XPlayer = XPlayer +1 ;                            }
                                }
                                else if(move == 'S'|| move=='s'){
                                    YPlayer = YPlayer +1 ;
                                printf("Player moves down to (%d,%d)\n",XPlayer,YPlayer);
                                
                                    if(YPlayer>height || YPlayer<1){
                                         printf("Player does not  move  down to (%d,%d)\n",XPlayer,YPlayer);
                                         printf("Player is hitting the wall !!\n");
                                        YPlayer = YPlayer -1 ;
                                    }
                                }
                                else if (move=='W'|| move=='w'){
                                    YPlayer = YPlayer -1 ;
                                printf("Player moves up to (%d,%d)\n",XPlayer,YPlayer);
                                    if(YPlayer>height || YPlayer<1){
                                        printf("Player does not  move  up to (%d,%d)\n",XPlayer,YPlayer);
                                        printf("Player is hitting the wall !!\n");
                                        YPlayer = YPlayer+1  ;
                                    }}
                                else{
                                    printf("\n Error Please just enter W-A-S-D\n");
                                }
                                rewind(game);
                                 fprintf(game, "%d %d %d %d %d %d\n", width, height, XPlayer, YPlayer, XDoor, YDoor);
                                printf("Game continues...\n"); }

                      if (XPlayer == XDoor && YPlayer == YDoor) {
                                printf("Congratulations! You escaped!\n");
                                printf("Generating a new board...\n");
                                remove("game_state.txt");
                                game = fopen("game_state.txt","w+");
                                  printf("Enter the width and height of board: ");
                                  scanf("%d %d", &width, &height);
                                  if(width <= 1 || height<=1 ){
                                    printf("Width and height must be greater than 1 .Please try again\n ");

                                    remove("game_state.txt");
                                    return 0 ;}

                                  XDoor = (rand() % width) + 1;
                                  YDoor = (rand() % height) + 1;
                                  XPlayer = (rand() % width) + 1;
                                  YPlayer = (rand() % height) + 1;

                                if(XDoor == XPlayer && YPlayer == YDoor){
                                    XDoor = (rand() % width) + 1;
                                    YDoor = (rand() % height) + 1;
                                    XPlayer = (rand() % width) + 1;
                                    YPlayer = (rand() % height) + 1;}
                                  printf("Width: %d, Height: %d, Player(%d,%d), Door(%d,%d)\n", width, height, XPlayer, YPlayer, XDoor, YDoor);
                                   fprintf(game, "%d %d %d %d %d %d\n", width, height, XPlayer, YPlayer, XDoor, YDoor);                     
                        fclose(game);   
                        return 0 ;
                            } }   }            
        
    
