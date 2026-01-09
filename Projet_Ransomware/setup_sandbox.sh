#!/bin/bash
# Setup du sandbox pour le TP Ransomware - Jour 5
# Ce script crée un environnement de test sécurisé avec ~50 fichiers

set -e

SANDBOX_DIR="sandbox"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "╔═══════════════════════════════════════════════════════╗"
echo "║   Setup Sandbox - TP Ransomware Jour 5              ║"
echo "╚═══════════════════════════════════════════════════════╝"
echo ""

# Vérifier si le sandbox existe déjà
if [ -d "$SANDBOX_DIR" ]; then
    # En mode non-interactif (pipe), supprimer automatiquement
    if [ -t 0 ]; then
        read -p "⚠️  Le dossier sandbox/ existe déjà. Le supprimer ? (o/N) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Oo]$ ]]; then
            rm -rf "$SANDBOX_DIR"
            echo "✓ Ancien sandbox supprimé"
        else
            echo "❌ Annulé. Supprimez manuellement le dossier sandbox/ avant de relancer."
            exit 1
        fi
    else
        # Mode automatique : lire la réponse depuis stdin
        read -r REPLY
        if [[ $REPLY =~ ^[Oo]$ ]]; then
            rm -rf "$SANDBOX_DIR"
            echo "✓ Ancien sandbox supprimé"
        else
            echo "❌ Annulé."
            exit 1
        fi
    fi
fi

# Créer la structure du sandbox
echo "[1/5] Création de la structure..."
mkdir -p "$SANDBOX_DIR"/{documents,images,archives,sensitive,logs,config}
mkdir -p "$SANDBOX_DIR"/documents/{reports,notes,drafts}
mkdir -p "$SANDBOX_DIR"/sensitive/{credentials,keys}
mkdir -p "$SANDBOX_DIR"/.hidden

echo "[2/6] Génération des fichiers documents (20 fichiers)..."
# Documents texte
for i in {1..10}; do
    cat > "$SANDBOX_DIR/documents/report_$i.txt" << EOF
RAPPORT CONFIDENTIEL #$i
Date: $(date +%Y-%m-%d)
Contenu: Données de test pour le TP ransomware
Status: En cours d'analyse

Ce fichier contient des informations de test.
Il sera utilisé pour démontrer le chiffrement.
Ligne de remplissage numéro $i
Fin du rapport.
EOF
done

# Notes markdown
for i in {1..5}; do
    cat > "$SANDBOX_DIR/documents/notes/note_$i.md" << EOF
# Note de Projet $i

## Objectifs
- Tester le scanner récursif
- Valider le chiffrement XOR
- Vérifier la checksum CRC32

## Résultats
Fichier de test numéro $i
EOF
done

# Drafts
for i in {1..5}; do
    echo "Brouillon $i - Texte de test $(date +%s)" > "$SANDBOX_DIR/documents/drafts/draft_$i.txt"
done

echo "[3/6] Génération des fichiers sensibles (15 fichiers)..."
# Fichiers "sensibles"
cat > "$SANDBOX_DIR/sensitive/credentials/passwords.txt" << EOF
# Base de données des accès (TEST)
admin:test123
user1:password
user2:qwerty
root:toor
EOF

cat > "$SANDBOX_DIR/sensitive/credentials/api_keys.txt" << EOF
API_KEY_TEST=abc123def456
SECRET_TOKEN=xyz789uvw012
DATABASE_PASSWORD=testdb2026
EOF

cat > "$SANDBOX_DIR/sensitive/keys/private_key.txt" << EOF
-----BEGIN TEST PRIVATE KEY-----
CECI EST UNE FAUSSE CLE PRIVEE POUR LE TP
NE JAMAIS UTILISER EN PRODUCTION
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMI
-----END TEST PRIVATE KEY-----
EOF

# Fichiers de configuration sensibles
for i in {1..10}; do
    cat > "$SANDBOX_DIR/sensitive/config_$i.conf" << EOF
[server]
host=192.168.$((RANDOM % 255)).$((RANDOM % 255))
port=$((4000 + i))
key=testkey$i
EOF
done

# Hidden files
echo "hidden_data_$RANDOM" > "$SANDBOX_DIR/.hidden/secret.txt"

# Fichiers de configuration
for i in {1..5}; do
    cat > "$SANDBOX_DIR/config/app_config_$i.ini" << EOF
[application]
name=TestApp$i
version=1.$i.0
debug=true
log_level=INFO

[database]
host=localhost
port=$((3306 + i))
username=testuser$i
database=testdb$i

[cache]
enabled=true
ttl=$((300 * i))
max_size=100MB
EOF
done

echo "[4/6] Génération des images (10 fichiers)..."
# Créer des vraies images BMP 100x100 sans dépendance externe
# Format BMP: header + données RGB

