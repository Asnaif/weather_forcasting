#include <stdio.h>



#include <stdlib.h>
#include <limits.h> // For INT_MAX and INT_MIN

#define MEMORY_SIZE 1024  // Total RAM size in KB
#define PAGE_SIZE 128      // Page size in KB
#define MAX_PROCESSES 10   // Maximum number of processes
#define TOTAL_PAGES (MEMORY_SIZE / PAGE_SIZE) // Number of pages

typedef struct {
    int id;
    int size;
    int pages;
    int allocated;
    int arrival_time;
} Process;

typedef struct {
    int process_id;
} PageTable;

// Global Variables
PageTable memory[TOTAL_PAGES];
Process processes[MAX_PROCESSES];
int fifo_index = 0; // For FIFO Page Replacement

// Function Declarations
void initialize_memory();
void allocate_memory(Process *p);
void deallocate_memory(int process_id, int num_processes);
void display_memory_state();
void display_process_table(int num_processes);
void save_memory_state();
void calculate_process_stats(int num_processes);

int main() {
    int num_processes = 0;

    initialize_memory();

    while (num_processes < MAX_PROCESSES) {
        printf("\nEnter process ID, memory size (KB), and arrival time for process %d (or enter -1 to stop): ", num_processes + 1);
        int id, size, arrival_time;
        scanf("%d", &id);

        if (id == -1) break; // Stop input if user enters -1

        scanf("%d %d", &size, &arrival_time);

        processes[num_processes].id = id;
        processes[num_processes].size = size;
        processes[num_processes].arrival_time = arrival_time;
        processes[num_processes].pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
        processes[num_processes].allocated = 0;

        allocate_memory(&processes[num_processes]);
        calculate_process_stats(num_processes + 1); // Update statistics
        num_processes++;
    }

    display_process_table(num_processes);

    // Let user manually deallocate a process
    while (1) {
        int process_id;
        printf("\nEnter a process ID to deallocate (-1 to exit): ");
        scanf("%d", &process_id);

        if (process_id == -1) break;

        deallocate_memory(process_id, num_processes);
        calculate_process_stats(num_processes); // Update stats after deallocation
        display_process_table(num_processes); // Show updated process table
    }

    return 0;
}

// Initialize memory (all pages are free)
void initialize_memory() {
    for (int i = 0; i < TOTAL_PAGES; i++) {
        memory[i].process_id = -1;
    }
}

// Allocate memory using paging
void allocate_memory(Process *p) {
    int allocated_pages = 0;

    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (memory[i].process_id == -1) {
            memory[i].process_id = p->id;
            allocated_pages++;
            if (allocated_pages == p->pages) {
                p->allocated = 1;
                printf("Process %d allocated %d pages.\n", p->id, p->pages);
                display_memory_state();
                save_memory_state();
                return;
            }
        }
    }

    // Not enough pages, apply FIFO Page Replacement
    printf("Process %d requires %d pages, but memory is full! Applying FIFO Replacement.\n", p->id, p->pages);
    for (int i = 0; i < p->pages; i++) {
        memory[fifo_index].process_id = p->id;
        fifo_index = (fifo_index + 1) % TOTAL_PAGES;
    }
    p->allocated = 1;
    display_memory_state();
    save_memory_state();
}

// Deallocate a process and update statistics
void deallocate_memory(int process_id, int num_processes) {
    int found = 0;
    int freed_size = 0;

    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (memory[i].process_id == process_id) {
            memory[i].process_id = -1;
            found = 1;
        }
    }

    // Find and update process status
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].id == process_id && processes[i].allocated) {
            freed_size = processes[i].size;
            processes[i].allocated = 0; // Mark process as deallocated
            break;
        }
    }

    if (found) {
        printf("Process %d deallocated. Freed %d KB.\n", process_id, freed_size);
        display_memory_state();
        save_memory_state();
    } else {
        printf("Process %d not found in memory.\n", process_id);
    }
}

// Calculate and display process stats (min/max/average size)
void calculate_process_stats(int num_processes) {
    if (num_processes == 0) return; // Avoid division by zero

    int total_size = 0, min_size = INT_MAX, max_size = INT_MIN;
    int used_memory = 0, free_memory = MEMORY_SIZE;

    for (int i = 0; i < num_processes; i++) {
        if (processes[i].allocated) {
            total_size += processes[i].size;
            if (processes[i].size < min_size) min_size = processes[i].size;
            if (processes[i].size > max_size) max_size = processes[i].size;
            used_memory += processes[i].size;
        }
    }

    if (total_size == 0) {
        min_size = 0; // No processes in memory
    }

    float avg_size = num_processes > 0 ? (float)total_size / num_processes : 0;
    free_memory = MEMORY_SIZE - used_memory;

    printf("\n📊 Memory Statistics:\n");
    printf("🔹 Average Process Size: %.2f KB\n", avg_size);
    printf("🔹 Min Process Size: %d KB\n", min_size);
    printf("🔹 Max Process Size: %d KB\n", max_size);
    printf("🔹 Total RAM Available: %d KB\n", MEMORY_SIZE);
    printf("🔹 Used Memory: %d KB\n", used_memory);
    printf("🔹 Free Memory: %d KB\n", free_memory);
}

// Display memory state
void display_memory_state() {
    printf("\nCurrent Memory State:\n");
    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (memory[i].process_id == -1)
            printf("[ Free ] ");
        else
            printf("[ P%d ] ", memory[i].process_id);
    }
    printf("\n");
}

// Display Process Table
void display_process_table(int num_processes) {
    printf("\nProcess Table:\n");
    printf("+------------+----------+--------+--------------+------------+\n");
    printf("| Process ID |  Size KB | Pages  | Arrival Time | Allocated  |\n");
    printf("+------------+----------+--------+--------------+------------+\n");

    for (int i = 0; i < num_processes; i++) {
        printf("| %10d | %8d | %6d | %12d | %10s |\n",
               processes[i].id, processes[i].size, processes[i].pages, processes[i].arrival_time,
               processes[i].allocated ? "YES" : "NO");
    }

    printf("+------------+----------+--------+--------------+------------+\n");
}

// Save memory state to a file
void save_memory_state() {
    FILE *file = fopen("memory_state.txt", "w");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "Memory State:\n");
    for (int i = 0; i < TOTAL_PAGES; i++) {
        fprintf(file, "[ P%d ] ", memory[i].process_id);
    }
    fprintf(file, "\n");

    fclose(file);
    printf("Memory state saved to 'memory_state.txt'\n");
}
