#!/bin/sh
# Hu-go! installation script
# BBP  30/04/00 (bbp@via.ecp.fr)
# MooZ 13/01/01 (mooz@free.fr)

# Help/Usage function
Usage()
{
    echo 'Usage: install.sh [option1 [option2 [option3 ...]]]'
    echo
    echo '  --help		this help screen'
    echo '  --version           show version'
    echo '  --default           default installation'
    echo '  --prefix            install hu-go! in the specified directory '
    echo '                      (default : /usr/share)'
    echo '  --nouninstallscript	do not generate the uninstallation script'
    echo '  --rompath={path}	path for roms (default : /usr/share/hugo)'
    echo '  --docpath={path}	path for doc (default : /usr/share/doc/hugo)'
    echo '  --binpath={path}	path for binaries (default : /usr/share/bin)'
    echo '  --configpath={path}	path for configuration files (default : /etc) @@@@ UNSUPPORTED @@@@ '
}

# Version function
Version()
{
    echo 'install.sh (HuGo!)'
    echo 'original version : BBP  (bbp@via.ecp.fr)'
    echo 'custom   version : MooZ (vcruz@free.fr)'
}

# Error handling
Error()
{
    case $mesg in

	missing)
	    echo 'Error : install.sh: missing parameter(s) !!!'
	    echo
	    Usage
	    ;;

	invalid)
	    echo 'Error : install.sh: invalid parameter(s) !!!'
	    echo
	    Usage
	    ;;

	abort)
	    echo 'Error : install.sh: installation aborted by user'
	    ;;

	conflict)
	    echo 'Error : install.sh: conflicting parameters !!!'
	    echo
	    Usage
	    ;;
    esac
    exit 1
}

# Extract parameters
Parameters()
{
    case $flag_mode in

	--default)
	    echo 'Running standard installation...'
	    bin_path="/usr/share/bin"
	    doc_path="/usr/share/doc/hugo"
	    rom_path="/usr/share/hugo"
	    cfg_path="/etc"
	    ;;

	--prefix=)
	    echo 'Running prefix installation...'
	    if test "$prefix"; then 
		bin_path="$prefix/bin"
		doc_path="$prefix/doc"
		rom_path="$prefix/hugo"
	        cfg_path="$prefix/cfg"
	    fi
	    if test "$binpath"; then bin_path="$binpath"; fi
	    if test "$docpath"; then doc_path="$rompath"; fi
	    if test "$rompath"; then rom_path="$docpath"; fi
	    if test "$cfgpath"; then cfg_path="$cfgpath"; fi
	    ;;
        *) # no install type defined, check envir
	    if test -z "$prefix" && ( test -z "$binpath" || test -z "$docpath" || test -z "$rompath" || test -z "$cfgpath" )
	      then mesg="missing"; Error
	      else flag_mode="--prefix="; Parameters
	    fi
	    ;;
    esac
}

# Check permition for instalation
Permitions()
{
    mkdir -p $bin_path || test ! -w "$bin_path" || (echo "You don't have write access to $bin_path, please change to appropriate user or ask your sysadmin."; exit 1)
    mkdir -p $doc_path || test ! -w "$doc_path" || (echo "You don't have write access to $doc_path, please change to appropriate user or ask your sysadmin."; exit 1)
    mkdir -p $rom_path || test ! -w "$rom_path" || (echo "You don't have write access to $rom_path, please change to appropriate user or ask your sysadmin."; exit 1)
    mkdir -p $cfg_path || test ! -w "$cfg_path" || (echo "You don't have write access to $cfg_path, please change to appropriate user or ask your sysadmin."; exit 1)

}

# Copy files to the apropriate location
GoInstall()
{
    doc_files='changemt.fr changes faq hugo.es hugo.fr hugo.txt cheat.txt'
    rom_files='dracx.hcd pong.pce'
    cfg_files='hu-go!.dat hu-go!.ini'

    # Binary
	cp $v -fp hugo $bin_path/
        chmod $vv 755 $bin_path/hugo

    # Config files
	mkdir $v -p $cfg_path/
	chmod $vv 755 $cfg_path
	for i in $(echo "$cfg_files"); do
	    cp -fp $i $cfg_path;
	    chmod 644 $cfg_path/$i;
	done


    # Documentation
	mkdir $v -p $doc_path/
	chmod $vv 755 $doc_path
	for i in $(echo "$doc_files"); do
	    cp -fp $i $doc_path
	    chmod 644 $doc_path/$i
	done
	

    # Roms
	mkdir $v -p $rom_path/
	chmod $vv 755 $rom_path/
	for i in $(echo "$rom_files"); do
	    cp -fp $i $rom_path
	    chmod 644 $rom_path/$i
	done

    echo
    echo 'Installation complete.'
    echo "Start with \$PATH/hugo, $exec_dir/hugo or ./hugo"
}

# Execution start point (parameter loop)
echo "@"

v=""
vv=""
flag_mode=""

echo "@"

prefix=`printenv prefix`
binpath=`printenv binpath`
docpath=`printenv docpath`
rompath=`printenv rompath`
cfgpath=`printenv cfgpath`

echo "@"

if test "$#" = 0 && test -z "$prefix" && ( test -z "$binpath" || test -z "$docpath" || test -z "$sharpath" || test -z "$cfgpath" ) then mesg="missing"; error ; fi

echo "@"

while test x"$1" != x ; do
case $1 in

    -h|--help) Usage; exit 0 ;;

    -V|--version) Version; exit 0 ;;

    -v|--verbose)
	v='--verbose'
	shift
	;;

    -vv)
	v='--verbose'
	vv='--verbose'
	shift
	;;

    --prefix=*)
	if test "$flag_mode" = "--prefix=" ||  test -z "$flag_mode" ; then
	    flag_mode="--prefix="

	    param=`echo "$1" | sed -e 's/--prefix=//'`
	    if test $param; then prefix=$param; fi
	    shift
	  else
	  mesg="conflict" ; Error
	fi
	;;

    --binpath=*)
	if test "$flag_mode" = "--prefix=" || test -z "$flag_mode" ; then
	    flag_mode="--prefix="

	    param=`echo "$1" | sed -e 's/--binpath=//'`
	    if test $param; then binpath=$param; fi
	    shift
	  else
	  mesg="conflict" ; Error
	fi
	;;

    --docpath=*)
	if test "$flag_mode" = "--prefix=" || test -z "$flag_mode" ; then
	    flag_mode="--prefix="

	    param=`echo "$1" | sed -e 's/--docpath=//'`
	    if test $param; then docpath=$param; fi
	    shift
	  else
	  mesg="conflict" ; Error
	fi
	;;

    --rompath=*)
	if test "$flag_mode" = "--prefix=" || test -z "$flag_mode" ; then
	    flag_mode="--prefix="

	    param=`echo "$1" | sed -e 's/--rompath=//'`
	    if test $param; then rompath=$param; fi
	    shift
	  else
	  mesg="conflict" ; Error
	fi
	;;

    --cfgpath=*)
	if test "$flag_mode" = "--prefix=" || test -z "$flag_mode" ; then
	    flag_mode="--prefix="

	    param=`echo "$1" | sed -e 's/--cfgpath=//'`
	    if test $param; then cfgpath=$param; fi
	    shift
	  else
	  mesg="conflict" ; Error
	fi
	;;

     *) mesg="invalid"; Error ;;
esac
done # Parameter loop

    Parameters
    Permitions
    GoInstall
exit 0




