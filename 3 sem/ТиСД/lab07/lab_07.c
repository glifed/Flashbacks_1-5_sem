#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define B BUFSIZ
#define C 0.3311
#define COMP 1

const int MAXLINE = 100;

typedef struct Node {
    char word[30];
    int Hz;
    struct Node *left;
    struct Node *right;
}Node;

typedef struct Node_bt {
    char word[30];
    int Hz;
    char height;
    struct Node_bt *left;
    struct Node_bt *right;
}Node_bt;

typedef struct lnode_t {
    char Word[20];
    int Count;
    struct lnode_t *next;
}lnode_t;

struct List{
    char Word[20];
    int Count;
};

typedef struct htable_t{
    struct List *table;
    char *used;
}htable_t;

Node *tree = NULL;
Node_bt *btree = NULL;
Node_bt *bpred = NULL;
Node_bt *bnode = NULL;
lnode_t *tnode = NULL;

size_t osize = 411, csize = 411;
lnode_t **otable = NULL;//calloc(osize, sizeof(lnode_t*));
htable_t ctable = {NULL, NULL};

char cmp_text1[30] = "special";
char cmp_text2[30] = "in";
char cmp_text3[30] = "goto";

char ofunc = 1, cfunc = 1;


size_t my_getline(char **lineptr, size_t *n, FILE *stream)
{
    *lineptr = NULL;
    *n = 0;
    char buffer[B] = "";
    char *a = NULL;
    size_t len = B - 1;
    int i = 0;

    while(!feof(stream) && len == B - 1)
    {
        fgets(buffer, B, stream);
        if ((a = (char*)realloc(*lineptr, B * (++i))) != NULL)
        {
            *lineptr = a;
            len = strcspn(buffer, "\r\n");

            if (len == 0)
            {
                memcpy(*lineptr + *n, "\0", 1);
            }

            else
            {
                memcpy(*lineptr + *n, buffer, len);
                memcpy(*lineptr + *n + len, "\0", 1);
                *n += len;
            }
        }
        else
        {
            *n = -1;
            free(*lineptr);
            break;
        }
    }

    if (feof(stream))
        *n = -1;

    return *n;
}

int cmp_string(char *a, char *b)
{
    int u = 0;
    int i = 0;
   // printf("%s\t%s\n", a, b);
    while (a[i] != '\0' && b[i] != '\0')
    {
        if(a[i] > b[i])
        {
            u = 1;
            break;
        }

        if (a[i] < b[i])
        {
            u = -1;
            break;
        }
        i++;
    }
    return u;
}

Node* getFreeNode(Node *head, char *a, int N)
{
    Node* tmp = (Node*) malloc(sizeof(Node));
    tmp->left = tmp->right = NULL;
    memcpy(tmp->word, a, N+1);
    //strcpy(tmp->word, a);
    tmp->Hz = 1;
    (head) = tmp;
    return tmp;
}

void find_tree(Node **head, char *a, int N)
{
    Node *tmp = NULL;

    if (*head == NULL)
    {
        Node *tmp = (Node*)malloc(sizeof(Node));
        memcpy(tmp->word, a, N);
        //strcpy(tmp->word, a);
        tmp->Hz = 0;
        tmp->left = (*head);
        tmp->right = (*head);
        (*head) = tmp;
    }
    else
    {
        tmp = *head;
        while (tmp)
        {
            if (cmp_string(a, tmp->word) > 0)
            {
                //printf("R\n");
                if (tmp->right)
                {
                    tmp = tmp->right;
                    continue;
                }
                else
                {
                    tmp->right = getFreeNode(tmp, a, N);
                    return;
                }
            }
            else if (cmp_string(a, tmp->word) < 0)
            {
                //printf("L\n");
                if (tmp->left)
                {
                    tmp = tmp->left;
                    continue;
                }
                else
                {
                    tmp->left = getFreeNode(tmp, a, N);
                    return;
                }
            }
            else if (cmp_string(a, tmp->word) == 0)
            {
                //printf("ok\n");
                tmp->Hz = tmp->Hz + 1;
                break;
            }
            else
            {
                exit(2);
            }
        }
    }
}

void add_tree(Node **head, FILE *f)
{
    char a[30];
    int k = 0;

    fflush(stdin);
    printf("Input word: \n");
    gets(a);

    int i = 0;
    while (a[i]!= '\0')
    {
        k++;
        i++;
    }
    //printf("%d", k);
    find_tree(head, a, k);

    for(int j = 0; j < k; j++)
        fprintf(f, "%c", a[j]);
    fprintf(f, "%s", "\n");
}

