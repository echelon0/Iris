
int StrLen(char *string) {
    int count = 0;
    while(*string) {
        count++;
        *string++;
    }
    return count;
}

void StrCopy(char *dest, char *source) {
    while(*source) {
        *dest++ = *source++;
    }
    *dest = '\0';
}

void StrCat(char *dest, char *source) {
    while(*dest) {
        *dest++;
    }
    while(*source) {
        *dest++ = *source++;
    }
    *dest = '\0';
}
