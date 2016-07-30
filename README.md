netns-exec
==========

Runs command in Linux network namespace as a normal user. Does
essentially the same as `sudo ip netns exec <netns> sudo -u "$USER"
<command>`, but doesn't require sudo privileges or password.

This is a simple suid root program built around the code of `ip netns
exec` command of [iproute2] utility suite. The iproute2 source is
embedded in this git repository as a submodule.


Usage
-----

Run command:

    netns-exec [--] <netns> <command> <args>...

Start shell:

    netns-exec [--] <netns>

Show help:

    netns-exec -h
    netns-exec --help

If you have problems with D-Bus, use `netns-exec-dbus` instead of
`netns-exec`. It takes the same parameters. See below for explanation.


D-Bus proxy
-----------

D-Bus doesn't work inside the network namespace if the D-Bus daemon is
using an abstract Unix domain socket. The script `netns-exec-dbus`
enables the use of D-Bus by creating a simple proxy that listens on a
regular Unix domain socket, which works across namespaces. It uses
[socat], which must be installed and in `$PATH`. It starts the command
with the environment variable `DBUS_SESSION_BUS_ADDRESS` set to point
to the proxy socket, which should make the program use the proxy
socket. The proxy is only started when necessary, and it and its
socket are automatically destroyed after the command terminates.


Building and installing
-----------------------

    git submodule update --init
    make
    sudo make install


Security note
-------------

This program is installed as suid root. Normally, only root can
execute anything in a different network namespace. This program
intentionally bypasses this restriction and should not be installed in
a machine where this is a problem.


[iproute2]: http://www.linuxfoundation.org/collaborate/workgroups/networking/iproute2
[socat]: http://www.dest-unreach.org/socat/
