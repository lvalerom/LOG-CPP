#!/bin/bash

function _remove_setup {
    rm -rf m4/
    rm -rf src/
    rm -rf config/
    rm AUTHORS COPYING ChangeLog NEWS README
    rm Makefile.am
    rm configure.ac
    rm autogen.sh
}

function _clear_setup {
    echo "clear TODO"
}

function _fill_autogen_script {
    echo "#!/bin/sh" >> autogen.sh
    echo "autoreconf --force --install -I config -I m4" >> autogen.sh
}

function _setup_proj {
    mkdir m4
    mkdir config
    mkdir src
    touch AUTHORS COPYING ChangeLog NEWS README
    touch src/Makefile.am
    touch Makefile.am
    touch configure.ac
    touch autogen.sh
    chmod +x autogen.sh
    $(_fill_autogen_script)
}

if [ "$#" -ge 1 ] ; then
    if [ $1 = "remove" ] ; then

	echo "Removing all starting files..."
	$(_remove_setup)
    elif [ $1 = "clear" ] ; then

	echo "Clearing all starting files..."
	$(_clear_setup)
    fi
    exit
fi

echo "Starting automake project setup..."

vPath=$(pwd)

$(_setup_proj)
