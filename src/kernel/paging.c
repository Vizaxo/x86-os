#include "paging.h"

struct p1_entry
pg_mk_p1(bool writable, bool supervisor, bool pwt,
	bool pcd, bool pat , bool global, void* addr,
	uint8_t protection, bool xd) {
	return (struct p1_entry){true, writable , supervisor, pwt,
			pcd, false, false , pat, global,
			((uint64_t)addr >> 11), protection, xd};
}

struct p2_entry
pg_mk_p2_p1(bool writable, bool supervisor, bool pwt,
	bool pcd, void* addr, bool xd) {
	return (struct p2_entry){true, writable, supervisor, pwt, pcd, false,
			false, ((uint64_t)addr >> 11), xd};
}

struct p3_entry
pg_mk_p3_p2(bool writable, bool supervisor, bool pwt,
	bool pcd, void* addr, bool xd) {
	return (struct p3_entry){true, writable,
			supervisor, pwt, pcd, false,
			false, ((uint64_t)addr >> 11), xd};
}

struct p4_entry
pg_mk_p4_p3(bool writable, bool supervisor, bool pwt,
	bool pcd, void* addr, bool xd) {
	return (struct p4_entry){true, writable,
			supervisor, pwt, pcd,
			false, ((uint64_t)addr >> 11), xd};
}

struct uint8_t* pg_get_page(struct p1_entry* pte) {
	return (struct uint8_t*)((uint64_t)pte->addr << 11);
}

struct p1_entry* pg_get_p1(struct p2_entry* pde) {
	return (struct p1_entry*)((uint64_t)pde->addr << 11);
}

struct p2_entry* pg_get_p2(struct p3_entry* pdpte) {
	return (struct p2_entry*)((uint64_t)pdpte->addr << 11);
}

struct p3_entry* pg_get_p3(struct p4_entry* pml4e) {
	return (struct p3_entry*)((uint64_t)pml4e->addr << 11);
}

void flush_page_cache(void) {
	asm("mov %cr3, %rcx");
	asm("mov %rcx, %cr3");
}
