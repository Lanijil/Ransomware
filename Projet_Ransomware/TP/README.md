# 📁 Structure du TP Jour 5

Ce dossier contient tous les fichiers nécessaires pour développer le ransomware éducatif.

## 📂 Fichiers fournis

### ✅ Headers (`.h`) - NE PAS MODIFIER
Déclarations des fonctions à implémenter :
- `scanner.h` - MODULE 1 : Scanner de fichiers
- `xor_crypto.h` - MODULE 2 : Chiffrement XOR
- `caesar_crypto.h` - MODULE 3 : Chiffrement César
- `rot13_crypto.h` - MODULE 4 : ROT13
- `checksum.h` - MODULE 5 : Vérification d'intégrité
- `timer.h` - MODULE 6 : Gestion du temps
- `config.h` - MODULE 7 : Parser de configuration
- `c2_client.h` - MODULE 8 : Client C2

### ✅ Programme principal - NE PAS MODIFIER
- `ransomware.c` - Menu interactif qui teste vos implémentations

### ✅ Fichier de travail - À COMPLÉTER
- `modules.c` - **C'EST ICI QUE VOUS CODEZ !** Contient toutes les fonctions à implémenter

### ✅ Configuration
- `config.txt` - Configuration de sécurité (whitelist/blacklist)
- `Makefile` - Compilation automatique (optionnel)

---

## 🎯 Comment travailler ?

### 📝 Étape 1 : Allez dans /vagrant/TP et ouvrir le fichier modules.c

```bash
cd /vagrant/TP
nano modules.c
```

Ce fichier contient **8 sections** (une par module) avec des commentaires `// TODO` pour chaque fonction.

**Structure du fichier :**
```c
// ============ MODULE 1 : SCANNER ============
int scan_directory(...) {
    // TODO : Implémenter selon MODULE 1 du TP_Jour5.md
}

// ============ MODULE 2 : XOR ============
void xor_buffer(...) {
    // TODO : Implémenter selon MODULE 2 du TP_Jour5.md
}

// ... 8 modules au total ...
```

### 📖 Étape 2 : Implémenter module par module

1. **Lire les instructions** dans `TP_Jour5.md` pour le MODULE 1
2. **Compléter les fonctions** dans `modules.c` (MODULE 1)
3. **Compiler** pour vérifier qu'il n'y a pas d'erreurs
4. **Tester** via le menu du ransomware
5. **Passer au MODULE 2** et répéter

### ⚙️ Étape 3 : Compiler

**Commande de compilation :**
```bash
gcc -Wall -Wextra -I. -o ransomware ransomware.c modules.c
```

**Explication des flags :**
- `-Wall -Wextra` : Affiche tous les warnings (erreurs potentielles)
- `-I.` : Cherche les fichiers `.h` dans le dossier courant
- `-o ransomware` : Nom du programme final
- `ransomware.c modules.c` : Les deux fichiers à compiler ensemble

**Alternative avec Makefile :**
```bash
make
```

### 🧪 Étape 4 : Tester

```bash
./ransomware
```

Un menu s'affiche avec 8 options correspondant aux 8 modules :
```
=== MENU PRINCIPAL ===
1. Scanner un répertoire    ← MODULE 1
2. Chiffrer XOR              ← MODULE 2
3. Chiffrer César            ← MODULE 3
4. Chiffrer ROT13            ← MODULE 4
5. Calculer checksum         ← MODULE 5
6. Tester timer              ← MODULE 6
7. Charger config            ← MODULE 7
8. Tester C2 client          ← MODULE 8
0. Quitter
```

**Testez chaque module dès qu'il est implémenté !**

---

## ⚠️ Erreurs de compilation courantes

### ❌ Erreur : "undefined reference to fonction"
```
/usr/bin/ld: /tmp/ccXXXX.o: undefined reference to 'scan_directory'
```

**Cause** : Vous avez oublié d'inclure `modules.c` dans la compilation

**Solution** :
```bash
gcc -I. -o ransomware ransomware.c modules.c
#                                  ^^^^^^^^^^  Ne pas oublier !
```

### ❌ Erreur : "No such file or directory" pour un `.h`
```
fatal error: scanner.h: No such file or directory
```

**Cause** : Le flag `-I.` est manquant

**Solution** :
```bash
gcc -I. -o ransomware ransomware.c modules.c
#    ^^^  Important !
```

### ❌ Erreur : "expected ';' before '}' token"
```
modules.c:45:1: error: expected ';' before '}' token
```

