#define RAM_START 0
#define RAM_END 4096

typedef unsigned int uint32;

typedef struct _Node {
    uint32 size;
    void *next;
} Node;

Node *head;


int kMemInit() {
    head = (Node *)RAM_START;
    head->size = RAM_END - sizeof(Node);
    head->next = RAM_END;
    
    return 0;
}

void *kMemAlloc(uint32 size) {
    uint32 sizeOfBlock = sizeof(Node) + size;
    if (sizeOfBlock > RAM_END) return 0;

    Node *prev = head;
    Node *curr;
    while (prev != RAM_END) {
        curr = prev->next;
        if (curr == RAM_END) {
            if (prev != RAM_START) return 0;

            Node *newNode = prev + sizeOfBlock;
            newNode->next = RAM_END;
            newNode->size = prev->size - sizeOfBlock;

            prev->size = size;

            head = newNode;
            
            curr = prev;
            break;
        }

        if (curr->size >= size) {
            Node *newNode = curr + sizeOfBlock;
            newNode->next = curr->next;
            newNode->size = curr->size - sizeOfBlock;

            prev->next = newNode;

            curr->size = size;
            break;
        }
        prev = curr;
    }
    return curr;
}


int kMemDealloc(Node *ptr) {
    Node *prev = head;
    Node *curr = prev->next;
    if (!curr) {

    }

    while (curr < ptr) {
        // merge prev & curr
        if (prev + prev->size + sizeof(Node) == curr) {
            prev->size += sizeof(Node) + curr->size;
            prev->next = curr->next;
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    prev->next = ptr;
    ((Node *)ptr)->next = curr;

    if (ptr + sizeof(Node) + ptr->size == curr) {
        ptr->size += (sizeof(Node) + curr->size);
        ptr->next = curr->next;
    }
        

    if (prev + sizeof(Node) + prev->size == ptr) {
        prev->size += (sizeof(Node) + ptr->size);
        prev->next = ptr->next;
    }


    return 0;
}