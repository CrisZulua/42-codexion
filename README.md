*This project has been made as part of the 42 curriculum by czuluaga.*

# Codexion

## Description

Codexion is a concurrency project inspired by the dining philosophers problem and shared resource management. In this case, a group of coders competes to take two adjacent dongles in order to compile their work. The main goal is to simulate concurrent coder execution with burnout limits, compile/debug/refactor times, and a cooldown period for dongles after they are released.

This solution uses POSIX threads (`pthread`) to model each coder and a monitor thread that supervises global state. Coders wait for shared resources, update their burnout status, and serialize log output to avoid mixed messages.

## Instructions

### Build

From the project root:

```sh
make
```

The project produces the executable `codexion`.

### Run

The command line accepts 8 parameters:

```sh
./codexion <n_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <n_compiles_required> <dongle_cooldown> <scheduler(fifo|edf)>
```

- `n_coders`: number of coders/threads.
- `time_to_burnout`: maximum time in milliseconds before burnout.
- `time_to_compile`: compile time in milliseconds.
- `time_to_debug`: debug time in milliseconds.
- `time_to_refactor`: refactor time in milliseconds.
- `n_compiles_required`: compilations required to finish.
- `dongle_cooldown`: cooldown in milliseconds after releasing a dongle.
- `scheduler`: priority policy; can be `fifo` or `edf`.

Example:

```sh
./codexion 5 800 200 200 200 3 100 fifo
```

### Clean

```sh
make fclean
```

## Project flow

1. `main` parses the arguments, initializes the simulation, and starts the monitor thread.
2. It creates one thread per coder, each running `coder_routine`.
3. Each coder enters a loop where it:
   - requests two adjacent dongles using `request_dongles`.
   - compiles with `compile` if it successfully acquires the dongles.
   - releases the dongles with `realease_dongles`.
   - if it has not completed all required compilations, performs debug and refactor.
4. The monitor thread periodically checks if any coder has burned out or if all coders have finished.
5. When the simulation ends, it joins all threads and frees allocated memory.

## Blocking cases handled

- All coders request both dongles at the same time. This strategy avoids a coder taking only one dongle and remaining blocked waiting for the second, which could cause a partial deadlock.
- The main blocking condition is handled with a global mutex (`sim->lock`) and a priority queue to control ordered access to the dongles.
- Starvation is avoided using a priority heap with two policies:
  - `fifo`: coders are served in arrival order.
  - `edf`: coders with less remaining time before burnout have priority.
- Burnout is controlled with a per-coder timer. If a coder exceeds its `to_bournout_ms`, the monitor stops the simulation and reports the burnout event.
- Logging is serialized with `print_lock` to ensure status messages do not interleave on standard output.

## Thread synchronization mechanisms

### Mutexes

- `pthread_mutex_t lock`: global mutex used to protect access to shared dongles and the `sim->running` state.
- `pthread_mutex_t print_lock`: mutex to serialize `printf` access and ensure consistent logs.
- `pthread_mutex_t burnout_mx`: per-coder mutex to protect reads and writes of `to_bournout_ms`.
- `pthread_mutex_t ended_mx`: per-coder mutex to protect reads and writes of `finished`.

### Condition variable

- `pthread_cond_t dongles_freed`: used to notify coders that a pair of dongles has been released. When a coder releases dongles, it calls `pthread_cond_broadcast` to wake all waiting threads.

### Coordination example

- A coder calls `request_dongles` and acquires `sim->lock`.
- If it is not its turn or the dongles are not free, it waits with `pthread_cond_timedwait`.
- When the dongles are released, `realease_dongles` updates their state and wakes all waiting threads with `pthread_cond_broadcast`.
- The first coder in the priority queue that has both dongles available is allowed to continue.

### Race conditions prevented

- Access to `sim->dongles` and simulation state is always done under `sim->lock`.
- Compilation count and completion state are protected by per-coder mutexes.
- Access to `sim->running` is protected by the same global mutex so the monitor and coders read/write a consistent state.

## Resources

- POSIX Threads documentation: https://man7.org/linux/man-pages/man7/pthreads.7.html
- `pthread_mutex_t` and `pthread_cond_t`: https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html
- Article about EDF scheduling: https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling
- Basic C thread synchronization tutorial: https://www.geeksforgeeks.org/multithreading-c-2/

## AI usage

AI was used to:

- document design decisions and concurrency mechanisms.
- draft this `README.md` with a clear structure and relevant content.

AI was not used to generate the solution code.
