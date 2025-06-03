#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Bt.h"

int main() {
    insert(10, "Pratham");
    insert(6, "Aman");
    insert(2, "Hardik");
    insert(3, "Burhan");
    insert(5, "Ridin");
    insert(7, "Tanya");
    insert(12, "Yash");
    insert(17, "Divyansh");
    insert(20, "Rahul");
    insert(30, "Shivam");

    int a = 1;
    while (a) {
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Print Tree \n");
        printf("4. Delete \n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            int key;
            char name[100];
            printf("Enter key: ");
            scanf("%d", &key);
            printf("Enter name: ");
            scanf(" %s", name);  // allows spaces
            insert(key, name);
            printf("Inserted (%d, %s)\n", key, name);

        } else if (choice == 2) {
            int key;
            printf("Enter key to search: ");
            scanf("%d", &key);
            search(key, root);

        } else if (choice == 3) {
            printf("\n\nLevel-order traversal:\n");
            levelOrder();

        } else if (choice == 4) {
            printf("Delete not implemented yet.\n");

        } else if (choice == 5) {
            a = 0;
            printf("Exiting.\n");

        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
