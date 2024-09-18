/*
 * An implementation of the Round Robin scheduling algorithm.
 * Author: Daniel Gooden (dgooden@myune.edu.au)
 * Modifies the fcfs.c file provided by David Paul
 */

#include <stdio.h>
#include <stdlib.h>

/* The process details we're interested in for the FCFS algorithm.*/
typedef struct rr_process {
  unsigned int pid;
  unsigned int processing_time;
  unsigned int arrival_time;
  unsigned int processed_time;
  unsigned int quantum_used;
  struct rr_process *next_process;
} rr_process;

/* The list of all processes we know about.*/
rr_process process_list = {0, 0, 0, 0, 0, NULL};

/*
 * Prints out the list of all rr_processes after the given one.
 * parameters:
 *   node: The rr_process to print all later processes from
 */
void print_list(rr_process *node) {
  while (node->next_process) {
    rr_process *next = node->next_process;
    printf("pid: %d, processing_time %d, arrival_time: %d, processed_time: %d, "
           "next_process.pid: %d\n",
           next->pid, next->processing_time, next->arrival_time,
           next->processed_time,
           next->next_process ? next->next_process->pid : 0);
    node = node->next_process;
  }
}

/*
 * Adds the rr_process pointed to by next to be immediately after node.
 * parameters:
 *   node: The rr_process to add the next fcfs_process after
 *   next: The new next_process for node
 */
void add_next(rr_process *node, rr_process *next) {
  next->next_process = node->next_process;
  node->next_process = next;
}

/*
 * Removes the next_process after the given node.
 * If the next_process has a next_process, that next_process becomes node's
 * next_process. parameters: node: The rr_process to remove the immediate
 * next_process from returns: The rr_process removed from next to the given node
 */
rr_process *remove_next(rr_process *node) {
  rr_process *next = node->next_process;
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
  // Construct the new rr_process
  rr_process *new_process = malloc(sizeof(rr_process));
  new_process->pid = process.pid;
  new_process->processing_time = process.processing_time;
  new_process->arrival_time = process.arrival_time;
  new_process->processed_time = 0;
  new_process->quantum_used = 0;
  new_process->next_process = NULL;

  // Determine where in the queue it should be added
  rr_process *end = &process_list;
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
    printf("Process list after adding process with pid %d:\n", process.pid);
    print_list(&process_list);
  }
}

/*
 * Determines the next process to the scheduled.
 * Implements a simple round-robin scheduling algorithm.
 * The round-robin is determined by the QUANTUM constant.
 * The process switches via a modulo operation on the processed_time.
 *
 *  returns:
 * The PID of the process to be scheduled next, or 0 if no process should be
 * scheduled
 */

#define QUANTUM 3
unsigned int get_next_scheduled_process() {
  rr_process *node = &process_list;
  if (node->next_process == NULL) {
    return 0;
  }

  // Get the first process in the list
  rr_process *current = node->next_process;

  // Execute the process for one unit of time
  current->processed_time++;
  current->quantum_used++;

  unsigned int pid = current->pid;

  // If the process has finished, remove it from the list
  if (current->processed_time == current->processing_time) {
    remove_next(node);
    free(current);

    // If in debug mode, print out the process list after it has changed
    if (debug) {
      printf("Process list after process with pid %d has completed:\n", pid);
      print_list(&process_list);
    }
    return pid;
  }

  // If the process has used up its quantum, move it to the end of the list
  if (current->quantum_used == QUANTUM) {
    remove_next(node);
    rr_process *end = &process_list;
    while (end->next_process) {
      end = end->next_process;
    }
    add_next(end, current);
  }

  return pid;
}
