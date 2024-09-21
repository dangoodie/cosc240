/*
 * An implementation of the scoring scheduling algorithm.
 * Author: Daniel Gooden (dgooden@myune.edu.au)
 * Modifies the fcfs.c file provided by David Paul
 */

#include <stdio.h>
#include <stdlib.h>

#define NUM_PRIORITY_QUEUES 2
#define MAX_PROCESS_HISTORY 100

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* The process details we're interested in for the FCFS algorithm.*/
typedef struct constant_process {
  unsigned int pid;
  unsigned int processing_time;
  unsigned int arrival_time;
  unsigned int processed_time;
  unsigned int quantum_used;
  struct constant_process *next_process;
} constant_process;

/* Priority queues */
constant_process priority_queues[NUM_PRIORITY_QUEUES] = {
    {0, 0, 0, 0, 0, NULL},
    {0, 0, 0, 0, 0, NULL},
};

// Global variable to keep track of the current process
constant_process *current = NULL;
unsigned int priority_level = 0;

/* The process history to keep track of for each process */
typedef struct process_history {
  unsigned int pid;
  unsigned int *total_time_run; // Pointer to the total time the process has run
  unsigned int wait_time;       // Time spent waiting for CPU
  unsigned int
      reschedule_count; // How many times the process has been rescheduled
  double score;         // Learning score, updated frequently
  struct process_history *next;
} process_history;

process_history *process_histories = NULL;

// Function to initialise the process history
void init_process_history(constant_process *process) {
  if (process_histories == NULL) {
    // Allocate sentinel node (if necessary)
    process_histories = malloc(sizeof(process_history));
    if (!process_histories) {
      perror("Failed to allocate memory for process history");
      exit(1); // Handle memory allocation failure
    }
    process_histories->pid = 0; // Sentinel node has no real PID
    process_histories->total_time_run = NULL;
    process_histories->wait_time = 0;
    process_histories->reschedule_count = 0;
    process_histories->score = 0.0;
    process_histories->next = NULL; // List initially empty after sentinel
  }

  // Now add the new process history after the sentinel node
  process_history *new_history = malloc(sizeof(process_history));
  if (!new_history) {
    perror("Failed to allocate memory for process history");
    exit(1);
  }

  new_history->pid = process->pid;
  new_history->total_time_run =
      &process->processed_time; // Pointer to process's processed_time
  new_history->wait_time = 0;
  new_history->reschedule_count = 0;
  new_history->score = 0.0;

  // Insert new history after the sentinel node
  new_history->next = process_histories->next;
  process_histories->next = new_history;
}

// Function to get the process history for a given PID
process_history *get_process_history(unsigned int pid) {
  process_history *history =
      process_histories->next; // Start after the sentinel

  // Traverse the list to find the history with the matching PID
  while (history != NULL) {
    if (history->pid == pid) {
      return history; // Found the process history
    }
    history = history->next;
  }

  return NULL; // History not found
}

// Function to free the process history for a given PID
void free_process_history(unsigned int pid) {
  process_history *prev = process_histories; // Start with sentinel
  process_history *current = process_histories->next;

  // Traverse the list to find the process history to free
  while (current != NULL) {
    if (current->pid == pid) {
      // Found the process, remove it from the list
      prev->next = current->next;
      free(current); // Free the process history
      return;
    }
    prev = current;
    current = current->next;
  }
}

// Function to calculate a process score based on its history
double calculate_new_score(process_history *history) {
  if (history->total_time_run == NULL) {
    fprintf(stderr, "Error: total_time_run pointer is NULL for PID %d\n",
            history->pid);
    return -1.0; // Or some other default/fallback value
  }

  double waiting_factor = history->wait_time + 1;
  double running_factor = *history->total_time_run + 1;
  double reschedule_penalty = history->reschedule_count + 1;

  // Score formula
  history->score =
      (waiting_factor / running_factor) - (reschedule_penalty * 0.5);
  return history->score;
}

/*
 * Prints out the list of all constant_processes after the given one.
 * parameters:
 *   node: The constant_process to print all later processes from
 */
void print_list(constant_process *node) {
  while (node->next_process) {
    constant_process *next = node->next_process;
    printf(
        "\tpid: %d, processing_time %d, arrival_time: %d, processed_time: %d, "
        "next_process.pid: %d\n",
        next->pid, next->processing_time, next->arrival_time,
        next->processed_time, next->next_process ? next->next_process->pid : 0);
    node = node->next_process;
  }
}

/*
 * Adds the constant_process pointed to by next to be immediately after node.
 * parameters:
 *   node: The constant_process to add the next fcfs_process after
 *   next: The new next_process for node
 */
void add_next(constant_process *node, constant_process *next) {
  next->next_process = node->next_process;
  node->next_process = next;
}

/*
 * Removes the next_process after the given node.
 * If the next_process has a next_process, that next_process becomes node's
 * next_process. parameters: node: The constant_process to remove the immediate
 * next_process from returns: The constant_process removed from next to the
 * given node
 */
constant_process *remove_next(constant_process *node) {
  constant_process *next = node->next_process;
  if (next) {
    node->next_process = next->next_process;
  }
  return next;
}

/*
 * Adds the given process to the ready queue, indicating it is ready to be
 * scheduled. Keeps a list of all processes, sorted by arrival time, then pid.
 * parameters:
 *   process - the process to add to the ready queue
 */
