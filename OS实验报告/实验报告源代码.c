#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

// ���̿��ƿ�ṹ
typedef struct {
    int pid;           // ����ID
    int arrival_time;  // ����ʱ��
    int burst_time;    // ִ��ʱ��
    int priority;      // ���ȼ����������ȼ����ȣ�
    int start_time;    // ��ʼִ��ʱ��
    int completion_time; // ���ʱ��
    int turnaround_time; // ��תʱ��
    int waiting_time;    // �ȴ�ʱ��
    int remaining_time;  // ʣ��ִ��ʱ�䣨������ռʽ���ȣ�
} Process;

// ������ʱ������
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

// ������ID����������������
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

// 1. �����ȷ���FCFS��
void fcfs(Process processes[], int n) {
    printf("\n===== �����ȷ���FCFS������ =====\n");

    // ������ʱ������
    sort_by_arrival(processes, n);

    int current_time = 0;

    for (int i = 0; i < n; i++) {
        // �����ǰʱ��С�ڽ��̵���ʱ�䣬��Ҫ�ȴ�
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }

        processes[i].start_time = current_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

        current_time = processes[i].completion_time;
    }

    // ����ƽ��ʱ��
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // ��PID�����������
    sort_by_pid(processes, n);
    printf("����\t����ʱ��\tִ��ʱ��\t���ʱ��\t��תʱ��\t�ȴ�ʱ��\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
            processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("ƽ���ȴ�ʱ��: %.2f\n", avg_waiting_time);
    printf("ƽ����תʱ��: %.2f\n", avg_turnaround_time);
}

// 2. ����ҵ���ȣ�SJF������ռʽ��
void sjf(Process processes[], int n) {
    printf("\n===== ����ҵ���ȣ�SJF������ =====\n");

    // ������ʱ������
    sort_by_arrival(processes, n);

    int current_time = 0;
    int completed = 0;
    int visited[MAX_PROCESSES] = { 0 }; // ��ǽ����Ƿ������

    while (completed < n) {
        int shortest_index = -1;
        int shortest_burst = 9999;

        // ���ҵ�ǰʱ���ѵ�����ִ��ʱ����̵Ľ���
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival_time <= current_time &&
                processes[i].burst_time < shortest_burst) {
                shortest_index = i;
                shortest_burst = processes[i].burst_time;
            }
        }

        // ���û���ҵ����������Ľ��̣�ʱ���ƽ�����һ�����̵ĵ���ʱ��
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

    // ����ƽ��ʱ��
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // ��PID�����������
    sort_by_pid(processes, n);
    printf("����\t����ʱ��\tִ��ʱ��\t���ʱ��\t��תʱ��\t�ȴ�ʱ��\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
            processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("ƽ���ȴ�ʱ��: %.2f\n", avg_waiting_time);
    printf("ƽ����תʱ��: %.2f\n", avg_turnaround_time);
}

