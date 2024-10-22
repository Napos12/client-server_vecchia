#include "functioncs.c"




char server_port[100];
char server_address[100];
char root_directory[100];
msg messaggio;

void check_arg_s(int argc, char* argv[])
{
    if(argc<7)
    {
        printf("argomenti mancanti\n");
        exit(-1);    
    }
    else
    {
        if(strcmp(argv[1],"-a")!=0 || strcmp(argv[3],"-p")!=0 || strcmp(argv[5],"-d")!=0 )
        {
            printf("Utilizzare formato: MyFTserver -a server_address -p server_port -d ft_root_directory\n");
            exit(-1);
        }
        else
        {
            
            strcpy(server_address,argv[2]);
            strcpy(server_port,argv[4]);
            strcpy(root_directory,argv[6]);
            
        }
    }
}


void handle_client(void *sockfd) 
{

    while(1)
    {
        printf("vado\n");
    }


   /* printf("vamos\n");
    int new_socket = *(int*)sockfd;
    
    printf("dale papi\n");
    if(recv(new_socket,&messaggio,sizeof(messaggio),0)<0)
    {
        perror("read error");
        exit(-1);
    }
    printf("messaggio: %d,%s,%s",messaggio.mode,messaggio.path,messaggio.file_name);
            
    send(new_socket,"daje forte",BUFFER_SIZE,0);

    recv(new_socket,NULL,BUFFER_SIZE,0);
    // socket del client
    
    // Variabile utilizzata per salvare i messaggi ricevuti dal client
    char buffer[BUFFER_SIZE];
    int bytes_read;
    
    FILE* file;
    FILE* fp;
    if(messaggio.mode==0)
    {

        char* path = strdup(root_directory);
        sprintf(path,"%s%s%s",path,messaggio.path,messaggio.file_name);
        file = fopen(path, "r");
        while ((bytes_read=fread(buffer,1,sizeof(buffer),file))>0) {
        if (send(new_socket, buffer, sizeof(buffer), 0) == -1) {
            perror("Errore nell'invio del file.");
            exit(1);
        }
        memset(buffer, 0, BUFFER_SIZE);
        close(file);
    }
    }
*/
}

int main(int argc,char* argv[])
{
    int sock,portnu;
    struct sockaddr_in server,client;
    int length;
    int current_sock;
    int opt=1;
    
    msg messaggio;

    check_arg_s(argc,argv);
    portnu=atoi(server_port);
    if(check_directory(root_directory)==0) mkdir(root_directory,0755);

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket error");
        exit(-1); 
    }

    server.sin_family= AF_INET;
    server.sin_port=htons(portnu);
    if(inet_pton(AF_INET, server_address, &server.sin_addr) <= 0) perror("Errore nella conversione dell'indirizzo IP");
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) perror("setsockopt");
    if(bind(sock,(struct sockaddr *)&server,sizeof(server))<0)
    {
        perror("bind error");
        exit(-1);
    }

    if(listen(sock,5)<0)
    {
        perror("listen error");
        exit(-1);
    }
    printf("Server attivo alla porta %d, attendo connessioni...\n", portnu);
    length=sizeof(client);
    pthread_t client_thread;
    while(1)
    {
        printf("DAJe lachi\n");
        current_sock=accept(sock,(struct sockaddr *)&client, (socklen_t *)&length);

        if(current_sock<0)
        {
            perror("accept error");
            exit(-1);
        }
        if(recv(current_sock,&messaggio,sizeof(messaggio),0)<0)
        {
            perror("read error");
            exit(-1);
        }
        printf("messaggio: %d,%s,%s",messaggio.mode,messaggio.path,messaggio.file_name);
        
        if (pthread_create(&client_thread, NULL, handle_client, (void*) current_sock) < 0) {
            perror("Thread creation error");
            close(current_sock);
            //free(client);
            
        }

        
        //pthread_detach(client_thread);
        /*int pid=fork();

        if(pid<0)
        {
            perror("fork error");
            exit(-1);        
        }
        if(pid==0)
        {
            if(read(current_sock,&messaggio,sizeof(messaggio))<0)
            {
                perror("read error");
                exit(-1);
            }
            printf("messaggio: %d,%s,%s",messaggio.mode,messaggio.path,messaggio.file_name);
            
            send(current_sock,"daje forte",BUFFER_SIZE,0);

            handle_client(current_sock,messaggio);

            close(current_sock);
            exit(0);

        }
        else if(pid>0)
        {
            printf("sono il pid porco de dio\n");
            close(current_sock);
        }*/
    }

}