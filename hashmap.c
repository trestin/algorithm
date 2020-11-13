#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INVALID_VAL (~0)

typedef struct {
    int key;
    int value;
    int next;
} MyHashSlot;

typedef struct {
    int size;
    int lastfree;
    MyHashSlot* slot;
} MyHashMap;

/** Initialize your data structure here. */

MyHashMap* myHashMapCreate() {
    int i;
    MyHashMap* obj = malloc(sizeof(MyHashMap));
    obj->size = 16;
    obj->lastfree = obj->size - 1;
    obj->slot = malloc(sizeof(MyHashSlot) * obj->size);
    for (i = 0; i < obj->size; ++i) {
        MyHashSlot *s = &obj->slot[i];
        s->key = INVALID_VAL;
        s->value = INVALID_VAL;
        s->next = -1;
    }
    return obj;
}

MyHashSlot* mainPosition(MyHashMap* obj, int key) {
    int hash = key & (obj->size - 1);
    return &obj->slot[hash];
}

void myHashMapPut(MyHashMap* obj, int key, int value);
void rehash(MyHashMap* obj) {
    int i;
    int old_size = obj->size;
    MyHashSlot* old_slot = obj->slot;
    obj->size = 2 * old_size;
    obj->lastfree = obj->size - 1;
    obj->slot = malloc(sizeof(MyHashSlot) * obj->size);
    for (i = 0; i < obj->size; ++i) {
        MyHashSlot *s = &obj->slot[i];
        s->key = INVALID_VAL;
        s->value = INVALID_VAL;
        s->next = -1;
    }
    for (i = 0; i < old_size; ++i) {
        MyHashSlot *s = &old_slot[i];
        if (s->value != INVALID_VAL)
            myHashMapPut(obj, s->key, s->value);
    }
    free(old_slot);
}

/** value will always be non-negative. */
void myHashMapPut(MyHashMap* obj, int key, int value) {
    MyHashSlot* s = mainPosition(obj, key);
    MyHashSlot* tmp = s;
    MyHashSlot* slot = NULL;
    while (1) {
        // 键重复，替换新值
        if (tmp->key == key) {
            tmp->value = value;
            return;
        }
        // 有位置可插入
        if (tmp->value == INVALID_VAL && !slot) {
            slot = tmp;
        }
        if (tmp->next < 0) {
            break;
        }
        tmp = &obj->slot[tmp->next];
    }
    if (slot) {
        slot->key = key;
        slot->value = value;
        return;
    }

    tmp = mainPosition(obj, s->key);
    // 主位置被占用，插入主位置，将旧值重新插入（另其哈希到其他位置）
    if (tmp != s) {
        while (tmp->next != s - obj->slot) {
            assert(tmp->next >= 0);
            tmp = &obj->slot[tmp->next];
        }
        tmp->next = s->next;
        int tmpkey = s->key;
        int tmpvalue = s->value;
        s->key = key;
        s->value = value;
        s->next = -1;
        if (tmpvalue != INVALID_VAL) {
            myHashMapPut(obj, tmpkey, tmpvalue);
        }
        return;
    }

    // 哈希冲突
    while(obj->lastfree >= 0) {
        tmp = &obj->slot[obj->lastfree--];
        if (tmp->key == INVALID_VAL) {
            tmp->key = key;
            tmp->value = value;
            tmp->next = s->next;
            s->next = tmp - obj->slot;
            return;
        }
    }

    rehash(obj);
    myHashMapPut(obj, key, value);
}

/** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
int myHashMapGet(MyHashMap* obj, int key) {
    MyHashSlot* s = mainPosition(obj, key);
    while(1) {
        if (s->key == key) {
            return s->value;
        }
        if (s->next < 0) {
            return -1;
        }
        s = &obj->slot[s->next];
    }
    return -1;
}

/** Removes the mapping of the specified value key if this map contains a mapping for the key */
void myHashMapRemove(MyHashMap* obj, int key) {
    MyHashSlot* s = mainPosition(obj, key);
    while(1) {
        if (s->key == key) {
            s->value = INVALID_VAL;
            return;
        }
        if (s->next < 0) {
            return;
        }
        s = &obj->slot[s->next];
    }
}

void myHashMapFree(MyHashMap* obj) {
    free(obj->slot);
    free(obj);
}


int main() {
    // Your MyHashMap struct will be instantiated and called as such:

    MyHashMap* obj = myHashMapCreate();

    int key = 1;
    int value;
    myHashMapPut(obj, key, 10); 
    value = myHashMapGet(obj, key);
    printf("%d\n", value);
    myHashMapRemove(obj, key);
     
    myHashMapFree(obj);

    return 0;
}
