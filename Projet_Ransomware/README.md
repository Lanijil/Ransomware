# TP Jour 5 : Ransomware Éducatif

## 📦 Contenu du Package

Ce dossier contient tout le nécessaire pour réaliser le TP Jour 5 sur le développement d'un ransomware éducatif.

### Structure

```
Distribution_Jour5_Pour_Etudiants/
├── TP_Jour5.md           # Instructions complètes du TP
├── Vagrantfile           # Configuration de la VM isolée
├── setup_sandbox.sh      # Script de génération des fichiers de test
├── README.md             # Ce fichier
└── TP/
    ├── modules.c         # Fichier à compléter (23 TODO)
    ├── ransomware.c      # Programme principal (fourni)
    ├── Makefile          # Compilation automatique
    ├── config.txt        # Fichier de configuration de test
    ├── *.h               # Headers des 8 modules
    └── test_files/       # 7 fichiers de test pour démarrer rapidement
```

## 🚀 Démarrage Rapide

### 1. Lancer la VM Vagrant

```bash
cd Distribution_Jour5_Pour_Etudiants
vagrant up
vagrant ssh
```

### 2. Dans la VM, travailler dans /vagrant/TP

```bash
cd /vagrant/TP
make                    # Compiler
./ransomware            # Tester manuellement
```

### 3. Développer les modules

Ouvrir `modules.c` et compléter les **23 TODO** en suivant les instructions du fichier [TP_Jour5.md](TP_Jour5.md).

## 📚 Documentation

Consultez [TP_Jour5.md](TP_Jour5.md) pour :
- Les objectifs pédagogiques
- Les instructions détaillées de chaque module
- Les exemples de code
- Les critères de validation

## 🎯 Modules à Implémenter (8 modules)

1. **Scanner** - Parcours récursif de fichiers
2. **XOR Crypto** - Chiffrement XOR avec clé
3. **Caesar Crypto** - Chiffrement César
4. **ROT13 Crypto** - Chiffrement ROT13
5. **Checksum** - Calcul CRC32
6. **Timer** - Gestion du temps
7. **Config** - Parseur de configuration INI
8. **C2 Client** - Client Command & Control

## 🧪 Tests Rapides

### Fichiers de test fournis (test_files/)

7 fichiers prêts à utiliser pour tester rapidement :
- `test_files/documents/` : rapport.txt, notes.md, secret.txt
- `test_files/logs/` : application.log, errors.log
- `test_files/.hidden/` : config.conf

### Sandbox complet (~50 fichiers)

Pour des tests plus complets, générer le sandbox :

```bash
cd /vagrant/TP
bash ../setup_sandbox.sh
# Génère ~50 fichiers dans TP/sandbox/
```

## 🛡️ Sécurité

⚠️ **RAPPEL IMPORTANT** : Ce TP est strictement éducatif

- La VM est isolée du réseau
- Travaillez UNIQUEMENT dans `/vagrant/TP/test_files/` ou `/vagrant/TP/sandbox/`
- Ne testez JAMAIS sur de vrais fichiers
- Le code est destiné à l'apprentissage uniquement

## ❓ Aide

- **Compilation échoue** : Vérifiez les TODO non complétés avec `grep -n "TODO" modules.c`
- **Tests échouent** : Relisez les instructions du module dans TP_Jour5.md
- **VM ne démarre pas** : Vérifiez VirtualBox et les logs avec `vagrant up --debug`

## 📝 Commandes Utiles

```bash
# Vagrant
vagrant up              # Démarrer la VM
vagrant ssh             # Se connecter
vagrant halt            # Arrêter la VM
vagrant destroy         # Supprimer la VM
vagrant reload          # Redémarrer avec nouveau Vagrantfile

# Dans la VM
cd /vagrant/TP          # Aller dans le dossier TP
make clean && make      # Recompiler
./ransomware            # Lancer le programme
tree test_files/        # Voir la structure des fichiers de test
```

## 🎓 Objectifs d'Apprentissage

À la fin de ce TP, vous aurez appris :
- Manipulation de fichiers en C (dirent.h, stat)
- Algorithmes de chiffrement simples
- Programmation réseau (sockets TCP)
- Gestion de la mémoire dynamique
- Parsing de fichiers de configuration
- Architecture modulaire en C

Bon courage ! 🚀
