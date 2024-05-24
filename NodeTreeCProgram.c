#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_name_node_struct
{
    char treeName[32];
    struct tree_name_node_struct *parent, *left, *right;
    struct item_node_struct *theTree;
} tree_name_node;

typedef struct item_node_struct
{
    char name[32];
    int count;
    struct item_node_struct *iparent, *left, *right;
    struct tree_name_node_struct *tparent;
} item_node;

//======================================================================================================================
FILE * get_file_pointer(char * name);
tree_name_node * new_tree_name_node(char name[]);   // Function to create new tree name node
tree_name_node * create_names_tree(FILE * fp,int number0fTrees); // create the Main names tree
tree_name_node * insert_name_tree_node(tree_name_node * parent, tree_name_node * new_node); //insert node to given parent
void inorder_name_tree(tree_name_node* parent,FILE * fp);
void set_left_child(tree_name_node *parent, tree_name_node *child);
void set_right_child(tree_name_node *parent, tree_name_node *child);
void print_name_tree_node(tree_name_node *b,FILE * fp);

tree_name_node * search_for_name_node(tree_name_node * parent ,char* targetName);
item_node * search_in_name_node(item_node * parent, char * targetItem);

void create_item_tree(FILE* fp,tree_name_node* parent,int numberOfItems);
void inorder_item_tree(item_node * parent,FILE * fp);
void search_command(tree_name_node* parent, char* tree_name ,char * item_name,FILE* fp);
void proceed_commands(FILE * fp, tree_name_node* parent,int numberOCommands,FILE* outFp);
void print_tree_names(tree_name_node* parent,FILE *fp);
void print_items(tree_name_node * parent,FILE * fp);

//======================================================================================================================

int main() {
    int numberOfTrees,numberOfItems,numberOfCommands;

    FILE * filePtr  = get_file_pointer("cop3502-as4-sample-input.txt");
    FILE * fp = fopen("cop3502-as4-output-bachleda-michal.txt","w");

    fscanf(filePtr,"%d %d %d",&numberOfTrees, &numberOfItems, &numberOfCommands);

    tree_name_node * parent = create_names_tree(filePtr,numberOfTrees);
    create_item_tree(filePtr,parent,numberOfItems);

    print_tree_names(parent,fp);
    print_items(parent,fp);

    proceed_commands(filePtr,parent,numberOfCommands,fp);
    fclose(filePtr);

    return 0;
}

//======================================================================================================================


FILE * get_file_pointer(char * name){
    FILE * filePtr = fopen(name,"r");

    if (filePtr == NULL)
        printf("File does not exit\n");
    else
        return fopen(name,"r");
    return NULL;
}

tree_name_node * new_tree_name_node(char name[]){
    tree_name_node * t = malloc(sizeof (tree_name_node));
    strcpy(t->treeName,name);
    t->left =NULL;
    t->parent = NULL;
    t->right = NULL;
    t->theTree = NULL;
    return t;
}

item_node * new_item_node(char name[],tree_name_node * tParent, int count){
    item_node * i = malloc(sizeof (item_node));
    strcpy(i->name,name);
    i->count =  count;
    i->left =NULL;
    i->right = NULL;
    i->iparent = NULL;
    i->tparent = tParent;
    return i;

}

void set_left_child(tree_name_node *parent, tree_name_node *child) {
    parent->left = child;
    child->parent = parent;
}

void set_left_child_item(item_node* parent , item_node* child){
    parent->left=child;
    child->iparent = parent;
}
/* Sets the right child.  Similar to set_left_child(). */

void set_right_child(tree_name_node *parent, tree_name_node *child) {
    parent->right = child;
    child->parent = parent;
}
void set_right_child_item(item_node* parent, item_node* child){
    parent->right = child;
    child->iparent = parent;
}

tree_name_node * insert_name_tree_node(tree_name_node * parent, tree_name_node * new_node){
    if(strcmp(new_node->treeName,parent->treeName) < 0){
        if(parent->left) { // already a left child; RECURSIVE case.
            return insert_name_tree_node(parent->left, new_node);
        } else { // no left child; BASIS case.
            set_left_child(parent, new_node);
            return new_node;
        }
    } else { // value >=, go RIGHT
        if(parent->right) { // already a right child; RECURSIVE case.
            return insert_name_tree_node(parent->right, new_node);
        } else { // no right child; BASIS case.
            set_right_child(parent, new_node);
            return new_node;
        }
    }

}

item_node * insert_item_node(item_node * parent, item_node * new_node){
    if(strcmp(new_node->name,parent->name) < 0){
        if(parent->left) { // already a left child; RECURSIVE case.
            return insert_item_node(parent->left, new_node);
        } else { // no left child; BASIS case.
            set_left_child_item(parent, new_node);
            return new_node;
        }
    } else { // value >=, go RIGHT
        if(parent->right) { // already a right child; RECURSIVE case.
            return insert_item_node(parent->right, new_node);
        } else { // no right child; BASIS case.
            set_right_child_item(parent, new_node);
            return new_node;
        }
    }

}

