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
