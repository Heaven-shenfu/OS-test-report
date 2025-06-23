#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

// 进程控制块结构
typedef struct {
    int pid;           // 进程ID
    int arrival_time;  // 到达时间
    int burst_time;    // 执行时间
    int priority;      // 优先级（用于优先级调度）
    int start_time;    // 开始执行时间
    int completion_time; // 完成时间
    int turnaround_time; // 周转时间
    int waiting_time;    // 等待时间
    int remaining_time;  // 剩余执行时间（用于抢占式调度）
} Process;

// 按到达时间排序
void sort_by_arrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// 按进程ID排序（用于输出结果）
void sort_by_pid(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].pid > processes[j + 1].pid) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// 1. 先来先服务（FCFS）
void fcfs(Process processes[], int n) {
    printf("\n===== 先来先服务（FCFS）调度 =====\n");

    // 按到达时间排序
    sort_by_arrival(processes, n);

    int current_time = 0;

    for (int i = 0; i < n; i++) {
        // 如果当前时间小于进程到达时间，需要等待
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }

        processes[i].start_time = current_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

        current_time = processes[i].completion_time;
    }

    // 计算平均时间
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // 按PID排序后输出结果
    sort_by_pid(processes, n);
    printf("进程\t到达时间\t执行时间\t完成时间\t周转时间\t等待时间\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
            processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("平均等待时间: %.2f\n", avg_waiting_time);
    printf("平均周转时间: %.2f\n", avg_turnaround_time);
}

// 2. 短作业优先（SJF，非抢占式）
void sjf(Process processes[], int n) {
    printf("\n===== 短作业优先（SJF）调度 =====\n");

    // 按到达时间排序
    sort_by_arrival(processes, n);

    int current_time = 0;
    int completed = 0;
    int visited[MAX_PROCESSES] = { 0 }; // 标记进程是否已完成

    while (completed < n) {
        int shortest_index = -1;
        int shortest_burst = 9999;

        // 查找当前时间已到达且执行时间最短的进程
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival_time <= current_time &&
                processes[i].burst_time < shortest_burst) {
                shortest_index = i;
                shortest_burst = processes[i].burst_time;
            }
        }

        // 如果没有找到符合条件的进程，时间推进到下一个进程的到达时间
        if (shortest_index == -1) {
            current_time++;
            continue;
        }

        visited[shortest_index] = 1;
        completed++;

        processes[shortest_index].start_time = current_time;
        processes[shortest_index].completion_time = current_time + processes[shortest_index].burst_time;
        processes[shortest_index].turnaround_time = processes[shortest_index].completion_time - processes[shortest_index].arrival_time;
        processes[shortest_index].waiting_time = processes[shortest_index].turnaround_time - processes[shortest_index].burst_time;

        current_time = processes[shortest_index].completion_time;
    }

    // 计算平均时间
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // 按PID排序后输出结果
    sort_by_pid(processes, n);
    printf("进程\t到达时间\t执行时间\t完成时间\t周转时间\t等待时间\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
            processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("平均等待时间: %.2f\n", avg_waiting_time);
    printf("平均周转时间: %.2f\n", avg_turnaround_time);
}

