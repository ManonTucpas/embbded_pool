void itoa(int value, char* buffer, int base) {
    int i = 0;
    int isNegative = 0;
    
    // Gère le cas zéro
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    
    // Gère les négatifs en base 10
    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }
    
    // Convertit en inversant l'ordre
    while (value != 0) {
        int reste = value % base;
        buffer[i++] = (reste > 9) ? (reste - 10) + 'a' : reste + '0';
        value = value / base;
    }
    
    // Ajoute le signe négatif
    if (isNegative)
        buffer[i++] = '-';
    
    buffer[i] = '\0';
    
    // Inverse la chaîne
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}