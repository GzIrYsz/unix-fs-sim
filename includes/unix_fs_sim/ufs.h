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
#include <stdint.h>

/**
 * @def MAGIC_NUMBER The magic number is the serial number of the filesystem. It must me present at the beginning of every partition.
 */
#define MAGIC_NUMBER 0x4F56A901

/**
 * @def NB_DATA_BLOCKS_INODE The number of direct data blocks in an inode.
 */
#define NB_DATA_BLOCKS_INODE 12

#define MAX_FILENAME 60

typedef enum {
    KB, MB, GB
} size_unit_t;

typedef enum {
    SMALL = 1024,
    MEDIUM = 2048,
    LARGE = 4096
} block_size_t;

/**
 * @struct file_t ufs.h
 * @brief Represents an opened file.
 * @var inode The inode of the file.
 * @var offset The position of the read/write head.
 */
typedef struct {
    char name[MAX_FILENAME];
    uint32_t inode;
    uint32_t offset;
} file_t;

/**
 * @struct data_t ufs.h
 * @brief This struct represents a data stored in the fs.
 */
typedef struct{
    uint8_t *data;
    uint32_t position;
    int size_data;
} data_t;

/**
 * @struct inode_t ufs.h
 * @brief This struct represents an inode stored in the fs.
 * @var memory_size_data
 * @var last_modification
 * @var last_access
 * @var
 */
typedef struct {
    uint32_t memory_size_data;
    uint32_t last_modification;
    uint32_t last_access;
    uint32_t file_type;
    uint32_t data_blocks[NB_DATA_BLOCKS_INODE];
} inode_t;

/**
 * @struct super_bloc_t ufs.h
 * @brief This struct represents a SuperBlock stored in the fs.
 * @var magic_number The serial number of the file system, used to determine if the file is a partition.
 * @var block_size The size of the blocks.
 * @var nb_blocks The number of blocks of the partition
 * @var nb_data The number of data blocks
 * @var nb_data_free The number of free data blocks
 * @var nb_inodes The number of inodes
 * @var nb_inodes_free The number of free inodes
 * @var nb_inode_blocks The number of inode blocks
 */
 typedef struct{
     uint32_t magic_number;
     uint32_t block_size;
     uint32_t nb_blocks;
     uint32_t nb_data;
     uint32_t nb_data_free;
     uint32_t nb_inodes;
     uint32_t nb_inodes_free;
     uint32_t nb_inode_blocks;
 } super_bloc_t;

 typedef struct {
     char name[MAX_FILENAME];
     uint32_t inode;
 } dir_entry_t;

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

/**
 * @brief Create a new partition.
 * @param path The path where the partition should be created (absolute or relative).
 * @param size The size of the partion
 * @return 0 if the partition was created, // TODO Finir la doc
 */
int mkpart(char *path, size_t size, size_unit_t unit);

/**
 * @brief Create a file system.
 * @param path The path of the partition where to create the file system.
 * @param block_size The size of the blocks (1024, 2048 or 4096 bytes).
 * @param nb_inodes The ratio of inodes to the data blocks.
 * @return
 */
int mkfs(char *path, block_size_t block_size, uint8_t nb_inodes);

/**
 * @brief Mount a filesystem so it can be used to read and create files.
 * @param path The path of the partition where the filesystem is located.
 * @return 0 if everything went well, -1 otherwise.
 */
int mount(char *path);

size_t size(file_t *f);