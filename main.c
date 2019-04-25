// main.c

#include "src/hash_table.h"

int main() {
	hash_table* ht = ht_new();
	ht_insert(ht, "name", "carlos");
	ht_delete(ht, "name");
	del_ht(ht);
}