# Fonction pour créer un BMP simple
create_bmp() {
    local file=$1
    local r=$2
    local g=$3
    local b=$4
    local width=100
    local height=100
    local row_padded=$(( ((width * 3 + 3) / 4) * 4 ))
    local data_size=$((row_padded * height))
    local file_size=$((54 + data_size))
    
    # Header BMP (54 bytes)
    printf '\x42\x4D'  # BM signature
    printf "\\$(printf '%03o' $((file_size & 0xFF)))"
    printf "\\$(printf '%03o' $(((file_size >> 8) & 0xFF)))"
    printf "\\$(printf '%03o' $(((file_size >> 16) & 0xFF)))"
    printf "\\$(printf '%03o' $(((file_size >> 24) & 0xFF)))"
    printf '\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00'
    printf '\x64\x00\x00\x00\x64\x00\x00\x00\x01\x00\x18\x00'
    printf '\x00\x00\x00\x00'
    printf "\\$(printf '%03o' $((data_size & 0xFF)))"
    printf "\\$(printf '%03o' $(((data_size >> 8) & 0xFF)))"
    printf "\\$(printf '%03o' $(((data_size >> 16) & 0xFF)))"
    printf "\\$(printf '%03o' $(((data_size >> 24) & 0xFF)))"
    printf '\x13\x0B\x00\x00\x13\x0B\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
    
    # Données pixels (BGR format)
    for ((y=0; y<height; y++)); do
        for ((x=0; x<width; x++)); do
            printf "\\$(printf '%03o' $b)"
            printf "\\$(printf '%03o' $g)"
            printf "\\$(printf '%03o' $r)"
        done
        # Padding pour aligner sur 4 bytes
        for ((p=width*3; p<row_padded; p++)); do
            printf '\x00'
        done
    done
}

# Créer 5 images BMP de couleurs différentes
create_bmp "$SANDBOX_DIR/images/image_1.bmp" 255 0 0    # Rouge
create_bmp "$SANDBOX_DIR/images/image_2.bmp" 0 0 255    # Bleu
create_bmp "$SANDBOX_DIR/images/image_3.bmp" 0 255 0    # Vert
create_bmp "$SANDBOX_DIR/images/image_4.bmp" 255 255 0  # Jaune
create_bmp "$SANDBOX_DIR/images/image_5.bmp" 255 0 255  # Magenta

# Créer 5 images PPM (format texte, toujours visualisable)
for i in {1..5}; do
    cat > "$SANDBOX_DIR/images/photo_$i.ppm" << EOF
P3
50 50
255
EOF
    # Générer un pattern de couleurs
    for ((y=0; y<50; y++)); do
        for ((x=0; x<50; x++)); do
            r=$(( (x * 5 + i * 20) % 256 ))
            g=$(( (y * 5 + i * 30) % 256 ))
            b=$(( ((x + y) * 3 + i * 40) % 256 ))
            echo "$r $g $b"
        done
    done >> "$SANDBOX_DIR/images/photo_$i.ppm"
done

echo "[5/6] Génération des logs (10 fichiers)..."
# Logs
for i in {1..10}; do
    cat > "$SANDBOX_DIR/logs/app_$i.log" << EOF
[$(date +%Y-%m-%d\ %H:%M:%S)] INFO: Application $i démarrée
[$(date +%Y-%m-%d\ %H:%M:%S)] DEBUG: Module scanner chargé
[$(date +%Y-%m-%d\ %H:%M:%S)] INFO: Scan terminé: $((RANDOM % 100)) fichiers
[$(date +%Y-%m-%d\ %H:%M:%S)] WARNING: Fichier .git ignoré
[$(date +%Y-%m-%d\ %H:%M:%S)] SUCCESS: Opération complétée
EOF
done

echo "[6/6] Génération des archives (5 fichiers)..."
# Créer quelques fichiers zip/tar (simulation)
for i in {1..5}; do
    echo "Archive de test $i - contenu binaire simulé: $(openssl rand -hex 100)" > "$SANDBOX_DIR/archives/backup_$i.zip"
done

# Fichier .gitignore pour éviter de commiter le sandbox
cat > "$SANDBOX_DIR/.gitignore" << EOF
# Ignorer tout le contenu du sandbox
*
!.gitignore
EOF

echo ""
echo "╔═══════════════════════════════════════════════════════╗"
echo "║              ✓ SANDBOX CRÉÉ AVEC SUCCÈS             ║"
echo "╚═══════════════════════════════════════════════════════╝"
echo ""

# Statistiques
TOTAL_FILES=$(find "$SANDBOX_DIR" -type f | wc -l)
TOTAL_DIRS=$(find "$SANDBOX_DIR" -type d | wc -l)
TOTAL_SIZE=$(du -sh "$SANDBOX_DIR" | cut -f1)

echo "📊 Statistiques:"
echo "   • Fichiers créés: $TOTAL_FILES"
echo "   • Dossiers: $TOTAL_DIRS"
echo "   • Taille totale: $TOTAL_SIZE"
echo ""
echo "📁 Structure:"
tree -L 2 "$SANDBOX_DIR" 2>/dev/null || find "$SANDBOX_DIR" -maxdepth 2 -print
echo ""
echo "🎯 Le sandbox est prêt pour vos tests !"
echo "   Testez le scanner: ./ransomware (option 1)"
echo ""
echo "⚠️  RAPPEL: N'exécutez JAMAIS le ransomware en dehors du sandbox !"
echo ""
