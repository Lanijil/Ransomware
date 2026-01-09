# Ransomware

## Les 8 modules (ordre TP_Jour5.md)

| # | Module | Fonctionnalité | Test menu | Statut |
|---|--------|----------------|-----------|--------|
| 1 | **Scanner** | Parcours récursif fichiers | Option 1 |
| 2 | **XOR Crypto** | Chiffrement symétrique | Option 2 |
| 3 | **César Crypto** | Décalage alphabétique | Option 3 |
| 4 | **ROT13** | César shift=13 (auto-réversible) | Option 4 |
| 5 | **CRC32** | Vérification intégrité | Option 5 |
| 6 | **Timer** | Délais humains | Option 6 |
| 7 | **Config** | Whitelist/Blacklist | Option 7 |
| 8 | **C2 Client** | Communication serveur | Option 8 |

## Utilisation rapide

```bash
# Dans la VM
cd vagrantTP
bash ../setupsandbox.sh    # Génère 50 fichiers test
make clean && make         # gcc -I. -o ransomware
./ransomware               # Menu interactif
```

1. Scanner → testfiles/ (7 fichiers)
2. XOR → secret.txt → secret.enc → secret.dec
3. César → rapport.txt (shift=3)
4. ROT13 → notes.md (2x = original)
5. CRC32 → checksum fichier
6. Timer → sleep(3)
7. Config → config.txt
8. C2 → 127.0.0.1:4444

gcc -Wall -Wextra -I. -o ransomware ransomware.c modules.c

## Objectif final

./ransomware 1 → sandbox/     # Scanner 50 fichiers
./ransomware 2 → encrypt      # Ransomware complet .locked
./ransomware 2 → decrypt      # Restaure tout

## Sécurité

✅ VM isolée (host-only)
✅ UNIQUEMENT testfiles/ ou sandbox/
✅ PAS d'accès internet
✅ Ne jamais tester sur vrais fichiers