// 3. 轮转调度（RR）
void rr(Process processes[], int n, int time_quantum) {
    printf("\n===== 轮转调度（RR），时间片 = %d =====\n", time_quantum);

    // 复制原始进程信息，避免修改原数据
    Process copy[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        copy[i] = processes[i];
        copy[i].remaining_time = processes[i].burst_time;
    }

    // 按到达时间排序
    sort_by_arrival(copy, n);

    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES * 10]; // 进程队列
    int front = 0, rear = 0;

    // 初始化队列，将到达时间为0的进程加入队列
    for (int i = 0; i < n; i++) {
        if (copy[i].arrival_time == 0) {
            queue[rear++] = i;
        }
    }

    while (completed < n) {
        if (front == rear) { // 队列为空，推进时间
            current_time++;
            // 检查是否有新进程到达
            for (int i = 0; i < n; i++) {
                if (copy[i].remaining_time > 0 && copy[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                }
            }
            continue;
        }

        int current_process = queue[front++];

        // 如果进程已经完成，跳过
        if (copy[current_process].remaining_time == 0) continue;

        // 执行一个时间片
        int execution_time = (copy[current_process].remaining_time < time_quantum) ?
            copy[current_process].remaining_time : time_quantum;

        copy[current_process].remaining_time -= execution_time;
        current_time += execution_time;

        // 检查是否有新进程在执行期间到达
        for (int i = 0; i < n; i++) {
            if (i != current_process && copy[i].remaining_time > 0 &&
                copy[i].arrival_time <= current_time && !is_in_queue(queue, front, rear, i)) {
                queue[rear++] = i;
            }
        }

        // 如果当前进程未完成，重新加入队列尾部
        if (copy[current_process].remaining_time > 0) {
            queue[rear++] = current_process;
        }
        else {
            // 进程完成，记录完成时间等信息
            completed++;
            copy[current_process].completion_time = current_time;
            copy[current_process].turnaround_time = copy[current_process].completion_time - copy[current_process].arrival_time;
            copy[current_process].waiting_time = copy[current_process].turnaround_time - copy[current_process].burst_time;
        }
    }

    // 计算平均时间
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += copy[i].waiting_time;
        avg_turnaround_time += copy[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // 按PID排序后输出结果
    sort_by_pid(copy, n);
    printf("进程\t到达时间\t执行时间\t完成时间\t周转时间\t等待时间\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            copy[i].pid, copy[i].arrival_time, copy[i].burst_time,
            copy[i].completion_time, copy[i].turnaround_time, copy[i].waiting_time);
    }
    printf("平均等待时间: %.2f\n", avg_waiting_time);
    printf("平均周转时间: %.2f\n", avg_turnaround_time);
}

// 辅助函数：检查进程是否在队列中
int is_in_queue(int queue[], int front, int rear, int pid) {
    for (int i = front; i < rear; i++) {
        if (queue[i] == pid) return 1;
    }
    return 0;
}

// 4. 优先级调度（非抢占式）
void priority(Process processes[], int n) {
    printf("\n===== 优先级调度（非抢占式） =====\n");

    // 按到达时间排序
    sort_by_arrival(processes, n);

    int current_time = 0;
    int completed = 0;
    int visited[MAX_PROCESSES] = { 0 }; // 标记进程是否已完成

    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = 9999; // 优先级数值越小，优先级越高

        // 查找当前时间已到达且优先级最高的进程
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival_time <= current_time &&
                processes[i].priority < highest_priority) {
                highest_priority_index = i;
                highest_priority = processes[i].priority;
            }
        }

        // 如果没有找到符合条件的进程，时间推进到下一个进程的到达时间
        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }

        visited[highest_priority_index] = 1;
        completed++;

        processes[highest_priority_index].start_time = current_time;
        processes[highest_priority_index].completion_time = current_time + processes[highest_priority_index].burst_time;
        processes[highest_priority_index].turnaround_time = processes[highest_priority_index].completion_time - processes[highest_priority_index].arrival_time;
        processes[highest_priority_index].waiting_time = processes[highest_priority_index].turnaround_time - processes[highest_priority_index].burst_time;

        current_time = processes[highest_priority_index].completion_time;
    }

    // 计算平均时间
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // 按PID排序后输出结果
    sort_by_pid(processes, n);
    printf("进程\t到达时间\t执行时间\t优先级\t完成时间\t周转时间\t等待时间\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
            processes[i].priority, processes[i].completion_time,
            processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("平均等待时间: %.2f\n", avg_waiting_time);
    printf("平均周转时间: %.2f\n", avg_turnaround_time);
}

int main() {
    int n;
    printf("请输入进程数量: ");
    scanf("%d", &n);

    Process processes[MAX_PROCESSES];//目前设置为10

    printf("请输入每个进程的到达时间和执行时间:\n");
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("进程 %d: 到达时间 = ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("        执行时间 = ");
        scanf("%d", &processes[i].burst_time);
        printf("        优先级 = ");  // 为优先级调度准备
        scanf("%d", &processes[i].priority);
    }

    // 创建进程副本，避免不同调度算法之间相互影响
    Process fcfs_processes[MAX_PROCESSES];
    Process sjf_processes[MAX_PROCESSES];
    Process rr_processes[MAX_PROCESSES];
    Process priority_processes[MAX_PROCESSES];

    for (int i = 0; i < n; i++) {
        fcfs_processes[i] = processes[i];
        sjf_processes[i] = processes[i];
        rr_processes[i] = processes[i];
        priority_processes[i] = processes[i];
    }

    // 执行各种调度算法（移除了MFQ调用）
    fcfs(fcfs_processes, n);
    sjf(sjf_processes, n);
    rr(rr_processes, n, 2); // 时间片设为2
    priority(priority_processes, n);

    return 0;
}