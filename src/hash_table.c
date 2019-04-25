/* Hash table implementation in C */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "prime.h"


static ht_pair* new_pair(const char* k, const char* v) {
	ht_pair* pair = malloc(sizeof(ht_pair));
	pair->key = strdup(k);
	pair->value = strdup(v);
	return pair;
}

static hash_table* new_ht_size(const int base_size) {
	hash_table* ht = xmalloc(sizeof(hash_table));
	ht->base_size = base_size;

	ht->size = next_prime(ht->base_size);
	ht->count = 0;
	ht->items = xcalloc((size_t)ht->size, sizeof(ht_pair*));
	return ht;
}

hash_table* new_ht() {
	return new_ht_size(HT_BASE_SIZE);
}

static void del_pair(ht_pair* pair) {
	free(pair->key);
	free(pair->value);
	free(pair);
}

void del_ht(hash_table* ht) {
	for (int i = 0; i < ht->size; i++) {
		ht_pair* temp = ht->items[i];
		if (temp != NULL) {
			del_pair(temp);
		}
	}
	free(ht->items);
	free(ht);
}

static int hash(const char* s, const int a, const int m) {
	long hash = 0;
	const int str_len = strlen(s);
	for (int i = 0; i < str_len; ++i) {
		hash += (long)pow(a, str_len - (i + 1)) * s[i];
		hash = hash % m;
	}
	return (int)hash;
}

static int get_hash(const char* s, const int num_buckets, const int attempt) {
	const int hash_a = hash(s, HT_PRIME_1, num_buckets);
	const int hash_b = hash(s, HT_PRIME_2, num_buckets);
	return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(hash_table* ht, const char* key, const char* value) {
	ht_pair* pair = new_pair(key, value);
	int index = get_hash(pair->key, ht->size, 0);

	ht_pair* current = ht->items[index];
	int i = 1;
	while (current != NULL) {
		if (current != &DELETED) {
			if (strcmp(current->key, key) == 0) {
				del_pair(current);
				ht->items[index] = pair;
				return;
			}
		}
		index = get_hash(pair->key, ht->size, i);
		current = ht->items[index];
		i++;
	}
	ht->items[index] = pair;
	ht->count++;
}

char* ht_search(hash_table* ht, const char* key) {
	int index = get_hash(key, ht->size, 0);
	ht_pair* pair = ht->items[index];
	int i = 1;
	while (pair != NULL) {
		if (pair != &DELETED) {
			if (strcmp(pair->key, key) == 0) {
				return pair->value;
			}
		}
		index = get_hash(key, ht->size, i);
		pair = ht->items[index];
		i++;
	}
	return NULL;
}

void ht_delete(hash_table* ht, const char* key) {
	int index = get_hash(key, ht->size, 0);
	ht_pair* pair = ht->items[index];
	int i = 1;
	while (pair != NULL) {
		if (pair != &DELETED) {
			if (strcmp(pair->key, key) == 0) {
				del_pair(pair);
				ht->items[index] = &DELETED;
			}
		}
		index = get_hash(pair->key, ht->size, i);
		pair = ht->items[index];
		i++;
	}
	ht->count--;
}

static void ht_resize(hash_table* ht, const int base_size) {
	if (base_size < HT_BASE_SIZE) return;

	hash_table* new_table = new_ht_size(base_size);
	for (int i = 0; i < ht->size; ++i) {
		ht_pair* current = ht->items[i];
		if (current != NULL && current != &DELETED) {
			ht_insert(new_table, current->key, current->value);
		}
	}

	ht->base_size = new_table->base_size;
	ht->count = new_table->count;

	// To delete new_ht, we give it ht's size and items
	const int tmp_size = ht->size;
	ht->size = new_table->size;
	new_table->size = tmp_size;

	ht_pair** tmp_items = ht->items;
	ht->items = new_table->items;
	new_table->items = tmp_items;

	del_ht(new_table);

}
