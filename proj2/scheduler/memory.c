#include "memory.h"


void virtual_memory_alloc() {
	disk = (int*)malloc(sizeof(int) * 0x100000); // 4MB
	memory = (int*)malloc(sizeof(int) * 0x100000); // 4MB

	memory_ffl_size = 0x1000; // 4KB
	lru = (int*)malloc(sizeof(int) * 0x1000); // 4KB
	disk_ffl = (int*)malloc(sizeof(int) * 0x1000); // 4KB
	memory_ffl = (int*)malloc(sizeof(int) * 0x1000); // 4KB

	ptbl1 = (TABLE*)malloc(sizeof(TABLE) * 10);
	ptbl2 = (TABLE*)malloc(sizeof(TABLE) * 10 * 0x40);
	
	memset(lru, 0, malloc_usable_size(lru));
	memset(disk, 0, malloc_usable_size(disk));
	memset(memory, 0, malloc_usable_size(memory));
	memset(disk_ffl, 0, malloc_usable_size(disk_ffl));
	memset(memory_ffl, 0, malloc_usable_size(memory_ffl));

	for (int i = 0; i < 10; i++) {
		ptbl1[i].tn = (int*)malloc(sizeof(int) * 0x40);
		ptbl1[i].valid_bit = (int*)malloc(sizeof(int) * 0x40);
		for (int j = 0; j < 10; j++) {
			ptbl1[i].tn[j] = 0;
			ptbl1[i].valid_bit[j] = 0;
		}
	}

	for (int i = 0; i < 10 * 0x40; i++) {
		ptbl2[i].fn = (int*)malloc(sizeof(int) * 0x40);
		ptbl2[i].state_bit = 0;
		ptbl2[i].valid_bit = (int*)malloc(sizeof(int) * 0x40);
		ptbl2[i].present_bit = (int*)malloc(sizeof(int) * 0x40);
		for (int j = 0; j < 10; j++) {
			ptbl2[i].fn[j] = 0;
			ptbl2[i].valid_bit[j] = 0;
			ptbl2[i].present_bit[j] = 0;
		}
	}
}


void copy_page(int* src, int src_idx, int* src_list, int* dest, int dest_idx, int* dest_list) {
	for (int i = 0; i < 0x100; i++) {
		dest[(dest_idx * 0x100) + i] = src[(src_idx * 0x100) + i];
		src[(src_idx * 0x100) + i] = 0;
	}
	dest_list[dest_idx] = 1;
	src_list[src_idx] = 0;
}


int search_lru(int* ffl) {
	int lru_page = 0;
	int lru_count = 999999999;
	
	for (int i = 0; i < 0x1000; i++) {
		if ((ffl[i] & 0x1) == 1) {
			if (lru[i] < lru_count) {
				lru_page = i;
				lru_count = lru[i];
				lru[i]++;
			}
		}
	} 
	return lru_page;
}


int search_frame(int* ffl, int option) {
	int fn = 0;

	for (int i = 0; i < 0x1000; i++) {
		if ((ffl[i] & 0x1) == 0) {
			if (option == 0) memory_ffl_size--;

			fn = i;
			ffl[i] = 1;
			break;
		}
	}
	return fn;
}


int search_table(TABLE* table) {
	int tn = 0;
	
	for (int i = 0; i < 10 * 0x40; i++) {
		if (ptbl2[tn].state_bit == 0) {
			ptbl2[tn].state_bit = 1;
			break;
		}
		tn++;
	}
	return tn;
}