void input_tree(FILE *f, Node **head)
{
    char *text;
    size_t j = 0;

    while(!feof(f))
    {
        text = NULL;
        my_getline(&text, &j, f);
        find_tree(head, text, j);
        //printf("str : %s %I64d\n", text, j);
    }
}

int find_word(Node **head, FILE *f)
{
    int ERR = 0;
    char a[30];
    fflush(stdin);
    int n = 0;
    printf("Input word: \n");

    gets(a);
    fflush(stdin);

    Node *tmp = NULL;

    int OK = 0;
    if (*head == NULL)
    {
        printf("Empty tree.\n");
    }
    else
    {
        tmp = *head;
        while (tmp)
        {
            if (cmp_string(a, tmp->word) > 0)
            {
               tmp = tmp->right;
               n++;
            }
            else if (cmp_string(a, tmp->word) < 0)
            {
                tmp = tmp->left;
                n++;
            }
            else if (strcmp(a, tmp->word) == 0)
            {
                OK = 1;
                printf("Done.\n");
                printf("Frequency this word = %d", tmp->Hz);
                ERR = 0;
                break;
            }
            else
            {
                OK = 0;
               // printf("%d ", strcmp("gok", "go"));
                break;
            }
        }
        if (OK == 0)
        {
            printf("Do you want add?\n1 - Yes\n2 - No\n");
            scanf("%d", &OK);
            if (OK == 1)
            {
                find_tree(head, a, strlen(a));
                for(int j = 0; j < strlen(a); j++)
                    fprintf(f, "%c", a[j]);
                fprintf(f, "%s", "\n");
            }
        }
    }
    return ERR;
}

void FindWord(Node *head, char *str, int *comp)
{
    Node *tmp = NULL;
    tmp = head;

    while (tmp)
    {
        if (cmp_string(str, tmp->word) > 0)
        {
           tmp = tmp->right;
           (*comp)++;
        }
        else if (cmp_string(str, tmp->word) < 0)
        {
            tmp = tmp->left;
            (*comp)++;
        }
        else if (strcmp(str, tmp->word) == 0)
            break;
        else
            break;
    }
}

Node* rinl(Node* node)
{
    return node->right ? rinl(node->right) : node;
}

Node* delright(Node* node)
{
    if(!node->right)
        return node->left;
    node->right = delright(node->right);
    return node;
}
// Remove node from balanced tree
Node *delnode(Node *node, char *value, int *in)
{
    if(!node)
        return NULL;
    if(strcmp(value, node->word) < 0)
        node->left = delnode(node->left, value, in);
    else if(strcmp(value,node->word) > 0)
        node->right = delnode(node->right, value, in);
    else
    {
        *in = 1;
        Node* left = node->left;
        Node* right = node->right;
        free(node);
        if(!left)
            return right;
        Node* max = rinl(left);
        max->left = delright(left);
        max->right = right;
        return max;
    }
    return node;
}

Node *delnodechr(Node *node, char value, int *in)
{
    if(!node)
        return NULL;
    if(value < node->word[0])
        node->left = delnodechr(node->left, value, in);
    else if(value > node->word[0])
        node->right = delnodechr(node->right, value, in);
    else
    {
        *in = 1;
        Node* left = node->left;
        Node* right = node->right;
        free(node);
        if(!left)
            return right;
        Node* max = rinl(left);
        max->left = delright(left);
        max->right = right;
        return max;
    }
    return node;
}
void output_tree(Node *head)
{
    if(head != NULL)
    {
        output_tree(head->left);
        printf("%s\t %d\n", head->word, head->Hz);
        output_tree(head->right);
    }
}

void PrintTree (Node *pNode, int n)
{

    if (pNode->left)
        PrintTree(pNode->left, n+1);

    for (int i = 0; i < n; i++)
    {
        printf("     ");
    }
    printf("%s (%d)\n\n", pNode->word, pNode->Hz);

    if (pNode->right)
        PrintTree(pNode->right, n+1);
}

void FindTree (Node *pNode, char *a, int *ok)
{
    if (pNode->left)
        FindTree(pNode->left, a, ok);

    if (strcmp(a, pNode->word) == 0)
        *ok = 1;

    if (pNode->right)
        FindTree(pNode->right, a, ok);
}

void VolumeTree (Node *pNode, int *V)
{
    if (pNode->left)
        VolumeTree(pNode->left, V);

    (*V) += strlen(pNode->word);
    (*V) += ((sizeof(pNode->left))*2)+4;

    if (pNode->right)
        VolumeTree(pNode->right, V);
}

