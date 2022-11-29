#include "memory.h"


void memory_to_list() {
	level1 = (TABLE*)malloc(sizeof(TABLE) * 0x400);
	level2 = (TABLE*)malloc(sizeof(TABLE) * 0x400 * 0x400);

	memory_frame_list_size = 0x400;
	lru = (int*)malloc(sizeof(int) * 0x400);
	disk = (int*)malloc(sizeof(int) * 0x80000);
	memory = (int*)malloc(sizeof(int) * 0x80000);
	disk_frame_list = (int*)malloc(sizeof(int) * 0x400);
	memory_frame_list = (int*)malloc(sizeof(int) * 0x400);

	memset(disk, 0, sizeof(int) * 0x80000);
	memset(memory, 0, sizeof(int) * 0x80000);
	memset(disk_frame_list, 0, sizeof(int) * 0x400);
	memset(memory_frame_list, 0, sizeof(int) * 0x400);

	for (int i = 0; i < 10; i++) {
		level1[i].valid_bit = (int*)malloc(sizeof(int) * 32);
		level1[i].table_address = (int*)malloc(sizeof(int) * 32);
	
		for (int j = 0; j < 32; j++) {
			level1[i].valid_bit[j] = 0;
			level1[i].table_address[j] = 0;
		}	
	}

	for (int i = 0; i < 10 * 0x400; i++) {
		level2[i].address = (int*)malloc(sizeof(int) * 32);	
		level2[i].swap_bit = (int*)malloc(sizeof(int) * 32);	
		level2[i].valid_bit = (int*)malloc(sizeof(int) * 32);
		
		for (int j = 0; j < 32; j++) {
			level2[i].address[j] = 0;
			level2[i].swap_bit[j] = 0;
			level2[i].valid_bit[j] = 0;
		}
	}
}


int find_lru_page(int* list) {
	int lru_page = 0;
	int* free_list = list;
	int lru_count = 999999;
	
	for (int i = 0; i < 0x400; i++) {
		if ((free_list[i] & 0x1) == 1) {
			if (lru[i] < lru_count) {
				lru_page = i;
				lru_count = lru[i];
				lru[i]++;
			}
		}
	} 
	return lru_page;
}


int find_free_table(TABLE* table) {
	int free_page = 0;
	
	for (int i = 0; i < 10 * 0x400; i++) {
		if (level2[free_page].level2_valid_bit == 0) {
			level2[free_page].level2_valid_bit = 1;
			break;
		}
		free_page++;
	}
	return free_page;
}


void MMU(int idx, int* virtual_address, int time) {
	int offset;
	int lru_idx;
	int free_frame;
	int level1_idx;
	int level2_idx;
	int memory_idx;
	int process_idx;
	int level2_page = 0;
	int swap_disk_address;
	int virtual_address_buffer;	
	
	printf("Time: %d\n", time);
	printf("--------------------------------------------\n");
	for (int i = 0; i < 10; i++) {
		if (memory_frame_list_size < 2) {
			printf("Swap Out\n");
			lru_idx = find_lru_page(memory_frame_list);
			level1_idx = (memory_frame_list[lru_idx] >> 27) & 0x1F;
			level2_idx = (memory_frame_list[lru_idx] >> 22) & 0x1F;
			process_idx = (memory_frame_list[lru_idx] >> 18) & 0xF;
			
			level2[level2_page].swap_bit[level2_idx] = 1;
			free_frame = find_free_frame(disk_frame_list, 1);
			level2[level2_page].address[level2_idx] = free_frame;
			copy_page(disk, free_frame, disk_frame_list, memory, lru_idx, memory_frame_list);
			memory_frame_list_size++;
			printf("Data Move: Memory[0x%x - 0x%x] -> Disk[0x%x - 0x%x]\n\n", (lru_idx * 0x400), (((lru_idx + 1) * 0x400) - 1), (free_frame * 0x400), (((free_frame + 1) * 0x400) - 1));
		}

		virtual_address_buffer = virtual_address[i];
		printf("vitual address[%d] = 0x%x\n", i, virtual_address_buffer);
		level1_idx = (virtual_address_buffer >> 15) & 0x1F;
		level2_idx = (virtual_address_buffer >> 10) & 0x1F;
		offset = virtual_address_buffer & 0x3FF;

		// level 1 page
		if (level1[idx].valid_bit == 0) {
			printf("level 1 miss\n");
			level2_page = find_free_table(level2);
			level1[idx].valid_bit[level1_idx] = 1;
			level1[idx].table_address[level1_idx] = level2_page;
		}
		else {		
			printf("level 1 hit\n");
			level2_page = level1[idx].table_address[level1_idx];	
		}

		// level 2 page
		if (level2[level2_page].valid_bit[level2_idx] == 0) {
			printf("level 2 miss\n");
			free_frame = find_free_frame(memory_frame_list, 0);
			level2[level2_page].valid_bit[level2_idx] = 1;
			level2[level2_page].address[level2_idx] = free_frame;

			memory_frame_list[free_frame] += ((level1_idx & 0x1F) >> 27);
			memory_frame_list[free_frame] += ((level2_idx & 0x1F) >> 22);
			memory_frame_list[free_frame] += ((process_idx & 0xF) >> 18);
		}
		else {
			printf("level 2 hit\n");
			if (level2[level2_page].swap_bit[level2_idx] == 1) {
				printf("swap in\n");
				free_frame = find_free_frame(memory_frame_list, 0);
				swap_disk_address = level2[level2_page].address[level2_idx];
				copy_page(memory, free_frame, memory_frame_list, disk, swap_disk_address, disk_frame_list);

				level2[level2_page].swap_bit[level2_idx] = 0;
				level2[level2_page].address[level2_idx] = free_frame;
				
				memory_frame_list[free_frame] += ((level1_idx & 0x1F) >> 27);
				memory_frame_list[free_frame] += ((level2_idx & 0x1F) >> 22);
				memory_frame_list[free_frame] += ((idx & 0xF) >> 18);
			}
			else {
				free_frame = level2[level2_page].address[level2_idx];
			}
			lru[free_frame]++;
		}

		memory_idx = memory[((free_frame * 0x400) + offset) / 4];
		if (((memory_idx >> 31) & 0x1) == 0) {
			memory[((free_frame * 0x400) + offset) / 4] = (time + 0x80000000);
		}
	}
}


int find_free_frame(int* free_frame_list, int option) {
	int free_frame = 0;
	int* free_list = free_frame_list;

	for (int i = 0; i < 0x400; i++) {
		if ((free_list[i] & 0x1) == 0) {
			if (option == 0) memory_frame_list_size--;

			lru[i]++;
			free_list[i] = 1;
			free_frame = i;
			break;
		}
	}
	return free_frame;
}


void copy_page(int* tar1, int tar1_idx, int* tar1_list, int* tar2, int tar2_idx, int* tar2_list) {
	for (int i = 0; i < 0x100; i++) {
		tar1[(tar1_idx * 0x100) + i] = tar2[(tar2_idx * 0x100) + i];
		tar2[(tar2_idx * 0x100) + i] = 0;
	}
	tar1_list[tar1_idx] = 1;
	tar2_list[tar2_idx] = 0;
}
