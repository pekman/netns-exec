#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "namespace.h"


#define EXIT_INVALID_ARGS 2
#define EXIT_NOT_FOUND 127
#define EXIT_CANNOT_EXEC 126
#define EXIT_OTHER_ERROR 125


static void usage(const char *argv0, bool is_error)
{
    fprintf(
        stderr,
        "\n"
        "usage:\n"
        "\n"
        "run command in netns:\n"
        "    %1$s [-h | --help] [--] <netns> <command> [<args> ...]\n"
        "\n"
        "run shell in netns:\n"
        "    %1$s [-h | --help] [--] <netns>\n"
        "\n",
        argv0);

    exit(is_error ? EXIT_INVALID_ARGS : 0);
}


static void drop_root_privileges(void)
{
    if (setuid(getuid()) < 0) {
        perror("error setting user id");
        exit(EXIT_OTHER_ERROR);
    }
    if (setgid(getgid()) < 0) {
        perror("error setting group id");
        exit(EXIT_OTHER_ERROR);
    }
}


int main(int argc, char *argv[])
{
    // parse arguments
    if (argc < 2 || (argc == 2 && strcmp(argv[1], "--") == 0))
        // no netns given
        usage(argv[0], true);
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        usage(argv[0], false);

    if (strcmp(argv[1], "--") == 0) {
        argv++;
        argc--;
    }
    else if (argv[1][0] == '-') {
        // invalid command line option
        fprintf(stderr, "Unknown option: %s\n", argv[1]);
        usage(argv[0], true);
    }


    // switch network namespace using iproute2's function
    if (netns_switch(argv[1]) < 0)
        return EXIT_OTHER_ERROR;


    drop_root_privileges();


    char **command_argv;

    // if no command given, start shell
    if (argc == 2) {
        static char *shell_argv[] = { "/bin/sh", NULL };
        char *shell = getenv("SHELL");
        if (shell != NULL)
            shell_argv[0] = shell;
        command_argv = shell_argv;
    }
    else {
        command_argv = &argv[2];
    }


    // execute the command
    execvp(command_argv[0], command_argv);

    fprintf(stderr, "error executing \"%s\": %s",
            command_argv[0], strerror(errno));
    if (errno == ENOENT)
        return EXIT_NOT_FOUND;
    else
        return EXIT_CANNOT_EXEC;
}
