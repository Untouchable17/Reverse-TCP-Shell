#!/bin/bash

function compiling_files(){
    if [ -f "ReverseShell.o" ] && [ -f "SecureTCPConnection.o" ]; then
        echo "[*] Files already compiled"
    else
        echo "[*] Compiling files..."
        make
        if [ $? -eq 0 ]; then
            echo "[SUCCESS] Compiled successfully."
            chmod +x "ReverseShell.o" "SecureTCPConnection.o"
        else
            echo "[FAILED] Failed to compile."
            exit
        fi
    fi
}

function install_depends(){
    echo "[*] Installing dependencies..."
    apt-get update && apt-get upgrade -y && apt-get install libpcap-dev -y
}


function set_starting_point(){
    echo "[*] Which program(s) do you want to run?"
    echo "1. TCP Reverse Shell"
    echo "2. Secure TCP Connection"
    echo "3. Both"
    read choice
    case $choice in
        1)
            echo "[*] Running TCP Reverse Shell..."
            ./ReverseShell.o
            ;;
        2)
            echo "[*] Running Secure TCP Connection..."
            ./SecureTCPConnection.o
            ;;
        3)
            echo "[*] Happy Hacking :)"
            ./ReverseShell.o &
            ./SecureTCPConnection.o &
            ;;
        *)
            echo "[*] Invalid choice."
            ;;
    esac
}


if [ "$EUID" -ne 0 ]; then
    echo "Run script as root"
    exit
else
    install_depends;
    if [ $? -eq 0 ]; then
        echo "[SUCCESS] Dependencies installed successfully."
        compiling_files;
        set_starting_point;
    else
        echo "[FAILED] Failed to install dependencies."
        exit
    fi
fi