**Cause** : Oubli de `;` à la fin d'une instruction

**Solution** : Vérifiez chaque ligne dans votre implémentation

### ❌ Erreur : Segmentation fault lors de l'exécution
```
Segmentation fault (core dumped)
```

**Causes possibles** :
- Accès à un pointeur NULL : `if (ptr == NULL) return;`
- Dépassement de tableau : Vérifiez les indices
- Oubli de `malloc()` : N'oubliez pas d'allouer la mémoire

**Débogage** :
```bash
gdb ./ransomware
(gdb) run
# Quand ça crash :
(gdb) backtrace
```

---

## 💡 Conseils pratiques

### ✅ Workflow recommandé
```bash
# 1. Ouvrir le fichier
nano modules.c

# 2. Implémenter UN module (ex: MODULE 1)
# 3. Sauvegarder (Ctrl+O, Entrée, Ctrl+X)

# 4. Compiler
gcc -Wall -Wextra -I. -o ransomware ransomware.c modules.c

# 5. Si erreurs : corriger et recompiler
# 6. Si OK : tester
./ransomware

# 7. Utiliser l'option du menu correspondante
# 8. Répéter pour le module suivant
```

### ✅ Créer des fichiers de test
```bash
# Pour tester le chiffrement
echo "Hello World" > test.txt

# Pour tester le scanner
mkdir -p test_dir/subdir
touch test_dir/file1.txt test_dir/file2.pdf test_dir/subdir/file3.doc

# Pour tester la config
cat config.txt  # Vérifier le contenu
```

### ✅ Compiler avec warnings détaillés
```bash
gcc -Wall -Wextra -Wpedantic -I. -o ransomware ransomware.c modules.c
```

Corrigez **tous les warnings** ! Ils indiquent souvent des bugs futurs.

### ⚠️ Ne PAS modifier ces fichiers
- ❌ Tous les `.h` (headers)
- ❌ `ransomware.c` (programme principal)
- ❌ `config.txt` (configuration de sécurité)

**Modifiez UNIQUEMENT `modules.c` !**

---

## 🆘 Besoin d'aide ?

### 1. Le programme ne compile pas
```bash
# Vérifiez que vous êtes dans le bon dossier
pwd
# Doit afficher : .../Distribution_Jour5/TP

# Vérifiez que modules.c existe
ls -l modules.c

# Essayez la compilation de base
gcc -I. -o ransomware ransomware.c modules.c
```

### 2. Le programme compile mais plante au lancement
- Vérifiez que vous avez implémenté le MODULE 1 au minimum
- Les fonctions non implémentées peuvent retourner des valeurs par défaut
- Utilisez `printf()` pour déboguer : `printf("DEBUG: Entrée dans fonction\n");`

### 3. Un module ne fonctionne pas comme attendu
- Relisez les instructions du module dans `TP_Jour5.md`
- Vérifiez les types de retour (int, void, char*, etc.)
- Testez avec des valeurs simples d'abord
- Comparez avec l'aide-mémoire du module dans `TP_Jour5.md`

### 4. Comment savoir si mon code est correct ?
Le menu du `ransomware` teste directement vos fonctions :
- Option 1 doit afficher les fichiers trouvés
- Option 2 doit créer un fichier chiffré
- Option 3-4-5 doivent transformer les fichiers
- Si le résultat est correct → Code validé ✓

---

## 🎯 Validation finale

Avant de terminer, vérifiez que :
- ✅ Compilation sans erreurs ni warnings
- ✅ Les 8 options du menu fonctionnent
- ✅ Vous pouvez chiffrer puis déchiffrer un fichier
- ✅ Le scanner trouve les fichiers récursivement
- ✅ La config charge correctement `config.txt`

**Commande de validation complète :**
```bash
gcc -Wall -Wextra -I. -o ransomware ransomware.c modules.c && ./ransomware
```

Si cette commande réussit et que le menu s'affiche : **TP validé !** 🎉

---

## 📚 MODULE 9 : Serveur C2 (Avancé)

Le MODULE 9 nécessite un fichier séparé car c'est un programme indépendant.

**Créer le fichier :**
```bash
nano c2_server.c
```

**Compiler avec threads :**
```bash
gcc -Wall -Wextra -I. -pthread -o c2_server c2_server.c
```

**Lancer dans un terminal séparé :**
```bash
./c2_server
```

**Tester depuis le ransomware :**
```bash
./ransomware
# Choisir option 8 (Client C2)
```

---

Bon courage ! 🚀
