
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    struct addrinfo src = {0};
    src.ai_flags = AI_PASSIVE;
    src.ai_protocol = IPPROTO_TCP;
    src.ai_socktype = SOCK_STREAM;
    src.ai_family = AF_INET;
    
    struct addrinfo* res = NULL;
    if(getaddrinfo("0.0.0.0","8088",&src,&res)){
        goto end;
    }
    
    int s = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(s == -1){
        goto end;
    }
    
    if(bind(s,res->ai_addr,res->ai_addrlen)){
        goto end;
    }
    if(listen(s,SOMAXCONN)){
        goto end;
    }
    printf("listening\n");
    while(1){
        struct sockaddr cs = {0};
        socklen_t cslen = sizeof(struct sockaddr);
        char buffer[512] = {0};
        int c = accept(s,&cs,&cslen);
        if(c){
            recv(c,buffer,sizeof(buffer),c);
            printf("%s\n",buffer);
        }
    }
    
    
end:
    if(res){
        freeaddrinfo(res);
    }
    if(s!=-1){
        close(s);
    }
    return 0;
}
