#include<stdio.h>
#include<stdlib.h>
#include<time.h>

char * getCurrentDataTime(){

 static char dateTime[100];
 time_t now;
    struct tm *tm_info;
    time(&now);
    tm_info = localtime(&now);
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", tm_info);

    return dateTime;
}

void adminMode(){

    int length ,rangeMin,rangeMax,duplicate ,attemps ; 
    int correct,wrong,misplaced ; 
     FILE* data = fopen("vault_config.txt","w");
        int success = 0;
 do{
         printf("Please enter game rules  \n");
         printf("Code length : ");
         scanf("%d",&length);
            if(length <= 0){
                printf("Please enter valid input \n");
                success = 0;
            }
            else{
                success = 1;
            }
        }while(success == 0);
            do{
        do{ printf("Maxdigit  :  ");
         scanf("%d",&rangeMax);
         printf("Mindigit : ");
         scanf("%d",&rangeMin);
         if(rangeMax>9 || rangeMin<0){
             printf("Please enter valid input \n");
             success = 0;
         }
         else{
             success = 1;
         } }while(success == 0);         
         if(rangeMin >= rangeMax){
             printf("Range max must be greater than rangemin \n");
             success = 0;
             while(getchar() != '\n'); 
         }
         else{
             success = 1;
         }}while(success == 0);

      

       do {
    printf("Maximum number of attempts: ");
    scanf("%d", &attemps);
    
    if (attemps <= 0) {
        printf("Please enter valid input for attempts (greater than 0)\n");
        success = 0;
    } else {
        success = 1;

        do {
            printf("Allow duplicate: (0 = No, 1 = Yes): ");
            scanf("%d", &duplicate);

            if (duplicate != 0 && duplicate != 1) {
                printf("Please enter valid input for duplicates (0 or 1)\n");
                success = 0;
            } else {
                if (!duplicate && (rangeMax - rangeMin + 1) < length) {
                    printf("Error: Not enough unique numbers to generate code without duplicates. Please try again\n");
                    return ;
                    success = 0;
                } else {
                    success = 1;
                }
            }

        } while (success == 0);
    }

} while (success == 0);

        
            
        do{
printf(" Enter points  for Correct digit in correct place (C) , Correct digit in wrong place (M),Penalty for wrong digit (W) :");
scanf("%d %d %d",&correct,&misplaced,&wrong);

            if(correct < 0 || misplaced < 0 || wrong < 0){
                printf("Please enter valid input \n");
                success = 0;
            }
            else{

                if(wrong> correct && wrong > misplaced){
                    printf("Penalty for wrong digit must be less than points for correct and misplaced digits \n");
                    success = 0;
                }
                else{
                    success = 1;
                }
                success = 1;
            }

        }while(success == 0);
         fprintf(data,"CODE_LENGTH=%d\n",length);
         fprintf(data,"DIGIT_MIN=%d\n",rangeMin);
         fprintf(data,"DIGIT_MAX=%d\n",rangeMax);
         fprintf(data,"MAX_ATTEMPTS=%d\n",attemps);
         fprintf(data,"ALLOW_DUPLICATES=%d\n",duplicate);  
         fprintf(data,"POINTS_CORRECT=%d\n",correct);
         fprintf(data,"POINTS_MISPLACED=%d\n",misplaced);
         fprintf(data,"PENALTY_WRONG=%d\n",wrong);
 
 fclose(data);
 
 }
 
 int * generate_code(){
 int i , j ;
     int codeLength , minDigit , maxDigit , allowDuplicates , maxAttempts; ;
        int correct,wrong,misplaced ; 
        int repeated ;
     FILE* data = fopen("vault_config.txt","r");

     if(data == NULL){
         printf("Error: Cannot open file vault_config.txt\n");
         return NULL;
     }

           fscanf(data,"CODE_LENGTH=%d\n",&codeLength);
              fscanf(data,"DIGIT_MIN=%d\n",&minDigit);
                fscanf(data,"DIGIT_MAX=%d\n",&maxDigit);
                fscanf(data,"MAX_ATTEMPTS=%d\n",&maxAttempts);
                fscanf(data,"ALLOW_DUPLICATES=%d\n",&allowDuplicates);
                fscanf(data,"POINTS_CORRECT=%d\n",&correct);
                fscanf(data,"POINTS_MISPLACED=%d\n",&misplaced);
                fscanf(data,"PENALTY_WRONG=%d\n",&wrong);
                
 
             if (!allowDuplicates && (maxDigit - minDigit + 1) < codeLength) {
                 printf("Error: Not enough unique numbers to generate code without duplicates.\n");
                 return NULL ;
             }
     FILE * guess = fopen("vault_code.txt","w");
     if(guess == NULL){
        printf("Error: Cannot open file vault_code.txt\n");
        return 0;
    }
             int *code = malloc(codeLength * sizeof(int));
             if (code == NULL) {
                 printf("Error: Memory allocation failed.\n");
                 return NULL ;
             }
         
             srand(time(NULL));
         
             for (i = 0; i < codeLength; ) {
                 int num = rand() % (maxDigit - minDigit + 1) + minDigit;
                 repeated = 0;
         
                 if (!allowDuplicates) {
                     for ( j = 0; j < i; j++) {
                         if (code[j] == num) {
                             repeated = 1;
                             break;
                         }
                     }
                 }
         
                 if (allowDuplicates || !repeated) {
                     code[i] = num;
                     i++;
                 }
             }
         
            
             for ( i = 0; i < codeLength; i++) {
               
                 fprintf(guess,"%d",code[i]);
             }
             printf("\n");
         
 
             fclose(data);
 
             fclose(guess);
           
 
             return code;
        
     
         }

