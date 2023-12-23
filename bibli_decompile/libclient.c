//
// Created by arnau on 22/12/2023.
//
long long sndmsg(char *a0, unsigned long a1)
{
    unsigned int v0;  // [bp-0x430]
    unsigned int v1;  // [bp-0x42c]
    unsigned short v2;  // [bp-0x428]
    unsigned short v3;  // [bp-0x426]
    char v4;  // [bp-0x424]
    char v5;  // [bp-0x418]
    char v6;  // [bp-0x18]

    strncpy(&v5, a0, 0x400);
    v6 = 0;
    v0 = socket(2, 1, 0);
    if (v0 < 0)
    {
        puts("\n Socket creation error ");
        return 4294967295;
    }
    v2 = 2;
    v3 = htons(1);
    if (inet_pton(2, "127.0.0.1", &v4) <= 0)
    {
        puts("\nInvalid address/ Address not supported ");
        return 4294967295;
    }
    v1 = connect(v0, &v2, 16);
    if (v1 < 0)
    {
        puts("\nConnection Failed ");
        return 4294967295;
    }
    send(v0, "1025", 5, 0);
    send(v0, &v5, strlen(&v5), 0);
    close(v0);
    return 0;
}