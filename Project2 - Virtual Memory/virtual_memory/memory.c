#include "memory.h"


/*
 * void memory_init()
 *
 * The following function initially allocates the data into the main memory.
 */
void memory_init() {
	uint32_t bin;
	uint32_t counter = 0;
	FILE* fp = fopen("test_prog/input4.bin", "r");
	
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	while (fread(&bin, 1, sizeof(int), fp) == 4) {
		memory[counter / 4] = bin;
		counter += 4;
	} 
	fclose(fp);
}


/*
 * void virtual_memory_alloc()
 *
 * The following function allocates the various queues and nodes that are used to implement the virtual address.
 */
void virtual_memory_alloc() {
	swap = 0;
	tlb_hit = 0;
	tlb_miss = 0;
	ptbl1_hit = 0;
	ptbl2_hit = 0;
	ptbl1_fault = 0;
	ptbl2_fault = 0;
	tlb_access = 0;
	disk_access = 0;
	memory_access = 0;

	fifo = createQueue(); // 4K (2^12)	
	lru = (int*)malloc(sizeof(int) * 0x1000); // 4K (2^12)
	lfu = (int*)malloc(sizeof(int) * 0x1000); // 4K (2^12)
	mfu = (int*)malloc(sizeof(int) * 0x1000); // 4K (2^12)

	memset(lru, 0, malloc_usable_size(lru));
	memset(lfu, 0, malloc_usable_size(lru));
	memset(mfu, 0, malloc_usable_size(lru));

	memory_ffl_size = 0x1000; // 4KB
	disk = (int*)malloc(sizeof(int) * 0x100000); // 4MB (4 bytes * 0x100000)
	memory = (int*)malloc(sizeof(int) * 0x100000); // 4MB (4 bytes * 0x100000)
	disk_ffl = (int*)malloc(sizeof(int) * 0x1000); // 4K (2^12)
	memory_ffl = (int*)malloc(sizeof(int) * 0x1000); // 4K (2^12)
	
	memset(disk, 0, malloc_usable_size(disk));
	memset(memory, 0, malloc_usable_size(memory));
	memset(disk_ffl, 0, malloc_usable_size(disk_ffl));
	memset(memory_ffl, 0, malloc_usable_size(memory_ffl));
	
	map = (int*)malloc(sizeof(int) * 10000);
	ptbl1 = (TABLE*)malloc(sizeof(TABLE) * 10); // set of 10 tables
	ptbl2 = (TABLE*)malloc(sizeof(TABLE) * 10 * 0x40); // number of entries in 64 tables
	tlb = (TLB**)malloc(sizeof(TLB*) * (1 << tlb_size));

	for (int i = 0; i < (1 << tlb_size); i++) {
		tlb[i] = (TLB*)malloc(sizeof(TLB));
		memset(tlb[i], 0, sizeof(TLB));
	}

	for (int i = 0; i < 10; i++) {
		ptbl1[i].tn = (int*)malloc(sizeof(int) * 0x40);
		ptbl1[i].valid_bit = (int*)malloc(sizeof(int) * 0x40);
		for (int j = 0; j < 10; j++) {
			ptbl1[i].tn[j] = 0;
			ptbl1[i].valid_bit[j] = 0;
		}
	}

	for (int i = 0; i < 10 * 0x40; i++) {
		ptbl2[i].state_bit = 0;
		ptbl2[i].fn = (int*)malloc(sizeof(int) * 0x40);
		ptbl2[i].valid_bit = (int*)malloc(sizeof(int) * 0x40);
		ptbl2[i].present_bit = (int*)malloc(sizeof(int) * 0x40);
		for (int j = 0; j < 10; j++) {
			ptbl2[i].fn[j] = 0;
			ptbl2[i].valid_bit[j] = 0;
			ptbl2[i].present_bit[j] = 0;
		}
	}

	FILE* fp = fopen("access_pattern.txt", "r");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	
	char* line;
	char str[1024];
	for (int i = 0; i < 10000; i++) {
		line = fgets(str, 1024, fp);
		map[i] = atoi(line);
	}

	fclose(fp);
}


