/**
* @file ufs.h
 * @brief The public API of the UNIX File System Simulation library.
 * @author Thomas REMY
 * @author Pierre FRANCK-PAPUCHON
 * @version 0.1.0
 * @date 03-19-2024
*/

#pragma once

/**
 * @struct file_t ufs.h
 * @brief This struct represents a file stored in the fs.
 */
typedef struct {
    int size;
} file_t;

/**
 * @struct inode_t ufs.h
 * @brief This struct represents an inode stored in the fs.
 */
typedef struct {
    int memorySizeData;
    int lastModification;
    int lastAccess;
    Data* bloc1;
    Data* bloc2;
    Data* bloc3;
    Data* bloc4;
    Data* bloc5;
    Data* bloc6;
    Data* bloc7;
    Data* bloc8;
}Inode;

/**
 * @struct inode_t ufs.h
 * @brief This struct represents an data stored in the fs.
 */
 typedef struct{
     int* data;
     int position;
     int sizeData;
 }Data;

/**
 * @struct inode_t ufs.h
 * @brief This struct represents an SuperBlock stored in the fs.
 */
 typedef struct{
     int size;
     int numberBloc;
     int numberBlocFree;
     int numberInode;
     int numberInodeFree;
     int size_tableInode;
     int size_tableData;
 }SuperBloc;

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