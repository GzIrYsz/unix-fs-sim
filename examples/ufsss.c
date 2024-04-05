/**
* @file ufsss.c
 * @brief Implementation of the Unix-Filesystem-Simulation Simple Shell.
 * @author Thomas REMY
 * @version 0.1.0
 * @date 04-01-2024
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

#include "logging/logging.h"
#include "unix_fs_sim/ufs.h"
#include "unix_fs_sim/exits.h"

#define FILE_MAX_SIZE 49152

logger_t *logger;
file_t *f;

void print_menu();
void create_partition();
void create_filesystem();
void mount_partition();
void open_file();
void read_file();
void write_file();
void change_offset();
void close_file();
void unmount_partition();
void print_usage();

int main(int argc, char **argv) {
    logger_config_t loggerConfig = {
            1024,
            true,
            DEBUG,
            true,
            true,
            TRACE,
            "../../logs/log_trace.log"
    };
    init_logger(loggerConfig);

    char menu_choice;
    bool end = false;
    while (!end) {
        print_menu();
        printf("> ");
        scanf(" %c", &menu_choice);
        switch (menu_choice) {
            case '1':
                create_partition();
                break;
            case '2':
                create_filesystem();
                break;
            case '3':
                mount_partition();
                break;
            case '4':
                open_file();
                break;
            case '5':
                read_file();
                break;
            case '6':
                write_file();
                break;
            case '7':
                change_offset();
                break;
            case '8':
                close_file();
                break;
            case '9':
                unmount_partition();
                break;
            case 'u':
                print_usage();
                break;
            case 'q':
                end = true;
                break;
        }
    }
    return EXIT_SUCCESS;
}

void print_menu() {
    printf("\n\n########################\n######### MENU #########\n########################\n\n");
    printf("1. Create partition\n");
    printf("2. Create filesystem\n");
    printf("3. Mount partition\n");
    printf("4. Open file\n");
    printf("5. Read file\n");
    printf("6. Write file\n");
    printf("7. Change offset\n");
    printf("8. Close file\n");
    printf("9. Unmount partition\n");
    printf("u. Print usage\n");
    printf("q. Quit\n\n");
}

void create_partition() {
    char loc[255];
    int size;
    printf("Please answer the questions below to create the partition.\n");
    printf("Location on disk: ");
    scanf(" %s", loc);
    printf("Size (in Kb): ");
    scanf(" %d", &size);

    if (mkpart(loc, size, KB) == -1) {
        exit(ERR_MKPART);
    }
    printf("Partition created.");
}

void create_filesystem() {
    char loc[255];
    printf("Please answer the questions below to format a partition.\n");
    printf("Location of the partition: ");
    scanf(" %s", loc);

    if (mkfs(loc, LARGE, 10) == -1) {
        exit(ERR_MKFS);
    }
    printf("Partition formatted.");
}

void mount_partition() {
    char loc[255];
    printf("Please answer the questions below to mount a partition.\n");
    printf("Location of the partition: ");
    scanf(" %s", loc);
    if (mount(loc) == -1) {
        exit(ERR_MOUNT);
    }
    printf("Partition mounted.");
}

void open_file() {
    char name[MAX_FILENAME];
    printf("Filename: ");
    scanf(" %[^\n]s", name);
    if ((f = my_open(name)) == NULL) {
        exit(ERR_OPEN);
    }
    printf("File opened. You can now work with it.");
}

void read_file() {
    char file_content[4096];
    if (my_read(f, file_content, 10) == -1) {
        exit(ERR_READ);
    }
    printf("%s", file_content);
}

void write_file() {
    char file_content[FILE_MAX_SIZE];
    printf("Content: ");
    scanf(" %[^\n]s", file_content);
    if (my_write(f, file_content, (int) strlen(file_content)) == -1) {
        exit(ERR_WRITE);
    }
    printf("%s", file_content);
}

void change_offset() {
    int offset;
    char base;
    printf("Base ([S]ET, [C]URR, [E]ND): ");
    scanf(" %c", &base);
    printf("Offset: ");
    scanf(" %d", &offset);
    switch (base) {
        case 'S':
            my_seek(f, offset, SEEK_SET);
            printf("Offset changed.");
            break;
        case 'C':
            my_seek(f, offset, SEEK_CUR);
            printf("Offset changed.");
            break;
        case 'E':
            my_seek(f, offset, SEEK_END);
            printf("Offset changed.");
            break;
        default:
            printf("Offset not recognized.");
            break;
    }
}

void close_file() {
    if (my_close(f) == -1) {
        exit(ERR_CLOSE);
    }
    printf("File closed.");
}

void unmount_partition() {
    if (umount() == -1) {
        exit(ERR_UMOUNT);
    }
    printf("Partition unmounted.");
}

void print_usage() {
    fs_usage();
}