#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int* container;
    int size;
    int capacity;
}SMinHeap;

static void fixDown(SMinHeap* heap, int parent) {
    int son = 2*parent + 1;
    int* nums = heap->container;
    while (son < heap->size) {
        if (son + 1 < heap->size && nums[son] > nums[son+1]) {
            son += 1;
        }
        if (nums[parent] > nums[son]) {
            int tmp = nums[parent];
            nums[parent] = nums[son];
            nums[son] = tmp;
            parent = son;
            son = 2*parent + 1;
        }
        else {
            break;
        }
    }
}

static void fixUp(SMinHeap* heap, int son) {
    int* nums = heap->container;
    while (son > 0) {
        int parent = (son - 1) / 2;
        if (nums[parent] > nums[son]) {
            int tmp = nums[parent];
            nums[parent] = nums[son];
            nums[son] = tmp;
            son = parent;
        }
        else {
            break;
        }
    }
}

static SMinHeap* makeMinHeap(int* nums, int size) {
    SMinHeap* heap = (SMinHeap*)malloc(sizeof(SMinHeap));
    heap->container = nums;
    heap->size = size;
    heap->capacity = size;
    for (int i = (size-2) / 2; i >= 0; --i) {
        fixDown(heap, i);
    }
    return heap;
}

SMinHeap* heap_create(int capacity) {
    SMinHeap* heap = (SMinHeap*)malloc(sizeof(SMinHeap));
    heap->container = (int*)malloc(sizeof(int)*capacity);
    heap->size = 0;
    heap->capacity = capacity;
    memset((void*)heap->container, 0, sizeof(int)*capacity);
    return heap;
}

void heap_free(SMinHeap* heap) {
    free(heap->container);
    free(heap);
}

void heap_push(SMinHeap* heap, int val) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->container = (int*)realloc(heap->container, heap->capacity);
    }
    heap->container[heap->size++] = val;
    fixUp(heap, heap->size - 1);
}

int heap_pop(SMinHeap* heap) {
    if (heap->size < 1) {
        printf("[error]heap is empty!\n");
        return -1;
    }
    int val = heap->container[0];
    heap->size -= 1;
    heap->container[0] = heap->container[heap->size];
    fixDown(heap, 0);
    return val;
}

bool empty(SMinHeap* heap) {
    return heap->size == 0;
}

void heap_sortx(int* nums, int size) {
    SMinHeap* heap = makeMinHeap(nums, size);
    for (int i = 1; i < size; ++i) {
        nums[size-i] = heap_pop(heap);
    }
    free(heap);
}