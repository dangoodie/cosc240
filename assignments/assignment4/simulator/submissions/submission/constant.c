/*
 * An implementation of the Round Robin scheduling algorithm.
 * Author: Daniel Gooden (dgooden@myune.edu.au)
 * Modifies the fcfs.c file provided by David Paul
 */

#include <stdio.h>
#include <stdlib.h>

/* The process details we're interested in for the FCFS algorithm.*/
typedef struct constant_process {
  unsigned int pid;
  unsigned int processing_time;
  unsigned int arrival_time;
  unsigned int processed_time;
  unsigned int quantum_used;
  struct constant_process *next_process;
} constant_process;

#define NUM_PRIORITY_QUEUES 4

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Priotity queues */
constant_process priority_queues[NUM_PRIORITY_QUEUES] = {{0, 0, 0, 0, 0, NULL},
                                                         {0, 0, 0, 0, 0, NULL},
                                                         {0, 0, 0, 0, 0, NULL},
                                                         {0, 0, 0, 0, 0, NULL}};
/*
 * Prints out the list of all constant_processes after the given one.
 * parameters:
 *   node: The constant_process to print all later processes from
 */
void print_list(constant_process *node) {
  while (node->next_process) {
    constant_process *next = node->next_process;
    printf("pid: %d, processing_time %d, arrival_time: %d, processed_time: %d, "
           "next_process.pid: %d\n",
           next->pid, next->processing_time, next->arrival_time,
           next->processed_time,
           next->next_process ? next->next_process->pid : 0);
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
  new_process->pid = process.pid;
  new_process->processing_time = process.processing_time;
  new_process->arrival_time = process.arrival_time;
  new_process->processed_time = 0;
  new_process->quantum_used = 0;
  new_process->next_process = NULL;

  // Determine where in the queue it should be added
  int priority_level = 0; // Default to highest priority
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
    for (int i = 0; i < NUM_PRIORITY_QUEUES; i++) {
      printf("Priority queue %d:\n", i);
      print_list(&priority_queues[i]);
    }
  }
}

/*
 * Determines the next process to the scheduled.
 * Implements a multi-level feedback queue with 4 priority levels.
 * Each priority level has a quantum of 3 units of time determined by the
 * QUANTUM constant. 3k units of time are given to the each process in the kth
 * priority level before moving to the next priority level.
 *
 *  returns:
 * The PID of the process to be scheduled next, or 0 if no process should be
 * scheduled
 */

// The quantum for each priority level
#define QUANTUM 3

// Global variable to keep track of the current process
constant_process *current = NULL;

unsigned int get_next_scheduled_process() {
  // Find the next process to schedule
  unsigned int priority_level = 0;
  if (current == NULL) {
    for (int i = 0; i < NUM_PRIORITY_QUEUES; i++) {
      if (priority_queues[i].next_process) {
        current = remove_next(&priority_queues[i]);
        priority_level = i;
        break;
      }
    }
  }

  // Nothing to schedule so return 0
  if (current == NULL) {
    return 0;
  }

  // Execute the process for one unit of time
  current->processed_time++;
  current->quantum_used++;

  unsigned int pid = current->pid;

  // If the process has finished, remove it from the list
  if (current->processed_time == current->processing_time) {
    free(current);
    current = NULL;

    // If in debug mode, print out the process list after it has changed
    if (debug) {
      for (int i = 0; i < NUM_PRIORITY_QUEUES; i++) {
        printf("Priority queue %d:\n", i);
        print_list(&priority_queues[i]);
      }
    }
    return pid;
  }

  // If the process has used up its quantum, move it to the next priority queue
  if (current->quantum_used == QUANTUM * priority_level + 1) {
    // Determine the new priority level
    unsigned int new_priority_level =
        MIN(priority_level + 1, NUM_PRIORITY_QUEUES - 1);

    // Move the process to the new priority level
    constant_process *end = &priority_queues[new_priority_level];
    while (end->next_process) {
      end = end->next_process;
    }
    current->quantum_used = 0;
    add_next(end, current);
    current = NULL;
  }

  return pid; // Return the PID of the process that was scheduled
}