void MMU(int* va_arr, int idx, int time) {
	int va;
	int disk_addr;
	int ptbl2_tn, fn;
	int ptbl1_pn, ptbl2_pn, offset;
	int data, lru_pn = 0, proc_num = 0;
	FILE* fp = fopen("vm_dump.txt", "a+");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	
	fprintf(fp, "-----------------------------------------\n");
	fprintf(fp, "Time: %04d\n", time);
	fprintf(fp, "-----------------------------------------\n");

	for (int i = 0; i < 10; i++) {
		va = va_arr[i];
		fprintf(fp, "Virtual Address %d: 0x%x\n", i, va);

		if (memory_ffl_size < 0x100) {
			fprintf(fp, "Swap Out [O]: ");
			lru_pn = search_lru(memory_ffl);
			ptbl1_pn = (memory_ffl[lru_pn] >> 26) & 0x3F;
			ptbl2_pn = (memory_ffl[lru_pn] >> 20) & 0x3F;
			proc_num = (memory_ffl[lru_pn] >> 16) & 0xF;

			ptbl2_tn = ptbl1[proc_num].tn[ptbl1_pn];
			ptbl2[ptbl2_tn].present_bit[ptbl2_pn] = 1;

			disk_addr = search_frame(disk_ffl, 1);
			ptbl2[ptbl2_tn].fn[ptbl2_pn] = disk_addr;
			copy_page(memory, lru_pn, memory_ffl, disk, disk_addr, disk_ffl);
			memory_ffl_size++;
			fprintf(fp, "Memory[0x%x ~ 0x%x] -> Disk[0x%x ~ 0x%x]\n", lru_pn * 0x400, ((lru_pn + 1) * 0x400) - 1, disk_addr * 0x400, ((disk_addr + 1) * 0x400) - 1);
		}
		else fprintf(fp, "Swap Out [X]\n");

		ptbl1_pn = (va >> 16) & 0x3F;
		ptbl2_pn = (va >> 10) & 0x3F;
		offset = va & 0x3FF;

		if (ptbl1[idx].valid_bit[ptbl1_pn] == 0) {
			fprintf(fp, "Page Level 1 Fault\n");
			ptbl2_tn = search_table(ptbl2);
			ptbl1[idx].tn[ptbl1_pn] = ptbl2_tn;
			ptbl1[idx].valid_bit[ptbl1_pn] = 1;
		}
		else {
			fprintf(fp, "Page Level 1 Hit\n");
			ptbl2_tn = ptbl1[idx].tn[ptbl1_pn];
		}
		
		if (ptbl2[ptbl2_tn].valid_bit[ptbl2_pn] == 0) {
			fprintf(fp, "Page Level 2 Fault\n");
			fn = search_frame(memory_ffl, 0);
			ptbl2[ptbl2_tn].fn[ptbl2_pn] = fn;
			ptbl2[ptbl2_tn].valid_bit[ptbl2_pn] = 1;

			memory_ffl[fn] += ((ptbl1_pn & 0x3F) << 26);
			memory_ffl[fn] += ((ptbl2_pn & 0x3F) << 20);
			memory_ffl[fn] += ((idx & 0xF) << 16);
		}
		else {
			fprintf(fp, "Page Level 2 Hit\n");
			
			if (ptbl2[ptbl2_tn].present_bit[ptbl2_pn] == 1) {
				fprintf(fp, "Swap In [O]: ");
				fn = search_frame(memory_ffl, 0);
				disk_addr = ptbl2[ptbl2_tn].fn[ptbl2_pn];
				copy_page(disk, disk_addr, disk_ffl, memory, lru_pn, memory_ffl);
				fprintf(fp, "Disk[0x%x ~ 0x%x] -> Memory[0x%x ~ 0x%x]\n", disk_addr * 0x400, ((disk_addr + 1) * 0x400) - 1, lru_pn * 0x400, ((lru_pn + 1) * 0x400) - 1);

				ptbl2[ptbl2_tn].fn[ptbl2_pn] = fn;
				ptbl2[ptbl2_tn].present_bit[ptbl2_pn] = 0;

				memory_ffl[fn] += ((ptbl1_pn & 0x3F) << 26);
				memory_ffl[fn] += ((ptbl2_pn & 0x3F) << 20);
				memory_ffl[fn] += ((idx & 0xF) << 16);
			}
			else {
				fprintf(fp,"Swap In [X]\n");
				fn = ptbl2[ptbl2_tn].fn[ptbl2_pn];
			}

			lru[fn]++;
		}

		fprintf(fp, "%d Memory Address: 0x%x ", i, fn * 0x400 + offset - (offset % 4));

		data = memory[(fn * 0x400 + offset) / 4];
		if (((data >> 31) & 0x1) == 0) {
			memory[(fn * 0x400 + offset) / 4] = (time + 0x80000000);
			fprintf(fp, "Data Write: %d\n\n", time);
		}
		else fprintf(fp, "Data Read: %d\n\n", data - 0x80000000);
	}
	fclose(fp);
}
