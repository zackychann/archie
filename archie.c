#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_INPUT_LENGTH 256
#define COMMAND_BUFFER_SIZE 512

// Function Prototypes
int check_archie_file();
int check_package_manager();
int check_git();
void install_git();
void install_yay();
void update_system(const char *package_manager);
void install_package(const char *package_manager, const char *package);
void remove_package(const char *package_manager, const char *package);
void purge_package(const char *package_manager, const char *package);
void clean_cache(const char *package_manager);
void clean_orphans(const char *package_manager);
void search_package(const char *package_manager, const char *package);
void display_help();
void prompt_install_yay();
void get_input(char *input, const char *prompt);
int is_valid_command(char command);
void handle_command(const char *input, const char *package_manager);
void handle_exec_command(const char *command, const char *package_manager);
void display_version();
char **get_pacman_commands();
char *command_generator(const char *text, int state);
char **command_completion(const char *text, int start, int end);
char* get_package_manager_version(const char *package_manager);

// Check if the Archie configuration file exists
int check_archie_file() {
    const char *home = getenv("HOME");
    char path[MAX_INPUT_LENGTH];
    snprintf(path, sizeof(path), "%s/.archie-use-paru", home);
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

// Determine the package manager in use
int check_package_manager() {
    if (check_archie_file()) return 2; // paru
    if (system("command -v yay > /dev/null 2>&1") == 0) return 1; // yay
    if (system("command -v paru > /dev/null 2>&1") == 0) return 2; // paru
    return 0; // none
}

// Check if Git is installed
int check_git() {
    return system("command -v git > /dev/null 2>&1") == 0;
}

// Install Git
void install_git() {
    printf("Installing git...\n");
    system("sudo pacman -S --noconfirm git");
}

// Install Yay
void install_yay() {
    printf("Installing yay...\n");
    system("mkdir -p $HOME/.cache/archie/made-by-gurov && "
           "cd $HOME/.cache/archie/made-by-gurov && "
           "git clone https://aur.archlinux.org/yay-bin.git && "
           "cd yay-bin && "
           "makepkg -scCi && "
           "cd && "
           "rm -rf $HOME/.cache/archie/");
    printf("Installation of yay is complete. Please restart your shell and relaunch the script.\n");
}

// Update the system using the specified package manager
void update_system(const char *package_manager) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s -Syu --noconfirm", package_manager);
    system(command);
}

// Install a package using the specified package manager
void install_package(const char *package_manager, const char *package) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s -S %s", package_manager, package);
    system(command);
}

// Remove a package using the specified package manager
void remove_package(const char *package_manager, const char *package) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s -R %s", package_manager, package);
    system(command);
}

// Purge a package using the specified package manager
void purge_package(const char *package_manager, const char *package) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s -Rns %s", package_manager, package);
    system(command);
}

// Clean the cache of the specified package manager
void clean_cache(const char *package_manager) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s -Sc --noconfirm", package_manager);
    system(command);
}

// Clean orphaned packages using the specified package manager
void clean_orphans(const char *package_manager) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s -Rns $(pacman -Qdtq)", package_manager);
    system(command);
}

// Search for a package using the specified package manager
void search_package(const char *package_manager, const char *package) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s %s", package_manager, package);
    system(command);
}

// Display available commands
void display_help() {
    printf("Available options are:\n");
    printf("u - Update the system\n");
    printf("i - Install a package\n");
    printf("r - Remove a package\n");
    printf("p - Purge a package\n");
    printf("c - Clean cache\n");
    printf("o - Clean orphaned packages\n");
    printf("s - Search for a package\n");
    printf("h - Help\n");
    printf("q - Quit\n");
}

// Prompt user to install Yay if not present
void prompt_install_yay() {
    char response[10];
    printf("Error: Neither yay nor paru is installed.\n");
    sleep(3);
    printf("Do you want to install yay? (y/n): ");
    scanf("%s", response);

    if (strcmp(response, "y") == 0 || strcmp(response, "yes") == 0) {
        if (!check_git()) install_git();
        install_yay();
    } else {
        printf("Exiting the program.\n");
        exit(0);
    }
}

