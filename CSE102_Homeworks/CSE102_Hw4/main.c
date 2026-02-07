#include<stdio.h>
#include<math.h>

void launchParameters(){

    double v,g,h ;
    FILE * data = fopen("rocket_data.txt", "w");
    if(data == NULL){
        printf("Error opening file!\n");
        return ;
    }

    printf("Enter the initial velocity (m/s) : ");
   
    if (scanf("%lf",&v) != 1  || v<0){
        printf("Invalid value for velocity !\n");
        while(getchar() != '\n');
        return ;
    }
    printf("Enter the acceleration due to gravity (m/s^2 , default 9.8) : ");
    if(scanf("%lf",&g) != 1  || g<=0){
    printf("Invalid value for gravity !\n");
        while(getchar() != '\n');
        g = 9.8 ;
        printf("Using default value for gravity : 9.8 m/s^2\n");
    }
    printf("Enter the initial height (m) : ");
    if(scanf("%lf",&h) != 1 || h<0 ){
        printf("Invalid value for height !\n");
        return ;
    }

fprintf(data,"The initial velocity is : %.2lf m/s\n",v);
fprintf(data,"The acceleration due to gravity is : %.2lf m/s^2\n",g);
fprintf(data,"The initial height is : %.2lf m\n",h);
printf("Rocket parameters saved to rocket_data.txt !\n");
fclose(data);


}

double calculateRocket(double v ,double g, double h , double t){
    return (-0.5 * g * t *t ) + (v * t ) + h;

}
double calculatealtitude(double v , double g , double h){
   
    double maxTime = v/g ;
    return calculateRocket(v,g,h,maxTime);
}
double calculateFlightTime(double v ,double g , double h ){
    double root_term = v*v + 2*g*h ;
   return  (v + sqrt(root_term)) / g ;
}

void simulateRocket(){
double v,g,h ,t ;
double lineH ;
FILE * data = fopen("rocket_data.txt","r");
if (data == NULL){
    printf("Error :  First enter launch parameters (Option 1 )  !\n");
    return ;
    
}
printf("Reading rocket parameters from file...\n");
    fscanf(data,"The initial velocity is : %lf m/s\n",&v);
    fscanf(data,"The acceleration due to gravity is : %lf m/s^2\n",&g);
    fscanf(data,"The initial height is : %lf m\n",&h);

printf("h(t)= %.1lf *t^2 +  %.1lf *t + %.1lf\n",-0.5*g,v,h);

double maxTime = calculateFlightTime(v,g,h);
double maxAltitude = calculatealtitude(v,g,h);
double step_y = maxAltitude / 10 ;

        for(lineH = maxAltitude ; lineH>= 0 ; lineH-= step_y){
            printf("%6.0f |",lineH);
            for( t = 0 ; t<= maxTime ; t+=1){
                double height = calculateRocket(v,g,h,t);
                if(height>= lineH - step_y/2 && height<lineH + step_y/2){
                    printf("  #");
                }
                else{
                    printf("   ");
                }
            }
        
        printf("\n");        
        }
int i ;
    printf("       ");
    for(i = 0 ; i<=maxTime ; i++){
        printf("|--");
    }
    printf("|");
    printf("\n     ");

    for( i= 0 ; i<=maxTime+1 ; i++){
        printf("%3d",i);
        }
        printf("\n");
fclose(data);
    }


void saveData(){
    FILE * save = fopen("trajectory.txt","w");
    if(save == NULL){
        printf("Error opening file !\n");
        return ;
    }
    double v,g,h ,t ;
    double lineH ;
    FILE * data = fopen("rocket_data.txt","r");
    
    if (data == NULL){
    
        printf("Error :  First enter launch parameters (Option 1 )  !\n");
    return ;
    }
        fscanf(data,"The initial velocity is : %lf m/s\n",&v);
        fscanf(data,"The acceleration due to gravity is : %lf m/s^2\n",&g);
        fscanf(data,"The initial height is : %lf m\n",&h);
        fclose(data);
    fprintf(save,"h(t)= %.1lf *t^2 +  %.1lf *t + %.1lf\n\n",-0.5*g,v,h);
    
    double maxTime = calculateFlightTime(v,g,h);
    double maxAltitude = calculatealtitude(v,g,h);
    double step_y = maxAltitude / 10 ;
    
    
            for(lineH = maxAltitude ; lineH>= 0 ; lineH-= step_y){
                fprintf(save,"%6.0f |",lineH);
                for(t = 0 ; t<= maxTime ; t+=1){
                    double height = calculateRocket(v,g,h,t);
                    if(height>= lineH - step_y/2 && height<lineH + step_y/2){
                        fprintf(save,"  #");
                    }
                    else{
                        fprintf(save,"   ");
                    }
                }
            
            fprintf(save,"\n");        
            }
    
        fprintf(save,"       ");
        int i ;
        for(i = 0 ; i<=maxTime ; i++){
            fprintf(save,"|--");
        }
        fprintf(save,"|");
        fprintf(save,"\n     ");
    
        for( i= 0 ; i<=maxTime+1 ; i++){
            fprintf(save,"%3d",i);
            }
            fprintf(save,"\n");

            fprintf(save,"Maximum altitude is : %.2lf meters\n",maxAltitude);
            fprintf(save,"Total flight duration  : %.2lf seconds \n",maxTime);

    printf("Maximum altitude is : %.2lf meters\n",maxAltitude);
    printf("Total flight duration : %.2lf seconds\n",maxTime);
            fclose(save);
            printf("Trajectory data saved to trajectory.txt !\n");
}

int main(){
    int choice;

    do{
    printf("-------------------------\n");
    printf("1.Enter launch parameters\n");
    printf("2.Simulate rocket launch\n");
    printf("3.Save trajectory data\n");
    printf("4.Exit\n");
    printf("Choice : ");
    scanf("%d",&choice);
    switch(choice){
    
        case 1 : launchParameters() ; break ;
        case 2 : simulateRocket(); break ;
        case 3 : saveData() ; break ;
        case 4 : printf("Exiting...\n"); break ;
        default : printf("Invalid choice !\n"); while(getchar()!= '\n') ; break ;
    }
    }while(choice!= 4);
    return 0 ;

}
