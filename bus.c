// rana's code ...
#include "spl.h"
// Bus Get Function .......................


int search()
{	
	FILE* right;
	FILE* left;
	int dir=2;
	int testpid;
	
	right= fopen("right.txt","r");
	while(!feof(right))
	{
		fscanf(right,"%d",&testpid);
		if(testpid==getpid())
		{
			dir=0;
			break;
		}
	}
	fclose(right);
	
	left= fopen("left.txt","r");
	while(!feof(left))
	{
		fscanf(left,"%d",&testpid);
		if(testpid==getpid())
		{
			dir=1;
			break;
		}
	}
	fclose(left);
	return dir;
}
int busget(key_t k,int flag)
{	
printf("id = %d",getpid());
        int busid;
        bool itexist=false; 
	FILE* fptr;
	fptr = fopen("table.txt", "a+");
	if(fptr == NULL)
	{printf("first open\n");
	 return -1;}
	key_t tempkey;
	int tempid;
	  
        while(!feof(fptr))
        {
                fscanf(fptr,"%i",&tempkey);  
                        
                if(tempkey==k)
                {
                printf("inside if \n");
                itexist=true;
                //return id for shared memory 
               
               break; 
                }
               
         }
         fclose(fptr);
          
         
              if(flag==0)
              {
		      if(itexist==true)
		      {
		      printf("inside flag=0 it exist \n");
		      busid=shmget(k,sizeof(struct bus ),IPC_CREAT|0777);
		     // struct bus *tptr = (struct bus *)shmat(busid,0,0);
		      return busid;
		      
		      }
		      
		      else 
		      {
		      printf("bus doesn't exist \n");
		      return -1;
		      
		      }
              
              }
              
            else
              {
              	if(itexist==true)
		      {
                     printf("inside flag!=0 it exist \n");		     
		     busid = shmget(k, sizeof(struct bus), 0);
	              //struct bus *ptr = (struct bus *) shmat(busid,0,0);
		      return busid;
		      
		      }
		      
		      else 
		      {
	                printf("inside flag !=0 will create it  \n");	      
			busid =shmget(k,sizeof(struct bus ),IPC_CREAT|0666);
			// int tableid = shmget(TABLE_KEY,sizeof(struct table),IPC_CREAT|0666);
			//struct bus*b=shmat(busid,(struct bus *)0,0);
			fptr = fopen("table.txt", "a+");
			if(fptr == NULL) 
			{
				printf("inside\n");
				return -1;				
			}

		
			fprintf(fptr,"%i\n",k);
			
			 fclose(fptr);
			 
			return busid;
		      
		      }
              
            
                          
              
              }
              
                       


	
}



// Bus Attache Function ..................

int busattach(int busid , int loc )
{      

         struct bus* temp;
         
        // right direction ... 
        if(loc==0)
        {
                        FILE *rptr;
			rptr=fopen("right.txt","a+");
			if(rptr==NULL)
			{
			printf("can't open the file to enter to the right  direction \n");
			return -1;
			}
			mkfifo("R_pipe", 0666);
			temp=shmat(busid,(struct bus*)0,0);
			if((struct bus*)0==(struct bus*)-1)
			{
		
			printf("attachement of the process unsuccessfull on the right direction \n");
			return -1;
			}
			
			//while(!feof(rptr));
			fprintf(rptr,"%d \n",getpid());
			temp->index=0;
			temp->pid=getpid();
			printf("the proccess in the right direction \n");
			printf("attached sucessfully \n");
			
			fclose(rptr);
			
       
       
       
	}
	// left direction ... 
	 else
	 {
	                FILE* lptr;
			lptr=fopen("left.txt","a+");
			if(lptr==NULL)
			{
			printf("can't open the file to enter to the left  direction \n");
			return -1;
			}
			mkfifo("L_pipe", 0666);
			temp=(struct bus*)shmat(busid,(struct bus*)0,0);
			if((struct bus*)0==(struct bus*)-1)
			{
		
			printf("attachement of the process unsuccessfull on the left direction \n");
			return -1;
			}
			
			//while(!feof(rptr));
			fprintf(lptr,"%d \n",getpid());
			temp->index=0;
			temp->pid=getpid();
			printf("the proccess in the left direction \n");
			printf("attached sucessfully \n");
			
			fclose(lptr);
			       

         }
         
         return 0;


}

// Write  Function ..........................
int buswrite (int busid , struct busmessage bmsg )
{

                        			
	int r,l;
		printf("before search\n");
	int dir=search();
	printf("%d\n",dir);
	if (dir==0)
	{
		r = open("R_pipe", O_WRONLY);
		if (r==-1)
		{
			printf("cant open the pipe !\n");
		}
		/* Write to the pipe */
		write(r, bmsg.data, strlen(bmsg.data)+1);
		close(r);
		
		
 
		
	}
	else if(dir==1)
	{

		l = open("L_pipe", O_WRONLY);
		/* Write to the pipe */
				if (l==-1)
		{
			printf("cant open the pipe !\n");
		}
		write(l, bmsg.data, strlen(bmsg.data)+1);
		close(l);
		
		//return val2;
	}
	else
	{
		printf("This proccess dose not attach to this bus !! \n");
		return -1;
	}
	
	return 0;

			

}


int busread(int busid , struct busmessage bmsg , int flag)
{


	
	int r,l;

	int dir=search();
	printf("%d\n",dir);
	char buf[200];

	if (dir==0)
	{
		r = open("R_pipe", O_RDONLY);
		/* Read from the pipe */
		int numread = read(r, buf, 200);
		buf[numread] = 0;
		printf("P1 read from the pipe: %s", buf);

		close(r);
 
		
	}
	else if(dir==1)
	{
		l = open("L_pipe", O_RDONLY);
		/* Read from the pipe */
		int numread = read(l, buf, 200);
		buf[numread] = 0;
		printf("P1 read from the pipe: %s\n", buf);

		close(r);
	}
	else
	{
		printf("This proccess dose not attach to this bus !! \n");
		return -1;
	}
	
	
	
	return 0;
	
			
			 

}


int detach(int busid)
{
 	
         	int di=search();
         	if (di==0)
         	{
	
         	}
         	else
         	{
         	/*int var1=busid;
         	int var2=0;
       	    system ("sed -i's/$var1/var2/g' $left.txt");*/

         	}
         
}
int busclose (int busid)
{
                        struct bus* clp;
                        clp=(struct bus*)shmat(busid,(struct bus*)0,0);
                        if((struct bus*)0==(struct bus*)-1)
			{
		
			printf("unsuccessful attachement\n");
			return -1;
			}
			shmdt(clp);
			return 0;
			
			


}


