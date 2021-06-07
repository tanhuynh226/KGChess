#include <stdio.h>
#include <string.h>

int main()
{
    int separator;
    char RecvBuf[256];
    strncpy(RecvBuf, "register tanhuynh randompass", sizeof(RecvBuf)-1);
    char accountinfo[40];
    char username[20];
    char password[20];
    strncpy(accountinfo, &RecvBuf[9], sizeof(accountinfo)-1);
    separator = strcspn(accountinfo, " ");
    strncpy(username, accountinfo, separator);
    username[separator] = '\0';
    strncpy(password, &accountinfo[separator+1], sizeof(password)-1);
    printf("accountinfo: %s\n", accountinfo);
    printf("username: %s, password: %s", username, password);
}