// Get user input with a prompt
void get_input(char *input, const char *prompt) {
    char *line = readline(prompt);
    if (line) {
        strcpy(input, line);
        free(line);
    } else {
        input[0] = '\0'; // Handle EOF
    }

    // Remove newline character
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) == 0) {
        printf("Please input something, not air.\n");
        get_input(input, prompt);
    }
}

// Check if the command is valid
int is_valid_command(char command) {
    return strchr("uirpcoshq", command) != NULL;
}

// Handle user commands
void handle_command(const char *input, const char *package_manager) {
    char choice = input[0];
    if (strlen(input) == 1 && is_valid_command(choice)) {
        switch (choice) {
            case 'u': update_system(package_manager); break;
            case 'i': {
                char package[MAX_INPUT_LENGTH];
                rl_attempted_completion_function = command_completion;
                get_input(package, "Enter package name to install: ");
                rl_attempted_completion_function = NULL;
                install_package(package_manager, package);
                break;
            }
            case 'r': {
                char package[MAX_INPUT_LENGTH];
                rl_attempted_completion_function = command_completion;
                get_input(package, "Enter package name to remove: ");
                rl_attempted_completion_function = NULL;
                remove_package(package_manager, package);
                break;
            }
            case 'p': {
                char package[MAX_INPUT_LENGTH];
                rl_attempted_completion_function = command_completion;
                get_input(package, "Enter package name to purge: ");
                rl_attempted_completion_function = NULL;
                purge_package(package_manager, package);
                break;
            }
            case 'c': clean_cache(package_manager); break;
            case 'o': clean_orphans(package_manager); break;
            case 's': {
                char package[MAX_INPUT_LENGTH];
                get_input(package, "Enter package name to search: ");
                search_package(package_manager, package);
                break;
            }
            case 'h': display_help(); break;
            case 'q': exit(0); break;
            default: printf("Invalid option. Please correct your grammar and type a valid option.\n"); break;
        }
    } else {
        if (is_valid_command(choice)) {
            printf("Did you mean \"%c\"? (y/n): ", choice);
            char response[10];
            get_input(response, "");
            if (strcmp(response, "y") == 0 || strcmp(response, "yes") == 0) {
                char new_input[2] = { choice, '\0' }; // Create a new input string with the valid command
                handle_command(new_input, package_manager);
            } else {
                display_help();
            }
        } else {
            printf("Invalid input. Please input a valid command.\n");
            display_help();
        }
    }
}

// Handle execution of commands from arguments
void handle_exec_command(const char *command, const char *package_manager) {
    if (strcmp(command, "u") == 0) {
        update_system(package_manager);
    } else if (strcmp(command, "i") == 0) {
        char package[MAX_INPUT_LENGTH];
        get_input(package, "Enter package name to install: ");
        install_package(package_manager, package);
    } else if (strcmp(command, "r") == 0) {
        char package[MAX_INPUT_LENGTH];
        get_input(package, "Enter package name to remove: ");
        remove_package(package_manager, package);
    } else if (strcmp(command, "p") == 0) {
        char package[MAX_INPUT_LENGTH];
        get_input(package, "Enter package name to purge: ");
        purge_package(package_manager, package);
    } else if (strcmp(command, "c") == 0) {
        clean_cache(package_manager);
    } else if (strcmp(command, "o") == 0) {
        clean_orphans(package_manager);
    } else if (strcmp(command, "s") == 0) {
        char package[MAX_INPUT_LENGTH];
        get_input(package, "Enter package name to search: ");
        search_package(package_manager, package);
    } else if (strcmp(command, "h") == 0) {
        display_help();
    } else {
        printf("Invalid command for --exec: %s\n", command);
    }
}

// Get the version of the specified package manager
char* get_package_manager_version(const char *package_manager) {
    char command[COMMAND_BUFFER_SIZE];
    snprintf(command, sizeof(command), "%s --version", package_manager);
    FILE *fp = popen(command, "r");
    if (fp == NULL) return strdup("unknown");

    char version[COMMAND_BUFFER_SIZE];
    if (fgets(version, sizeof(version), fp) != NULL) {
        version[strcspn(version, "\n")] = '\0'; // Remove newline character
    } else {
        strcpy(version, "unknown");
    }
    pclose(fp);

    // Extract version number
    char *version_start = strstr(version, " ");
    return strdup(version_start ? version_start + 1 : version);
}

