<h1 align="center">
    <a href="https://github.com/Untouchable17/Reverse-TCP-Shell">
        <img src="https://i.ibb.co/5LwHcv7/shadowshell.png" width="700">
    </a>
</h1>

<p align="center">
<a href="https://github.com/Untouchable17/Reverse-TCP-Shell"><img src="https://img.shields.io/static/v1?label=version&message=4.0.0&color=green"></a>
<a href="https://github.com/Untouchable17/Reverse-TCP-Shell/issues?q=is:issue+is:closed"><img src="https://img.shields.io/github/issues-closed/Untouchable17/Reverse-TCP-Shell?color=orange"></a>
</p>

<h1 align="center">TCP Reverse Shell C++</h1>

<b>TCP reverse shell</b> is a type of shell in which the attacker creates a connection from the target machine back to the attacker's machine, rather than the attacker connecting directly to the target. In this script, the attacker typically sets up a listener on a specified port on their own machine, and the victim machine is then compromised with malware that establishes a connection to the attacker's listener. Once the connection is established, the attacker has access to a command prompt on the victim machine, and can execute commands and interact with the system as if they were sitting at the machine itself. This technique is commonly used in cyber attacks and penetration testing
<h2 align="center">Installation</h2>

<p align="center">Installing and using program process:</p>

<p align="center">Execute all commands on behalf of the superuser and set your machine ip and port in files</p>

1. Download or clone this repository.
```
git clone https://github.com/Untouchable17/Reverse-TCP-Shell
```
2. Make the file executable with the chmod +x command
```
chmod +x install.sh
```
<br/>
3. Run the bash script `install.sh`

<br>This script will automatically download and install all the necessary dependencies, compile the programs, and then offer you to choose 1 of 3 start options


> Other Method: Manual Installation
1. Downloading or cloning this GitHub repository.
```
git clone https://github.com/Untouchable17/TCP-Reverse-Shell
```
2. Update system packages and install libpcap
```
apt-get update && apt-get upgrade -y
apt-get install libpcap-dev -y
```
3. Compile files
```
g++ ReverseShell.cpp -o ReverseShell.o
g++ SecureTCPConnection.cpp -o SecureTCPConnection.o
```
4. Run one of the program
```
./ReverseShell.o or ./SecureTCPConnection.o
```
<br>You can add the correct path to global like this: `export PATH="$PATH:$(pwd)"` and then you can run the program by just entering their name ReverseShell.o or SecureTCPConnection.o


> For Windows
1. Compile the program using the flag -lws2_32
```
g++ winshell.cpp -o winshell.exe -lws2_32
```
2. Start the event listener and run the compiled program

<br>
<h2 align="center">Install and build the apk file for Android</h2>
<b>Method 1:</b> To build, use the `gradle build` command in the `Android Studio` terminal. This command will compile the source code and create an `APK file` that can be installed on an Android device
<br>
<br>

<b>Method 2:</b>Install `Android SDK` (Native Development Kit) and compile the program code in C++ with the command `ndk-build` and then assemble the APK file using `ant release` command

<br>
<h2 align="center">How to use</h2>
<p>At first, start listening on the hacker's computer (or on your own) to the port you specified in the file (by default, port 4444), and then run the compiled infected file on the victim's computer</p>

1. Starting port listening
```
nc -nlvp SET_PORT (ex. 4444)
```
2. Execute the infected program on the victim's computer
3. Congratulations, now you have a shell
4. If interactivity has not appeared, execute this command
```
script -a /dev/null
```

```
change shell: chsh -s /bin/{shell}
```

```
show list shells: cat /etc/shells
```

<h2 align="center">A little explanation of the code and a teaser</h2>

- This program is a type of software that allows an attacker to gain remote access to a computer or network without the knowledge or permission of the system owner.

- This code creates a TCP socket that connects to the specified server and port. Then the child process is created using the fork() call. The child process runs the /bin/bash shell, which allows an attacker to execute arbitrary commands on a remote system.

- In the parent process, commands are read from a remote client via a socket. The received commands are passed to the child process for execution using the open() call, and the results of executing commands are sent back to the client via the socket. The parent process constantly waits for commands from the client and executes them until the client closes the connection.

- In general, this code can be used for illegal access to a computer or network, so its use for illegal purposes is prohibited.


## Contact Developer


    Telegram:           @secdet17
    Group:              t.me/secdet_team
    Email:              tylerblackout17@gmail.com

