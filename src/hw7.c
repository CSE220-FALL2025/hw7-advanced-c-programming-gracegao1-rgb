#include "hw7.h"

// Grace Gao 115533642

// new matrix format:
// A=NR NC[1 2 3 ... NC ; 1 ... NC ; ...]\n

// formula definitions
// B=(A op B)
// precendence (high to low): transpose ( ' ), mult ( * ), add ( + )

// MATRIX_SF -- FOR CREATING MATRICES
// allocate mem for struct: matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
// struct has name, num_rows, num_cols, values
// deallocate: free(m);

// BST_SF -- FOR CREATING MATRIX NODES
// has left_child, right_child, mat (itself)
// left_child, right_child NULL if no child

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    // create node for mat
    bst_sf *node = malloc(sizeof(bst_sf));
    node->left_child = NULL;
    node->right_child = NULL;
    node->mat = mat;
    // if root is NULL take node to be the new root
    if(root == NULL){
        return node;
    }
    // set curr to root
    bst_sf *curr = root;
    // while curr has children
    while(curr->left_child || curr->right_child){
        // if name is less than curr name
        if(mat->name < curr->mat->name){
            // if left_child is NULL set left_child to node
            // then break
            if(!curr->left_child){
                curr->left_child = node;
                break;
            }
            // check left_child
            curr = curr->left_child;
        }
        // if name is greater than curr name
        else{
            // if right_child is NULL set right_child to node
            if(!curr->right_child){
                curr->right_child = node;
                break;
            }
            /// check right_child
            curr = curr->right_child;
        }
    }
    // if no children set left or right child to node
    // only runs if node not already inserted
    if(!curr->left_child && !curr->right_child){
        if(mat->name < curr->mat->name){
            curr = curr->left_child;
        }
        else{
            curr = curr->right_child;
        }
    }
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    // if root NULL report not found
    if(root == NULL){
        return NULL;
    }
    // set current to root
    bst_sf *curr = root;
    // while current still has children
    while(curr->left_child || curr->right_child){
        // if name is less than curr name go left
        if(name < curr->mat->name){
            // if no left child report not found
            if(!curr->left_child){
                return NULL;
            }
            curr = curr->left_child;
        }
        // if name is greater than curr name go right
        else if(name > curr->mat->name){
            // if no right child report not found
            if(!curr->right_child){
                return NULL;
            }
            curr = curr->right_child;
        }
        // if name equals curr name return curr matrix
        else{
            return curr->mat;
        }
    }
    // if curr is a match return curr matrix
    if(curr->mat->name == name){
        return curr->mat;
    }
    // else curr has no children to check, return not found
    return NULL;
}

void free_bst_sf(bst_sf *root) {
    // if root is NULL immediately return
    if(root == NULL){
        return;
    }
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);
    free(root->mat);
    free(root);
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    // set num_rows and num_cols to be the same as mat1 and mat2
    // given that the addition is valid so no check required
    int num_rows = mat1->num_rows;
    int num_cols = mat1->num_cols;
    // allocate memory for matrix of the sum
    matrix_sf *sum = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    sum->name = strcat(strcat("add_", mat1->name),mat2->name);
    sum->num_rows = num_rows;
    sum->num_cols = num_cols;
    // perform addition
    for(int i = 0; i < len(mat1->values); i++){
        sum->values[i] = mat1->values[i] + mat2->values[i];
    }
    return sum;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    // result has mat1_rows and mat2_cols
    int mat1_rows = mat1->num_rows;
    int mat1_cols = mat1->num_cols;
    int mat2_cols = mat2->num_cols;
    // allocate memory for matrix of the product
    matrix_sf *product = malloc(sizeof(matrix_sf)+mat1_rows*mat2_cols*sizeof(int));
    product->name = strcat(strcat("mult_", mat1->name),mat2->name);
    product->num_rows = mat1_rows;
    product->num_cols = mat2_cols;
    // for mat1_rows * mat2_cols (num of values in product)
    // perform matrix multiplication
    int index = 0;
    for(int i = 0; i < mat1_rows; i++){
        for(int j = 0; j < mat2_cols; j++){
            int sum = 0;
            for(int k = 0; k < mat1_cols; k++){
                sum += mat1->values[(i*mat1_cols)+k] * mat2->values[i + (mat2_cols*k)];
            }
            product->values[index] = sum;
            index++;
        }
    }
   return product;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    // swap row and column counts
    int num_rows = mat->num_cols;
    int num_cols = mat->num_rows;
    // allocate memory for transpose of matrix
    matrix_sf *transpose = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    transpose->name = strcat("transpose_", mat->name);
    transpose->num_rows = num_rows;
    transpose->num_cols = num_cols;
    // import values from original matrix to new matrix
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            transpose->values[(i*num_cols) + j] = mat->values[(j*num_rows) + i];
        }
    }
    return transpose;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    // get row and column count (first two positive integers)
    int dim[2] = {0,0};
    int to_parse = 2, curr = 0, start = 0;
    char *p;
    while(expr[curr]){
        if(to_parse == 0){
            break;
        }
        if(isspace(expr[curr])){
            strncpy(p, expr + start, curr - start + 1);
            dim[2 - to_parse] = atoi(p);
            to_parse--;
            start = curr;
        }
        curr++;
    }
    // if while-loop terminated before adding cols
    // add cols here
    if(to_parse > 0){
        strncpy(p, expr + start, curr - start + 1);
        dim[2 - to_parse] = atoi(p);
    }
    // allocate memory for matrix
    matrix_sf *create = malloc(sizeof(matrix_sf)+(dim[0])*(dim[1])*sizeof(int));
    create->name = name;
    create->num_rows = dim[0];
    create->num_cols = dim[1];
    // while the null terminator is not encountered
    int i = 0;
    while(expr[curr]){
        // if the current char is not a digit
        // parse for an integer from expr[start] to expr[curr]
        if(!isdigit(expr[curr])){
            // check that the leading chars are not [] or ;
            while(!isdigit(expr[start]) && expr[start] != "-"){
                start++;
            }
            strncpy(p, expr + start, curr - start + 1);
            create->values[i++] = atoi(p);
            start = curr;
        }
        curr++;
    }
    // parse the last digit in string
    while(!isdigit(expr[start]) && expr[start] != "-"){
        start++;
    }
    strncpy(p, expr + start, curr - start + 1);
    create->values[i] = atoi(p);
    return create;
}

char* infix2postfix_sf(char *infix) {
    return NULL;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    return NULL;
}

matrix_sf *execute_script_sf(char *filename) {
   return NULL;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
