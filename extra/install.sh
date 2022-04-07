#! /bin/sh
# script d'installation de Hu-go!
# BBP 30/04/00 (bbp@via.ecp.fr)

############################# Liste des fichiers ##############################
# NE PAS UTILISER DE FICHIERS DONT LE NOM CONTIENT UN ESPACE OU RETOUR CHARIOT !

# fichiers de doc, vont par défaut dans /usr/share/doc/hugo
DOCFILES='changemt.fr changes faq hugo.es hugo.fr hugo.txt cheat.txt'
DOCPATH='/usr/share/doc/hugo'

# roms, vont par défaut dans /usr/share/hugo
ROMFILES='dracx.hcd pong.pce'
ROMPATH='/usr/share/hugo'

# binaires, vont par défaut dans /usr/share/bin
BINFILES='hugo'
BINPATH='/usr/share/bin'

# fichiers de config, donnés par paires : le premier élément de la paire va est
# renommé en le deuxième élément de la paire et va dans le /etc
CONFIGFILES='hu-go\!.dat /etc/hugo.dat hu-go\!.ini /etc/hugo.ini'
CONFIGPATH='/etc'

# fichiers inutiles sous linux, seront effacés
USELESSFILES='file_id.diz matrix.bmp'

############################# Autres variables ################################

# créer un fichier uninstall.sh ? 1 pour oui (défaut), 0 pour non
CREATEUNINSTALL=1

############################# Fonction Help ###################################

# Cette fonction affiche un message d'aide sur l'utilisation de install

function Help
{
  echo 'usage : install.sh [option1 [option2 [option3 ...]]]'
  echo 'where optionx is one of the following :'
  echo '  --help		this help screen'
  echo '  --nouninstallscript	do not generate the uninstallation script'
  echo '  --rompath={path}	path for roms (default : /usr/share/hugo)'
  echo '  --docpath={path}	path for doc (default : /usr/share/doc/hugo)'
  echo '  --binpath={path}	path for binaries (default : /usr/share/bin)'
  echo '  --configpath={path}	path for configuration files (default : /etc) - DO NOT USE THIS OPTION ! Hu-go! does not support it yet'
}

############################# Analyse des options de la ligne de commande #####

while [ "$#" != '0' ]; do

  if [ "$1" = '--help' ]; then
    Help
    exit 0
  fi

  if echo "$1" | grep -- '^--docpath=' >/dev/null; then
    DOCPATH=$(echo "$1" | sed 's/^--docpath=//g')
    if [ "$DOCPATH" = '' ]; then
      echo 'Invalid documentation files path'
      exit 2
    fi
    echo "Documentation files path set to $DOCPATH"
    shift
    continue
  fi

  if echo "$1" | grep -- '^--binpath=' >/dev/null; then
    BINPATH=$(echo "$1" | sed 's/^--binpath=//g')
    if [ "$BINPATH" = '' ]; then
      echo 'Invalid binary files path'
      exit 2
    fi
    echo "Binary files path set to $BINPATH"
    shift
    continue
  fi

  if echo "$1" | grep -- '^--configpath=' >/dev/null; then
    CONFIGPATH=$(echo "$1" | sed 's/^--configpath=//g')
    if [ "$CONFIGPATH" = '' ]; then
      echo 'Invalid config files path'
      exit 2
    fi
    echo "Config files path set to $CONFIGPATH"
    shift
    continue
  fi

  if echo "$1" | grep -- '^--rompath=' >/dev/null; then
    ROMPATH=$(echo "$1" | sed 's/^--rompath=//g')
    if [ "$ROMPATH" = '' ]; then
      echo 'Invalid rom files path'
      exit 2
    fi
    echo "Rom files path set to $ROMPATH"
    shift
    continue
  fi

  if [ "$1" = '--nouninstallscript' ]; then
    echo 'Canceling creation of uninstall.sh'
    CREATEUNINSTALL='0'
    shift
    continue
  fi

  echo "Invalid option $1, use install.sh --help for usage"
  Help
  exit 1

