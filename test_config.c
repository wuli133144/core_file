  

  #include <unistd.h>
  #include "limits.h"
  #include <stdio.h>
  #include <fcntl.h>
  #include <time.h>

  int main(){

     #if 0
      int nopenmax=sysconf(_SC_OPEN_MAX);
      if(nopenmax<0){
      	fprintf(stderr, "%s\n","sysconf error");
      	goto err;
      }
      
      printf("%d\n", nopenmax);

      #endif 
     
     int fd=open("./my.cf",O_RDWR|O_CREAT,0666);
     if(fd<0) {
     	/* code */
     	goto err;
     }
     else {
        
        printf("%s\n", "create file successZ!");
     	close(fd);
     	/* code */
     }
      
    do {
          
          printf("%s\n", "test  proc");
          sleep(1000);
       }while(1);
    
  	  return 0;

  	  err:
  	     return 1;
  }
