//
// Created by arnau on 22/12/2023.
//
extern unsigned short address;
extern unsigned short g_4040b2;
extern unsigned int g_4040b4;
extern unsigned int server_fd;

long long startserver(unsigned long a0)
{
    unsigned int v0;  // [bp-0xc]
    unsigned long long v2;  // rax

    v0 = 1;
    server_fd = socket(2, 1, 0);
    if (server_fd < 0)
    {
        perror("socket failed");
        exit(1); /* do not return */
    }
    else if (setsockopt(server_fd, 1, 15, &v0, 4))
    {
        perror("setsockopt");
        exit(1); /* do not return */
    }
    else
    {
        address = 2;
        g_4040b4 = 0;
        g_4040b2 = htons((unsigned short)(server_fd >> 16) CONCAT (unsigned short)server_fd);
        if (bind(server_fd, &address, 16) < 0)
        {
            perror("bind failed");
            exit(1); /* do not return */
        }
        (unsigned int)v2 = listen(server_fd, 3);
        if ((unsigned int)v2 < 0)
        {
            perror("listen");
            exit(1); /* do not return */
        }
        return v2;
    }
}

extern void address;
extern unsigned int addrlen;
extern unsigned int new_socket;
extern unsigned int server_fd;
extern unsigned long long valread;

long long getmsg(char *a0)
{
    char *v0;  // [bp-0x420]
    void* v1;  // [bp-0x418]
    void* v2;  // [bp-0x410]
    char v3;  // [bp-0x408], Other Possible Types: void*
    unsigned int v4;  // [bp-0xc]
    unsigned long long v6;  // rcx
    void* v7;  // rdi
    unsigned long v8;  // d

    v0 = a0;
    v1 = 0;
    v2 = 0;
    v6 = 126;
    for (v7 = &v3; v6; v7 += v8 * 8)
    {
        v6 -= 1;
        v3 = 0;
    }
    *((char *)v7) = 0;
    new_socket = accept(server_fd, &address, &addrlen);
    if (new_socket >= 0)
    {
        valread = read(new_socket, &v1, 5);
        v4 = atoi(&v1);
        valread = read(new_socket, &v1, v4);
        close(new_socket);
        strcpy(v0, &v1);
        return 0;
    }
    perror("accept");
    exit(1); /* do not return */
}

extern unsigned int server_fd;

long long stopserver()
{
    close(server_fd);
    return 0;
}