int digit_counter(int number ) {
   
            int counter = 0;
            if (number == 0) {
                counter = 1;
            } else {
                if (number < 0) {
                    number = -number;
                }
                
                while (number != 0) {
                    number = number / 10;
                    counter++;
                }
            }
            
            return counter ;
        }
int strlength(char * str){
    int i = 0 ;
    while(str[i] != '\0'){
        i++;
    }
    return i ;
}
int * get_guess(char * number , int length){
int i ;
    int * digits = malloc(length * sizeof(int));
    if (digits == NULL) {
        printf("Error: Memory allocation failed.\n");
        return NULL ;
    }
    
    for ( i = 0 ; i <length; i++) {
        if(number[i] < '0' || number[i] > '9'){
            printf("Error: Invalid input. Please enter a valid number.\n");
            free(digits);
            return NULL ;
        }
        digits[i] = number[i] - '0';
    
    }
    
    return digits;
}



void game(){
int i  ;
    char *currentDateTime = getCurrentDataTime();
    int codeLength , minDigit , maxDigit , allowDuplicates , maxAttempts ,j;
    int correct,wrong,misplaced ;
    FILE* log = fopen("game_log.txt","w");
    FILE* data = fopen("vault_config.txt","r");
    if(data == NULL){
       printf("Sorry, the game can't be started right now.\n");
printf("It looks like the configuration file 'vault_config.txt' is missing or not set up yet.\n");
printf("Please make sure to complete the setup in [A]dmin mode first.\n");
        return ;
    }
    if (log == NULL) {
        printf("Error: Cannot open file game_log.txt\n");
        return;
    }
   int * secret = generate_code(); 
    if (secret == NULL) {
        fclose(data);
        fclose(log);
        return;
    }
    char guess[100]; ;

    fscanf(data,"CODE_LENGTH=%d\n",&codeLength);
    fscanf(data,"DIGIT_MIN=%d\n",&minDigit);
    fscanf(data,"DIGIT_MAX=%d\n",&maxDigit);
    fscanf(data,"MAX_ATTEMPTS=%d\n",&maxAttempts);
    fscanf(data,"ALLOW_DUPLICATES=%d\n",&allowDuplicates);
    fscanf(data,"POINTS_CORRECT=%d\n",&correct);
    fscanf(data,"POINTS_MISPLACED=%d\n",&misplaced);
    fscanf(data,"PENALTY_WRONG=%d\n",&wrong);

    
    fclose(data);



    fprintf(log,"--- Vault Codebreaker Game Log --- \n");
    fprintf(log,"Game date : %s\n", currentDateTime);
    fprintf(log,"Secret Code : ");

    for( i = 0 ; i<codeLength; i++){
        fprintf(log,"%d",secret[i]);
    }
    fprintf(log,"\n");
    fprintf(log,"Code Length: %d\n",codeLength);
    fprintf(log,"Digit range: %d - %d\n",minDigit,maxDigit);
    fprintf(log,"Duplicates allowed: %d\n",allowDuplicates);
    fprintf(log,"Max attempts: %d\n",maxAttempts);

fprintf(log,"\n");
    printf("Welcome to the game!\n");
    printf("You have %d attempts to guess the code.\n\n", maxAttempts);
    printf("The code consists of %d digits, each between %d and %d.\n", codeLength, minDigit, maxDigit);
    printf("You will receive points for correct and misplaced digits, and a penalty for wrong digits.\n");
    printf("Please enter your guess:\n");
   

int sum = 0 ;
int k = 1 ;
    while(k<=maxAttempts){
    int point = 0 ;

        int c_counter = 0 ;
    printf("Attempt %d: ", k );
    scanf("%s", guess);
    int guessLength = strlength(guess);
    if (guessLength != codeLength) {
        printf("Invalid guess. Please enter a %d-digit number.\n", codeLength);
        continue;
    }
    fprintf(log,"Attempt %d: ", k);
    for( i = 0 ; i<codeLength ; i++){
        fprintf(log,"%c",guess[i]);
    }
   
  

    char* feedback = malloc(codeLength * sizeof(char));
    int * guessArrey = get_guess(guess,codeLength);
    if(guessArrey == NULL){
        free(feedback);
        free(guessArrey);
        continue;
    }

    for( i = 0 ; i<codeLength ; i++){
        if(guessArrey[i] == secret[i]){
            feedback[i] = 'C';
            point += correct;
            sum +=correct;
            c_counter++;
        } else {
            int found = 0;
            for( j = 0 ; j<codeLength ; j++){
                if(guessArrey[i] == secret[j]){
                    found = 1;
                    break;
                }
            }
            if (found) {
                feedback[i] = 'M';
                point += misplaced;
            sum +=misplaced;

            } else {
                feedback[i] = 'W';
                point += wrong;
                sum += wrong;

            }
        }
    }
    
    printf("=> Feedback: ");
    fprintf(log," => Feedback: ");
    for( i = 0 ; i<codeLength ; i++){
        printf("%c ", feedback[i]);
        fprintf(log,"%c ", feedback[i]);
    }
   
    printf("|  Score : %d\n",point);
    fprintf(log,"|  Score : %d\n",point);
    
    if(c_counter == codeLength){
        printf("Congratulations! You guessed the code correctly!\n");
        printf("Final score is: %d\n", sum);
        fprintf(log,"\nFinal score is: %d\n", sum);
        free(feedback);
        free(guessArrey);

        break;
    }



if(k == maxAttempts){
    printf("Sorry, you've used all your attempts.\n");
    printf("Final score is: %d\n", sum);
    fprintf(log,"\nFinal score is: %d\n", sum);

    for( i = 0 ; i<codeLength ; i++){
        printf("%d",secret[i]);
    }
    printf("\n");
    break;

}

k++;
    free(guessArrey);
    free(feedback);
    }
    free(secret);


if(sum>=90){
    printf("Rank : Code Master\n");
    fprintf(log,"Rank : Code Master\n");
}
else if(sum>=70 && sum<90){
    printf("Rank : Cipher Hunter \n");
    fprintf(log,"Rank : Cipher Hunter \n");
}
else if(sum>=50 && sum<70){
    printf("Rank : Number Sleuth \n");
    fprintf(log,"Rank : Number Sleuth \n");
}
else if(sum>= 30 && sum<=49){
    printf("Rank : Safe Kicker \n");
    fprintf(log,"Rank : Safe Kicker \n");
}
else if(sum>=10 && sum<30){
    printf("Rank : Lucky Breaker \n");
    fprintf(log,"Rank : Lucky Breaker \n");
}
else if(sum<=0){
    printf("Rank : Code Potato \n");
    fprintf(log,"Rank : Code Potato \n");

}
fclose(log);



}
int main(){

    char choice;
    do{
        printf("Welcome to the Vault Game!\n");
        printf("A. Admin Mode\n");
        printf("P. Player Mode\n");
        printf("S. Exit\n");
        printf("Please enter your choice: ");
        scanf(" %c", &choice);
    
        switch (choice) {
            case 'a' :
            case 'A' :
                adminMode();
                break;
            case 'p' :
            case 'P' :
                game();
                break;
            case 's' :
            case 'S':
                printf("Exiting the game.\n");
                break ;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    }while(choice != 'S' && choice != 's');
   

    return 0;


}