tree_name_node * create_names_tree(FILE * fp,int number0fTrees){
    char name[35];
    fscanf(fp,"%s",name);
    tree_name_node * parent =  new_tree_name_node(name);

    while (number0fTrees >1){
        fscanf(fp,"%s",name);

        tree_name_node * new_node = new_tree_name_node(name);
        insert_name_tree_node(parent,new_node);
        number0fTrees--;
    }
    return parent;
}

void create_item_tree(FILE * fp,tree_name_node * parent, int numberOfItems){
    char parent_name[35];
    char item_name[35];
    int count;
    while (numberOfItems > 0){
        fscanf(fp,"%s %s %d",parent_name,item_name,&count);
        tree_name_node * name_parent = search_for_name_node(parent,parent_name);
        if(name_parent->theTree == NULL){
            name_parent->theTree = new_item_node(item_name,name_parent,count);
        }
        else{
            item_node * new_node = new_item_node(item_name,name_parent,count);
            insert_item_node(name_parent->theTree,new_node);
        }


        numberOfItems--;
    }
}

void print_name_tree_node(tree_name_node *b,FILE * fp) {
    fprintf(fp,"%s ", b->treeName);
}

void print_item_tree_node(item_node * b,FILE * fp){
    fprintf(fp,"%s ", b->name);

}

void inorder_item_tree(item_node * parent,FILE * fp){ //traversal (traverse_in_order)
    if(parent->left) {
        inorder_item_tree(parent->left,fp);
    }
    print_item_tree_node(parent,fp);

    if (parent->right) {
        inorder_item_tree(parent->right,fp);
    }
}

void inorder_name_tree(tree_name_node * parent, FILE * fp){
    if(parent->left) {
        inorder_name_tree(parent->left,fp);
    }
    print_name_tree_node(parent,fp);
    if (parent->right){
        inorder_name_tree(parent->right,fp);
    }
}

tree_name_node * search_for_name_node(tree_name_node * parent ,char* targetName){
    if(parent == NULL) { // FAILURE case
        return NULL;
    } else if(strcmp(targetName,parent->treeName) == 0) { // BASIS case
        return parent;
    } else if(strcmp(targetName,parent->treeName) < 0) { // LEFT recursive case
        return search_for_name_node(parent->left, targetName);
    } else { // target >= parent -> payload - RIGHT recursive case
        return search_for_name_node(parent->right, targetName);
    }
}

item_node * search_in_name_node(item_node * parent, char * targetItem){
    if(parent == NULL) { // FAILURE case
        return NULL;
    } else if(strcmp(targetItem,parent->name) == 0) { // BASIS case
        return parent;
    } else if(strcmp(targetItem,parent->name) < 0) { // LEFT recursive case
        return search_in_name_node(parent->left, targetItem);
    } else { // target >= parent -> payload - RIGHT recursive case
        return search_in_name_node(parent->right, targetItem);
    }

}

void search_command(tree_name_node * parent,char * tree_name, char * item_name,FILE* fp){
    tree_name_node * name_parent = search_for_name_node(parent,tree_name);
    if(name_parent == NULL){
        fprintf(fp,"%s does not exist\n",tree_name);
    }
    else{
        item_node * item = search_in_name_node(name_parent->theTree,item_name);

        if(item == NULL){
            fprintf(fp,"%s not found in %s\n",item_name,tree_name);
        }
        else{
            fprintf(fp,"%d %s found in %s\n",item->count,item_name,tree_name);
        }
    }
}

void inorder_count(item_node * parent,char * item_name,FILE* fp){
    static int count = 1;
    if(strcmp(item_name,parent->name) == 0) {
        fprintf(fp,"item before %s: %d\n",item_name,count);

    };
    if(parent->left) inorder_count(parent->left,item_name,fp);
    count++;
    if(parent->right) inorder_count(parent->right,item_name,fp);

}

void item_before_command(tree_name_node* parent,char * tree_name, char * item_name,FILE * fp){
    tree_name_node * treeNameNode = search_for_name_node(parent,tree_name);
    inorder_count(treeNameNode->theTree,item_name,fp);
}

int height_tree(item_node * t){
    int left = 0;
    int right = 0;

    if(t == NULL) return -1;
    /* Recursion: The height of a nonleaf is one more than the height of its
       tallest child.  Find the height of both children... */

    if(t->left != NULL) left = height_tree(t->left) + 1;
    if(t->right != NULL) right = height_tree(t->right) + 1;

    /* ...and return the tallest. */

    if(left > right) {
        return left;
    }
    return right;
}

void height_command(tree_name_node* root,char * tree_name, FILE * fp){
    tree_name_node * t = search_for_name_node(root,tree_name);
    int left = height_tree(t->theTree->left);
    int right = height_tree(t->theTree->right);
    int difference = abs(left-right);

    char status[] = "not balanced";
    if (difference < 1){
        strcpy(status,"balanced");
    }
    fprintf(fp,"%s: left height %d, right height %d, difference %d, %s\n",tree_name,left,right,difference,status);
}

