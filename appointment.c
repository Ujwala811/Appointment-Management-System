#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 30 // Maximum number of patients

// Structure for patient details
struct Patient {
    char name[50];
    int age;
    int token;
};

// linked list Node structure for the queue
struct Node {
    struct Patient data;
    struct Node* next;
};

// Function prototypes
void enqueue(struct Node** front, struct Node** rear, struct Patient patient, int* patientCount);
void dequeue(struct Node** front, struct Node** rear, int* patientCount);
void displayCurrentPatient(struct Node* front, int patientCount);
void displayCompletedAppointments(struct Node* front, int patientCount);

int main() {
    struct Node *front = NULL, *rear = NULL;
    int choice;
    int patientCount = 0; // Counter for patient count
    printf("\nWelcome to Appointment Management System\n");
    do {
        printf("\n\nPatients remaining: %d\n\n", patientCount);
        printf("1. Add Patient\n");
        printf("2. Remove Patient\n");
        printf("3. Display Current Patient\n");
        printf("4. Display Completed Appointments\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                if (patientCount < MAX_PATIENTS) {
                    struct Patient newPatient;
                    printf("Enter patient name: ");
                    scanf("%s", newPatient.name);
                    printf("Enter patient age: ");
                    scanf("%d", &newPatient.age);
                    newPatient.token = 0; // Initialize token to 0 for automatic assignment
                    enqueue(&front, &rear, newPatient, &patientCount);
                } else {
                    printf("Appointments are closed. Maximum number of patients reached.\n");
                }
                break;
            }
            case 2:
                dequeue(&front, &rear, &patientCount);
                break;
            case 3:
                displayCurrentPatient(front, patientCount);
                break;
            case 4:
                displayCompletedAppointments(front, patientCount);
                break;
            case 5:
                printf("Exiting program. Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 5);

    while (front != NULL) {
        struct Node* temp = front;
        front = front->next;
        free(temp);
    }

    return 0;
}

void enqueue(struct Node** front, struct Node** rear, struct Patient patient, int* patientCount) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed. Unable to add patient.\n");
        return;
    }

    int token;
    do {
        printf("Enter token number for the patient (1-%d): ", MAX_PATIENTS);
        scanf("%d", &token);

        if (token < 1 || token > MAX_PATIENTS) {
            printf("Invalid token number. Please enter a number between 1 and %d.\n", MAX_PATIENTS);
        } else {
            struct Node* temp = *front;
            while (temp != NULL) {
                if (temp->data.token == token) {
                    printf("Token %d is already taken by another patient. Please choose another token.\n", token);
                    break;
                }
                temp = temp->next;
            }
            if (temp == NULL) {
                break; // Token is valid and not taken, exit loop
            }
        }
    } while (1);

    patient.token = token;
    newNode->data = patient;
    newNode->next = NULL;

    struct Node* current = *front;
    struct Node* prev = NULL;
    while (current != NULL && current->data.token < token) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        newNode->next = *front;
        *front = newNode;
    } else {
        newNode->next = current;
        prev->next = newNode;
    }

    if (newNode->next == NULL) {
        *rear = newNode;
    }

    FILE* file = fopen("patient_details.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
    } else {
        fprintf(file, "Name: %s, Age: %d, Token: %d\n", patient.name, patient.age, patient.token);
        fclose(file);
    }

    (*patientCount)++;
    printf("Patient %s added successfully with token number: %d\n", patient.name, patient.token);
}

void dequeue(struct Node** front, struct Node** rear, int* patientCount) {
    if (*front == NULL) {
        printf("No patients in the queue.\n");
        return;
    }

    struct Node* temp = *front;
    *front = (*front)->next;
    if (*front == NULL) {
        *rear = NULL;
    }
    free(temp);
    (*patientCount)--;
    printf("Patient removed successfully.\n");
}

void displayCurrentPatient(struct Node* front, int patientCount) {
    if (front == NULL) {
        printf("No patients in the queue.\n");
        return;
    }

    printf("Current patient:\n");
    printf("Name: %s\n", front->data.name);
    printf("Age: %d\n", front->data.age);
    printf("Token number: %d\n", front->data.token);
}

void displayCompletedAppointments(struct Node* front, int patientCount) {
    if (front == NULL) {
        printf("No completed appointments.\n");
        return;
    }

    printf("Completed Appointments:\n");
    struct Node* current = front;
    while (current != NULL) {
        printf("Name: %s, Age: %d, Token: %d\n", current->data.name, current->data.age, current->data.token);
        current = current->next;
    }
}

