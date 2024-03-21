/**
 * @file ufs.h
 * @brief The public API of the UNIX File System Simulation library.
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-19-2024
*/

#pragma once

#include <stddef.h>

/**
 * @struct file_t ufs.h
 * @brief This struct represents a file stored in the fs.
 */
typedef struct {
    int size;
} file_t;

/**
 * @struct data_t ufs.h
 * @brief This struct represents a data stored in the fs.
 */
typedef struct{
    int *data;
    int position;
    int size_data;
} data_t;

/**
 * @struct inode_t ufs.h
 * @brief This struct represents an inode stored in the fs.
 */
typedef struct {
    int memory_size_data;
    int last_modification;
    int last_access;
    data_t *bloc1;
    data_t *bloc2;
    data_t *bloc3;
    data_t *bloc4;
    data_t *bloc5;
    data_t *bloc6;
    data_t *bloc7;
    data_t *bloc8;
} inode_t;

/**
 * @struct super_bloc_t ufs.h
 * @brief This struct represents a SuperBlock stored in the fs.
 */
 typedef struct{
     int size;
     int number_bloc;
     int number_bloc_free;
     int number_inode;
     int number_inode_free;
     size_t size_table_node;
     size_t size_table_data;
 } super_bloc_t;

/**
 * @brief Formats the named partition as a new ufs partition with 4Ko blocks.
 * @param partition_name The name of the partition to format.
 * @return 0 if the format went well, -1 if an error occurs
 */
int my_format(char *partition_name);

/**
 * @brief Opens a file based on its name.
 * @param file_name The name of the file to open.
 * @return A struct representing the file.
 */
file_t* my_open(char *file_name);

/**
 * @brief Writes the content in the buffer into the named file.
 * @param f The file where to store the data.
 * @param buffer The buffer to store in the file.
 * @param nb_bytes The number of bytes to write.
 * @return The number of bytes actually written in the file.
 */
int my_write(file_t *f, void *buffer, int nb_bytes);

/**
 * @brief Reads the content of the file and writes it into the buffer.
 * @param f The file where to read the data.
 * @param buffer The buffer where to store the content.
 * @param nb_bytes The number of bytes to read from the file.
 * @return The number of bytes actually read.
 */
int my_read(file_t *f, void *buffer, int nb_bytes);

/**
 * @brief Moves the read/write pointer in the file.
 * @param f The file.
 * @param offset The number of bytes.
 * @param base The base.
 */
void my_seek(file_t *f, int offset, int base);