/*
 * void virtual_memory_free()
 *
 * The following function frees the various queues and nodes that are used to implement the virtual address.
 */
void virtual_memory_free() {
	// free the page level 1
	for (int i = 0; i < 10; i++) {
		free(ptbl1[i].tn);
		free(ptbl1[i].valid_bit);
	}

	// free the page level 2
	for (int i = 0; i < 10 * 0x40; i++) {
		free(ptbl2[i].fn);
		free(ptbl2[i].valid_bit);
		free(ptbl2[i].present_bit);
	}

	// free the TLB
	for (int i = 0; i < (1 << tlb_size); i++) {
		free(tlb[i]);
	}

	free(map);
	free(tlb);
	free(lru);
	free(lfu);
	free(mfu);
	free(disk);
	free(ptbl1);
	free(ptbl2);
	free(memory);
	free(disk_ffl);
	free(memory_ffl);
	removeQueue(fifo);
}


/*
 * void copy_page(int* src, int src_idx, int* src_list, int* dest, int dest_idx, int* dest_list)
 *
 * The following function swaps the pages between the memory and the disk.
 * After swapping, it initialize the pages into the empty pages.
 */
void copy_page(int* src, int src_idx, int* src_list, int* dest, int dest_idx, int* dest_list) {
	for (int i = 0; i < 0x100; i++) {
		dest[(dest_idx * 0x100) + i] = src[(src_idx * 0x100) + i];
		src[(src_idx * 0x100) + i] = 0; // initialize the swaped memory to zero
	}
	dest_list[dest_idx] = 1;
	src_list[src_idx] = 0;
}


/*
 * int search_random(int* ffl)
 *
 * Selects the page to be replaced according to the Random policy.
 * Random policy selects the next page to be replaced randomly.
 */
int search_random() {
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	srand((unsigned int)time(NULL));
	return rand() % 0x1000;
}


/*
 * int search_fifo(Queue* q)
 *
 * Selects the page to be replaced according to the FIFO policy.
 * FIFO policy selects the next page to be replaced which is used most lately.
 */
int search_fifo(Queue* q) {
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	Node* node = dequeue(q);
	return node->fn;
}


/*
 * int search_lru(int* ffl)
 *
 * Selects the page to be replaced according to the LRU policy.
 * LRU policy selects the next page to be replaced which is used least recently.
 */
int search_lru(int* ffl) {
	int lru_page = 0;
	
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	for (int i = 0; i < 0x1000; i++) {
		if ((ffl[i] & 0x1) == 1) {
			if (lru[i] < lru[lru_page]) lru_page = i;
		}
	} 
	return lru_page;
}


/*
 * int search_lfu(int* ffl)
 *
 * Selects the page to be replaced according to the LFU policy.
 * LFU policy selects the next page to be replaced which is used least frequently.
 */
int search_lfu(int* ffl) {
	int lfu_page = 0;
	
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	for (int i = 0; i < 0x1000; i++) {
		if ((ffl[i] & 0x1) == 1) {
			if (lfu[i] < lfu[lfu_page]) lfu_page = i;
		}
	}
	
	lfu[lfu_page] = 0;
	return lfu_page;
} 


/*
 * int search_mfu(int* ffl)
 *
 * Selects the page to be replaced according to the MFU policy.
 * MFU policy selects the next page to be replaced which is used most frequently.
 */
int search_mfu(int* ffl) {
	int mfu_page = 0;
	
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	for (int i = 0; i < 0x1000; i++) {
		if ((ffl[i] & 0x1) == 1) {
			if (mfu[i] > mfu[mfu_page]) mfu_page = i;
		}
	}

	mfu[mfu_page] = 0;
	return mfu_page;
}