// Display the version of the program
void display_version() {
    int pm_check = check_package_manager();
    const char *package_manager = (pm_check == 1) ? "yay" : (pm_check == 2) ? "paru" : "none";
    char *pm_version = get_package_manager_version(package_manager);

    printf("    __     Archie v1.3 - Fast & easy package management for Arch Linux\n"
           " .:--.'.   Written in C, powered by YAY and Pacman.\n"
           "/ |   \\ |  %s %s\n"  // Package manager and version
           "`\" __ | |  \n"
           " .'.''| |  \n"
           "/ /   | |_ This program may be freely redistributed under the terms of the GNU General Public License.\n"
           "\\ \\._,\\ '/ Created & maintained by Gurov\n"
           " `--'  `\"  With the help of scklss and Keiran\n", package_manager, pm_version);
    free(pm_version);
}

// Get available pacman commands
char **get_pacman_commands() {
    FILE *fp;
    char path[1035];
    char **commands = NULL;
    int command_count = 0;

    // Execute the pacman -Ssq command
    fp = popen("pacman -Ssq", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    // Read the output and store it in the commands array
    while (fgets(path, sizeof(path), fp) != NULL) {
        command_count++;
        commands = realloc(commands, sizeof(char *) * (command_count + 1));
        path[strcspn(path, "\n")] = 0; // Remove newline character
        commands[command_count - 1] = strdup(path);
    }

    // Close the file and null-terminate the array
    pclose(fp);
    commands[command_count] = NULL;

    return commands;
}

// Generate command matches for completion
char *command_generator(const char *text, int state) {
    static int list_index, len;
    static char **commands = NULL;

    if (!commands) commands = get_pacman_commands();

    // Initialize on first call
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    // Return the next match from the command list
    while (commands[list_index]) {
        if (strncmp(commands[list_index], text, len) == 0) {
            return strdup(commands[list_index++]);
        } else {
            list_index++;
        }
    }

    // No more matches
    return NULL;
}

// Command completion function
char **command_completion(const char *text, int start, int end) {
    (void)start; // Suppress unused parameter warning
    (void)end;   // Suppress unused parameter warning
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, command_generator);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0) {
            display_version();
            return 0;
        } else if (strcmp(argv[1], "--exec") == 0 || strcmp(argv[1], "-E") == 0) {
            const char *package_manager;
            int pm_check = check_package_manager();

            if (pm_check == 1) {
                package_manager = "yay";
            } else if (pm_check == 2) {
                package_manager = "paru";
            } else {
                prompt_install_yay();
                return 1;
            }

            if (argc > 2) {
                handle_exec_command(argv[2], package_manager);
            } else {
                char command[MAX_INPUT_LENGTH];
                get_input(command, "Enter command to execute (u, i, r, p, c, o, s, h): ");
                handle_exec_command(command, package_manager);
            }
            return 0; // Exit after executing the command
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-H") == 0) {
            printf("Usage:\n");
            printf("          archie\n");
            printf("          archie <flag> [...]\n\n");
            printf("flags:\n");
            printf("          archie {-H --help}\n");
            printf("          archie {-V --version}\n");
            printf("          archie {-E --exec} <command>\n\n");
            printf("commands:\n");
            printf("          archie {-E --exec} u (update the system)\n");
            printf("          archie {-E --exec} i (install a package)\n");
            printf("          archie {-E --exec} r (remove a package)\n");
            printf("          archie {-E --exec} p (purge a package)\n");
            printf("          archie {-E --exec} c (clean cache)\n");
            printf("          archie {-E --exec} o (clean orphaned packages)\n");
            printf("          archie {-E --exec} s (search for a package)\n");
            return 0;
        }
    }

    const char *package_manager;
    int pm_check = check_package_manager();

    if (pm_check == 1) {
        package_manager = "yay";
    } else if (pm_check == 2) {
        package_manager = "paru";
    } else {
        prompt_install_yay();
        return 1;
    }

    printf("Welcome to Archie, type \"h\" for help\n");

    while (1) {
        char input_line[MAX_INPUT_LENGTH];
        get_input(input_line, "$ ");
        if (*input_line) {
            add_history(input_line);
            handle_command(input_line, package_manager);
        }
    }

    return 0;
}
