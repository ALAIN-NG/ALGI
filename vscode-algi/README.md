# Extension VS Code — ALGI Language Support

Cette extension apporte au langage **ALGI** (le langage Bamiléké du compilateur QuantL) :

- **Coloration syntaxique** complète pour les fichiers `.algi` (mots-clés, chaînes, nombres, commentaires, opérateurs...).
- **Icône de fichier « AL »** dans l'explorateur de VS Code pour repérer les fichiers `.algi` en un coup d'œil.

## Contenu

```
vscode-algi/
├── package.json                     # Manifeste de l'extension
├── language-configuration.json      # Commentaires, crochets, auto-fermeture, folding
├── syntaxes/
│   └── algi.tmLanguage.json         # Grammaire TextMate (coloration)
├── icons/
│   ├── algi-file-icon.svg           # Icône "AL" pour les .algi
│   ├── default-file-icon.svg        # Icône générique (autres fichiers)
│   ├── default-folder-icon.svg      # Icône de dossier fermé
│   ├── default-folder-open-icon.svg # Icône de dossier ouvert
│   └── algi-icon-theme.json         # Thème d'icônes déclarant l'association .algi -> icône AL
└── install.sh                       # Script d'installation locale
```

## Installation

### Option 1 — Script automatique (recommandé)

```bash
cd vscode-algi
./install.sh
```

Puis redémarrez VS Code.

### Option 2 — Installation manuelle

Copiez le dossier `vscode-algi` dans le dossier d'extensions de VS Code :

- **Linux** : `~/.vscode/extensions/`
- **macOS** : `~/.vscode/extensions/`
- **Windows** : `%USERPROFILE%\.vscode\extensions\`

Redémarrez VS Code ensuite.

### Option 3 — Empaqueter en `.vsix` (pour distribution)

Si vous avez `vsce` installé (`npm install -g @vscode/vsce`) :

```bash
cd vscode-algi
vsce package
```

Cela génère un fichier `algi-language-0.1.0.vsix` que vous pouvez installer via :
`Extensions > ... > Install from VSIX...` dans VS Code.

## Activer la coloration syntaxique

Rien à faire ! Dès qu'un fichier `.algi` est ouvert, la coloration s'applique automatiquement
grâce à l'association d'extension déclarée dans `package.json`.

## Activer l'icône « AL »

Les icônes de fichiers dans VS Code dépendent d'un **thème d'icônes actif** (un seul actif à la fois,
comme pour les thèmes de couleurs). Pour activer celui de cette extension :

1. `Ctrl+Shift+P` (ou `Cmd+Shift+P` sur macOS)
2. Tapez **"Preferences: File Icon Theme"**
3. Sélectionnez **"ALGI Icons"**

Les fichiers `.algi` afficheront alors l'icône **AL**, et tous les autres fichiers/dossiers
garderont une icône générique neutre (ce thème est volontairement minimal et dédié à ALGI ;
si vous utilisez déjà un thème d'icônes complet comme "Material Icon Theme" ou "Seti", vous
devrez choisir entre les deux car un seul thème d'icônes peut être actif à la fois).

## Mots-clés reconnus (extraits du lexer `src/algi.l`)

| Catégorie              | Mots-clés |
|-------------------------|-----------|
| Contrôle                | `ke` (if), `kommenu` (else), `w` (while), `pou` (for), `mi` (fin de bloc), `tem` (return), `peu` (break), `tela` (try), `tatem` (catch) |
| Déclarations            | `he` (let), `fonksion` (function), `kwouo` (class), `feh` (new) |
| Fonctions intégrées     | `tehap` (print), `keksie` (input), `sanwe` (eval), `waan` (sum), `tie` (subtract), `denden` (equals), `nwagniehwe` (sort), `ge` (typeof), `ganfa` (open), `stegnie` (exit) |
| Booléens / null         | `nene` (true), `pok` (false), `n` (null) |
| Logique                 | `e` (and), `ou` (or), `poke` (not) |
| Opérateurs mots         | `est` (is), `dans` (in), `comme` (as) |
| Types                   | `saa` (int), `nka'tche`/`reyel` (float), `djanghom` (string) |
| Liste                   | `bi` |
