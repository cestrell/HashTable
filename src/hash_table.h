/* Hash table implememtation in C */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define HT_BASE_SIZE 53
#define HT_PRIME_1 13
#define HT_PRIME_2 7


typedef struct {
	char* key;
	char* value;
} ht_pair;

typedef struct {
	int base_size;
	int size;
	int count;
	ht_pair** items;
} hash_table;

static ht_pair DELETED = { nullptr, nullptr };

void ht_insert(hash_table* ht, const char* key, const char* value);

char* ht_search(hash_table* ht, const char* key);

void ht_delete(hash_table* ht, const char* key);
