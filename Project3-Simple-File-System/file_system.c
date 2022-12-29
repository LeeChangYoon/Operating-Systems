#include "file_system.h"

void print_super_block(struct super_block s) {
    fprintf(OUT_STREAM, "[Super Block]\n\n");
    usleep(50000);
	fprintf(OUT_STREAM, "partition_type\t\t : %#x (%d)\n", s.partition_type, s.partition_type);
    usleep(50000);
	fprintf(OUT_STREAM, "inode_size\t\t : %#x (%d)\n", s.inode_size, s.inode_size);
    usleep(50000);
    fprintf(OUT_STREAM, "block_size\t\t : %#x (%d)\n", s.block_size, s.block_size);
    usleep(50000);
    fprintf(OUT_STREAM, "first_inode\t\t : %#x (%d)\n", s.first_inode, s.first_inode);
    usleep(50000);
    fprintf(OUT_STREAM, "num_inodes\t\t : %#x (%d)\n", s.num_inodes, s.num_inodes);
    usleep(50000);
    fprintf(OUT_STREAM, "num_inode_blocks\t : %#x (%d)\n", s.num_inode_blocks, s.num_inode_blocks);
    usleep(50000);
    fprintf(OUT_STREAM, "num_free_inodes\t\t : %#x (%d)\n", s.num_free_inodes, s.num_free_inodes);
    usleep(50000);
    fprintf(OUT_STREAM, "num_blocks\t\t : %#x (%d)\n", s.num_blocks, s.num_blocks);
    usleep(50000);
    fprintf(OUT_STREAM, "num_free_blocks\t\t : %#x (%d)\n", s.num_free_blocks, s.num_free_blocks);
    usleep(50000);
    fprintf(OUT_STREAM, "first_data_block\t : %#x (%d)\n", s.first_data_block, s.first_data_block);
    usleep(50000);
    fprintf(OUT_STREAM, "volume_name\t\t : %s\n", s.volume_name);
    fprintf(OUT_STREAM, "%s\n", s.padding);

    usleep(1000000);
    for (int i = 0; i < 20; i++) {
        fprintf(OUT_STREAM, ". ");
    }

    fprintf(OUT_STREAM, "\n\n");
}

void print_inode(struct inode i, int idx) {
    fprintf(OUT_STREAM, "[Inode %d]\n\n", idx);
	fprintf(OUT_STREAM, "mode\t\t\t : %#x (%d)\n", i.mode, i.mode);
	fprintf(OUT_STREAM, "locked\t\t\t : %#x (%d)\n", i.locked, i.locked);
	fprintf(OUT_STREAM, "date\t\t\t : %#x (%d)\n", i.date, i.date);
	fprintf(OUT_STREAM, "size\t\t\t : %#x (%d)\n", i.size, i.size);
	fprintf(OUT_STREAM, "indirect_block\t : %#x (%d)\n", i.indirect_block, i.indirect_block);
	fprintf(OUT_STREAM, "blocks[0]\t\t : %#x (%d)\n", i.blocks[0], i.blocks[0]);
    fprintf(OUT_STREAM, "blocks[1]\t\t : %#x (%d)\n", i.blocks[1], i.blocks[1]);
    fprintf(OUT_STREAM, "blocks[2]\t\t : %#x (%d)\n", i.blocks[2], i.blocks[2]);
    fprintf(OUT_STREAM, "blocks[3]\t\t : %#x (%d)\n", i.blocks[3], i.blocks[3]);
    fprintf(OUT_STREAM, "blocks[4]\t\t : %#x (%d)\n", i.blocks[4], i.blocks[4]);
    fprintf(OUT_STREAM, "blocks[5]\t\t : %#x (%d)\n\n\n", i.blocks[5], i.blocks[5]);
}

FILE* mount(char* disk_img_name, SuperBlock** super_block, Inode** inode_table) {

    FILE* fp;

    if ((fp = fopen(disk_img_name , "rb")) == NULL) {
        fprintf(ERR_STREAM, "Failed to open file\n");
    }

    *super_block = malloc_s(sizeof(**super_block), __func__);

    fread(*super_block, sizeof(**super_block), 1, fp);

    print_super_block(**super_block);

    *inode_table = malloc_s(sizeof(**inode_table) * (*super_block)->num_inodes, __func__);

    for (int i = 0; i < (*super_block)->num_inodes; i++) {
        fread(&((*inode_table)[i]), sizeof(**inode_table), 1, fp);

#ifdef DEBUG_MODE
        print_inode((*inode_table)[i], i);
#endif
    }

    return fp;
}

