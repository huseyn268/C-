#include<stdio.h>

double get_balance();
void update_balance(double new_balance);
void check_balance();
void withdraw_money();
void deposit_money();
void menu();

double get_balance() {
    double money;
    FILE *atm = fopen("account.txt", "r");
    if (atm==NULL) {
        money = 100.0;
        atm = fopen("account.txt", "w");
        if (atm == NULL) {
            printf("Opening file failed.\n");
            return money;
        } else {
            fprintf(atm, "%lf", money);
            fclose(atm);
        }
    } else {
        fscanf(atm, "%lf", &money);
        fclose(atm);
        return money;
    }
  return money ;
}

void update_balance(double new_balance) {
    FILE *atm = fopen("account.txt", "w");
    if (atm == NULL) {
        printf("Error opening file!\n");
        return;
    }   
        fprintf(atm, "%.2lf", new_balance);
    fclose(atm);

}
void check_balance(){
double money = get_balance();

printf("Current balance: %.2lf TRY \n", money);
}

void withdraw_money(){
    double money = get_balance();
    double withdraw;   
    printf("Enter amount to withdraw : ");
    scanf("%lf",&withdraw);

    if (withdraw <= 0) {
        printf("Invalid amount! Withdrawal must be a positive value.\n");
        return ;
    }

   if(withdraw<=money){
   money = money - withdraw ;
    update_balance(money);
    printf("Withdraw successful! New balance: %.2lf TRY (Saved account.txt)\n",money);
   } 
   else{
    printf("Insufficient funds! Your balance is only %.2lf TRY.\n",money);
   }
   }     
void deposit_money(){

    double deposit ;
    double money = get_balance(); 
    printf("Enter amount to deposit : ");
    scanf("%lf",&deposit);

    if (deposit <= 0) {
        printf("Invalid amount! Deposit must be a positive value.\n");
        return;
    }
        money = money + deposit ;
        update_balance(money);
        printf("Deposit successful! New balance: %.2lf TRY (Saved account.txt)\n",money);
}


void menu(){

    int op ;

    printf("\n----- Virtual ATM -----\n\n");
    printf("1. Check Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Exit\n");

    printf("Choose an option: ");
    scanf("%d",&op);

    switch (op){
    case 1 : check_balance();   break;

    case 2 : deposit_money(); break;

    case 3 :  withdraw_money(); break ;
    
    case 4 : printf("Exiting ..... \n"); break ;
    default:
        printf("Error please just enter a number between 1 - 4 \n");
     break;
    }
}

int main(){

    menu();
  return 0 ;
}
