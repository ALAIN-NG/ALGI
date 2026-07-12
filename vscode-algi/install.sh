#!/usr/bin/env bash
# Installe l'extension ALGI (coloration syntaxique + icônes de fichiers) dans VS Code
# sans passer par le Marketplace : simple copie dans le dossier d'extensions local.

set -e

EXT_NAME="quantl.algi-language-0.1.0"

# Détecte le dossier d'extensions VS Code selon l'OS
if [ -d "$HOME/.vscode/extensions" ]; then
    TARGET_DIR="$HOME/.vscode/extensions/$EXT_NAME"
elif [ -d "$HOME/.vscode-server/extensions" ]; then
    TARGET_DIR="$HOME/.vscode-server/extensions/$EXT_NAME"
else
    TARGET_DIR="$HOME/.vscode/extensions/$EXT_NAME"
    mkdir -p "$HOME/.vscode/extensions"
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Installation de l'extension ALGI dans : $TARGET_DIR"
rm -rf "$TARGET_DIR"
mkdir -p "$TARGET_DIR"
cp -r "$SCRIPT_DIR"/* "$TARGET_DIR"/

echo ""
echo "Extension installée. Redémarrez VS Code, puis :"
echo "  1. Ouvrez un fichier .algi -> la coloration syntaxique s'applique automatiquement."
echo "  2. Pour l'icône 'AL' : Ctrl+Shift+P (ou Cmd+Shift+P) > 'Preferences: File Icon Theme' > choisir 'ALGI Icons'."
