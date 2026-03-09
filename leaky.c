#include <stdio.h>
int main() {
    int bucket_size, n, outgoing, incoming, store = 0;

    printf("Enter Bucket Size: ");
    scanf("%d", &bucket_size);
    printf("Enter Outgoing Rate: ");
    scanf("%d", &outgoing);
    printf("Enter No of Inputs: ");
    scanf("%d", &n);

    while (n > 0) {
        printf("\nEnter Incoming Packet Size: ");
        scanf("%d", &incoming);

        if (incoming <= (bucket_size - store)) {
            store += incoming;
            printf("Bucket Buffer Size %d out of %d\n", store, bucket_size);
        } else {
            printf("Dropped %d packets\n", incoming - (bucket_size - store));
            store = bucket_size;
            printf("Bucket Buffer Size %d out of %d\n", store, bucket_size);
        }

        store = store - outgoing;
        if (store < 0) store = 0;
        printf("After Outgoing: %d packets left in buffer\n", store);

        n--;
    }

    while (store > 0) {
        printf("\nNo more incoming packets.\n");
        store = store - outgoing;
        if (store < 0) store = 0;
        printf("Draining... After Outgoing: %d packets left in buffer\n", store);
    }

    printf("\nBucket is empty.\n");
    return 0;
}