void FindTreeChar(Node *pNode, char a, int *ok)
{
    if (pNode->left)
        FindTreeChar(pNode->left, a, ok);

    if (a == pNode->word[0])
        *ok = 1;

    if (pNode->right)
        FindTreeChar(pNode->right, a, ok);
}

void print_tree(Node *tree, int level, int connector, char *ptr, char *stack)
{
    if(tree)
    {
        stack[level] = 'r';
        print_tree(tree->right,level + 1, 218, ptr, stack);
        for(int i = 0;i < level - 1;i++)
        {
            if (stack[i] != stack[i + 1])
                printf("%c      ", 179);
            else
                printf("       ");
        }
        if (connector)
            printf("%c%s%s%4d\n",connector, ptr, tree->word, tree->Hz);
        else
            printf("%s%4d\n", tree->word, tree->Hz);
        stack[level] = 'l';
        print_tree(tree->left, level + 1, 192, ptr, stack);
    }
}

void print_btree(Node_bt *tree, int level, int connector, char *ptr, char *stack)
{
    if(tree)
    {
        stack[level] = 'r';
        print_btree(tree->right,level + 1, 218, ptr, stack);
        for(int i = 0;i < level - 1;i++)
        {
            if (stack[i] != stack[i + 1])
                printf("%c      ", 179);
            else
                printf("       ");
        }
        if (connector)
            printf("%c%s%s%4d\n",connector, ptr, tree->word, tree->Hz);
        else
            printf("%s%4d\n", tree->word, tree->Hz);
        stack[level] = 'l';
        print_btree(tree->left, level + 1, 192, ptr, stack);
    }
}

void apply_pre(Node *tree, void (*f)(Node*, void*), void *arg)
{
    if (tree == NULL) // ���� �� ������ ����
        return;

    f(tree, arg); //  ������ ������ ������
    apply_pre(tree->left, f, arg); // �������� ������ ���������
    apply_pre(tree->right, f, arg); // ������� ���������
}

void apply_bpre(Node_bt *tree, void (*bf)(Node_bt*, void*), void *arg)
{
    if (tree == NULL) // ���� �� ������ ����
        return;

    bf(tree, arg); //  ������ ������ ������
    apply_bpre(tree->left, bf, arg); // �������� ������ ���������
    apply_bpre(tree->right, bf, arg); // ������� ���������
}

void to_dot(Node *tree, void *param)
{
    FILE *f = param;

    if (tree->left)
        //fprintf(f, "\"%d (%d)\" -> \"%d (%d)\";\n", tree->data, tree->height, tree->left->data, tree->left->height);
        fprintf(f, "%s -> %s;\n", tree->word, tree->left->word);

    if (tree->right)
        //fprintf(f, "\"%d (%d)\" -> \"%d (%d)\";\n", tree->data, tree->height, tree->right->data, tree->right->height);
        fprintf(f, "%s -> %s;\n", tree->word, tree->right->word);
}

void to_bdot(Node_bt *tree, void *param)
{
    FILE *f = param;

    if (tree->left)
        //fprintf(f, "\"%d (%d)\" -> \"%d (%d)\";\n", tree->data, tree->height, tree->left->data, tree->left->height);
        fprintf(f, "%s -> %s;\n", tree->word, tree->left->word);

    if (tree->right)
        //fprintf(f, "\"%d (%d)\" -> \"%d (%d)\";\n", tree->data, tree->height, tree->right->data, tree->right->height);
        fprintf(f, "%s -> %s;\n", tree->word, tree->right->word);
}

void export_to_dot(FILE *f, const char *tree_name, Node *tree)
{
    fprintf(f, "digraph %s {\n", tree_name);

    apply_pre(tree, to_dot, f);

    fprintf(f, "}\n");
}
void exportb_to_dot(FILE *f, const char *tree_name, Node_bt *tree)
{
    fprintf(f, "digraph %s {\n", tree_name);

    apply_bpre(tree, to_bdot, f);

    fprintf(f, "}\n");
}

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    return d;
}

unsigned char height(Node_bt* node)
{
    return node ? node->height : 0;
}

int bfactor(Node_bt *node)
{
    return height(node->right) - height(node->left);
}

void fixheight(Node_bt* node)
{
    if (!node)
        return;
    fixheight(node->left);
    fixheight(node->right);
    unsigned char hleft = height(node->left);
    unsigned char hright = height(node->right);
    node->height = (hleft > hright ? hleft : hright) + 1;
}
// Right rotation around node
Node_bt* rotateright(Node_bt* node)
{
    Node_bt* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}