// void print_blocks(union read_block b, char is_dir) {
//     if (is_dir) {
//         for (int i = 0; i < 32; i++) {
//             fprintf(OUT_STREAM, "\ninode\t\t\t : %x\n", b.d[i].inode);
//            	fprintf(OUT_STREAM, "dir_length\t\t : %x\n", b.d[i].dir_length);
// 	        fprintf(OUT_STREAM, "name_len\t\t : %x\n", b.d[i].name_len);
// 	        fprintf(OUT_STREAM, "file_type\t\t : %x\n", b.d[i].file_type);
//             fprintf(OUT_STREAM, "name\t\t\t : %s\n", b.d[i].name);
//         }
//     } else {
//         for (int i = 0; i < 1024; i += 2) {
//             if (i % 0x10 == 0) {
//                 fprintf(OUT_STREAM, "\n");
//             }
            
//             fprintf(OUT_STREAM, "%.2x%.2x ", b.b.d[i], b.b.d[i+1]);
//         }
        
//         fprintf(OUT_STREAM, "\n\n");

//         for (int i = 0; i < 1024; i++) {            
//             fprintf(OUT_STREAM, "%c", b.b.d[i]);
//         }

//     }

//     fprintf(OUT_STREAM, "\n\n");
// }

char* current_dir_to_string(Directory* current_directory, int len) {

    char* path = malloc_s(len, __func__);

    int idx = 0;

    strcpy(path, current_directory->directory[idx]);
    idx++;

    while (current_directory->directory[idx] != -1) {
        strcat(path, current_directory->directory[idx]);
        idx++;
    }

    return path;
}

void* read_block(Inode inode, MountEnv* mount_env) {
    int block_size = mount_env->super_block->block_size;
    int block_index = inode.size / block_size;

    int is_dir = ((inode.mode >> 12) << 12) == INODE_MODE_DIR_FILE;
    //int permission = inode.mode ^ is_dir;

    if (is_dir) {
        mount_env->dir_files = malloc(sizeof(*(mount_env->dir_files)));
        mount_env->dir_files->size = 0;

        for (int i = 0; i <= block_index; i++) {
            int read_point = mount_env->block_starting_point + i * block_size;

            if (read_point > mount_env->block_finishing_point) {
                fprintf(stderr, "ERROR : Read outside partition area");
                exit(-1);
            }

            fseek(mount_env->disk_img, read_point, SEEK_SET);

            while (ftell(mount_env->disk_img) < read_point + mount_env->super_block->block_size) {
                unsigned int inode_num;
                unsigned int dir_length;
                unsigned int name_len;
                unsigned int file_type;

                fread(&inode_num, sizeof(inode_num), 1, mount_env->disk_img);
                fread(&dir_length, sizeof(dir_length), 1, mount_env->disk_img);
                fread(&name_len, sizeof(name_len), 1, mount_env->disk_img);
                fread(&file_type, sizeof(file_type), 1, mount_env->disk_img);

                // if (inode_num < mount_env->super_block->first_inode) {
                //     continue;
                // }

                if (inode_num * dir_length * name_len * file_type == 0) {
                    continue;
                }

                int dentry_name_length = (name_len / 16 + 1) * 16;

                char* directory_name = malloc(sizeof(*directory_name) * dentry_name_length);

                fread(directory_name, dentry_name_length, 1, mount_env->disk_img);

                int hash = hash_func(directory_name, name_len);

                Pair* pair = (Pair*)malloc_s(sizeof(*pair), __func__);
                pair->inode = inode_num;
                pair->hash = hash;
                strcpy(pair->name, directory_name);

                mount_env->dir_files->file_list[mount_env->dir_files->size] = directory_name;
                mount_env->dir_files->size++;

                if (mount_env->dir_files->size >= 1000) {
                    // need realloc
                    break;
                }

                insert_node(mount_env->set, pair);
            }
        }

        return mount_env;
    } else {
        int read_point = mount_env->block_starting_point + inode.blocks[0] * block_size;

        if (read_point > mount_env->block_finishing_point) {
            fprintf(stderr, "ERROR : Read outside partition area");
            exit(-1);
        }

        fseek(mount_env->disk_img, read_point, SEEK_SET);

        Block* blocks = malloc(sizeof(*blocks) * (block_index + 1));

        for (int i = 0; i <= block_index; i++) {
            fread(blocks + i, sizeof(*blocks), 1, mount_env->disk_img);
        }

        return blocks;
    }

    return NULL;
}