#include<stdio.h>


//Function that calculates turnaround time,waiting time,average waiting time and average turnaround time
int time(int p[],int bt[],int wt[],int n){
	int i,j,t,tat[4];
	float awt=0,atat=0;
	//tat stores turnaround time while wt stores waiting time. awt and atat are average waiting time and average turnaround time.
	//bt stores burst time.
	printf("process  burst_time  waiting_time turn_around_time \n");
	
        for(i=0;i<n;i++)
        {
        wt[i]=0;
        tat[i]=0;
        for(j=0;j<i;j++){
        	wt[i]=wt[i]+bt[j];}
        tat[i]=tat[i]+bt[i];
        awt=awt+wt[i];
        atat=atat+tat[i];
        //This line prints process no., burst time, waiting time and turnaround time.
        printf("%d \t %d \t     %d \t          %d \n",p[i],bt[i],wt[i],tat[i]);
        
        }
        
        awt=awt/n;
        atat=atat/n;
        printf("Average waiting time=%f\n",awt);
        printf("Average turnaround time=%f\n",atat);
        //getchar();
        }
	
void main(){
	int i,j,n,t;
	float awt=0,atat=0;
	//qt is quantum time used in round robin.
	int qt=2;
        //n value stores no. of processes.
        //bt stores burst times of processes.		
	int s;	
	printf("Enter no of processes:");
	scanf("%d",&n);
	int p[n],bt[n],wt[n],tat[n],pr[n];
	for(i=0;i<n;i++){
	  p[i]=i+1;}
	for(i=0;i<n;i++){
	printf("Enter burst time of process no.%d:",i+1);	
	scanf("%d",&bt[i]);}
	
	printf("Enter choice, 1 for sjp, 2 for fcfs,3 for priority scheduling,4 for round robin:");
	scanf("%d",&s);
	if (s==1){  //sjp is selected.
	// applying bubblesort technique to sort according to their burst time.
	for(i=0;i<n;i++)
        {
        
        for(j=0;j<n-i-1;j++)
        {
            if(bt[j]>bt[j+1]){  
            t=bt[j];
            bt[j]=bt[j+1];
            bt[j+1]=t;
        
  
            t=p[j];
            p[j]=p[j+1];
            p[j+1]=t;
        }}}        
        // variable are send to function to print time quantities.  
        time(p,bt,wt,n);
        }
        
        
        else if(s==2){ //first come first serve is selected.
        time(p,bt,wt,n);}  //variable are send to function to print time quantities.
        
        
        else if(s==3){ //priority scheduling is required.
        int pos;
        //priority values are inputed from the user.
        
        for(i=0;i<n;i++){
          printf("Enter priority of processes no. %d:",i+1);
	  scanf("%d",&pr[i]);}
	 // processes are sorted according to their priority. Bubblesort technique is used.
        for(i=0;i<n;i++)
        {
        pos=i;        
        for(j=i+1;j<n;j++)
        {
         if(pr[j]<pr[pos]){
         	pos=j;}
        }
        //processes are sorted.
        t=pr[i];
        pr[i]=pr[pos];
        pr[pos]=t;
        //burst time is sorted.
         t=bt[i];
        bt[i]=bt[pos];
        bt[pos]=t;
        }
        wt[0]=0;
        time(p,bt,wt,n);  //Time quantities are calculated.
         } 
        
        
        else if(s==4){  //round robin is required.
        int temp,count,r[4],sq=0;
        count=0;
        // temp=temporary variable.
        //printf("Enter quantum time:");
        //scanf("%d",&qt);
        for(i=0;i<n;i++){
          r[i]=bt[i];}
        //r variable denotes remaining burst time.
        while(1){
        for(i=0;i<n;i++){
        	temp=qt;
        	if(r[i]==0){
        		count++;
        		continue;}
        	//if remaining burst time< quantum number, reduce quantum no value from remaing burst time.
        	if(r[i]>qt){ 
        		r[i]=r[i]-qt;}
        	else if(r[i]>=0){  
        		temp=r[i];
        		r[i]=0;
        		}
        	sq=sq+temp;
        	tat[i]=sq;
        }
        if(n==count){break;} //If all processes are executed, break from the while loop.
        }
        //Next section prints waiting time and turnaround time.
        printf("process  burst_time  waiting_time turn_around_time \n");
	
        for(i=0;i<n;i++)
        {
        wt[i]=tat[i]-bt[i];
        awt=awt+wt[i];
        atat=atat+tat[i];
        printf("%d \t %d \t     %d \t          %d \n",p[i],bt[i],wt[i],tat[i]);
        }
        
        awt=awt/n;
        atat=atat/n;
        printf("Average waiting time=%f\n",awt);
        printf("Average turnaround time=%f\n",atat);
        }
        	
        	
        	
        
        	
        
        
        
}
        
        
  