// Left rotation around node
Node_bt* rotateleft(Node_bt* node)
{
    Node_bt* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

Node_bt* balance(Node_bt* p)
{
    fixheight(p);
    // Right-left rotation
    if(bfactor(p) == 2)
    {
        if(bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    // Left-right rotation
    if(bfactor(p) == -2)
    {
        if(bfactor(p->left) > 0)
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p;
}

Node_bt* createb(char *value, int count)
{
    Node_bt *node = malloc(sizeof(Node_bt));
    if (node)
    {
        strcpy(node->word, value);
        node->Hz = count;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}
// Insert tree node_t into a binary tree
Node_bt* insertb(Node_bt *tree, Node_bt *node, int *rc)
{
    if (!tree)
        return node;

    if (strcmp(node->word, tree->word) == 0)
        tree->Hz++;
    else if (strcmp(node->word, tree->word) < 0)
        tree->left = insertb(tree->left, node, rc);
    else
        tree->right = insertb(tree->right, node, rc);

    return balance(tree);
}
// Create balanced tree from binary tree
void treebal(Node *tree, Node_bt **btree)
{
    if (!tree)
        return;

    int rc = 0;
    Node_bt *node = createb(tree->word, tree->Hz);
    if (node)
    {
        *btree = insertb(*btree, node, &rc);
        treebal(tree->left, btree);
        treebal(tree->right, btree);
    }
}

Node_bt* linr(Node_bt* node)
{
    return node->left ? linr(node->left) : node;
}
// Remove min in right subtree
Node_bt* delleft(Node_bt* node)
{
    if(!node->left)
        return node->right;
    node->left = delleft(node->left);
    return balance(node);
}
// Remove node from balanced tree
Node_bt *removeb(Node_bt *node, char *value, int *in)
{
    if(!node)
        return NULL;
    if(strcmp(value, node->word) < 0)
        node->left = removeb(node->left, value, in);
    else if(strcmp(value, node->word) > 0)
        node->right = removeb(node->right, value, in);
    else
    {
        *in = 0;
        Node_bt* left = node->left;
        Node_bt* right = node->right;
        free(node);
        if(!right)
            return left;
        Node_bt* min = linr(right);
        min->right = delleft(right);
        min->left = left;
        return balance(min);
    }
    return balance(node);
}

Node_bt* lookupb(Node_bt *tree, char *value, Node_bt **pred, int *cmp)
{
    if (!tree)
        return NULL;
    (*cmp)++;
    if (strcmp(tree->word, value) == 0)
        return tree;
    else
    {
        *pred = tree;
        if (strcmp(tree->word, value) > 0)
            return lookupb(tree->left, value, pred, cmp);
        else
            return lookupb(tree->right, value, pred, cmp);
    }
}



void freetreeb(Node_bt *tree)
{
    if (!tree)
        return;

    freetreeb(tree->left);
    freetreeb(tree->right);
    free(tree);
}

int n(char *c)
{
    int q = 0;
    while(*c != '\0')
    {
        q += *c;
        c++;
    }
    return q;
}

int hash1(int value, int size)//��� �������
{
    return value % size;
}

int hash2(int value, int size)//2�� ��� �������
{
    return (float)size * ((C * (float)value) - (int)(C * (float)value));
}

int hashf(int value, char func, int size)//2 ��� ������� � ������ � ��� �� func ��������
{
    if (func == 1)
        return hash1(value, size);
    else
        return hash2(value, size);
    return 0;
}

int nodecount(Node *tree)
{
    if (!tree)
        return 1;
    int nodes = nodecount(tree->left) + nodecount(tree->right);
    return nodes;
}

void* free_open(lnode_t **table, int size)
{
    if (!table)
        return NULL;
    for(int i = 0; i < size; i++)
    {
        while (table[i])
        {
            lnode_t *temp = table[i]->next;
            free(table[i]);
            table[i] = temp;
        }
    }
    return NULL;
}

lnode_t* search_open(lnode_t **table, char *value, int *comp, int size)
{
    *comp = 1;
    lnode_t *node = table[hashf(n(value), ofunc, size)];
    while (node)
    {
        if (strcmp(node->Word, value) == 0)
        {
            return node;
        }
        (*comp)++;
        node = node->next;
    }
    //*comp = 0;
    return NULL;
}

int insert_open(lnode_t **hashtable, char *value, int count, int size)
{
    int rc = 0;
    if (search_open(hashtable, value, &rc, size))
        return 0;
    lnode_t *node = malloc(sizeof(lnode_t));
    if (!node)
        return -5;
    strcpy(node->Word, value);
    node->next = NULL;
    node->Count = count;
    int hash = hashf(n(value), ofunc, size);
    if (!hashtable[hash])
        hashtable[hash] = node;
    else
    {
        lnode_t *temp = hashtable[hash];
        hashtable[hash] = node;
        hashtable[hash]->next = temp;
    }
    return 0;
}


void full_open(Node *tree, lnode_t **table, int *rc, int size)
{
    if (!tree)
        return;
    if (insert_open(table, tree->word, tree->Hz, size))
    {
        *rc = -5;
        return;
    }
    full_open(tree->left, table, rc, size);
    full_open(tree->right, table, rc, size);
}

int count_open(lnode_t **table, int size)
{
    int nodes = 0;
    for (int i = 0; i < size; i++)
    {
        if (table[i])
            nodes++;
    }
    return nodes;
}

float check_open(lnode_t **table, int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        int comp = 0;
        lnode_t *node = table[i];
        while (node)
        {
            search_open(table, node->Word, &comp, size);
            node = node->next;
        }
        sum += comp;
    }
    return sum / (float)count_open(table, size);
}

void print_open(lnode_t **hashtable, int size)
{
    int empty = 1;
    printf("\n*** Table with open hashing ***\n");
    for (int i = 0; i < size; i++)
    {
        lnode_t *node = hashtable[i];
        if (node)
        {
            empty = 0;
            printf("Hash %2d || List: ", i);
            while (node)
            {
                printf("%20s%4d ", node->Word, node->Count);
                node = node->next;
            }
            printf("\n");
        }
    }
    if (empty)
        printf("\nTable is empty\n");
}

void VolumeOpen(lnode_t **hashtable, int size, int *V)
{
    for (int i = 0; i < size; i++)
    {
        lnode_t *node = hashtable[i];
        if (node)
        {
            while (node)
            {
                (*V)+= strlen(node->Word) + 4;
                (*V) += sizeof(node->next);
                node = node->next;
            }
        }
    }
}


int del_open(lnode_t **table, char *value, int size)
{
    int hash = hashf(n(value), ofunc, size);
    lnode_t *node = table[hash], *prev = NULL, *temp;
    while (node && strcmp(node->Word, value))
    {
        prev = node;
        node = node->next;
    }
    if (node && prev)
    {
        temp = node->next;
        free(node->next);
        prev->next = temp;
        return 0;
    }
    else if (node)
    {
        temp = table[hash]->next;
        free(table[hash]);
        table[hash] = temp;
        return 0;
    }
    return 1;
}

int create_close(htable_t *table, int size)
{
    table->table = calloc(size, sizeof(struct Node));
    if (!table->table)
        return -5;
    table->used = calloc(size, sizeof(char));
    if (!table->used)
    {
        free(table->table);
        return -5;
    }
    return 0;
}

// insert in hash table with close hashing
int insert_close(htable_t table, char *value, int count, int size)
{
    int hash = hashf(n(value), cfunc, size), comp = 0;
    while  (table.used[hash] && strcmp(table.table[hash].Word, value) && comp < size)
    {
        hash = (hash + 1) % size;
        comp++;
    }
    if (!table.used[hash])
    {
        table.used[hash] = 1;
        strcpy(table.table[hash].Word, value);
        table.table[hash].Count = count;
    }
    else if (comp >= size)
        return -11;
    else
        return -8;
    return 0;
}

// Create table with open hashing from tree
void full_close(Node *tree, htable_t table, int size)
{
    if (!tree)
        return;
    if (insert_close(table, tree->word, tree->Hz, size) == -11)
        return;
    full_close(tree->left, table, size);
    full_close(tree->right, table, size);
}

// Print hash table with close hashing
void print_close(htable_t table, int size)
{
    int empty = 1;
    printf("\n*** Table with close hashing ***\n");
    for (int i = 0; i < size; i++)
        if (table.used[i])
        {
            empty = 0;
            printf("Hash:%3d || Value:%14s%4d\n", i, table.table[i].Word, table.table[i].Count);
        }
    if (empty)
        printf("\nEmpty table\n");
}

void VolumeClose(htable_t table, int size, int *V)
{
    for (int i = 0; i < size; i++)
        if (table.used[i])
        {
            (*V) += strlen(table.table[i].Word);
            (*V) += 4;
        }
}


// Search value in hash table with close hashing
char *search_close(htable_t table, char *value, int *comp, int size)
{
    int hash = hashf(n(value), cfunc, size);
    *comp = 1;
    while  (table.used[hash] && strcmp(table.table[hash].Word, value) && *comp < size)
    {
        hash = (hash + 1) % size;
       (*comp)++;
    }
    if (strcmp(table.table[hash].Word, value) == 0)
        return value;
    return value - 1;
}

// Delete key from table with close hashing
int del_close(htable_t table, char *value, int size)
{
    int comp = 0;
    int hash = hashf(n(value), cfunc, size);
    while  (table.used[hash] && strcmp(table.table[hash].Word, value) && comp < size)
    {
        hash = (hash + 1) % size;
        comp++;
    }
    if (strcmp(table.table[hash].Word, value) == 0)
    {
        table.used[hash] = 0;
        strcpy(table.table[hash].Word, "\0");
        return 1;
    }
    return 0;
}

// Average number of comparisons
float check_close(htable_t table, int size)
{
    float sum = 0;
    int comp = 0;
    for (int i = 0; i < size; i++)
        if(strcmp(search_close(table, table.table[i].Word, &comp, size), table.table[i].Word) == 0)
            sum += comp;
    return sum / (float)size;
}

// Free hash table with close hashing
void free_close(htable_t *table)
{
    if (!table)
        return;
    free(table->table);
    free(table->used);
    table->table = NULL;
    table->used = NULL;
}

int isnum(char *str)
{
    if (*str == '-')
        str++;
    if (!*str)
        return 0;
    for (; *str; str++)
        if (*str - '0' > 9 || *str - '0' < 0)
            return 0;
    return 1;
}

int readNum(int *val)
{
    char buf[10000];
    printf("\nEnter number (1 or 2): ");
    fflush(stdin);
    gets(buf);
    fflush(stdin);
    if (!isnum(buf))
    {
        printf("\nWrong input\n");
        return 0;
    }
    *val = atoi(buf);
    return 1;
}


void compare_time(void)//��������
{
    int t1 = 0, tf1 = 0, c1 = 0, v1 = 0;
    int comp = COMP;

    t1 = tick();
    FindWord(tree, cmp_text1, &comp);
    t1 = tick() - t1;
    tf1 += t1;
    c1 = comp;

    comp = COMP;
    t1 = tick();
    FindWord(tree, cmp_text2, &comp);
    t1 = tick() - t1;
    tf1 += t1;
    c1 = comp;

    comp = COMP;
    t1 = tick();
    FindWord(tree, cmp_text3, &comp);
    t1 = tick() - t1;
    tf1 += t1;
    c1 = comp;

    VolumeTree(tree, &v1);
    printf("Time in tree %d\n",tf1/3);
    printf("Comparisons %d\n", c1/3);
    printf("Volume of memory in tree %d\n", v1);
    t1 = 0;
    tf1 = 0;
    c1 = 0;

    comp = COMP;
    t1 = tick();
    bnode = lookupb(btree, cmp_text1, &bpred, &comp);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    comp = COMP;
    t1 = tick();
    bnode = lookupb(btree, cmp_text2, &bpred, &comp);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    comp = COMP;
    t1 = tick();
    bnode = lookupb(btree, cmp_text3, &bpred, &comp);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    v1 += nodecount(tree);

    printf("Time balanced tree %d\n", tf1/3);
    printf("Comparisons %d\n", c1/3);
    printf("Volume of memory in balanced tree %d\n", v1);
    c1 = 0;
    t1 = 0;
    tf1 = 0;
    v1 = 0;

    t1 = tick();
    tnode = search_open(otable, cmp_text1, &comp, osize);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    t1 = tick();
    tnode = search_open(otable, cmp_text2, &comp, osize);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    t1 = tick();
    tnode = search_open(otable, cmp_text3, &comp, osize);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    VolumeOpen(otable, osize, &v1);
    printf("Time otable %d\n", tf1/3);
    printf("Comparisons %d\n", c1/3);
    printf("Volume of memory in otable %d\n", v1);
    c1 = 0;
    t1 = 0;
    tf1 = 0;
    v1 = 0;

    t1 = tick();
    search_close(ctable, cmp_text1, &comp, csize);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;
    t1 = tick();
    search_close(ctable, cmp_text2, &comp, csize);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    t1 = tick();
    search_close(ctable, cmp_text3, &comp, csize);
    t1 = tick() - t1;
    tf1 += t1;
    c1 += comp;

    VolumeClose(ctable, csize, &v1);
    printf("Time ctable %d\n", tf1/3);
    printf("Comparisons %d\n", c1/3);
    printf("Volume of memory in ctable %d\n", v1);
}

int main (void)
{
    setbuf(stdout, NULL);

    FILE *f, *g, *h;

    char buf[10000];
    char ptr[7];
    char str[40];
    char st;

    f = fopen("input.txt", "r");
    g = fopen("input.txt", "a");
    h = fopen("graph.txt", "w");
    int zn = 1, menu, rc;
    int comp;
    char temp[30];

    input_tree(f, &tree);

    while(zn != 0)
    {
        printf("\n1 - Output tree\n");
        printf("2 - Add in tree\n");
        printf("3 - Delete from tree\n");
        printf("4 - Search in tree\n");
        printf("5 - Create balanced tree\n");
        printf("6 - Print balanced tree\n");
        printf("7 - Add in balanced tree\n");
        printf("8 - Delete from balanced tree\n");
        printf("9 - Search in balanced tree\n");
        printf("10 - Free balanced tree\n");
        printf("11 - Create otable\n");
        printf("12 - Print otable\n");
        printf("13 - Search in otable\n");
        printf("14 - Add in otable\n");
        printf("15 - Delete from otable\n");
        printf("16 - Free otable\n");
        printf("17 - Create ctable\n");
        printf("18 - Print ctable\n");
        printf("19 - Search in ctable\n");
        printf("20 - Add in ctable\n");
        printf("21 - Delete from ctable\n");
        printf("22 - Free ctable\n");
        printf("23 - Task(removal words)\n");
        printf("24 - Compare\n");
        printf("25 - Change function for open hashing\n");
        printf("26 - Change function for close hashing\n");
        printf("0 - Esc\n");
        printf("\nInput value of menu: ");
        scanf("%d", &menu);
        printf("\n");

        if (menu == 1)
        {
            for (int i = 0; i < 5; i++)
                ptr[i] = 196;
            ptr[5] = '>'; ptr[6] = '\0';

            print_tree(tree, 0, 0, ptr, buf);
            printf("\n");
            fflush(stdin);
            export_to_dot(h, "Tree", tree);
            fclose(h);

        }
        if (menu == 2)
        {
            add_tree(&tree, g);
            printf("\n");
        }
        if (menu == 3)
        {
            char str[50];
            printf("Input a string\n");
            fflush(stdin);
            gets(str);
            comp = -6;
            int znach = 0;
            FindTree(tree, str, &znach);
            if (znach == 1)
            {
                tree = delnode(tree, str, &comp);
                printf("Done\n");
            }
            else
            {
                printf("There is no such word\n");
            }
        }
        if (menu == 4)
        {
            find_word(&tree, g);
            printf("\n");
        }
        if (menu == 5)
        {
            if (!tree)
                printf("\nEmpty source tree\n");
            else if (btree)
                printf("\nTree is not empty now, first clear the tree\n");
            else
                treebal(tree, &btree);
            printf("\n");
        }
        if (menu == 6)
        {
            if (btree ==  NULL)
                printf("No");
            else
            {
                for (int i = 0; i < 5; i++)
                    ptr[i] = 196;
                ptr[5] = '>'; ptr[6] = '\0';

                print_btree(btree, 0, 0, ptr, buf);
                //PrintTree(tree, 0);
                printf("\n");
                fflush(stdin);
                exportb_to_dot(h, "Tree", btree);
                fclose(h);
            }
        }

        if (menu == 7)
        {
            scanf("%s", str);
            bnode = createb(str, 1);
            if (bnode)
            {
                rc = 1;
                btree = insertb(btree, bnode, &rc);
                //check(rc);
            }
           // else
            //    check(MEMORY_ALLOCATION_ERROR);
        }
        if (menu == 8)
        {
            scanf("%s", str);
            comp = -6;
            btree = removeb(btree, str, &comp);
        }
        if (menu == 9)
        {
            scanf("%s",str);
            bnode = lookupb(btree, str, &bpred, &comp);
            //printf("\nComparisons: %d\n", comp);
            if (bnode)
            {
                printf("\nFound: %20s %3d\n", bnode->word, bnode->Hz);
            }
            else
                printf("\nNo such node in the tree\n");
        }
        if (menu == 10)
        {
            if (btree == NULL)
                printf("NULL");
            else
            {
                freetreeb(btree);
                btree = NULL;
            }
        }

        if (menu == 11)
        {
            if (!tree)
            {
                printf("Empty tree\n");
            }
            osize = (nodecount(tree) - 1);
            otable = calloc(osize, sizeof(lnode_t*));
            if (!otable)
            {
                printf("Didn't can allocate the memory\n");
            }
            full_open(tree, otable, &rc, osize);

            if (rc == -5)
            {
                otable = free_open(otable, osize);
            }
            printf("\nAverage number of comparisons: %.3f\n", check_open(otable, osize));

        }
        if (menu == 12)
        {
            if (!otable)
                printf("Empty table\n");
            else
                print_open(otable, osize);
        }
        if (menu == 13)
        {
            if (!otable)
            {
                printf("Empty table\n");
            }
            scanf("%s", str);
            tnode = search_open(otable, str, &comp, osize);

            if (tnode)
            {
                printf("\nFound:\n");
                printf("Value:  %s%4d\n", tnode->Word, tnode->Count);
                printf("Comparisons: %d\n", comp);
            }
            else
                printf("\nValue not found\n");
        }
        if (menu == 14)
        {
            scanf("%s", str);
            insert_open(otable, str,1 , osize);
        }
        if (menu == 15)
        {
            if (!otable)
            {
                printf("Empty table\n");
            }
            scanf("%s",str);
            if (!del_open(otable, str, osize))
                printf("Done\n");
            else
                printf("No such element\n");
        }
        if (menu == 16)
        {
            otable = free_open(otable, osize);
        }
        if (menu == 17)
        {
            if (!tree)
            {
                printf("empty tree\n");
            }
            csize = (nodecount(tree) - 1) * 2;
            if (create_close(&ctable, csize))
            {
                printf("Error allocation\n");
            }
            full_close(tree, ctable, csize);
            printf("\nAverage number of comparisons: %.3f\n", check_close(ctable, csize));
            if (check_close(ctable, csize) < 4)
                printf("Ok\n");
            else
                printf("Error compare\n");
        }
        if (menu == 18)
        {
            if (!(&ctable)->table)
                printf("empty table\n");
            else
                print_close(ctable, csize);
        }
        if (menu == 19)
        {
            scanf("%s", str);
            strcpy(temp, search_close(ctable, str, &comp, csize));
            if (strcmp(temp, str) == 0)
            {
                printf("\nFound:\n");
                printf("Value: %s\n", str);
                printf("Hash: %d\n", hashf(n(str), cfunc, csize));
                printf("Comparisons: %d\n", comp);
            }
            else
                printf("\nValue not found\n");
        }
        if (menu == 20)
        {
            scanf("%s",str);
            rc = insert_close(ctable, str, 1, csize);
            if (rc == -11)
                printf("\nTable owerflow\n");
            else if (rc == -8)
                printf("Already in\n");
            else
                printf("Done\n");
        }

        if (menu == 21)
        {
            if (!(&ctable)->table)
            {
                printf("Empty table\n");
            }
            scanf("%s", str);
            if (del_close(ctable, str, csize))
                printf("Done\n");
            else
                printf("No such element\n");
        }

        if (menu == 22)
        {
            free_close(&ctable);
        }

        if (menu == 23)
        {
            printf("Input a string\n");
            fflush(stdin);
            st = getchar();
            int z = 1;
            while (z != 0)
            {
                z = 0;
                comp = -6;
                FindTreeChar(tree, st, &z);
                tree = delnodechr(tree, st, &comp);
                if (z == 1)
                    printf("Done\n");
            }

            if (btree != NULL)
            {
                freetreeb(btree);
                btree = NULL;
                treebal(tree, &btree);
            }

            if (otable != NULL)
            {
                otable = free_open(otable, osize);
                osize = (nodecount(tree) - 1);
                otable = calloc(osize, sizeof(lnode_t*));
                if (!otable)
                {
                    printf("Didn't can allocate the memory\n");
                }
                full_open(tree, otable, &rc, osize);

                if (rc == -5)
                {
                    otable = free_open(otable, osize);
                }
            }
            if ((&ctable)->table)
            {
                free_close(&ctable);
                csize = (nodecount(tree) - 1) * 2;
                if (create_close(&ctable, csize))
                {
                    printf("Error allocation\n");
                }
                full_close(tree, ctable, csize);
            }
        }
        if (menu == 24)
            compare_time();

        if (menu == 25)
        {
            if (!readNum(&rc))
                return 1;
            free_open(otable, osize);
            free(*otable);
            osize = (nodecount(tree) - 1);
            *otable = calloc(osize, sizeof(lnode_t*));
            ofunc = rc;
        }

        if (menu == 26)
        {
            if (!readNum(&rc))
                return 1;
            free_close(&ctable);
            csize = (nodecount(tree) - 1)*2;
            create_close(&ctable, csize);
            cfunc = rc;
        }

        if (menu == 0)
            zn = 0;
    }
}