int count_items(item_node * t ){
    int val = t->count;

    /* The basis case is implicit - if we're a leaf, we will simply return our
       payload. */

    if(t->left) {
        val += count_items(t->left);
    }
    if(t->right) {
        val += count_items(t->right);
    }

    return val;

}
item_node * minValueItem(item_node * node)
{
    item_node * current = node;

    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

tree_name_node * minValueTree(tree_name_node * node){
    tree_name_node * current = node;

    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

tree_name_node * deleteTree(tree_name_node* root,char * key){
    if (root == NULL){
        return root;
    }

    // If the key to be deleted
    // is smaller than the root's
    // key, then it lies in left subtree
    if (strcmp(root->treeName,key) > 0){
        root->left = deleteTree(root->left, key);
    }
        // If the key to be deleted
        // is greater than the root's
        // key, then it lies in right subtree
    else if (strcmp(root->treeName,key) < 0){
        root->right = deleteTree(root->right, key);
    }

        // if key is same as root's key,
        // then This is the node
        // to be deleted
    else {
        // node with only one child or no child
        if (root->left == NULL) {
            tree_name_node * temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            tree_name_node * temp = root->left;
            free(root);
            return temp;
        }

        // node with two children:
        // Get the inorder successor
        // (smallest in the right subtree)
        tree_name_node * temp = minValueTree(root->right);

        // Copy the inorder
        // successor's content to this node
        strcpy(root->treeName , temp->treeName);
        root->theTree = temp->theTree;

        // Delete the inorder successor
        root->right = deleteTree(root->right, temp->treeName);
    }
    return root;
}

item_node * deleteItem(item_node * root, char * key)
{
    // base case
    if (root == NULL){
        return root;
    }

    // If the key to be deleted
    // is smaller than the root's
    // key, then it lies in left subtree
    if (strcmp(root->name,key) > 0){
        root->left = deleteItem(root->left, key);
    }
        // If the key to be deleted
        // is greater than the root's
        // key, then it lies in right subtree
    else if (strcmp(root->name,key) < 0){
        root->right = deleteItem(root->right, key);
    }
        // if key is same as root's key,
        // then This is the node
        // to be deleted
    else {
        // node with only one child or no child
        if (root->left == NULL) {
            item_node * temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            item_node * temp = root->left;
            free(root);
            return temp;
        }

        // node with two children:
        // Get the inorder successor
        // (smallest in the right subtree)
        item_node * temp = minValueItem(root->right);

        // Copy the inorder
        // successor's content to this node
        strcpy(root->name , temp->name);
        root->count = temp->count;

        // Delete the inorder successor
        root->right = deleteItem(root->right, temp->name);
    }
    return root;
}

void delete_command(tree_name_node * root,char * tree_name, char * item_name,FILE* fp){
    item_node * itemNode = search_for_name_node(root,tree_name)->theTree;
    itemNode =  deleteItem(itemNode,item_name);
    if(itemNode){
        fprintf(fp,"%s deleted from %s\n",item_name,tree_name);
    }
}

void delete_tree_command(tree_name_node* root, char * name,FILE* fp){
    tree_name_node * t = deleteTree(root,name);
    if(t){
        fprintf(fp,"%s deleted\n",name);
    }
}

void count_command(tree_name_node* root,char* tree_name,FILE* fp){
    item_node * item_root = search_for_name_node(root,tree_name)->theTree;

    int count  = count_items(item_root);
    fprintf(fp,"%s count %d\n",tree_name,count);
}

void proceed_commands(FILE* fp,tree_name_node * parent,int numberOfCommands,FILE * outFp){
    char first_command[35];
    char second_command[35];
    char third_command[35];

    fprintf(outFp,"=====Processing Commands=====\n");
    while(numberOfCommands > 0){
        fscanf(fp,"%s",first_command);

        if(strcmp(first_command,"search") == 0){
            fscanf(fp,"%s %s",second_command,third_command);
            search_command(parent,second_command,third_command,outFp);
        }
        else if(strcmp(first_command,"item_before") == 0){
            fscanf(fp,"%s %s",second_command,third_command);
            item_before_command(parent,second_command,third_command,outFp);
        }
        else if(strcmp(first_command,"height_balance") == 0){
            fscanf(fp,"%s",second_command);
            height_command(parent,second_command,outFp);
        }
        else if(strcmp(first_command,"count") == 0){
            fscanf(fp,"%s",second_command);
            count_command(parent,second_command,outFp);
        }
        else if(strcmp(first_command,"delete") == 0){
            fscanf(fp,"%s %s",second_command,third_command);
            delete_command(parent,second_command,third_command,outFp);
        }
        else if(strcmp(first_command,"delete_tree") == 0){
            fscanf(fp,"%s",second_command);
            delete_tree_command(parent,second_command,outFp);
        }
        numberOfCommands--;
    }
}

void print_tree_names(tree_name_node* parent, FILE * fp){
    inorder_name_tree(parent,fp);
    fprintf(fp,"\n");
}

void print_items(tree_name_node * parent,FILE * fp){
    if(parent->left) print_items(parent->left,fp);

    fprintf(fp,"===%s===\n",parent->treeName);
    inorder_item_tree(parent->theTree,fp);
    fprintf(fp,"\n");

    if(parent->right) print_items(parent->right,fp);

}