/*
 * int search_sca(Queue* q)
 *
 * Selects the page to be replaced according to the SCA policy.
 * SCA policy selects the next page to be replaced which is used most lately,
 * but give one more chance for the selection.
 */
int search_sca(Queue* q) {
	Node* node = dequeue(q);
	
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	while (1) {
		if (node->sca == 1) {
			enqueue(q, 0, 0, 0, node->fn, 0, node->esca);
			node = dequeue(q);
		}
		else {
			return node->fn;
		}
	}
}

/*
 * int search_esca(Queue* q)
 *
 * Selects the page to be replaced according to the ESCA policy.
 * SCA policy selects the next page to be replaced which is used most lately,
 * but give three more chance for the selection.
 */
int search_esca(Queue* q) {
	Node* node = dequeue(q);
	
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	while (1) {
		if (node->esca > 0) {
			enqueue(q, 0, 0, 0, node->fn, node->sca, node->esca - 1);	
			node = dequeue(q);
		}
		else {
			return node->fn;
		}
	}
}


/*
 * int search_frame(int* ffl, int option)
 *
 * The following function searches the free-frame from the given free-frame list.
 * It returns the frame number of the free-frame. 
 */
int search_frame(int* ffl, int option) {
	int fn = 0;
	
	// free-frame list entry: 
	// level 1 page number(6bits), level 2 page number(6bits), process number(4bits), empty bits(15bits), state bit(1bit)
	for (int i = 0; i < 0x1000; i++) {
		// distinguish the frame usage with state bit
		if ((ffl[i] & 0x1) == 0) {
			// if free frame is found, decrease the size of the list.
			if (option == 0) memory_ffl_size--;
			ffl[i] = 1;
			fn = i;
			break;
		}
	}
	return fn;
}


/*
 * int search_table(TABLE* table)
 *
 * The following function searches the page table level 2 that is not allocated to the page table level 1.
 * It returns the table number of the allocated page table level 2.
 */
int search_table(TABLE* table) {
	int tn = 0;
	
	// search the free page from the table
	for (int i = 0; i < 10 * 0x40; i++) {
		if (ptbl2[tn].state_bit == 0) {
			ptbl2[tn].state_bit = 1;
			break;
		}
		tn++;
	}
	return tn;
}


/*
 * void MMU(int* va_arr, int idx, int time)
 *
 * MMU (Memoy Mapped Unit) function performs the following sequence of operations.
 * 1. Perform swap in and out according to the state of the memory state.
 * 2. Divide the virtual address into three segment -> page number of page table level 1 and level 2, and offset.
 * 3. Check the hit and fault of the page table level 1.
 * 4. Check the hit and fault of the page table level 2.
 * 5. Perform read and write operation of the memory by using physical address.
 */