void add_to_ready_queue(const process_initial process) {
  // Construct the new constant_process
  constant_process *new_process = malloc(sizeof(constant_process));
  if (!new_process) {
    perror("Failed to allocate memory for new process");
    exit(1);
  }
  new_process->pid = process.pid;
  new_process->processing_time = process.processing_time;
  new_process->arrival_time = process.arrival_time;
  new_process->processed_time = 0;
  new_process->quantum_used = 0;
  new_process->next_process = NULL;

  // Initialise the process history
  init_process_history(new_process);

  // Determine where in the queue it should be added
  int priority_level = 0; // Default to fast queue
  constant_process *end = &priority_queues[priority_level];
  // Skip past processes with an earlier arrival time
  while (end->next_process &&
         end->next_process->arrival_time < new_process->arrival_time) {
    end = end->next_process;
  }
  // Skip past processes with the same arrival time but an earlier PID
  while (end->next_process &&
         end->next_process->arrival_time == new_process->arrival_time &&
         end->next_process->pid < new_process->pid) {
    end = end->next_process;
  }

  // Add new process to the queue in the correct location
  add_next(end, new_process);

  // If in debug mode, print out the process list after it has changed
  if (debug) {
    if (current) {
      printf("Current process: pid %d\n", current->pid);
    }
    printf("Process list after process with pid %d has been added:\n",
           new_process->pid);
    for (int i = 0; i < NUM_PRIORITY_QUEUES; i++) {
      printf("- Priority queue %d:\n", i);
      print_list(&priority_queues[i]);
    }
  }
}

/*
 * Moves the process with the given PID to the smart queue.
 */
void move_to_smart_queue(constant_process *process) {
  constant_process *end = &priority_queues[1]; // Q2 (smart queue)

  while (end->next_process) {
    end = end->next_process;
  }

  // Move process to the end of the smart queue
  process->next_process = NULL;
  end->next_process = process;

  if (debug) {
    printf("Process with PID %d moved to Smart Queue (Q2)\n", process->pid);
  }
}

/*
 * Updates the wait time for all processes in the queue.
 */
void update_wait_times() {
  for (int i = 0; i < NUM_PRIORITY_QUEUES; i++) {
    constant_process *node = &priority_queues[i];
    while (node->next_process) {
      constant_process *next = node->next_process;
      process_history *history =
          get_process_history(next->pid); // Use get_process_history()
      if (history) {
        history->wait_time++;
      }
      node = next;
    }
  }
}

/*
 * Selects the best process from the smart queue based on the process history.
 * The process with the highest score is selected.
 * Returns the selected process.
 */
constant_process *select_best_process_from_smart_queue() {
  constant_process *best_process = NULL;
  double best_score = -1.0;

  constant_process *node =
      priority_queues[1].next_process; // Start with Q2 (smart queue)
  while (node != NULL) {
    process_history *history =
        get_process_history(node->pid); // Use get_process_history()
    if (history == NULL) {
      fprintf(stderr, "Error: No process history for PID %d\n", node->pid);
      break; // Exit the loop or handle it appropriately
    }

    double score = calculate_new_score(history); // Recalculate score

    if (score > best_score) {
      best_score = score;
      best_process = node;
    }

    node = node->next_process;
  }

  // Remove the best process from the smart queue
  if (best_process != NULL) {
    constant_process *prev = &priority_queues[1];
    while (prev->next_process != best_process) {
      prev = prev->next_process;
    }
    prev->next_process = best_process->next_process; // Remove from queue
    best_process->next_process = NULL;
  }

  if (debug && best_process != NULL) {
    printf("Best process selected from Smart Queue: PID %d with score %.2f\n",
           best_process->pid, best_score);
  }

  return best_process;
}

/*
 * Determines the next process to the scheduled.
 * Implements a scoring algorithm to determine the best process to
 * schedule next.
 *  returns:
 * The PID of the process to be scheduled next, or 0 if no process should be
 * scheduled
 */
unsigned int get_next_scheduled_process() {
  if (current == NULL) {
    // First check Fast Queue (Q1)
    if (priority_queues[0].next_process) {
      current = remove_next(&priority_queues[0]);
      priority_level = 0;
    }
    // If no process in Fast Queue, check Smart Queue (Q2)
    else if (priority_queues[1].next_process) {
      current = select_best_process_from_smart_queue();
      priority_level = 1;
    }
  }

  // If no process found, return 0
  if (current == NULL) {
    return 0;
  }

  // Update wait times for all processes
  update_wait_times();

  // Run the current process
  current->processed_time++;
  current->quantum_used++;

  unsigned int pid = current->pid;
  process_history *history =
      get_process_history(pid); // Use get_process_history()
  if (history == NULL) {
    fprintf(stderr, "Error: No process history for PID %d\n", pid);
  }

  // Check if the process is done
  if (current->processed_time == current->processing_time) {
    free(current);
    free_process_history(pid);
    current = NULL;
    return pid;
  }

  // Move process to Smart Queue (Q2) if quantum exceeded
  if (current->quantum_used >=
      (priority_level + 1) * 2) { // Quantum of 2 for Q1, 4 for Q2
    move_to_smart_queue(current);
    current->quantum_used = 0;
    current = NULL;
  }

  return pid;
}