// 3. ��ת���ȣ�RR��
void rr(Process processes[], int n, int time_quantum) {
    printf("\n===== ��ת���ȣ�RR����ʱ��Ƭ = %d =====\n", time_quantum);

    // ����ԭʼ������Ϣ�������޸�ԭ����
    Process copy[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        copy[i] = processes[i];
        copy[i].remaining_time = processes[i].burst_time;
    }

    // ������ʱ������
    sort_by_arrival(copy, n);

    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES * 10]; // ���̶���
    int front = 0, rear = 0;

    // ��ʼ�����У�������ʱ��Ϊ0�Ľ��̼������
    for (int i = 0; i < n; i++) {
        if (copy[i].arrival_time == 0) {
            queue[rear++] = i;
        }
    }

    while (completed < n) {
        if (front == rear) { // ����Ϊ�գ��ƽ�ʱ��
            current_time++;
            // ����Ƿ����½��̵���
            for (int i = 0; i < n; i++) {
                if (copy[i].remaining_time > 0 && copy[i].arrival_time <= current_time) {
                    queue[rear++] = i;
                }
            }
            continue;
        }

        int current_process = queue[front++];

        // ��������Ѿ���ɣ�����
        if (copy[current_process].remaining_time == 0) continue;

        // ִ��һ��ʱ��Ƭ
        int execution_time = (copy[current_process].remaining_time < time_quantum) ?
            copy[current_process].remaining_time : time_quantum;

        copy[current_process].remaining_time -= execution_time;
        current_time += execution_time;

        // ����Ƿ����½�����ִ���ڼ䵽��
        for (int i = 0; i < n; i++) {
            if (i != current_process && copy[i].remaining_time > 0 &&
                copy[i].arrival_time <= current_time && !is_in_queue(queue, front, rear, i)) {
                queue[rear++] = i;
            }
        }

        // �����ǰ����δ��ɣ����¼������β��
        if (copy[current_process].remaining_time > 0) {
            queue[rear++] = current_process;
        }
        else {
            // ������ɣ���¼���ʱ�����Ϣ
            completed++;
            copy[current_process].completion_time = current_time;
            copy[current_process].turnaround_time = copy[current_process].completion_time - copy[current_process].arrival_time;
            copy[current_process].waiting_time = copy[current_process].turnaround_time - copy[current_process].burst_time;
        }
    }

    // ����ƽ��ʱ��
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += copy[i].waiting_time;
        avg_turnaround_time += copy[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // ��PID�����������
    sort_by_pid(copy, n);
    printf("����\t����ʱ��\tִ��ʱ��\t���ʱ��\t��תʱ��\t�ȴ�ʱ��\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            copy[i].pid, copy[i].arrival_time, copy[i].burst_time,
            copy[i].completion_time, copy[i].turnaround_time, copy[i].waiting_time);
    }
    printf("ƽ���ȴ�ʱ��: %.2f\n", avg_waiting_time);
    printf("ƽ����תʱ��: %.2f\n", avg_turnaround_time);
}

// �����������������Ƿ��ڶ�����
int is_in_queue(int queue[], int front, int rear, int pid) {
    for (int i = front; i < rear; i++) {
        if (queue[i] == pid) return 1;
    }
    return 0;
}

// 4. ���ȼ����ȣ�����ռʽ��
void priority(Process processes[], int n) {
    printf("\n===== ���ȼ����ȣ�����ռʽ�� =====\n");

    // ������ʱ������
    sort_by_arrival(processes, n);

    int current_time = 0;
    int completed = 0;
    int visited[MAX_PROCESSES] = { 0 }; // ��ǽ����Ƿ������

    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = 9999; // ���ȼ���ֵԽС�����ȼ�Խ��

        // ���ҵ�ǰʱ���ѵ��������ȼ���ߵĽ���
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival_time <= current_time &&
                processes[i].priority < highest_priority) {
                highest_priority_index = i;
                highest_priority = processes[i].priority;
            }
        }

        // ���û���ҵ����������Ľ��̣�ʱ���ƽ�����һ�����̵ĵ���ʱ��
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

    // ����ƽ��ʱ��
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    // ��PID�����������
    sort_by_pid(processes, n);
    printf("����\t����ʱ��\tִ��ʱ��\t���ȼ�\t���ʱ��\t��תʱ��\t�ȴ�ʱ��\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
            processes[i].priority, processes[i].completion_time,
            processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("ƽ���ȴ�ʱ��: %.2f\n", avg_waiting_time);
    printf("ƽ����תʱ��: %.2f\n", avg_turnaround_time);
}

int main() {
    int n;
    printf("�������������: ");
    scanf("%d", &n);

    Process processes[MAX_PROCESSES];//Ŀǰ����Ϊ10

    printf("������ÿ�����̵ĵ���ʱ���ִ��ʱ��:\n");
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("���� %d: ����ʱ�� = ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("        ִ��ʱ�� = ");
        scanf("%d", &processes[i].burst_time);
        printf("        ���ȼ� = ");  // Ϊ���ȼ�����׼��
        scanf("%d", &processes[i].priority);
    }

    // �������̸��������ⲻͬ�����㷨֮���໥Ӱ��
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

    // ִ�и��ֵ����㷨���Ƴ���MFQ���ã�
    fcfs(fcfs_processes, n);
    sjf(sjf_processes, n);
    rr(rr_processes, n, 2); // ʱ��Ƭ��Ϊ2
    priority(priority_processes, n);

    return 0;
}