void MMU(int* va_arr, int idx, int time) {
	if (time == 10000 && flag) return;
	if (time == 10000) flag = 1;

	int va;
	FILE* fp;
	int disk_addr;
	int ptbl2_tn, fn;
	int key, tlb_idx = 0;
	int ptbl1_pn, ptbl2_pn, offset;
	int data, swap_pn = 0, proc_num = 0;
	
	switch (set_replacement) {
	case 1: fp = fopen("results/random_dump.txt", "a+"); break;
	case 2: fp = fopen("results/fifo_dump.txt", "a+"); break;
	case 3: fp = fopen("results/lru_dump.txt", "a+"); break;
	case 4: fp = fopen("results/lfu_dump.txt", "a+"); break;
	case 5: fp = fopen("results/mfu_dump.txt", "a+"); break;
	case 6: fp = fopen("results/sca_dump.txt", "a+"); break;
	case 7: fp = fopen("results/esca_dump.txt", "a+"); break;
	default:
		perror("replacement");
		exit(EXIT_FAILURE);
	}

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

		// 1. perform swap in and out according to the state of the memory state.
		if (memory_ffl_size < 0x100) {
			swap += 1;
			memory_access += 100;
			fprintf(fp, "Swap Out [O]: ");
			
			switch(set_replacement) {	
			case 1: swap_pn = search_random(memory_ffl); break;
			case 2: swap_pn = search_fifo(fifo); break;
			case 3: swap_pn = search_lru(memory_ffl); break;
			case 4: swap_pn = search_lfu(memory_ffl); break;
			case 5: swap_pn = search_mfu(memory_ffl); break;
			case 6: swap_pn = search_sca(fifo); break;
			case 7: swap_pn = search_esca(fifo); break;
			default:
				perror("replacement");
				exit(EXIT_FAILURE);
			}

			ptbl1_pn = (memory_ffl[swap_pn] >> 26) & 0x3F;
			ptbl2_pn = (memory_ffl[swap_pn] >> 20) & 0x3F;
			proc_num = (memory_ffl[swap_pn] >> 16) & 0xF;

			ptbl2_tn = ptbl1[proc_num].tn[ptbl1_pn];
			ptbl2[ptbl2_tn].present_bit[ptbl2_pn] = 1;
			
			memory_ffl_size++;
			disk_access += 2000000 + 3340;
			disk_addr = search_frame(disk_ffl, 1);
			ptbl2[ptbl2_tn].fn[ptbl2_pn] = disk_addr;
			copy_page(memory, swap_pn, memory_ffl, disk, disk_addr, disk_ffl);
			fprintf(fp, "Memory[0x%x ~ 0x%x] -> Disk[0x%x ~ 0x%x]\n", swap_pn * 0x400, ((swap_pn + 1) * 0x400) - 1, disk_addr * 0x400, ((disk_addr + 1) * 0x400) - 1);
		}
		else fprintf(fp, "Swap Out [X]\n");

		// 2. divide the virtual address into three segment -> page number of page table level 1 and level 2, and offset
		ptbl1_pn = (va >> 16) & 0x3F;
		ptbl2_pn = (va >> 10) & 0x3F;
		offset = va & 0x3FF;

		// check TLB
		if (tlb_size) {
			tlb_access += 1;
			key = (va >> 10) & 0xFFF;
			tlb_idx = (va >> (22 - tlb_size)) & ((1 << tlb_size) - 1);
		}
		if (tlb_size && tlb[tlb_idx]->valid && key == tlb[tlb_idx]->key) {
			tlb_hit++;
			fn = tlb[tlb_idx]->value;
			fprintf(fp, "TLB Hit\n");
		}
		else {
			if (tlb_size) {
				tlb_miss++;
				tlb[tlb_idx]->valid = 1;
				tlb[tlb_idx]->key = key;
				fprintf(fp, "TLB Miss\n");
			}
	
			// 3. fault of the page table level 1
			if (ptbl1[idx].valid_bit[ptbl1_pn] == 0) {
				ptbl1_fault++;
				memory_access += 100;
				fprintf(fp, "Page Level 1 Fault\n");
				ptbl2_tn = search_table(ptbl2);
				ptbl1[idx].tn[ptbl1_pn] = ptbl2_tn;
				ptbl1[idx].valid_bit[ptbl1_pn] = 1;
			}
			// 3. hit of the page table level 1
			else {
				ptbl1_hit++;
				memory_access += 100;
				fprintf(fp, "Page Level 1 Hit\n");
				ptbl2_tn = ptbl1[idx].tn[ptbl1_pn];
			}
			
			// 3. fault of the page table level 2
			if (ptbl2[ptbl2_tn].valid_bit[ptbl2_pn] == 0) {
				ptbl2_fault++;
				memory_access += 100;
				fprintf(fp, "Page Level 2 Fault\n");
	
				// search for the free frame
				fn = search_frame(memory_ffl, 0);
				ptbl2[ptbl2_tn].fn[ptbl2_pn] = fn;
				ptbl2[ptbl2_tn].valid_bit[ptbl2_pn] = 1;
	
				// store the page number of page table level 1 and level 2, and offset into the free-frame list of the memory
				memory_ffl[fn] += ((ptbl1_pn & 0x3F) << 26);
				memory_ffl[fn] += ((ptbl2_pn & 0x3F) << 20);
				memory_ffl[fn] += ((idx & 0xF) << 16);
			}
			// 3. hit of the page table level 2
			else {
				fprintf(fp, "Page Level 2 Hit\n");
				
				// 1. perform swap in and out according to the state of the memory state.
				if (ptbl2[ptbl2_tn].present_bit[ptbl2_pn] == 1) {
					swap += 1;
					ptbl2_fault++;
					memory_access += 100;
					fprintf(fp, "Swap In [O]: ");
					disk_access += 2000000 + 3340;
					fn = search_frame(memory_ffl, 0);
					disk_addr = ptbl2[ptbl2_tn].fn[ptbl2_pn];
					copy_page(disk, disk_addr, disk_ffl, memory, swap_pn, memory_ffl);
					fprintf(fp, "Disk[0x%x ~ 0x%x] -> Memory[0x%x ~ 0x%x]\n", disk_addr * 0x400, ((disk_addr + 1) * 0x400) - 1, swap_pn * 0x400, ((swap_pn + 1) * 0x400) - 1);
	
					ptbl2[ptbl2_tn].fn[ptbl2_pn] = fn;
					ptbl2[ptbl2_tn].present_bit[ptbl2_pn] = 0;
	
					memory_ffl[fn] += ((ptbl1_pn & 0x3F) << 26);
					memory_ffl[fn] += ((ptbl2_pn & 0x3F) << 20);
					memory_ffl[fn] += ((idx & 0xF) << 16);
				}
				else {
					ptbl2_hit++;
					memory_access += 100;
					fprintf(fp,"Swap In [X]\n");
					fn = ptbl2[ptbl2_tn].fn[ptbl2_pn];
				}
			}
			if (tlb_size) tlb[tlb_idx]->value = fn;
		}
	
		lfu[fn]++;
		mfu[fn]++;
		lru[fn] = time;
		if (searchQueue(fifo, fn)) enqueue(fifo, 0, 0, 0, fn, 1, 3);
		else enqueue(fifo, 0, 0, 0, fn, 1, 3);

		fprintf(fp, "%d Memory Address: 0x%x ", i, fn * 0x400 + offset - (offset % 4));
		
 		// 5. perform read and write operation of the memory by using physical address.
		memory_access += 100;
		data = memory[(fn * 0x400 + offset) / 4];
		if (((data >> 31) & 0x1) == 0) {
			memory[(fn * 0x400 + offset) / 4] = (time + 0x80000000);
			fprintf(fp, "Data Write: %d\n\n", time);
		}
		else fprintf(fp, "Data Read: %d\n\n", data - 0x80000000);
	}

	if (flag == 1) {
		fprintf(fp, "-----------------------------------------\n");
		fprintf(fp, "Result\n");
		fprintf(fp, "Swap: %d\n", swap);
		fprintf(fp, "TLB Access Time: %ldns\n", tlb_access);
		fprintf(fp, "Disk Access Time: %ldns\n", disk_access);
		fprintf(fp, "Memory Access Time: %ldns\n\n", memory_access);
		fprintf(fp, "TLB Hit Rate: %.3f%%\n", tlb_hit / (tlb_hit + tlb_miss) * 100);
		fprintf(fp, "Page Table Level 1 Hit Rate: %.3f%%\n", ptbl1_hit / (ptbl1_hit + ptbl1_fault) * 100); 
		fprintf(fp, "Page Table Level 2 Hit Rate: %.3f%%\n", ptbl2_hit / (ptbl2_hit + ptbl2_fault) * 100); 
		fprintf(fp, "-----------------------------------------\n");
	}
	fclose(fp);
}