done

############################# Création si nécessaire du fichier uninstall.sh ##

if [ "$CREATEUNINSTALL" = '1' ]; then
  rm -f uninstall.sh 2>&1 >/dev/null
  rm -f tmp-end-of-uninstall 2>&1 >/dev/null
  touch uninstall.sh
  chmod 744 uninstall.sh
  touch tmp-end-of-uninstall
  chmod 644 tmp-end-of-uninstall
  echo '#! /bin/sh' >> uninstall.sh
  echo '' >> uninstall.sh
  echo '' >> tmp-end-of-uninstall.sh
fi

############################# Création des répertoires ########################

# fonction qui créé un répertoire s'il n'existe pas déjà, et affiche un message
# d'erreur si on a pas les droits d'écriture ou d'exécution dessus

function MKDIR
{
  if [ -d "$1" ]; then
    if ! [ -w "$1" -a -x "$1" ]; then
      echo "Directory $1 is not writeable or not executable, aborting installation"
      echo 'May be you should execute this script as root or change its arguments'
      exit 3
    fi
  else
    echo "Creating directory $1"
    if ! mkdir "$1"; then
      echo "Unable to create directory $1, aborting installation"
      echo 'May be you should execute this script as root or change its arguments'
      echo '(using default values requires root privileges on most systems)'
      exit 4
    fi
    chmod 755 "$1"
    if echo "$1" | grep '^/' >/dev/null; then
      echo "rmdir '$1'" >>tmp-end-of-uninstall
    else
      echo "rmdir '$(pwd)/${1}'" >>tmp-end-of-uninstall
    fi
  fi
}

if [ "$DOCFILES" != '' ]; then
  MKDIR "$DOCPATH"
  for i in $(echo "$DOCFILES"); do
    cp "$i" "$DOCPATH"
    chmod 644 "$DOCPATH/$i"
    echo "rm -f '$DOCPATH/$i'" >>uninstall.sh
  done
fi

if [ "$ROMFILES" != '' ]; then
  MKDIR "$ROMPATH"
  for i in $(echo "$ROMFILES"); do
    cp "$i" "$ROMPATH"
    chmod 644 "$ROMPATH/$i"
    echo "rm -f '$ROMPATH/$i'" >>uninstall.sh
  done
fi

if [ "$BINFILES" != '' ]; then
  MKDIR "$BINPATH"
  for i in $(echo "$BINFILES"); do
    cp "$i" "$BINPATH"
    chmod 755 "$BINPATH/$i"
    echo "rm -f '$BINPATH/$i'" >>uninstall.sh
  done
fi

if [ "$CONFIGFILES" != '' ]; then
  MKDIR "$CONFIGPATH"
  name1=$(echo "$CONFIGFILES" | awk '{ print $1 }')
  name2=$(echo "$CONFIGFILES" | awk '{ print $2 }')
  CONFIGFILES=$(echo "$CONFIGFILES" | sed 's/^[ 	]*[^ 	]*[ 	]*[^ 	]*[ 	]*//g')
    if [ "$i" = 'hu-go!.ini' ]; then
      awk '{ if ($0 ~ /^rom_path=/) { print rom_path='"$ROMPATH"' } else { print $0 } }' "$name1" > "$CONFIGPATH/$name2"
    else
      cp "$i" "$CONFIGPATH"
    fi
    chmod 644 "$CONFIGPATH/$i"
    echo "rm -f '$CONFIGPATH/$i'" >>uninstall.sh
fi

############################# fin de la génération de uninstall.sh et nettoyage

if [ "$CREATEUNINSTALL" = '1' ]; then
  cat tmp-end-of-uninstall >>uninstall.sh
  rm -f tmp-end-of-uninstall
  echo "Uninstallation script generated in $(pwd)/uninstall.sh"
fi

for i in $(echo "$USELESSFILES"); do
  rm -f "$i"
done

echo ''
echo 'Installation succeded'
echo "Don't forget to read the docs in $DOCPATH :)"

