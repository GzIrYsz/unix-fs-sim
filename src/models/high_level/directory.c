/**
 * @file directory.c
 * @brief
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-28-2024
 */

#include "directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "logging/logging.h"
#include "../low_level/block.h"

extern logger_t* logger;

int get_offset(partition_t *p){
    double nb_data = (double) p->super_bloc.nb_data;
    double block_size = (double) p->super_bloc.block_size;
    double nb_inodes = (double) p->super_bloc.nb_inodes;
    return (off_t) (ceil(nb_data/block_size)+ ceil(nb_inodes/block_size)+(p->super_bloc.nb_blocks/10)+1)*block_size;
}

int create_directory(partition_t *p){
    logger->trace("Directory created");
    return 0;
}

int read_directory(partition_t *p){
    int directory_offset = get_offset(p);

    if(lseek(p->fd, directory_offset, SEEK_SET) == -1){
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    dir_entry_t* directory = p->directory;
    for (int i = 0; i < p->super_bloc.nb_inodes-p->super_bloc.nb_inodes_free; i++) {
        if (read(p->fd, (directory+i)->name, 60) == -1){
            logger->error("An error occurred when trying to read the directory name");
            return -1;
        }
        if (read(p->fd, &(directory+i)->inode, sizeof(uint32_t)) == -1){
            logger->error("An error occurred when trying to read the directory inode");
            return -1;
        }
    }

    p->directory = directory;

    logger->trace("Directory read");
    return 0;
}

int update_directory(partition_t *p){
    int directory_offset = get_offset(p);
    if(lseek(p->fd, directory_offset, SEEK_SET) == -1){
        logger->error("An error occurred when trying to move the head.");
        return -1;
    }

    dir_entry_t* directory = p->directory;
    for (int i = 0; i < p->super_bloc.nb_inodes; i++) {
        if(write(p->fd, (directory+i)->name, 60) == -1){
            logger->error("An error occurred when trying to update the name in your directory");
            return -1;
        }
        if (write(p->fd, &(directory+i)->inode, 4) == -1){
            logger->error("An error occured when trying to update the inode in your directory");
            return -1;
        }
    }

    p->directory = directory;
    logger->trace("Directory updated");
    return 0;
}

int delete_directory(partition_t *p){
    logger->trace("Directory deleted");
    return 0;
}

int insertion_entry(partition_t *p, dir_entry_t dir){
    if(dir.inode > p->super_bloc.nb_inodes){
        logger->error("You are trying to associate a non-existant inode");
        return -1;
    }

    int cmp_letter = 0;
    int index_dir = 0;

    while (index_dir < p->super_bloc.nb_inodes - p->super_bloc.nb_inodes_free && (int) dir.name[cmp_letter] <= (int) (p->directory+index_dir)->name[cmp_letter] && cmp_letter < 60){
        if ((int) dir.name[cmp_letter] == (int) (p->directory->name+index_dir)[cmp_letter]){
            cmp_letter++;
        }else{
            index_dir++;
        }
    }

    if( cmp_letter >= 60){
        logger->error("You're trying to create directory entry with a name already use.");
        return -1;
    }

    dir_entry_t tmp;

    for (int j = index_dir; j < p->super_bloc.nb_inodes - p->super_bloc.nb_inodes_free +1; j++) {
        strcpy(tmp->name, (p->directory+j)->name);
        tmp->inode = (p->directory+j)->inode;
        strcpy((p->directory+j)->name, dir.name);
        (p->directory+j)->inode = dir.inode;
        strcpy(dir.name, tmp->name);
        dir.inode = tmp->inode;
    }

    logger->trace("New directory entry added");
    return 0;
}

int delete_entry(partition_t *p, dir_entry_t dir){
    if(dir.inode > p->super_bloc.nb_inodes){
        logger->error("You're trying to delete a non-existante inode");
        return -1;
    }

    int i = 0;

    while(dir.inode != p->directory[i].inode && i <= p->super_bloc.nb_inodes-p->super_bloc.nb_inodes_free){
        i++;
    }

    if(i >= p->super_bloc.nb_inodes-p->super_bloc.nb_inodes_free){
        logger->error("You're trying to delete a not alloued inode");
        return -1;
    }

    dir_entry_t tmp;

    for (int j = 0; j < p->super_bloc.nb_inodes-p->super_bloc.nb_inodes_free-1; j++) {
        p->directory[j].inode = p->directory[j+1].inode;
        strcpy(p->directory[j].name, p->directory[j+1].name);
    }

    logger->trace("Entry deleted");
    return 0;
}